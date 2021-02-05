#include <stdio.h>      // for printf() and fprintf()
#include <sys/socket.h> // for socket(), sendto() and recvfrom()
#include <arpa/inet.h>  // for sockaddr_in and inet_addr()
#include <stdlib.h>     // for atoi() and exit()
#include <string.h>     // for memset()
#include <unistd.h>     // for close()

#define T_A 1
#define T_NS 2
#define T_CNAME 5
#define T_SOA 6
#define T_PTR 12
#define T_MX 15

#define BUFSIZE 1024

unsigned char *servName;
unsigned short servPort = 53;

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

void getIP(int option, char *buffer)
{
    unsigned char reader[BUFSIZE];
    unsigned char tempName[BUFSIZE];
    FILE *fp;

    memset(tempName, 0, BUFSIZE);
    memcpy(tempName, servName, BUFSIZE);

    fp = fopen(strcat(tempName, ".config"), "r");

    if (fp == NULL)
    {
        printf("Cannot found config of %s\n", servName);
        exit(1);
    }

    while (option--)
    {
        fgets(reader, BUFSIZE, fp);
    }

    fclose(fp);

    memcpy(buffer, strtok(reader, "#"), BUFSIZE); // Clear comment
}

void getRootIP(char *buffer)
{
    getIP(2, buffer);
}

void getServIP(char *buffer)
{
    getIP(1, buffer);
}

int getNameLen(char *pointer)
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

// return name(dot)
void readDotName(unsigned char *dotName, unsigned char *pointer, unsigned char *buffer)
{
    unsigned char *p, *pp; // Pointer for buffer
    int dotNameLen;
    int i, j, n;

    p = pointer; // p point to pointer

    /* Calculate name length */
    dotNameLen = getDotNameLen(pointer);

    /* Convert INName to name */
    j = 0; // index of name
    while (*p != 0)
    {
        /* If a offset */
        if (*p == (unsigned char)0xc0)
        {
            int offset = *(++p);
            pp = p + 1; // Move over offset, store the pointer
            p = buffer + offset;
        }

        n = *p; // Store the little num
        p++;    // Move pointer to char
        for (i = 0; i < n; i++)
        {
            dotName[j++] = *p++;
        }
        dotName[j++] = '.'; // Add dot at last
    }
    dotName[--j] = '\0'; // Add end sign
}

// return 0-no 1-part of it 2-get it
int checkCache(unsigned char *pointer)
{
    FILE *fp;
    int isFound = 0;
    unsigned char reader[BUFSIZE];
    unsigned char dotName[BUFSIZE];
    unsigned char tempName[BUFSIZE];

    memset(tempName, 0, BUFSIZE);
    memcpy(tempName, servName, BUFSIZE);

    fp = fopen(strcat(tempName, ".data"), "r");

    if (strcmp(servName, "LocalServer") == 0)
    {
        struct QuestionParam *qp;
        qp = (struct QuestionParam *)(2 + pointer + 12 + getNameLen(2 + pointer + 12));
        readDotName(dotName, 2 + pointer + 12, 2 + pointer);

        while (fgets(reader, BUFSIZE, fp) != NULL) // Get line in cache
        {
            unsigned char name[128], ttl[10], class[10], type[10], data[128];

            sscanf(reader, "%[^,],%[^,],%[^,],%[^,],%s", name, ttl, class, type, data);
            if (strcmp(name, dotName) == 0 && ntohs(qp->type) == getTypeNum(type))
            {
                isFound = 1;
                break;
            }
        }
    }
    else
    {
        struct QuestionParam *qp;
        qp = (struct QuestionParam *)(pointer + 12 + getNameLen(pointer + 12));
        readDotName(dotName, pointer + 12, pointer);

        while (fgets(reader, BUFSIZE, fp) != NULL)
        {
            unsigned char name[128], ttl[10], class[10], type[10], data[128];

            sscanf(reader, "%[^,],%[^,],%[^,],%[^,],%s", name, ttl, class, type, data);

            if (strcmp(dotName, name) == 0 && ntohs(qp->type) == getTypeNum(type)) // If there the name we want in database
            {
                isFound = 2;
                break;
            }
            else if (strstr(dotName, name) != NULL)
            {
                isFound = 1;
                break;
            }
        }
    }

    fclose(fp);
    return isFound;
}

void updateCache(unsigned char *response)
{
    FILE *fp;
    int i, j;
    struct Header *header;
    unsigned char *p;
    unsigned short len;

    header = (struct Header *)response;

    // header + nameLen + type&class
    p = response + 12 + getNameLen(response + 12) + 4; // Move pointer to answer
    struct Resource answer[ntohs(header->answerNum)];
    // struct Resource authority[ntohs(header->authorityNum)];
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
        p += 10;                                     // Move pointer over param

        /* Read data */
        len = ntohs(answer[i].param->len);
        if (ntohs(answer[i].param->type) == T_A || ntohs(answer[i].param->type) == T_MX || ntohs(answer[i].param->type) == T_CNAME) // If A type
        {
            answer[i].data = (unsigned char *)malloc(len + 1); // Allocate memory for data
            for (j = 0; j < len; j++)
            {
                answer[i].data[j] = p[j];
            }
            answer[i].data[len] = '\0';
            p += len; //Move point over data
        }
        else
        {
            answer[i].data = (unsigned char *)malloc(len + 1); // Allocate memory for data
            for (j = 0; j < len; j++)
            {
                answer[i].data[j] = p[j];
            }
            answer[i].data[len] = '\0';
            p += (len - 2);
        }
    }

    /* Read additional */
    for (i = 0; i < ntohs(header->additionalNum); i++)
    {
        additional[i].name = (unsigned char *)malloc(BUFSIZE);
        readDotName(additional[i].name, p, response);
        p += getNameLen(p);

        additional[i].param = (struct ResourceParam *)p;
        p += 10;

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

    /* WRITE */
    unsigned char tempName[BUFSIZE];

    memset(tempName, 0, BUFSIZE);
    memcpy(tempName, servName, BUFSIZE);

    fp = fopen(strcat(tempName, ".data"), "a");

    struct sockaddr_in a; // Use for storing addr to convert

    /* Write answer */
    for (i = 0; i < ntohs(header->answerNum); i++)
    {
        if (ntohs(answer[i].param->type) == T_A)
        {
            a.sin_addr.s_addr = *(long *)answer[i].data; // Do not need ntohl
            fprintf(fp, "%s,%d,%s,%s,%s\n", answer[i].name, ntohl(answer[i].param->ttl), "IN", "A", inet_ntoa(a.sin_addr));
        }
        else if (ntohs(answer[i].param->type) == T_NS)
        {
            a.sin_addr.s_addr = *(long *)answer[i].data; // Do not need ntohl
            fprintf(fp, "%s,%d,%s,%s,%s\n", answer[i].name, ntohl(answer[i].param->ttl), "IN", "NS", inet_ntoa(a.sin_addr));
        }
        else if (ntohs(answer[i].param->type) == T_MX)
        {
            unsigned char temp[BUFSIZE];
            readDotName(temp, answer[i].data + 2, response);
            fprintf(fp, "%s,%d,%s,%s,%s\n", answer[i].name, ntohl(answer[i].param->ttl), "IN", "MX", temp);
        }
        else if (ntohs(answer[i].param->type) == T_CNAME)
        {
            unsigned char temp[BUFSIZE];
            readDotName(temp, answer[i].data, response);
            fprintf(fp, "%s,%d,%s,%s,%s\n", answer[i].name, ntohl(answer[i].param->ttl), "IN", "CNAME", temp);
        }
    }

    /* Write addtional */
    for (i = 0; i < ntohs(header->additionalNum); i++)
    {
        if (ntohs(additional[i].param->type) == T_A)
        {
            a.sin_addr.s_addr = *(long *)additional[i].data; // Do not need ntohl
            fprintf(fp, "%s,%d,%s,%s,%s\n", additional[i].name, ntohl(additional[i].param->ttl), "IN", "A", inet_ntoa(a.sin_addr));
        }
    }

    /* Free memory */
    for (i = 0; i < ntohs(header->answerNum); i++)
    {
        free(answer[i].name);
        free(answer[i].data);
    }

    for (i = 0; i < ntohs(header->additionalNum); i++)
    {
        free(additional[i].name);
        free(additional[i].data);
    }

    fclose(fp);
}

// Writer INName at pointer p
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

int getTypeNum(unsigned char *type)
{
    int typeNum = 1;

    if (strcmp(type, "A") == 0)
        typeNum = 1; // A
    else if (strcmp(type, "NS") == 0)
        typeNum = 2;
    else if (strcmp(type, "CNAME") == 0)
        typeNum = 5; // MX
    else if (strcmp(type, "PTR") == 0)
        typeNum = 12;
    else if (strcmp(type, "MX") == 0)
        typeNum = 15; // CNAME

    return typeNum;
}
// return answer num(0) and length(1)
void copyCache(unsigned char *buffer, int *ans)
{
    int isFound = 0;
    unsigned char *p = buffer;
    unsigned char tempName[BUFSIZE];
    unsigned char dotName[BUFSIZE];
    unsigned char reader[BUFSIZE];

    p += 12; // Move over header;
    readDotName(dotName, p, buffer);
    p += getNameLen(p);
    unsigned short queryType = ntohs(*(unsigned short *)p);
    p += 2;
    unsigned short queryClass = ntohs(*(unsigned short *)p);
    p += 2;

    /* Compere with DB */
    FILE *fp;

    memset(tempName, 0, BUFSIZE);
    memcpy(tempName, servName, BUFSIZE);

    fp = fopen(strcat(tempName, ".data"), "r");

    /* Find the answer */
    while (fgets(reader, BUFSIZE, fp) != NULL)
    {
        unsigned char name[128], ttl[10], class[10], type[10], data[128];

        sscanf(reader, "%[^,],%[^,],%[^,],%[^,],%s", name, ttl, class, type, data);

        int typeNum = getTypeNum(type);

        /* Found a part of domain name */
        if (strcmp(dotName, name) == 0 && typeNum == queryType)
        {
            int nameLen = writeINName(name, p);
            p += nameLen;
            ans[0] += nameLen;

            struct ResourceParam *rp;
            rp = (struct ResourceParam *)p;

            rp->type = htons(typeNum);
            rp->class = htons(1);
            rp->ttl = htonl(atoi(ttl));

            p += 10; // Move over to data
            ans[0] += 10;

            int dataLen = 0;
            if (typeNum == T_MX)
            {
                *(unsigned short *)p = htons(5);
                p += 2;
                dataLen = writeINName(data, p);
                p += dataLen;
                ans[0] += (dataLen + 2);
                rp->len = htons(dataLen + 2);
            }
            else if (typeNum == T_CNAME)
            {
                dataLen = writeINName(data, p);
                p += dataLen;
                ans[0] += dataLen;
                rp->len = htons(dataLen);
            }
            else
            {
                *(unsigned long *)p = inet_addr(data);
                dataLen = sizeof(inet_addr(data));
                p += dataLen;
                ans[0] += dataLen;
                rp->len = htons(dataLen);
            }

            /* MX */
            if (queryType == T_MX)
            {
                while (fgets(reader, BUFSIZE, fp) != NULL)
                {
                    sscanf(reader, "%[^,],%[^,],%[^,],%[^,],%s", name, ttl, class, type, data);

                    if (strstr(name, dotName) != NULL)
                    {
                        dataLen = writeINName(name, p);
                        p += dataLen;
                        ans[0] += dataLen;

                        rp = (struct ResourceParam *)p;
                        rp->type = htons(1);  // A
                        rp->class = htons(1); // IN
                        rp->ttl = htonl(atoi(ttl));
                        p += 10;
                        ans[0] += 10;

                        *(unsigned long *)p = inet_addr(data);
                        dataLen = sizeof(inet_addr(data));
                        rp->len = htons(dataLen);
                        p += dataLen;
                        ans[0] += dataLen;

                        ans[2]++;
                        break;
                    }
                }
            }
            ans[1]++;
            isFound = 1;

            break;
        }
    }

    if (strcmp(servName, "LocalServer") != 0)
    {
        /* If not answer, find next server IP */
        if (!isFound)
        {
            fseek(fp, 0, SEEK_SET);
            while (fgets(reader, BUFSIZE, fp) != NULL)
            {
                unsigned char name[128], ttl[10], class[10], type[10], data[128];

                sscanf(reader, "%[^,],%[^,],%[^,],%[^,],%s", name, ttl, class, type, data);

                int typeNum = getTypeNum(type);

                /* Found a part of domain name */
                if (strstr(dotName, name) != NULL && typeNum == 1)
                {
                    int nameLen = writeINName(name, p);
                    p += nameLen;
                    ans[0] += nameLen;

                    struct ResourceParam *rp;
                    rp = (struct ResourceParam *)p;

                    rp->type = htons(typeNum);
                    rp->class = htons(1);
                    rp->ttl = htonl(atoi(ttl));

                    p += 10; // Move over to data
                    ans[0] += 10;

                    int dataLen = 0;
                    if (typeNum == T_MX || typeNum == T_CNAME)
                    {
                        dataLen = writeINName(data, p);
                        p += dataLen;
                        ans[0] += dataLen;
                    }
                    else
                    {
                        *(unsigned long *)p = inet_addr(data);
                        dataLen = sizeof(inet_addr(data));
                        p += dataLen;
                        ans[0] += dataLen;
                    }
                    rp->len = htons(dataLen);
                    ans[2]++;

                    break;
                }
            }
        }
    }
    fclose(fp);
}

int queryOthers(unsigned char *query)
{
    int sock;
    int isFound = 0;
    int MAXTIMES = 10;
    struct sockaddr_in servAddr;
    unsigned char dotName[BUFSIZE];
    unsigned char queryBuffer[BUFSIZE];
    unsigned char responseBuffer[BUFSIZE];
    char servIP[BUFSIZE];

    getRootIP(servIP);

    memset(queryBuffer, 0, BUFSIZE);
    memcpy(queryBuffer, query + 2, BUFSIZE);
    readDotName(dotName, queryBuffer + 12, queryBuffer);

    int nameLen = getNameLen(queryBuffer + 12);

    /* Get question type */
    struct QuestionParam *qp;
    qp = (struct QuestionParam *)(queryBuffer + 12 + nameLen);
    int queryType = ntohs(qp->type);

    /* Create socket */
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        printf("socket() failed.\n");

    /* Set recive time out for socket */
    struct timeval tv = {10, 0};
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));

    while (MAXTIMES--)
    {
        /* SEND QUERY */
        /* Construct the server address structure */
        memset(&servAddr, 0, sizeof(servAddr));       /* Zero out structure */
        servAddr.sin_family = AF_INET;                /* Internet addr family */
        servAddr.sin_addr.s_addr = inet_addr(servIP); /* Server IP address */
        servAddr.sin_port = htons(servPort);          /* Server port */

        /* Send to server */
        printf("Send query to %s server\n", servIP);
        int queryLen = 12 + getNameLen(queryBuffer + 12) + 4; // Get query length
        if (sendto(sock, queryBuffer, queryLen, 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) != queryLen)
        {
            printf("sendto() sent a different number of bytes than expected.\n");
            break;
        }

        /* RECIVE RESPONSE */
        int servAddrLen = sizeof(servAddr);
        memset(responseBuffer, 0, BUFSIZE);
        if (recvfrom(sock, responseBuffer, BUFSIZE, 0, (struct sockaddr *)&servAddr, &servAddrLen) < 0)
        {
            printf("No response from another\n");
            break;
        }
        printf("Get response from %s server\n", servIP);

        unsigned char *p;
        struct Resource answer;

        /* Read dot name */
        // header + nameLen + type&class
        p = &responseBuffer[12 + getNameLen(responseBuffer + 12) + 4]; // Move pointer to answer
        answer.name = (unsigned char *)malloc(BUFSIZE);
        readDotName(answer.name, p, responseBuffer); // Read dot name of response in answer

        answer.param = (struct ResourceParam *)(p + getNameLen(p));

        /* Get rcode */
        struct Header *header;
        header = (struct Header *)responseBuffer;

        if (header->rcode == 3) // No this one, We get a fault...
        {
            isFound = 0;
            break;
        }
        else if (strcmp(dotName, answer.name) == 0 && ntohs(answer.param->type) == queryType) // Get it!
        {
            updateCache(responseBuffer); // Move over 2 byte for length bytes
            isFound = 1;
            break;
        }
        else
        {
            printf("No answer, ask others\n");
            /* Get off, ask others */
            p += getNameLen(p); // Move pointer over name

            /* Read parameter */
            answer.param = (struct ResourceParam *)p; // Give param value
            p += sizeof(struct ResourceParam);        // Move pointer over param

            /* Read data */
            int j;
            int len = ntohs(answer.param->len);
            if (ntohs(answer.param->type) == T_NS) // If NS type
            {
                answer.data = (unsigned char *)malloc(len + 1); // Allocate memory for data
                for (j = 0; j < len; j++)
                {
                    answer.data[j] = p[j];
                }
                answer.data[len] = '\0';
                p += len; //Move point over data
            }
            else
            {
                answer.data = (unsigned char *)malloc(len + 1);
                for (j = 0; j < len; j++)
                {
                    answer.data[j] = p[j];
                }
                answer.data[len] = '\0';
                p += len;
            }

            /* Get next server IP */
            struct sockaddr_in a;
            a.sin_addr.s_addr = *(long *)answer.data;

            memset(servIP, 0, BUFSIZE);
            memcpy(servIP, inet_ntoa(a.sin_addr), BUFSIZE);
            free(answer.data);
            free(answer.name);
        }
    }

    close(sock);

    return isFound; // No found
}

// Return length of response, option: 0-udp 1-tcp
int createResponse(unsigned char *buffer, int option)
{
    int queryLen = 0;
    int ans[] = {0, 0, 0};
    unsigned char *p = buffer;

    if (option)
    {
        p += 2; // Move over for length bytes
    }

    copyCache(p, ans); // Copy answer into buffer

    /* Modify header */
    struct Header *header;
    header = (struct Header *)p; // Reserve 2 bytes for length
    header->qr = 1;              // Response packet
    header->ra = 1;
    header->answerNum = htons(ans[1]);
    header->additionalNum = htons(ans[2]);

    /* Add length of packet */
    queryLen = 12 + getNameLen(p + 12) + 4 + ans[0]; // Excluding len char

    if (option) // Is a tcp repsonse, add length char
    {
        *((unsigned short *)(p - 2)) = htons(queryLen);
        queryLen += 2;
    }

    return queryLen; // Including len char
}

int createTCPResponse(unsigned char *buffer)
{
    return createResponse(buffer, 1); // buffer add 2 for len bytes
}

int createUDPResponse(unsigned char *buffer)
{
    return createResponse(buffer, 0);
}

// return the length of fault packet, option: 0-udp 1-tcp
int createFail(unsigned char *buffer, int option)
{
    int queryLen;
    unsigned char *p = buffer;

    if (option)
    {
        p += 2;
    }

    /* Modify header */
    struct Header *header;
    header = (struct Header *)p;   // Reserve 2 bytes for length
    header->qr = 1;                // Response packet
    header->rcode = htons(3 << 8); // Rcode 0011(3) : No such name

    queryLen = 12 + getNameLen(p + 12) + 4;

    /* Add length of packet */
    if (option) // Is a tcp repsonse, add length char
    {
        *((unsigned short *)(p - 2)) = htons(queryLen);
        queryLen += 2;
    }

    return queryLen; // 2 len bytes + queryLen
}

// return the length of fault packet
int createTCPFail(unsigned char *buffer)
{
    return createFail(buffer, 1);
}

int createUDPFail(unsigned char *buffer)
{
    return createFail(buffer, 0);
}

int getPacketSize(unsigned char *buffer)
{
    int i;
    int size = 0;
    unsigned char *p = buffer;
    struct Header *header;

    header = (struct Header *)buffer;

    int queNum = ntohs(header->questionNum);
    int resNum = ntohs(header->answerNum) + ntohs(header->authorityNum) + ntohs(header->additionalNum);

    size += 12;
    p += 12;

    /* Count que */
    for (i = 0; i < queNum; i++)
    {
        size += getNameLen(p);
        size += 4;
        p = p + getNameLen(p) + 4;
    }

    /* Count res */
    for (i = 0; i < resNum; i++)
    {
        size += getNameLen(p);
        size += 10;
        p = p + getNameLen(p) + 8;

        int dataLen = ntohs(*(unsigned short *)p);
        p += 2;
        size += dataLen;
        p += dataLen;
    }

    return size;
}

void getNextServer(struct sockaddr_in *nextServAddr, unsigned char *buffer)
{
    int isFound = 0;
    unsigned char *p = buffer;
    unsigned char tempName[BUFSIZE];
    unsigned char dotName[BUFSIZE];
    unsigned char reader[BUFSIZE];

    p += 12; // Move over header;
    readDotName(dotName, p, buffer);
    p += getNameLen(p);
    unsigned short queryType = ntohs(*(unsigned short *)p);
    p += 2;
    unsigned short queryClass = ntohs(*(unsigned short *)p);
    p += 2;

    /* Compere with DB */
    FILE *fp;

    memset(tempName, 0, BUFSIZE);
    memcpy(tempName, servName, BUFSIZE);

    fp = fopen(strcat(tempName, ".data"), "r");

    /* If not answer, find next server IP */
    unsigned char servIP[BUFSIZE];
    while (fgets(reader, BUFSIZE, fp) != NULL)
    {
        unsigned char name[128], ttl[10], class[10], type[10], data[128];

        sscanf(reader, "%[^,],%[^,],%[^,],%[^,],%s", name, ttl, class, type, data);

        int typeNum = getTypeNum(type);

        /* Found a part of domain name */
        if (strstr(dotName, name) != NULL)
        {
            if (strcmp(type, "A") == 0)
            {
                memcpy(servIP, data, BUFSIZE);
                break;
            }
        }
    }

    memset(nextServAddr, 0, sizeof(struct sockaddr_in)); // Zero out structure
    nextServAddr->sin_family = AF_INET;                  // Internet addr family
    nextServAddr->sin_addr.s_addr = inet_addr(servIP);   // Server IP address
    nextServAddr->sin_port = htons(servPort);            // Server port

    fclose(fp);
}

int main(int argc, char *argv[])
{
    int sock;                        // TCP socket
    int clinSock;                    // Client socket
    struct sockaddr_in servAddr;     // Server address
    struct sockaddr_in lastServAddr; // Last Server address for iteration
    struct sockaddr_in nextServAddr; // Next Server address for recursion
    struct sockaddr_in clinAddr;     // Client address
    unsigned char buffer[BUFSIZE];   // Store Packet
    char servIP[BUFSIZE];

    if (argc != 2)
    {
        printf("Usage: %s <Server Name>\n", argv[0]);
        exit(1);
    }

    servName = argv[1];
    getServIP(servIP);

    if (strcmp(servName, "LocalServer") == 0)
    {
        /* Create a TCP socket */
        if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("socket() failed\n");
            exit(1);
        }
    }
    else
    {
        /* Create a UDP socket */
        if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        {
            printf("socket() failed\n");
            exit(1);
        }
    }

    /* Construct the server address structure */
    memset(&servAddr, 0, sizeof(servAddr));       // Zero out structure
    servAddr.sin_family = AF_INET;                // Internet addr family
    servAddr.sin_addr.s_addr = inet_addr(servIP); // Server IP address
    servAddr.sin_port = htons(servPort);          // Server port

    /* Bind with sock */
    if (bind(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        printf("Bind socket failed\n");
        exit(1);
    }
    printf("Bind socket successfully\n");

    if (strcmp(servName, "LocalServer") == 0)
    {
        /* Listen the sock */
        if (listen(sock, 10))
        {
            printf("Listen socket failed\n");
            exit(1);
        }
        printf("Listen socket...\n");
    }

    if (strcmp(servName, "LocalServer") == 0)
    {
        /* Wait DNS query packet from Client */
        while (1)
        {
            /* Accept Client sock */
            int clinLen = sizeof(clinAddr);
            clinSock = accept(sock, (struct sockaddr *)&clinAddr, &clinLen);
            printf("Accept client %s on TCP port %d\n", inet_ntoa(clinAddr.sin_addr), ntohs(clinAddr.sin_port));

            if (clinSock == -1)
            {
                printf("Accept sock failed\n");
                continue;
            }

            /* Get query packet */
            printf("Recived query packet\n");
            memset(buffer, 0, BUFSIZE);
            if (recv(clinSock, buffer, BUFSIZE, 0) == -1)
            {
                printf("Get query packet failed\n");
                continue;
            }

            /* Check cache */
            printf("Find in database...\n");
            if (checkCache(buffer))
            {
                printf("Send packet to client\n");
                /* There is cache, create DNS response packet */
                int responseLen = createTCPResponse(buffer); // Create DNS response, get response length
                /* Send DNS response to client */
                if (send(clinSock, buffer, responseLen, 0) == -1)
                {
                    printf("Send DNS response failed\n");
                    continue;
                }
            }
            else
            {
                /* There is not cache, send DNS require to other server */
                printf("Ask other name server\n");
                if (queryOthers(buffer))
                {
                    /* Query DNS succseefully */
                    printf("Found data by other name server\n");
                    int responseLen = createTCPResponse(buffer); // Create DNS response, get response length
                    /* Send response to client */
                    if (send(clinSock, buffer, responseLen, 0) == -1)
                    {
                        printf("Send DNS response failed\n");
                        continue;
                    }
                    printf("Send response successfully\n");
                }
                else
                {
                    /* Query DNS fault */
                    printf("No found data\n");
                    int responseLen = createTCPFail(buffer); // Create DNS query failed packet
                    if (send(clinSock, buffer, responseLen, 0) == -1)
                    {
                        printf("Send DNS response failed\n");
                        continue;
                    }
                }
            }
            close(clinSock);
            printf("Query end\n");
        }
    }
    else
    {
        while (1)
        {
            /* Get query packet */
            memset(buffer, 0, BUFSIZE);
            int lastServAddrLen = sizeof(lastServAddr);
            if (recvfrom(sock, buffer, BUFSIZE, 0, (struct sockaddr *)&lastServAddr, &lastServAddrLen) == -1)
            {
                printf("Get query packet failed\n");
                continue;
            }
            printf("Recived query packet\n");

            /* Get rd */
            struct Header *header;
            header = (struct Header *)buffer;

            if (header->rd == 1)
            {
                /* Check database */
                printf("Find in database...\n");
                if (checkCache(buffer) == 0)
                {
                    /* Query DNS fault */
                    printf("No found data\n");
                    int responseLen = createUDPFail(buffer); // Create DNS query failed packet
                    if (sendto(sock, buffer, responseLen, 0, (struct sockaddr *)&lastServAddr, sizeof(lastServAddr)) == -1)
                    {
                        printf("Send DNS response failed\n");
                        continue;
                    }
                }
                else if (checkCache(buffer) == 1)
                {
                    /* Send DNS response to client */
                    getNextServer(&nextServAddr, buffer);
                    printf("Send to %s\n", inet_ntoa(nextServAddr.sin_addr));
                    int nextServAddrLen = sizeof(nextServAddr);
                    if (sendto(sock, buffer, getPacketSize(buffer), 0, (struct sockaddr *)&nextServAddr, nextServAddrLen) == -1)
                    {
                        printf("Send DNS response failed\n");
                        continue;
                    }
                    printf("recive from %s\n", inet_ntoa(nextServAddr.sin_addr));
                    if (recvfrom(sock, buffer, BUFSIZE, 0, (struct sockaddr *)&nextServAddr, &nextServAddrLen) == -1)
                    {
                        printf("Get query packet failed\n");
                        continue;
                    }
                    printf("Send back to %s\n", inet_ntoa(lastServAddr.sin_addr));
                    if (sendto(sock, buffer, getPacketSize(buffer), 0, (struct sockaddr *)&lastServAddr, sizeof(lastServAddr)) == -1)
                    {
                        printf("Send DNS response failed\n");
                        continue;
                    }
                }
                else if (checkCache(buffer) == 2)
                {
                    printf("Send to %s\n", inet_ntoa(lastServAddr.sin_addr));
                    int responseLen = createUDPResponse(buffer);
                    if (sendto(sock, buffer, responseLen, 0, (struct sockaddr *)&lastServAddr, sizeof(lastServAddr)) == -1)
                    {
                        printf("Send DNS response failed\n");
                        continue;
                    }
                }
            }
            else
            {
                /* Check database */
                printf("Find in database...\n");
                if (checkCache(buffer) == 0)
                {
                    /* Query DNS fault */
                    printf("No found data\n");
                    int responseLen = createUDPFail(buffer); // Create DNS query failed packet
                    if (sendto(sock, buffer, responseLen, 0, (struct sockaddr *)&lastServAddr, sizeof(lastServAddr)) == -1)
                    {
                        printf("Send DNS response failed\n");
                        continue;
                    }
                }
                else if (checkCache(buffer) == 2 || checkCache(buffer) == 1)
                {
                    printf("Found it\n");
                    /* There is data, create DNS response packet */
                    int responseLen = createUDPResponse(buffer); // Create DNS response, get response length
                    /* Send DNS response to client */
                    if (sendto(sock, buffer, responseLen, 0, (struct sockaddr *)&lastServAddr, sizeof(lastServAddr)) == -1)
                    {
                        printf("Send DNS response failed\n");
                        continue;
                    }
                }
            }

            printf("Query end\n");
        }
    }

    close(sock);

    return 0;
}