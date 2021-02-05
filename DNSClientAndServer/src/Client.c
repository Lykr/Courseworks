#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define T_A 1
#define T_NS 2
#define T_CNAME 5
#define T_SOA 6
#define T_PTR 12
#define T_MX 15

#define BUFSIZE 1024

struct Header
{
	unsigned short id;
	/* Flags， [Note] Bit field store: last is big */
	unsigned char rd : 1;
	unsigned char tc : 1;
	unsigned char aa : 1;
	unsigned char opcode : 4;
	unsigned char qr : 1;

	unsigned char rcode : 4;
	unsigned char cd : 1;
	unsigned char ad : 1;
	unsigned char z : 1; // Different with Note
	unsigned char ra : 1;

	unsigned short questionNum;
	unsigned short answerNum;
	unsigned short authorityNum;
	unsigned short additionalNum;
};

struct QuestionParam
{
	unsigned short type;
	unsigned short class;
};

struct Question
{
	unsigned char *name;
	struct QuestionParam *param;
};

#pragma pack(push, 1) // Set 1 byte as min of structure
struct ResourceParam
{
	unsigned short type;
	unsigned short class;
	unsigned int ttl;
	unsigned short len;
};
#pragma pack(pop)

struct Resource
{
	unsigned char *name;
	struct ResourceParam *param;
	unsigned char *data;
};

int queryRD;

char *getLocalIP()
{
	unsigned char *reader = (unsigned char *)malloc(BUFSIZE); //255.255.255.255\0\n
	FILE *fp;

	fp = fopen("Client.config", "r");

	if (fp == NULL)
	{
		printf("Cannot found config of Client\n");
		exit(1);
	}

	if (fgets(reader, BUFSIZE, fp) == NULL)
	{
		printf("Get config failed\n");
		exit(1);
	}

	fclose(fp);

	return strtok(reader, "#"); // Clear comment
}

int getNameLen(unsigned char *pointer)
{
	int len = 0;
	unsigned char *p;
	p = pointer;

	while (*p != 0)
	{
		if (*p == 0xc0)
			return len + 2;

		len++;  // Add len
		p += 1; // Move pointer
	}

	return len + 1; // Add 1 for '0'
}

int getDotNameLen(unsigned char *pointer)
{
	return getNameLen(pointer) - 2; // 5baidu3com0(11) -> baidu.com(9)
}

unsigned char *getPTRName(char *name)
{
	unsigned char *PTRName = (unsigned char *)malloc(strlen(name) + 13 + 1); // + .in-addr.arpa(13) + '\0'
	int i;

	/* UNDER CONSTRUCTION */

	return PTRName;
}

// return name(dot)
void readDotName(unsigned char *dotName, unsigned char *pointer, unsigned char *buffer)
{
	unsigned char *p, *pp; // Pointer for buffer
	int i, j, n;

	p = pointer; // p point to pointer

	/* Convert INName to name */
	j = 0; // index of name
	while (*p != 0)
	{
		/* If a offset */
		if (*p == (unsigned char)0xc0)
		{
			int offset = *(++p);
			pp = p + 1; // Move over offset, store the pointer
			p = 2 + buffer + offset;
		}

		n = *p; // Store the little num
		p++;	// Move pointer to char
		for (i = 0; i < n; i++)
		{
			dotName[j++] = *p++;
		}
		dotName[j++] = '.'; // Add dot at last
	}
	dotName[--j] = '\0'; // Add end sign
}

int writeINName(char *name, unsigned char *p)
{
	int i;
	int len = 0;
	int poi = 0;
	strcat(name, "."); // Add root

	for (i = 0; i < strlen((char *)name); i++)
	{
		if (name[i] == '.')
		{
			*p++ = i - poi;
			len++;
			for (; poi < i; poi++)
			{
				*p++ = name[poi];
				len++;
			}
			poi++;
		}
	}
	*p++ = '\0';
	len++;

	return len; // The length of INName
}

int createQuery(char *queryName, char *queryType, unsigned char *pointer)
{
	/* Define DNS Header */
	struct Header *header;
	header = (struct Header *)(pointer + 2);
	header->id = (unsigned short)htons(getpid());
	header->qr = 0;
	header->opcode = 0;
	header->aa = 0;
	header->tc = 0;
	header->rd = queryRD;
	header->ra = 0;
	header->z = 0;
	header->ad = 0;
	header->cd = 0;
	header->rcode = 0;
	header->questionNum = htons(1);
	header->answerNum = 0;
	header->authorityNum = 0;
	header->additionalNum = 0;

	/* Define DNS Question */
	unsigned short queryTypeNum = 1;
	int INNameLen;
	int packetLen;

	if (strcmp(queryType, "A") == 0)
		queryTypeNum = 1; // A
	else if (strcmp(queryType, "CNAME") == 0)
		queryTypeNum = 5; // MX
	else if (strcmp(queryType, "PTR") == 0)
		queryTypeNum = 12;
	else if (strcmp(queryType, "MX") == 0)
		queryTypeNum = 15; // CNAME
	else
		return -1;

	if (queryTypeNum == 12)
	{
		INNameLen = writeINName(getPTRName(queryName), 2 + pointer + sizeof(struct Header)); // Put query domain name, get its length
	}
	else
	{
		INNameLen = writeINName(queryName, 2 + pointer + sizeof(struct Header)); // Put query domain name, get its length
	}

	/* Put type and class */
	struct QuestionParam *p;
	p = (struct QuestionParam *)&pointer[2 + sizeof(struct Header) + INNameLen];
	p->type = htons(queryTypeNum);
	p->class = htons(1); // IN type

	packetLen = sizeof(struct Header) + INNameLen + 4;				 // lenByte + Header + INNameLen + Type&Classs
	*((unsigned short *)pointer) = htons((unsigned short)packetLen); // Store length byte

	return packetLen + 2;
}

void printResponse(unsigned char *response)
{
	struct Header *header;
	unsigned char *p;
	unsigned short len;
	int i, j;

	header = (struct Header *)(response + 2); // 2 bytes for length
	printf("\nThe response contains: ");
	printf("\n %d Questions", ntohs(header->questionNum));
	printf("\n %d Answers", ntohs(header->answerNum));
	printf("\n %d Authoritative Servers", ntohs(header->authorityNum));
	printf("\n %d Additional records\n\n", ntohs(header->additionalNum));

	// length + header + nameLen + type&class
	p = &response[2 + 12 + getNameLen(response + 2 + 12) + 4]; // Move pointer to answer
	struct Resource answer[ntohs(header->answerNum)];
	struct Resource authority[ntohs(header->authorityNum)];
	struct Resource additional[ntohs(header->additionalNum)];

	/* READ */
	/* Read answers */
	for (i = 0; i < ntohs(header->answerNum); i++)
	{
		/* Read dot name */
		answer[i].name = (unsigned char *)malloc(BUFSIZE);
		readDotName(answer[i].name, p, response); // Read dot name

		p += getNameLen(p); // Move pointer over name

		/* Read parameter */
		answer[i].param = (struct ResourceParam *)p; // Give param value
		p += sizeof(struct ResourceParam);			 // Move pointer over param

		/* Read data */
		len = ntohs(answer[i].param->len);
		if (ntohs(answer[i].param->type) == T_A) // If A type
		{
			answer[i].data = (unsigned char *)malloc(len + 1); // Allocate memory for data
			for (j = 0; j < len; j++)
			{
				answer[i].data[j] = p[j];
			}
			answer[i].data[len] = '\0';
			p += len; //Move point over data
		}
		else if (ntohs(answer[i].param->type) == T_CNAME)
		{
			answer[i].data = (unsigned char *)malloc(len + 1); // Allocate memory for data
			for (j = 0; j < len; j++)
			{
				answer[i].data[j] = p[j];
			}
			answer[i].data[len] = '\0';
			p += len; //Move point over data
		}
		else if (ntohs(answer[i].param->type) == T_MX)
		{
			answer[i].data = (unsigned char *)malloc(len + 1); // Allocate memory for data
			for (j = 0; j < len; j++)
			{
				answer[i].data[j] = p[j];
			}
			answer[i].data[len] = '\0';
			p += len; //Move point over data
		}
	}

	/* Read authorities */
	for (i = 0; i < ntohs(header->authorityNum); i++)
	{

		authority[i].name = (unsigned char *)malloc(BUFSIZE);
		readDotName(authority[i].name, p, response);
		p += getNameLen(p);

		authority[i].param = (struct ResourceParam *)p;
		p += sizeof(struct ResourceParam);

		len = ntohs(authority[i].param->len);
		authority[i].data = (unsigned char *)malloc(len + 1);
		for (j = 0; j < len; j++)
		{
			authority[i].data[j] = p[j];
		}
		authority[i].data[len] = '\0';
		p += len;
	}

	/* Read additional */
	for (i = 0; i < ntohs(header->additionalNum); i++)
	{
		additional[i].name = (unsigned char *)malloc(BUFSIZE);
		(additional[i].name, p, response);
		p += getNameLen(p);

		additional[i].param = (struct ResourceParam *)p;
		p += sizeof(struct ResourceParam);

		len = ntohs(additional[i].param->len);
		if (ntohs(additional[i].param->type) == T_A)
		{
			additional[i].data = (unsigned char *)malloc(len + 1);

			for (j = 0; j < len; j++)
			{
				additional[i].data[j] = p[j];
			}
			additional[i].data[len] = '\0';
			p += len;
		}
		else
		{
			additional[i].data = (unsigned char *)malloc(len + 1);
			for (j = 0; j < len; j++)
			{
				additional[i].data[j] = p[j];
			}
			additional[i].data[len] = '\0';
			p += len;
		}
	}

	/* PRINT */
	struct sockaddr_in a; // Use for storing addr to convert
	/* Print answer */
	printf("Answer Records: %d\n", ntohs(header->answerNum));
	for (i = 0; i < ntohs(header->answerNum); i++)
	{
		printf("+Name: %s\n", answer[i].name);

		if (ntohs(answer[i].param->type) == T_A)
		{
			a.sin_addr.s_addr = *(long *)answer[i].data; // Do not need ntohl
			printf("--IPv4 address: %s\n", inet_ntoa(a.sin_addr));
		}
		else if (ntohs(answer[i].param->type) == T_CNAME)
		{
			// Canonical name for an alias
			unsigned char temp[BUFSIZE];
			readDotName(temp, answer[i].data, response);
			printf("--Alias name: %s\n", temp);
		}
		else if (ntohs(answer[i].param->type) == T_MX)
		{
			printf("--Prefenence: %d\n", ntohs(*(unsigned short *)answer[i].data));
			unsigned char temp[BUFSIZE];
			readDotName(temp, answer[i].data + 2, response);
			printf("--Mail Exchange: %s\n", temp);
		}
	}

	/* Print authorities */
	printf("Authoritive Records: %d\n", ntohs(header->authorityNum));
	for (i = 0; i < ntohs(header->authorityNum); i++)
	{

		printf("+Name: %s\n", authority[i].name);
		if (ntohs(authority[i].param->type) == T_NS)
		{
			printf("--Nameserver: %s\n", authority[i].data);
		}
		else if (ntohs(authority[i].param->type) == T_SOA)
		{
			unsigned char *dp;
			dp = authority[i].data;
			unsigned char temp0[BUFSIZE], temp1[BUFSIZE];
			readDotName(temp0, dp, response);
			printf("--Primary name server: %s\n", temp0);
			dp += getNameLen(dp);
			readDotName(temp1, dp, response);
			printf("--Responsible authority's mailbox: %s\n", temp1);
			dp += getNameLen(dp);
			printf("--Serial Number: %d\n", ntohl(*(unsigned long *)dp));
			dp += 4;
			printf("--Refresh Interval: %d\n", ntohl(*(unsigned long *)dp));
			dp += 4;
			printf("--Retry Interval: %d\n", ntohl(*(unsigned long *)dp));
			dp += 4;
			printf("--Expire limit: %d\n", ntohl(*(unsigned long *)dp));
			dp += 4;
			printf("--Minimum TTL: %d\n", ntohl(*(unsigned long *)dp));
		}
	}

	/* Print additional resource records */
	printf("Additional Records: %d \n", ntohs(header->additionalNum));
	for (i = 0; i < ntohs(header->additionalNum); i++)
	{
		printf("+Name: %s\n", additional[i].name);
		if (ntohs(additional[i].param->type) == T_A)
		{
			a.sin_addr.s_addr = *(long *)additional[i].data;
			printf("--IPv4 address: %s\n", inet_ntoa(a.sin_addr));
		}
	}
}

int main(int argc, char **argv)
{
	int bufferLen;
	int sock_tcp;
	struct sockaddr_in serv_addr;
	unsigned char buffer[BUFSIZE]; // Store Packet

	if (argc != 4)
	{
		printf("Usage: %s <Domain name> <Query Type> <RA>\n", argv[0]);
		exit(1);
	}

	if (strcmp(argv[2], "A") != 0 && strcmp(argv[2], "CNAME") != 0 && strcmp(argv[2], "MX") != 0 )
	{
		printf("Wrong type\n");
		exit(1);
	}

	queryRD = atoi(argv[3]);

	if ((sock_tcp = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Create socket failed\n");
		exit(1);
	}

	/* Set recive time out for socket */
	struct timeval tv = {10, 0};
	setsockopt(sock_tcp, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(getLocalIP());
	serv_addr.sin_port = htons(53);

	/* Connect socket */
	if (connect(sock_tcp, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1)
	{
		printf("Connect socket failed\n");
		exit(1);
	}

	printf("Create DNS Query Packet...\n");
	memset(buffer, 0, BUFSIZE);
	bufferLen = createQuery(argv[1], argv[2], buffer); // Create DNS Query Packet

	if (bufferLen == -1)
	{
		printf("Wrong type\n");
		exit(1);
	}

	printf("Sending DNS Query Packet...\n");
	if (send(sock_tcp, buffer, bufferLen, 0) == -1) // Send DNS query packet data
	{
		printf("Send DNS query packet data failed\n");
		exit(1);
	}

	memset(buffer, 0, BUFSIZE); // Clear buffer for storing RR

	printf("Reciving DNS Query Packet...\n");
	if (recv(sock_tcp, buffer, BUFSIZE, 0) == -1) // Recive DNS Response
	{
		printf("Recive DNS response failed\n");
		exit(1);
	}
	else
	{
		printResponse(buffer); // Show RR
	}

	close(sock_tcp);

	return 0;
}