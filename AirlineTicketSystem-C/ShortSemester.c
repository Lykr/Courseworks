/******************************************************************************
Copyright @2016
FileName: ShortSemester.cpp
Author: DingYang, LiuZe, GuoYuhang, WangChen
Version : 1.0
Date: 2016-09-06
Description: ShortSemester
*******************************************************************************
Function List:

void select_way();                                                                  //choose Module
==============================Registration Module==============================
void registration();                                                                //user registration
char *register_id();                                                                //query user enter and return a point of id
char *register_password();                                                          //query user enter and return a point of password
char *register_sex();                                                               //query user enter and return a point of sex
char *register_name();                                                              //query user enter and return a point of name
char *register_email();                                                             //query user enter and return a point of email
char *register_phone();                                                             //query user enter and return a point of phone
=================================Login Module==================================
void login(int);                                                                    //login as user or admin
=================================User Function=================================
void user_option();                                                                 //user interface
-------------------------------------------------------------------------------
void user_information();                                                            //manage user's personal information
-------------------------------------------------------------------------------
void user_flight_information(struct order_information order[]);                     //This part is for user to search flight information
int ifagain(int);                                                                   //if the user began the search again
int section2search_1(struct plane,struct plane [],struct plane [], int,int,int);    //search by place and time-part 1(section2)
int section2search_2(struct plane,struct plane [],struct plane [],int,int,int );    //search by place and time-part 1(section2)
void printform(struct plane [], int);                                               //print one line of the char
void section1();                                                                    //main body of searching in flight number and time
void section2();                                                                    //main body of searching in place and time
void section1get(struct plane []);                                                  //get the demand of user(section1)
void section1search(struct plane []);                                               //searching(section1)
void section2filter_1(struct plane [], int, int);                                   //filter in time(section2)
void section2filter_2(struct plane [], int);                                        //filter in Airline company(section2)
void section2filter_3(struct plane [], int);                                        //filter in remain cabin type(section2)
void section2filter_4(struct plane [], int);                                        //filter in type of the plane(section2)
void section2filter_5(struct plane [], int);                                        //filter in accuracy(section2)
void section2filter_6(struct plane [], struct plane, int);                          //rearrange by time(section2)
void section2filter_7(struct plane [], struct plane, int);                          //rearrange by price(section2)
void section2filter_8(struct plane [], struct plane, int);                          //let system recommend(section2)
-------------------------------------------------------------------------------
void user_buy(struct order_information [], int, int);                               //book ticket interface
int user_if_jar(struct order_information [], int);                                  //whether the conflict between the two flights
void user_buy_search(struct order_information [], int, int);                        //search the ticket user want to buy
void user_buy_ticket(struct order_information [], int [], int, int);                //buy ticket
-------------------------------------------------------------------------------
void user_order();                                                                  //user order interface
void user_make_order(struct order_information [], int, int);                        //write order information into txt
void user_order_cancel(int);                                                        //cancel order
void user_order_pay(int);                                                           //pay order
void user_order_operate();                                                          //operate order, pay or cancel
=================================Admin Function================================
void admin_option();                                                                //admin interface
-------------------------------------------------------------------------------
void admin_information();                                                           //manage admin's personal information
-------------------------------------------------------------------------------
void admin_order_list(int company_num);                                             //query order information which belong the company
-------------------------------------------------------------------------------
void admin_flight_management(int company_num);                                      //manage flight
-------------------------------------------------------------------------------
void admin_flight_information(int company_num);                                     //query flight information
-------------------------------------------------------------------------------
void admin_order_statistics(int company_num);                                       //main part of the order list for admin
void print_order_1(struct order_information [],int,char []);                        //print all the order depend on the number of order
void print_order_2(struct order_information [],struct plane [],int,int,char []);    //print all the order depend on board rate
void print_order_3(struct order_information [],struct plane [],int,int,char []);    //print all the order in  each flight's income
-------------------------------------------------------------------------------
int row();                                                                          //tell the program how many flight are there
int row2();                                                                         //tell the program how many order are there
int match_id(int);                                                                  //match id and password
int check_email(char []);                                                           //check the format of email
int check_flight_num(char []);                                                      //check the format of flight num
int check_id(char []);                                                              //check the format of id
int check_phone(char []);                                                           //check the format of phone
int check_date(char []);                                                            //check the format of date
int check_order_no(char []);                                                        //match id and order No.
int get_order_no();                                                                 //get the last one order No.
int ticket_can_buy();                                                               //get the number of tickets can buy
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

void select_way();
void registration();
char *register_id();
char *register_password();
char *register_sex();
char *register_name();
char *register_email();
char *register_phone();
void login(int);
void user_option();
void user_information();
void user_flight_information(struct order_information order[]);
int ifagain(int);
int section2search_1(struct plane,struct plane [],struct plane [], int,int,int);
int section2search_2(struct plane,struct plane [],struct plane [],int,int,int );
void printform(struct plane [], int);
void section1();
void section2();
void section1get(struct plane []);
void section1search(struct plane []);
void section2filter_1(struct plane [], int, int);
void section2filter_2(struct plane [], int);
void section2filter_3(struct plane [], int);
void section2filter_4(struct plane [], int);
void section2filter_5(struct plane [], int);
void section2filter_6(struct plane [], struct plane, int);
void section2filter_7(struct plane [], struct plane, int);
void section2filter_8(struct plane [], struct plane, int);
void user_buy(struct order_information [], int, int);
int user_if_jar(struct order_information [], int);
void user_buy_search(struct order_information [], int, int);
void user_buy_ticket(struct order_information [], int [], int, int);
void user_order();
void user_make_order(struct order_information [], int, int);
void user_order_cancel(int);
void user_order_pay(int);
void user_order_operate();
void admin_option();
void admin_information();
void admin_order_list(int company_num);
void admin_flight_management(int company_num);
void admin_flight_information(int company_num);
void admin_order_statistics(int company_num);
void print_order_1(struct order_information [],int,char []);
void print_order_2(struct order_information [],struct plane [],int,int,char []);
void print_order_3(struct order_information [],struct plane [],int,int,char []);
int row();
int row2();
int match_id(int);
int check_email(char []);
int check_flight_num(char []);
int check_id(char []);
int check_phone(char []);
int check_date(char []);
int check_order_no(char []);
int get_order_no();
int ticket_can_buy();

struct place
{
	char off[15];
	char land[15];
};

struct plane
{
    char pl[7];        //flight number
    char off[16];        //place of take off
    char land[16];        //place of landing
    int th;        //take off time-hour
    int tm;        //take off tome-minute
    int fh;        //flying time-hour
    int fm;        //flying time-minute
    char ty[5];        //plane type
    int ac;        //accuracy
    int pr1;        //F class price
    int pr2;        //C class price
    int pr3;        //Y class price
    int group;        //group number of the flier
    int left1;        //F class total seat
    int left2;        //C class total seat
    int left3;        //Y class total seat
    int type;        //weather the plane is leaving or arriving
};

struct flight_information
{
    char flight[7];
    char departure[16];
    char arrival[16];
    char time_take[6];
    char time_fly[6];
    char model[5];
    int rate;
    int money1;
    int money2;
    int money3;
    char crew[3];
    int seat1;
    int seat2;
    int seat3;
};

struct order_information
{
    int order_no;
    char date[11];        //time of the flight
    char flight[7];        //flight number
    int ticket_type;        //cabin type of the order
    int ticket_num;        //how many ticket he/she bought
};

struct money
{
    char date[11];        //time of the flight
    char flight[7];        //flight number
    int num1;        //order number of F class
    int num2;        //order number of C class
    int num3;        //order number of Y class
    int mon1;        //price of F class per ticket
    int mon2;        //price of C class per ticket
    int mon3;        //price of Y class per ticket
    int toincome;        //total income of the flight
};

char id[11];

int main()
{
	select_way();

	return 0;
}

void select_way()
{
	char option[2]; //store user selection

	printf("Welcome to E-Flight Booking system.\n\nPlease choose the way you login.\n1. User\n2. Admin\n3. User registration\n0. Exit\n\n");

	do
	{
		fflush(stdin);
		gets(option);
		system("cls");

		if (strcmp(option, "1") != 0 && strcmp(option, "2") != 0 && strcmp(option, "3") != 0 && strcmp(option, "0") != 0)
		{
			printf("Welcome to E-Flight Booking system.\n\nYou should choose the way by enter 1, 2, 3 or 0, please enter again."
                    "\n1. User\n2. Admin\n3. User registration\n0. Exit\n\n");
            continue;
		}
		else
		{
		    break;
		}
	}
	while (1);

	system("cls");

	if (strcmp(option, "0") == 0)
	{
		printf("Exit Success\n\n");
		exit(0);
	}
	else if (strcmp(option, "3") == 0)
	{
		registration();
	}
	else
	{
		login(atoi(option)); //turn string-o to int-o
	}
}

void registration()
{
	char *id_enter, *name_enter, *email_enter, *password_enter, *sex_enter, *phone_enter; //the data enterd by user back as point
	FILE *outFile;

	/* Query Data */
	id_enter = register_id();
	password_enter = register_password();
	name_enter = register_name();
	sex_enter = register_sex();
	email_enter = register_email();
	phone_enter = register_phone();

	/* write data into User.txt */
	outFile = fopen("User.txt", "a");
	fprintf(outFile, "%-10s%-10s%-20s%-10s%-15s%-30s\n", id_enter, password_enter, name_enter, sex_enter, phone_enter, email_enter);
	fclose(outFile);

    /* Release memory */
	free(id_enter);
	free(password_enter);
	free(name_enter);
	free(sex_enter);
	free(email_enter);
	free(phone_enter);

	printf("Register Success.\n\n");

	system("pause");
	system("cls");

	select_way(); //back login page
}

char *register_id()
{
	char *id_enter = (char*)malloc(sizeof(char)*11); //the data entered by user
	char user[11]; //the data read from database
	FILE *inFile;

	printf("Please enter the ID you want.\n(The ID should have 6-10 characters, and be consist of letter and number)\n(Enter 0 to cancel register)\n\n");

	do
	{
		fflush(stdin);
		gets(id_enter);
		system("cls");

		if (strcmp(id_enter, "0") == 0)
		{
			select_way();
		}
	}
	while(check_id(id_enter));

	inFile = fopen("User.txt", "r");
	while(!feof(inFile))
	{

		fscanf(inFile, "%s%*[^\n]\n", user);

		if (strcmp(id_enter, user) == 0)
		{
			printf("This ID has been registered.\n\n");
			system("pause");
			system("cls");
			registration();
		}
	}
	fclose(inFile);

	return id_enter;
}

char *register_password()
{
	char *password_enter = (char*)malloc(sizeof(char)*11);
	char password_reenter[11];

	printf("Please enter the password of your ID.\n(The password should have 6-10 characters)\n(Enter 0 to cancel register)\n\n");

	do
	{
		fflush(stdin);
		gets(password_enter);
		system("cls");

		if (strcmp(password_enter, "0") == 0)
		{
			select_way();
		}
		else if (strlen(password_enter) < 6 || strlen(password_enter) > 10)
		{
			printf("The password should have 6-10 characters, please enter again.\n(Enter 0 to cancel register)\n\n");
			continue;
		}
		else
		{
			printf("Please re-enter the password to confirm.\n(Enter 0 to cancel register)\n\n");

			fflush(stdin);
			gets(password_reenter);
			system("cls");

			if (strcmp(password_reenter, "0") == 0)
			{
				select_way();
			}
			else if (strcmp(password_reenter, password_enter) != 0)
			{
				printf("The password do not match, please enter again.\n(Enter 0 to cancel register)\n\n");
				continue;
			}
			else
            {
                break;
            }
		}
	}
	while(strlen(password_enter) < 6 || strlen(password_enter) > 10 || strcmp(password_reenter, password_enter) != 0);

	return password_enter;
}

char *register_sex()
{
	char *sex_enter = (char*)malloc(sizeof(char)*7);

	printf("Please choose your Sex.\n1. Male\n2. Female\n\n(Enter 0 to cancel register)\n\n");

	do
	{
		fflush(stdin);
		gets(sex_enter);
		system("cls");

		if (strcmp(sex_enter, "0") == 0)
		{
			select_way();
		}
		else if (strcmp(sex_enter, "1") != 0 && strcmp(sex_enter, "2") != 0)
		{
			printf("Please choose your Sex by enter 1 or 2.\n1. Male\n2. Female\n\n(Enter 0 to cancel register)\n\n");
			continue;
		}
		else
		{
			break;
		}
	}
	while(1);

	if (strcmp(sex_enter, "1") == 0)
	{
		strcpy(sex_enter, "Male");
	}
	else
	{
		strcpy(sex_enter, "Female");
	}

	return sex_enter;
}

char *register_name()
{
	char *name_enter = (char*)malloc(sizeof(char)*21);

	printf("Please enter your Name.\n(Enter 0 to cancel register)\n\n");

	do
	{
		fflush(stdin);
		gets(name_enter);
		system("cls");

		if (strcmp(name_enter, "0") == 0)
		{
			select_way();
		}
		else if (strlen(name_enter) > 20)
		{
			printf("Your name should under 20 characters, please enter again.\n(Enter 0 to cancel register)\n\n");
			continue;
		}
		else
		{
			break;
		}
	}
	while(1);

	return name_enter;
}

char *register_email()
{
	char *email_enter = (char*)malloc(sizeof(char)*31);

	printf("Please enter your Email.\n(Enter 0 to cancel register)\n\n");

	do
	{
		fflush(stdin);
		gets(email_enter);
		system("cls");

		if (strcmp(email_enter, "0") == 0)
		{
			select_way();
		}
	}
	while(check_email(email_enter));

	return email_enter;
}

char *register_phone()
{
	char *phone_enter = (char*)malloc(sizeof(char)*12);

	printf("Please enter your phone number.\n(Enter 0 to cancel register)\n\n");

	do
	{
		fflush(stdin);
		gets(phone_enter);
		system("cls");

		if (strcmp(phone_enter, "0") == 0)
		{
			select_way();
		}
	}
	while(check_phone(phone_enter));

	return phone_enter;
}

void login(int option)
{
	while(1)
	{
		printf("Please enter your ID.\n(Enter 0 to reselect login way)\n\n");

		do
		{
			fflush(stdin);
			gets(id);
			system("cls");

			if (strcmp(id, "0") == 0)
			{
				select_way();
			}
		}
		while(check_id(id));

		if (match_id(option))
		{
			system("cls");
			printf("Login Success\n\n");
			system("pause");
			system("cls");

			switch (option)
			{
					case 1:
					user_option();
					break;
				case 2:
					admin_option();
					break;
			default:
					break;
			}

			system("cls");
		}
		else
		{
			system("cls");
			printf("ID or passowrd is wrong, please enter again.\n\n");
			system("pause");
			system("cls");
			continue;
		}
	}
}

int match_id(int option)
{
	char password_enter[11]; //the password enter by admin
	char ID[11], password[11]; //information from admin.txt
	FILE *inFile;

	/* read information from the specified document*/
	switch (option)
	{
		case 1:
			inFile = fopen("User.txt", "r");
			break;
		case 2:
			inFile = fopen("Admin.txt", "r");
			break;
		default:
			break;
	}

	if (inFile == NULL)
	{
		printf("A required database is missing, please check the file of the program.\n\n");
		system("pause");
		exit(0);
	}
	else
	{
		printf("Please enter the password.\n(Enter 0 to reselect login way)\n\n");

		fflush(stdin);
		gets(password_enter);

		if (strcmp(password_enter, "0") == 0)
		{
			system("cls");
			select_way();
		}

		while (!feof(inFile))
		{
            fscanf(inFile, "%s", ID); //read id and password from txt

			if (strcmp(ID, id) == 0 )
			{
			    {
			        fscanf(inFile, "%s", password);

			        if (strcmp(password_enter, password) == 0)
                    {
                        return 1;
                    }
                    else
                    {
                        fscanf(inFile, "%*[^\n]%*c");
                        continue;
                    }
			    }
			}
			else
			{
			    fscanf(inFile, "%*[^\n]%*c");
				continue;
			}
		}
		return 0;
	}

	fclose(inFile);
}

void user_option()
{
    struct order_information order[5];
	char option[2]; //store user selection
	int order_flight_num = 0, order_ticket_num = 0;

	printf("Please choose the item which you need.\n\n1. Manage personal information\n"
           "2. Query flight information\n3. Book airline ticket\n4. Manage orders\n0. Logout\n\n");

	/* input validation */
	do
	{
		fflush(stdin);
		gets(option);
		system("cls");

		if (!(strcmp(option, "1") == 0 || strcmp(option, "2") == 0 ||
		strcmp(option, "3") == 0 || strcmp(option, "4") == 0 ||
		strcmp(option, "0") == 0))
		{
		printf("Please choose the item which you need.\n\n1. Manage personal information\n"
			"2. Query flight information\n3. Book airline ticket\n4. Manage orders\n0. Logout\n\n"
            "You should choose the way by enter 1, 2, 3, 4 or 0, please enter again.\n\n");
		continue;
		}
		else
		{
		break;
		}
	}
	while (1);

	system("cls");

	switch(atoi(option)) //turn string-option to int-option
	{
		case 1:
			user_information();
			break;
		case 2:
			user_flight_information(order);
			break;
		case 3:
		    order[order_flight_num].order_no = get_order_no() + 1;
			user_buy(order, order_flight_num, order_ticket_num);
			break;
		case 4:
			user_order();
			break;
		case 0:
			select_way();
		default:
			break;
	}
}

void user_information()
{
    int row=0;
    int i;
    char option[2];
    char user[11], password[11], name[21], sex[7], phone[12], email[31];
    char password_enter[11], email_enter[31], phone_enter[12];
    FILE *inFile, *outFile;

    inFile = fopen("User.txt", "r");
    while(!feof(inFile))
    {
        fscanf(inFile, "%s", user);
        row++;

        if (strcmp(user, id) == 0)
        {
            fscanf(inFile, "%s%s%s%s%s", password, name, sex, phone, email);
            break;
        }
        else
        {
            fscanf(inFile, "%*[^\n]%*c");
            continue;
        }
    }
    fclose(inFile);

    printf("|    ID    |        Name        |   Sex   |    Phone    |            Email             |\n"
           "|----------|--------------------|---------|-------------|------------------------------|\n"
           "|%10s|%20s|%8s |%12s |%30s|\n\n", user, name, sex, phone, email);
    printf("Please select the information you want to change: \n"
           "1. Change my email\n2. Change my password\n3. Change my phone number\n0. Return menu\n\n");

    do
    {
        fflush(stdin);
        gets(option);
        system("cls");

        if (strcmp(option, "1") != 0 && strcmp(option, "2") != 0 && strcmp(option, "3") != 0 && strcmp(option, "0") != 0)
        {
            printf("|    ID    |        Name        |   Sex   |    Phone    |            Email             |\n"
                   "|----------|--------------------|---------|-------------|------------------------------|\n"
                   "|%10s|%20s|%8s |%12s |%30s|\n\n", user, name, sex, phone, email);
            printf("You should enter 1, 2, 3 or 0, please enter again: \n"
                   "1. Change my email\n2. Change my password\n3. Change my phone number\n0. Return menu\n\n");
        }
        else
        {
            break;
        }
    }while(1);

    outFile = fopen("User.txt", "r+");
    switch(atoi(option))
    {
        case 1:
            printf("Please enter the email address you want to change: \n(Enter 0 to return)\n\n");

            do
            {
                fflush(stdin);
                gets(email_enter);
                system("cls");

                if (strcmp(email_enter, "0") == 0)
                {
                    user_information();
                }
            }
            while(check_email(email_enter));

            fseek(outFile, 1L*((row-1)*97)+65, 0);
            fprintf(outFile, "%-30s", email_enter);
            printf("Change successfully\n\n");
            system("pause");
            break;

        case 2:
            printf("Please enter the password you want to change: \n(Enter 0 to return)\n\n");

            do
            {
                fflush(stdin);
                gets(password_enter);
                system("cls");

                if (strcmp(password_enter, "0") == 0)
                {
                    user_information();
                }
                else if (strlen(password_enter) > 10 || strlen(password_enter) < 6 )
                {
                    printf("Password should have 6-10 characters, please enter again.\n(Enter 0 to return)\n\n");
                    continue;
                }
                else
                {
                    break;
                }

            }
            while(1);

            fseek(outFile, 1L*((row-1)*97)+10, 0);
            fprintf(outFile, "%-10s", password_enter);
            printf("Change successfully\n\n");
            system("pause");
            break;

        case 3:
            printf("Please enter the phone number you want to change: \n(Enter 0 to return)\n\n");
            do
            {
                fflush(stdin);
                gets(phone_enter);
                system("cls");

                if (strcmp(phone_enter, "0") == 0)
                {
                    user_information();
                }
            }
            while(check_phone(phone_enter));

            fseek(outFile, 1L*((row-1)*97)+50, 0);
            fprintf(outFile, "%-10s", phone_enter);
            printf("Change successfully\n\n");
            system("pause");
            system("cls");
            break;

        case 0:
            user_option();
            break;
     }
     fclose(outFile);

     user_information();
}

void user_flight_information(struct order_information order[])
{
    int order_flight_num=0;
    int order_ticket_num=0;
    int z=0;
    char s[11]={0};

    printf("Welcome to flight checking system.\n");     //"welcome" part
    do
    {
        printf("-------------------------------------------------------------------\n"      //choose the searching way you want
               "Please choose the way you'd like to search for the flight you need:\n"
               "(Enter '1' and '2' to choose,'0' to exit the checking system)\n"
               "1.Search by flight number and time you'd like plane to take off;\n"
               "2.Search by the place you'd like to take off,\n"
               "  to land and time you'd like plane to take off;\n");
        fflush(stdin);
        scanf("%d",&z);
        switch(z)
        {
        case 1:
            system("cls");
            section1();     //searching way NO.1
            z=ifagain(z);       //weather to start a new search
            break;
        case 2:
            system("cls");
            section2();     //searching way NO.2
            z=ifagain(z);       //weather to start a new search
            break;
        case 0:
            z=1;        //straight exit the program
            break;
        default:
            printf("You have enter the wrong character.please try again,or you can enter '0' to exit the checking system.\n");      //try again
            z=0;
        }
    }while(z==0);



    user_buy(order, order_flight_num, order_ticket_num);
}

int ifagain(int a)
{
    int x;

    printf("Do you want to  go look for another flight or exit the checking system?\n");
    printf("(enter '1' for look more and other numbers for exit.)\n");
    fflush(stdin);
    scanf("%d",&x);
    if(x==1)        //weather to exit or not
    {
        a=0;
    }else
    {
        a=1;
    }

    return a;
}

void printform(struct plane fl[],int m)
{
    printf("|-------------------------------------------------------------------------|\n"      //all 16 informations include
           "|      |%-12s|        |      |     |        |F:%-5d|       |F:%-4d|\n"
           "|%s|to          |%2d:%-2d   |%dh%dm |%s |%d%%     |C:%-5d|NO.%-4d|C:%-4d|\n"
           "|      |%-12s|        |      |     |        |Y:%-5d|       |Y:%-4d|\n",
           fl[m].off,fl[m].pr1,fl[m].left1,fl[m].pl,fl[m].th,fl[m].tm,fl[m].fh,fl[m].fm,fl[m].ty,
           fl[m].ac,fl[m].pr2,fl[m].group,fl[m].left2,fl[m].land,fl[m].pr3,fl[m].left3);
}

void section1()
{
    int x=row();        //calculate the total number of the flight the system have

    struct plane fl[x];     //create the struct to store all flight information

    do
    {
        printf("Please enter the flight number:\n"      //get the flight number
               "(two letter in cap and four number.enter a string start with '0' to exit.)\n");
        fflush(stdin);
        scanf("%s",fl[1].pl);
        printf("\n");
    }while(check_flight_num(fl[1].pl));     //check

    section1search(fl);     //begin the search

}

void section1search(struct plane fl[])
{

    FILE *rfile;
    int a=1,b,c,o;
    char blank[7];

    while(a!=3)     //get the time range
    {
        printf("Please choose the time range of the flight taking off:\n"
               "(enter  to choose the range, enter other number to exit)\n"
               "1. 00:00-6:00\n2. 6:00-12:00\n3. 12:00-18:00\n4. 18:00-24:00\n");
        fflush(stdin);
        scanf("%d",&b);
        if((b>0)&&(b<5))
        {
            system("cls");
            printf("The information of the flight that suit your demands is/are: \n"        //begin printing the char
                   "(If no flight are shown, we are sorry that no flight suit your demands.)\n\n"
                   "|Flight|Take Off and|Take Off|total |Plane|Accuracy| Prise |Aircrew|Cabin |\n"
                   "|Number|  Land In   | Time   |time  |Model|        | (RMB) |  NO.  | Type |\n");


        rfile=fopen("flight.txt","r");
        while((fscanf(rfile,        //searching for the flight that suit user's need
                      "%s\t%s%s%d:%d\t%d:%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
                      &fl[0].pl,&fl[0].off,&fl[0].land,&fl[0].th,&fl[0].tm,&fl[0].fh,&fl[0].fm,
                      &fl[0].ty,&fl[0].ac,&fl[0].pr1,&fl[0].pr2,&fl[0].pr3,&fl[0].group,
                      &fl[0].left1,&fl[0].left2,&fl[0].left3,&o)!=EOF))
        {
            if((strncmp(fl[1].pl,fl[0].pl,7)==0)
               &&((fl[0].th*60+fl[0].tm)>=(b-1)*360)
               &&((fl[0].th*60+fl[0].tm)<=b*360))
            {
                printform(fl,0);
            }
        }
        fclose(rfile);
        printf("\n\n");
        }else
        {
            a=3;
        }
    }
}

void section2()
{
    int x=row();
    struct plane fl[x],fl2[x],fl0,flr;
    FILE *wfile,*rfile;
    char blank[16]={'='};
    int a=1,b=5,c,d,num0=0,num1=0,o;

    do
    {
        num0=0;
        a=0;
        printf("Please enter where you need your plane to take off:\n"      //get the place of taking off
               "(In China only,begin with '0' to skip and other numbers to exit)\n");
        fflush(stdin);
        scanf("%s",&fl0.off);
        if((fl0.off[0]>='1')&&fl0.off[0]<='9')
        {
            break;
        }
        printf("Please enter where you need your plane to land\n"       //get the place of landing
               "(In China only,begin with '0' to skip and other numbers to exit)\n");
        fflush(stdin);
        scanf("%s",&fl0.land);
        if((fl0.land[0]>='1')&&fl0.land[0]<='9')
        {
            break;
        }
        printf("Please choose the time range of the flight taking off:\n"       //get the time range
               "(enter  to choose the range, enter '0' to skip and other numbers to exit)\n"
               "1. 00:00-6:00\n2. 6:00-12:00\n3. 12:00-18:00\n4. 18:00-24:00\n");
        fflush(stdin);
        scanf("%d",&b);
        if((b<0)||(b>4))
        {
            break;
        }
        rfile=fopen("flight.txt","r");
        while((fscanf(rfile,
                      "%s\t%s%s%d:%d\t%d:%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
                      &fl[num0].pl,&fl[num0].off,&fl[num0].land,&fl[num0].th,&fl[num0].tm,&fl[num0].fh,&fl[num0].fm,
                      &fl[num0].ty,&fl[num0].ac,&fl[num0].pr1,&fl[num0].pr2,&fl[num0].pr3,&fl[num0].group,
                      &fl[num0].left1,&fl[num0].left2,&fl[num0].left3,&o)!=EOF))
        {
            num0++;
        }
        fclose(rfile);

        printf("The information of the flight that suit your demands is/are: \n"        //begin printing the char
                "(If no flight are shown, we are sorry that no flight suit your demands.)\n\n"
                "|Flight|Take Off and|Take Off|total |Plane|Accuracy| Prise |Aircrew|Cabin |\n"
                "|Number|  Land In   | Time   |time  |Model|        | (RMB) |  NO.  | Type |\n");

        for(c=0;c<x;c++)
        {
            num1=section2search_1(fl0,fl,fl2,c,b,num1);     //search by place and time(section2)
        }

        printf("Would you like to filter or rearrange the flight above in below ways?\n"        //provide 8 ways of filtering (or rearranging) the list
               "(Please enter the number you want. Enter '0' to went back and\n "
               "search for new flight, other numbers to exit)\n"
               "1.Time Range\n2.Airline Company\n3.Cabin Type\n4.Type of the plane\n5.Level of Accuracy\n"
               "6.Rearrange by Time (the earliest on the top)\n7.Rearrange by economy class's price\n"
               "8.let us recommend you some fight\n");
        fflush(stdin);
        scanf("%d",&d);
        switch(d)
        {
        case 0:
            break;
        case 1:
            section2filter_1(fl2,b,num1);
            break;
        case 2:
            section2filter_2(fl2,num1);
            break;
        case 3:
            section2filter_3(fl2,num1);
            break;
        case 4:
            section2filter_4(fl2,num1);
            break;
        case 5:
            section2filter_5(fl2,num1);
            break;
        case 6:
            section2filter_6(fl2,flr,num1);
            break;
        case 7:
            section2filter_7(fl2,flr,num1);
            break;
        case 8:
            section2filter_8(fl2,flr,num1);
            break;
        }
    }while(a!=3);
}

int section2search_1(struct plane fl0,struct plane fl[],struct plane fl2[],int c,int b,int num1)
{
    if(fl0.off[0]=='0')
    {
        if(fl0.land[0]=='0')
        {
            if(b==0)
            {
                printform(fl,c);
                fl2[num1]=fl[c];
                num1++;
            }else
            if(((fl[c].th*60+fl[c].tm)>=(b-1)*360)&&((fl[c].th*60+fl[c].tm)<=b*360))
            {
                printform(fl,c);
                fl2[num1]=fl[c];
                num1++;
            }
        }else
        if(strncmp(fl[c].land,fl0.land,16)==0)
        {
            if(b==0)
            {
                printform(fl,c);
                fl2[num1]=fl[c];
                num1++;
            }else
            if(((fl[c].th*60+fl[c].tm)>=(b-1)*360)&&((fl[c].th*60+fl[c].tm)<=b*360))
            {
                printform(fl,c);
                fl2[num1]=fl[c];
                num1++;
            }
        }
    }else
    {
        num1=section2search_2(fl0,fl,fl2,c,b,num1);
    }
    return (num1);
}

int section2search_2(struct plane fl0,struct plane fl[],struct plane fl2[],int c,int b,int num1)
{
    if((strncmp(fl[c].off,fl0.off,16)==0))
    {
        if(fl0.land[0]=='0')
        {
            if(b==0)
            {
                printform(fl,c);
                fl2[num1]=fl[c];
                num1++;
            }else
            if(((fl[c].th*60+fl[c].tm)>=(b-1)*360)&&((fl[c].th*60+fl[c].tm)<=b*360))
            {
                printform(fl,c);
                fl2[num1]=fl[c];
                num1++;
            }
        }else
        if(strncmp(fl[c].land,fl0.land,16)==0)
        {
            if(b==0)
            {
                printform(fl,c);
                fl2[num1]=fl[c];
                num1++;
            }else
            if(((fl[c].th*60+fl[c].tm)>=(b-1)*360)&&((fl[c].th*60+fl[c].tm)<=b*360))
            {
                printform(fl,c);
                fl2[num1]=fl[c];
                num1++;
            }
        }
    }
    return (num1);
}

void section2filter_1(struct plane fl2[],int b,int num1)
{
    int n=0,e=0;
    while(n==0)
    {
        fflush(stdin);
        if(b!=0)        //if user have choose a time range before
        {
            printf("you have already choose one time range before,"
                   "system will delete the search conditions to let you start a new search\n");
            break;
        }else       //choose the new time range
        {
            printf("Please choose the time range of the flight taking off:\n"
                   "(enter  to choose the range, enter other numbers to exit)\n"
                   "1. 00:00-6:00\n2. 6:00-12:00\n3. 12:00-18:00\n4. 18:00-24:00\n");
            fflush(stdin);
            scanf("%d",&b);
            if((b>4)||(b<1))
            {
                printf("Restart searching system.\n");
                break;
            }else
            {
                printf("The information of the flight that suit your demands is/are: \n"
                       "(If no flight are shown, we are sorry that no flight suit your demands.)\n\n"
                       "|Flight|Take Off and|Take Off|total |Plane|Accuracy| Prise |Aircrew|Cabin |\n"
                       "|Number|  Land In   | Time   |time  |Model|        | (RMB) |  NO.  | Type |\n");
                for(e=0;e<num1;e++)
                {
                    if(((fl2[e].th*60+fl2[e].tm)>=(b-1)*360)&&((fl2[e].th*60+fl2[e].tm)<=b*360))
                    {
                        printform(fl2,e);
                    }
                }
                printf("If you need more searching,you can now search again below,\n"
                       " or if there is nothing you need,you can leave the program.\n\n\n\n");
                break;
            }
        }
    }
}

void section2filter_2(struct plane fl2[],int num1)
{
    int e;
    char blank[3];

    printf("Please choose the Airline code you want:\n"     //get the Airline
           "(Eg:CA for Air China)\n");
    scanf("%s",&blank);
    printf("The information of the flight that suit your demands is/are: \n"        //begin the searching
           "(If no flight are shown, we are sorry that no flight suit your demands.)\n\n"
           "|Flight|Take Off and|Take Off|total |Plane|Accuracy| Prise |Aircrew|Cabin |\n"
           "|Number|  Land In   | Time   |time  |Model|        | (RMB) |  NO.  | Type |\n");
    for(e=0;e<num1;e++)
    {
        if(strncmp(fl2[e].pl,blank,2)==0)
        {
           printform(fl2,e);
        }
    }
    printf("If you need more searching,you can now search again below,\n"
           " or if there is nothing you need,you can leave the program.\n\n\n\n");
}

void section2filter_3(struct plane fl2[],int num1)
{
    int e;
    char blank[2];

    printf("Please type in the code of cabin type you want:\n"      //get the cabin type user need
                   "(Enter 'F' for first class,'C' for business class,'Y' for economy class)\n");
            fflush(stdin);
            scanf("%s",&blank);
            printf("The information of the flight that suit your demands is/are: \n"
                   "(If no flight are shown, we are sorry that no flight suit your demands.)\n\n"
                   "|Flight|Take Off and|Take Off|total |Plane|Accuracy| Prise |Aircrew|Cabin |\n"
                   "|Number|  Land In   | Time   |time  |Model|        | (RMB) |  NO.  | Type |\n");
            if(blank[0]=='F')       //begin the search
            {
                for(e=0;e<num1;e++)
                {
                    if(fl2[e].left1!=0)
                    {
                        printform(fl2,e);
                    }
                }
            }else
            if(blank[0]=='C')
            {
                for(e=0;e<num1;e++)
                {
                    if(fl2[e].left2!=0)
                    {
                        printform(fl2,e);
                    }
                }
            }else
            if(blank[0]=='Y')
            {
                for(e=0;e<num1;e++)
                {
                    if(fl2[e].left3!=0)
                    {
                        printform(fl2,e);
                    }
                }
            }else
            {
                printf("Wrong cabin code. Search reset.\n");
            }
            printf("If you need more searching,you can now search again below,\n"
                   " or if there is nothing you need,you can leave the program.\n\n\n\n");
}

void section2filter_4(struct plane fl2[],int num1)
{
    int e;
    char blank[5];

    printf("Please enter the plane type you want\n"     //get the type of the plane
           "(One cap letter with three number,eg:A380)\n");
    fflush(stdin);
    scanf("%s",&blank);
    printf("The information of the flight that suit your demands is/are: \n"
           "(If no flight are shown, we are sorry that no flight suit your demands.)\n\n"
           "|Flight|Take Off and|Take Off|total |Plane|Accuracy| Prise |Aircrew|Cabin |\n"
           "|Number|  Land In   | Time   |time  |Model|        | (RMB) |  NO.  | Type |\n");
    for(e=0;e<num1;e++)
    {
        if(strncmp(fl2[e].ty,blank,4)==0)
        {
            printform(fl2,e);
        }
    }
    printf("If you need more searching,you can now search again below,\n"
           " or if there is nothing you need,you can leave the program.\n\n\n\n");
}

void section2filter_5(struct plane fl2[],int num1)
{
    int e,num3,n=0;
    while(n==0)
    {
        printf("Please enter the lowest accuracy you prefer:\n"     //get the lowest accuracy user prefer
               "(Enter the number from 1 to 100, enter  other number to exit)\n");
        fflush(stdin);
        scanf("%d",&num3);
        if((num3<=100)||(num3>=0))
        {
            printf("The information of the flight that suit your demands is/are: \n"
                   "(If no flight are shown, we are sorry that no flight suit your demands.)\n\n"
                   "|Flight|Take Off and|Take Off|total |Plane|Accuracy| Prise |Aircrew|Cabin |\n"
                   "|Number|  Land In   | Time   |time  |Model|        | (RMB) |  NO.  | Type |\n");
            for(e=0;e<num1;e++)
            {
                if(fl2[e].ac>=num3)
                {
                    printform(fl2,e);
                }
            }
        }else
        {
            printf("Restart searching system.");
            break;
        }
        printf("If you need more searching,you can now search again below,\n"
               " or if there is nothing you need,you can leave the program.\n\n\n\n");
        break;
    }
}

void section2filter_6(struct plane fl2[],struct plane flr,int num1)
{
    int e,e2,f;

    printf("The information of the flight that suit your demands is/are: \n"
           "(If no flight are shown, we are sorry that no flight suit your demands.)\n\n"
           "|Flight|Take Off and|Take Off|total |Plane|Accuracy| Prise |Aircrew|Cabin |\n"
           "|Number|  Land In   | Time   |time  |Model|        | (RMB) |  NO.  | Type |\n");
    for(e=0;e<num1;e++)     //begin filtering
    {
        flr.th=24;
        e2=0;
        for(f=0;f<num1;f++)
        {
            if((flr.th*60+flr.tm)>(fl2[f].th*60+fl2[f].tm))
            {
                flr.th=fl2[f].th;
                flr.tm=fl2[f].tm;
                e2=f;
            }
        }
        printform(fl2,e2);
        fl2[e2].th=24;
    }
    printf("If you need more searching,you can now search again below,\n"
           " or if there is nothing you need,you can leave the program.\n\n\n\n");
}

void section2filter_7(struct plane fl2[],struct plane flr,int num1)
{
    int e,e2,f;
    printf("The information of the flight that suit your demands is/are: \n"
           "(If no flight are shown, we are sorry that no flight suit your demands.)\n\n"
           "|Flight|Take Off and|Take Off|total |Plane|Accuracy| Prise |Aircrew|Cabin |\n"
           "|Number|  Land In   | Time   |time  |Model|        | (RMB) |  NO.  | Type |\n");
    for(e=0;e<num1;e++)     //begin filtering
    {
        flr.pr1=9000;
        flr.pr2=9000;
        flr.pr3=9000;
        e2=0;
        for(f=0;f<num1;f++)
        {
            if(flr.pr3>fl2[f].pr3)
            {
                flr.pr3=fl2[f].pr3;
                flr.pr2=fl2[f].pr2;
                flr.pr1=fl2[f].pr1;
                e2=f;
            }else
            if((flr.pr3==fl2[f].pr3)&&(flr.pr2>fl2[f].pr2))
            {
                flr.pr2=fl2[f].pr2;
                flr.pr1=fl2[f].pr1;
                e2=f;
            }else
            if((flr.pr3==fl2[f].pr3)&&(flr.pr2=fl2[f].pr2)&&(flr.pr1>fl2[f].pr1))
            {
                flr.pr1=fl2[f].pr1;
                e2=f;
            }

        }
        printform(fl2,e2);
        fl2[e2].pr3=10000;
    }
    printf("If you need more searching,you can now search again below,\n"
           " or if there is nothing you need,you can leave the program.\n\n\n\n");
}

void section2filter_8(struct plane fl2[],struct plane flr,int num1)
{
    int e,e2,f;
    printf("Considering your need,we recommend sevel flights below for you:\n;"
           "(If no flight are shown, we are sorry that we have no flight recommend.)\n\n"
           "|Flight|Take Off and|Take Off|total |Plane|Accuracy| Prise |Aircrew|Cabin |\n"
           "|Number|  Land In   | Time   |time  |Model|        | (RMB) |  NO.  | Type |\n");
    for(e=0;e<3;e++)
    {

        flr.pr1=9000;
        flr.pr2=9000;
        flr.pr3=9000;
        e2=0;
        for(f=0;f<num1;f++)
        {
            if(flr.pr3>fl2[f].pr3)
            {
                flr.pr3=fl2[f].pr3;
                flr.pr2=fl2[f].pr2;
                flr.pr1=fl2[f].pr1;
                e2=f;
            }else
            if((flr.pr3==fl2[f].pr3)&&(flr.pr2>fl2[f].pr2))
            {
                flr.pr2=fl2[f].pr2;
                flr.pr1=fl2[f].pr1;
                e2=f;
            }else
            if((flr.pr3==fl2[f].pr3)&&(flr.pr2=fl2[f].pr2)&&(flr.pr1>fl2[f].pr1))
            {
                flr.pr1=fl2[f].pr1;
                e2=f;
            }

        }
        if((fl2[e2].th>6)&&(fl2[e2].th<18))
        {
            if(fl2[e2].ac=90)
            {
                printform(fl2,e2);
            }
        }
        fl2[e2].pr3=10000;
    }
    printf("If you need more searching,you can now search again below,\n"
           " or if there is nothing you need,you can leave the program.\n\n\n\n");
}

void user_buy(struct order_information order[], int order_flight_num, int order_ticket_num)
{
    char time_now[9], date[9];
    time_t timep;
    struct tm *timeinfo;

    timep = time(NULL);
    timeinfo = localtime(&timep);
    strftime(time_now, sizeof(time_now), "%Y%m%d", timeinfo);

    if (ticket_can_buy() <= 0)
    {
        printf("You can only buy 5 tickets at same date.\n\n");
        system("pause");
        system("cls");

        user_option();
    }

    printf("You still can buy %d tickets.\n\n", ticket_can_buy() - order_ticket_num);
    system("pause");
    system("cls");

    printf("Please enter the Flight number you want to buy.\n(Enter 0 to return menu)\n\n");

	do
    {
        do
        {
            fflush(stdin);
            gets(order[order_flight_num].flight);
            system("cls");

            if (strcmp(order[order_flight_num].flight, "0") == 0)
            {
                user_option();
            }
        }
        while (check_flight_num(order[order_flight_num].flight)); //input validation

        system("cls");

        printf("Please enter the Date when you set off.\n(Enter 0 to return menu)\n\n");

        do
        {
            fflush(stdin);
            gets(order[order_flight_num].date);
            system("cls");

            sscanf(order[order_flight_num].date, "%4s-%2s-%2s", &date[0], &date[4], &date[6]);

            if (strcmp(order[order_flight_num].date, "0") == 0)
            {
                user_option();
            }
            else
            {
                if (check_date(order[order_flight_num].date) == 0)
                {
                    if (atoi(date) == atoi(time_now))
                    {
                        printf("You must book at least one day in advance, please enter the date again.\n\n");
                        continue;
                    }
                    else if (atoi(date) < atoi(time_now))
                    {
                        printf("You must enter a date in future, please enter the date again.\n\n");
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        while (1);
    }
	while (user_if_jar(order, order_flight_num));

	system("cls");

	user_buy_search(order, order_flight_num, order_ticket_num);
}

int user_if_jar(struct order_information order[], int order_flight_num)
{
    int i;
    char order_date[5][9], order_time_hour[5][3], order_time_min[5][3];
    int order_place_take[5], order_place_land[5];
    struct flight_information message;
    FILE *inFlight;

    if (order_flight_num == 0)
    {
        return 0;
    }
    else
    {
        for (i=0; i <= order_flight_num; i++)
        {
            inFlight = fopen("Flight.txt", "r");
            while (!feof(inFlight))
            {
                fscanf(inFlight, "%s", message.flight);

                if (strcmp(message.flight, order[i].flight) == 0) //find the row which has same flight number
                {
                    fscanf(inFlight, "%s%s%s%s%s%d%d%d%d%s%d%d%d%*d\n",
                    message.departure, message.arrival,
                    message.time_take, message.time_fly, message.model, &message.rate,
                    &message.money1, &message.money2, &message.money3, message.crew,
                    &message.seat1, &message.seat2, &message.seat3); //read the information about this flight

                    sscanf(message.flight, "%*2s%1d%1d", &order_place_take[i], &order_place_land[i]);
                    sscanf(message.time_take, "%2s:%2s", order_time_hour[i], order_time_min[i]);
                }
                else
                {
                    fscanf(inFlight, "%*[^\n]%*c");
                    continue;
                }
            }
            fclose(inFlight);
        }

        for (i=1; i <= order_flight_num; i++)
        {
            printf("%d %d\n",order_place_land[i-1],order_place_take[i]);
            if (order_place_land[i-1] != order_place_take[i])
            {
                printf("The palce of last flight land and the place of next flight take off must be the same,\nplease enter again.\n\n");
                system("pause");
                system("cls");
                return 1;
            }
        }

        for (i=1; i <= order_flight_num; i++)
        {
            sscanf(order[i].date, "%4s-%2s-%2s", &order_date[0], &order_date[4], &order_date[6]);

            if (atoi(order_date[i-1]) > atoi(order_date[i]))
            {
                printf("The date of last flight cannot far than next date of flight, please enter again.\n\n");
                return 1;
            }
            else if (atoi(order_date[i-1]) == atoi(order_date[i]))
            {
                if ((atoi(order_time_hour[i]) - atoi(order_time_hour[i-1])) == 4)
                {
                    if (atoi(order_time_min[i]) < atoi(order_time_min[i-1]))
                    {
                        printf("Two of the same date flight departure time must be more than 4 hours.\n\n");
                        return 1;
                    }
                }
                else if ((atoi(order_time_hour[i]) - atoi(order_time_hour[i-1]) < 4))
                {
                    printf("Two of the same date flight departure time must be more than 4 hours.\n\n");
                    return 1;
                }
            }
        }

        return 0;
    }
}

void user_buy_search(struct order_information order[], int order_flight_num, int order_ticket_num)
{
    int i;
    char option[2]; //store the option entered by user
    char flight[7], date[11], user[11], time[20];
    int seat1_buy=0, seat2_buy=0, seat3_buy=0;
    int seat1_order_buy=0, seat2_order_buy=0, seat3_order_buy=0;
    int order_status, ticket_num, seat_type; //0-resever 1-bought 2-cancel 3-done
    int seat_left[3]; //store seat1_left, seat2_left, seat3_left
    struct flight_information message;
	FILE *inFlight, *inOrders;

    printf("The information of the flight that suit your demands is: \n\n"
        "|Flight|Departure|Take Off| Total | Plane |On-Time|Ticket| Prise |Remaining|\n"
        "|Number|& Arrival|  Time  | Time  | Model |  Rate | Type | (RMB) |  seats  |\n"
        "|--------------------------------------------------------------------------|\n");

    inOrders = fopen("Orders.txt", "r");
    while (!feof(inOrders))
    {
        fscanf(inOrders, "%*d%d", &order_status);

        if (order_status == 0 || order_status == 1)
        {
            fscanf(inOrders, "%*[^\n]%*c");

            while (!feof(inOrders))
            {
                fscanf(inOrders, "%*d%s", flight);

                if (strcmp(flight, order[order_flight_num].flight) == 0)
                {
                    fscanf(inOrders, "%*d%s", date);

                    if (strcmp(date, order[order_flight_num].date) == 0)
                    {
                        fseek(inOrders, -30L, 1);
                        fscanf(inOrders, "%d%*s%d%*[^\n]%*c", &ticket_num, &seat_type);

                        switch(seat_type)
                        {
                            case 1:
                                seat1_buy += ticket_num;
                                break;
                            case 2:
                                seat2_buy += ticket_num;
                                break;
                            case 3:
                                seat3_buy += ticket_num;
                                break;
                            default:
                                break;
                        }

                        continue;
                    }
                }
                else
                {
                    if (strcmp(flight, "0") == 0 || strcmp(flight, "1") == 0 || strcmp(flight, "2") == 0)
                    {
                        fseek(inOrders, -11L, 1);
                        break;
                    }
                    else
                    {
                        fscanf(inOrders, "%*[^\n]%*c");
                        continue;
                    }
                }
            }
        }
        else
        {
            fscanf(inOrders, "%*[^\n]%*c");
            continue;
        }
    }
    fclose(inOrders);

    if (order_flight_num != 0)
    {
        for (i=0; i<order_flight_num; i++)
        {
            switch (order[i].ticket_type)
            {
                case 1:
                    seat1_order_buy += order[i].ticket_num;
                    break;
                case 2:
                    seat2_order_buy += order[i].ticket_num;
                    break;
                case 3:
                    seat3_order_buy += order[i].ticket_num;
                    break;
            }
        }
    }

	inFlight = fopen("Flight.txt", "r");
	while (!feof(inFlight))
    {
        fscanf(inFlight, "%s", message.flight);

        if (strcmp(message.flight, order[order_flight_num].flight) == 0) //find the row which has same flight number
        {
            fscanf(inFlight, "%s%s%s%s%s%d%d%d%d%s%d%d%d%*d\n",
                message.departure, message.arrival,
                message.time_take, message.time_fly, message.model, &message.rate,
                &message.money1, &message.money2, &message.money3, message.crew,
                &message.seat1, &message.seat2, &message.seat3); //read the information about this fligh

                seat_left[0] = message.seat1 - seat1_buy - seat1_order_buy;
                seat_left[1] = message.seat2 - seat2_buy - seat2_order_buy;
                seat_left[2] = message.seat3 - seat3_buy - seat3_order_buy;

            printf("|      |%9s|        |       |       |       |FIRST |%6d |%6d   |\n"
                "|%6s|         |%7s |%6s |%6s |%4d%%  |  BUS |%6d |%6d   |\n"
                "|      |%9s|        |       |       |       | ECON |%6d |%6d   |\n\n",
                message.departure, message.money1, seat_left[0], message.flight,
                message.time_take, message.time_fly,
                message.model, message.rate, message.money2, seat_left[1],
                message.arrival, message.money3, seat_left[2]); //display the information

            break;
        }
        else
        {
            fscanf(inFlight, "%*[^\n]%*c");
            continue;
        }
    }
    fclose(inFlight);

    if (strcmp(message.flight, order[order_flight_num].flight) != 0)
    {
        system("cls");
        printf("Sorry, there is not the Flight you search.\n\n");
        system("pause");
        system("cls");

        user_buy(order, order_flight_num, order_ticket_num);
    }
    else
    {
        user_buy_ticket(order, seat_left, order_flight_num, order_ticket_num);
    }
}

void user_buy_ticket(struct order_information order[], int seat_left[], int order_flight_num, int order_ticket_num)
{
    char option[2], seat_type[2], ticket_num[2];

    if (seat_left[0] == 0 && seat_left[1] == 0 && seat_left[2] == 0)
    {
        printf("We are sorry for all tickets of this flight in that date sold out.\n\n");
        system("pause");
        system("cls");

        user_buy(order, order_flight_num, order_ticket_num);
    }
    else
    {
        printf("Do you want to buy this tiket?\n1. Yes\n2. No\n\n");

        do
        {
            fflush(stdin);
            gets(option);
            system("cls");

            if (strcmp(option, "2") == 0)
            {
                user_buy(order, order_flight_num, order_ticket_num);
            }
            else if (strcmp(option, "1") == 0)
            {
                break;
            }
            else
            {
                printf("Please enter 1 or 2, please.");
                system("pause");
                system("cls");

                continue;
            }
        }
        while (1);

        printf("What type of ticket you want to buy?\n1. First-Class\n2. Bussiness-Class\n3. Economy-Class\n0. I change my mind\n\n");

        do
        {
            fflush(stdin);
            gets(seat_type);
            system("cls");

            if (strcmp(seat_type, "0") == 0)
            {
                user_buy_search(order, order_flight_num, order_ticket_num);
            }
            else if (strcmp(seat_type, "1") != 0 && strcmp(seat_type, "2") != 0 && strcmp(seat_type, "3") != 0)
            {
                printf("What type of ticket you want to buy?\n(You should enter 1, 2, 3 or 0, please enter again)\n1. First-Class\n"
                       "2. Bussiness-Class\n3. Economy-Class\n0. I do not want to buy.\n\n");
                continue;
            }
            else if (strcmp(seat_type, "1") == 0 && seat_left[0] == 0 ||
                     strcmp(seat_type, "2") == 0 && seat_left[1] == 0 ||
                     strcmp(seat_type, "3") == 0 && seat_left[2] == 0)
            {
                printf("The type of ticket you choose sold out, please choose again.\n\n");
                system("pause");
                system("cls");
                continue;
            }
            else
            {
                break;
            }
        }
        while(1);
        order[order_flight_num].ticket_type = atoi(seat_type);

        printf("How many you want to buy?\n(You still can buy %d ticket)\n(Enter 0 to return)\n\n", ticket_can_buy()- order_ticket_num);

        do
        {
            fflush(stdin);
            gets(ticket_num);
            system("cls");

            if (strcmp(ticket_num, "0") == 0)
            {
                user_buy_search(order, order_flight_num, order_ticket_num);
            }
            else if (ticket_num[0] >= '1' && ticket_num[0] <= '4' && atoi(ticket_num) <= (ticket_can_buy() - order_ticket_num))
            {
                break;
            }
            else
            {
                printf("You should enter a number less than %d.\n\n", ticket_can_buy() - order_ticket_num);
                system("pause");
                system("cls");
                continue;
            }
        }
        while (1);
        order[order_flight_num].ticket_num = atoi(ticket_num);

        user_make_order(order, order_flight_num, order_ticket_num);
    }
}

void user_make_order(struct order_information order[], int order_flight_num, int order_ticket_num)
{
    char option[2];
    char time_now[20];
    time_t timep;
    struct tm *timeinfo;
    int i;

    FILE *outOrders;

    timep = time(NULL);
    timeinfo = localtime(&timep);
    strftime(time_now, sizeof(time_now), "%Y-%m-%d-%H-%M-%S", timeinfo);

    printf("Do you want to add new flight in this order?\n1. Yes\n2. No\n\n");

    do
    {
        fflush(stdin);
        gets(option);
        system("cls");

        if (strcmp(option, "1") == 0)
        {
            if (ticket_can_buy()<=order_ticket_num)
            {
                printf("You can only buy 5 tickets a day.\n1. Abandon this order\n2. Save this order\n\n");

                do
                {
                    fflush(stdin);
                    gets(option);
                    system("cls");

                    if (strcmp(option, "1") == 0)
                    {
                        user_option();
                    }
                    else if (strcmp(option, "2") == 0)
                    {
                        break;
                    }
                    else
                    {
                        printf("You can only buy 5 tickets a day.\n1. Abandon this order\n2. Save this order\n\n"
                               "You should enter 1 or 2, please enter again.\n\n");
                        system("pause");
                        system("cls");
                    }
                }while(1);
            }
            else
            {
                order_ticket_num+=order[order_flight_num].ticket_num;
                order_flight_num++;
                user_buy(order, order_flight_num, order_ticket_num);
            }
            break;
        }
        else if (strcmp(option, "2") == 0)
        {
            outOrders = fopen("Orders.txt", "a");
            fprintf(outOrders, "\n%-10d0    %-10s%-20s", order[0].order_no, id, time_now);

            for (i=0; i<=order_flight_num; i++)
            {
                fprintf(outOrders, "\n%-5d%-10s%-5d%-25s", order[i].ticket_num, order[i].flight, order[i].ticket_type, order[i].date);
            }
            fclose(outOrders);


            system("cls");
            break;
        }
        else
        {
            printf("You should enter 1 or 2, please enter again.\n\n");
            system("pause");
            system("cls");
            continue;
        }
    }while(1);

    user_order_pay(order[0].order_no);
}

void user_order()
{
    char option[2];
    char user[11], flight[11], date[11], type[11], status[11], order_status[11];
    int order_no, ticket_num, ticket_type;
    FILE *inOrders;

    printf("| Order No. |    Date    |   Flight   |Ticket Type| Quantity |  Status  |\n"
           "|-----------|------------|------------|-----------|----------|----------|\n");

    inOrders = fopen("Orders.txt", "r");
    while(!feof(inOrders))
    {
        fscanf(inOrders, "%d%s%s", &order_no, order_status, user);

        if (strcmp(user, id) == 0)
        {
            fscanf(inOrders, "%*[^\n]%*c");
            fscanf(inOrders, "%d%s%d%s%*[^\n]%*c",  &ticket_num, flight, &ticket_type, date);

            switch (ticket_type)
            {
                case 1:
                    strcpy(type, "First");
                    break;
                case 2:
                    strcpy(type, "Business");
                    break;
                case 3:
                    strcpy(type, "Economy");
                    break;
                default:
                    break;

            }
            switch (atoi(order_status))
            {
                case 0:
                    strcpy(status, "Reserved");
                    break;
                case 1:
                    strcpy(status, "Bought");
                    break;
                case 2:
                    strcpy(status, "Cancelled");
                    break;
                default:
                    break;
            }

            printf("|%-9d  |%-11s |%-11s |%-9s  |%-5d     |%-9s |\n", order_no, date, flight, type, ticket_num, status);

            while (!feof(inOrders))
            {
                fscanf(inOrders, "%*d%s", flight);

                if (strcmp(flight, "0") == 0 || strcmp(flight, "1") == 0 || strcmp(flight, "2") == 0)
                {
                    fseek(inOrders, -11L, 1);
                    break;
                }
                else
                {
                    fseek(inOrders, -11L, 1);
                    fscanf(inOrders, "%d%*s%d%s%*[^\n]%*c", &ticket_num, &ticket_type, date);

                    switch (ticket_type)
                    {
                        case 1:
                            strcpy(type, "First");
                            break;
                        case 2:
                            strcpy(type, "Business");
                            break;
                        case 3:
                            strcpy(type, "Economy");
                            break;
                        default:
                            break;

                    }

                    switch (atoi(order_status))
                    {
                        case 0:
                            strcpy(status, "Reserve");
                            break;
                        case 1:
                            strcpy(status, "Bought");
                            break;
                        case 2:
                            strcpy(status, "Cancel");
                            break;
                        default:
                            break;
                    }

            printf("|           |%-11s |%-11s |%-9s  |%-5d     |          |\n", date, flight, type, ticket_num);
                }
            }

            printf("|-----------|------------|------------|-----------|----------|----------|\n");
            continue;
        }
        else
        {
            fscanf(inOrders, "%*[^\n]%*c");
            continue;
        }
    }
    fclose(inOrders);

    user_order_operate();
}

void user_order_operate()
{
    char option[2];
    char order_no[6];

    printf("Please enter the No. of order you want to operate.\n(Enter 0 to return)\n\n");

    do
	{
		fflush(stdin);
		gets(order_no);

        if (strcmp(order_no, "0") == 0)
		{
            system("cls");
            user_option();
		}
	}
	while (check_order_no(order_no));

	printf("Please select the operation you want to perform it.\n1. Pay it\n2. Cancel it\n(Enter 0 to return)\n\n");

    do
	{
		fflush(stdin);
		gets(option);

        if (strcmp(option, "0") == 0)
		{
		    system("cls");
            user_order();
		}
		else if (strcmp(option, "1") == 0)
        {
            user_order_pay(atoi(order_no));
        }
        else if (strcmp(option, "2") == 0)
        {
            user_order_cancel(atoi(order_no));
        }
        else
        {
            printf("You should enter 1 or 2.\n(Enter 0 to return)\n\n");
            continue;
        }
	}
	while (1);
}

void user_order_pay(int order_no)
{
    int row=0;
    int order_no_read;
    char option[2], order_status[11];
    FILE *inOrders, *outOrders;

    printf("Do you want to pay this order?\n(You can cancel or pay a order in order management)\n1. Yes\n2. No\n\n");

    do
    {
        fflush(stdin);
        gets(option);
        system("cls");

        if (strcmp(option, "1") == 0)
        {
            inOrders = fopen("Orders.txt", "r");
            outOrders = fopen("Orders.txt", "r+");
            while(!feof(inOrders))
            {
                fscanf(inOrders, "%d%s", &order_no_read, order_status);
                row++;

                if (order_no == order_no_read && (strcmp(order_status, "0") == 0 ||
                                                  strcmp(order_status, "1") == 0 ||
                                                  strcmp(order_status, "2") == 0))
                {
                    if(strcmp(order_status, "0") == 0)
                    {
                        fseek(outOrders, 1L*((row-1)*47+10), 0);
                        fprintf(outOrders, "1    ");
                        break;
                    }
                    else if(strcmp(order_status, "2") == 0)
                    {
                        printf("This order has been canceled, you cannot operate it.\n\n");
                        system("pause");
                        system("cls");
                        user_order();
                        break;
                    }
                    else
                    {
                        printf("This order has been paid, you cannot operate it.\n\n");
                        system("pause");
                        system("cls");
                        user_order();
                        break;
                    }
                }
                else
                {
                    fscanf(inOrders, "%*[^\n]%*c");
                    continue;
                }
            }
            fclose(inOrders);
            fclose(outOrders);

            printf("Purchase Success.\n\n");
            break;
        }
        else if (strcmp(option, "2") == 0)
        {
            break;
        }
        else
        {
            printf("You should enter 1 or 2, please enter again.\n\n");
            system("pause");
            system("cls");
            continue;
        }
    }
    while (1);

    system("pause");
    system("cls");

    user_order();
}

void user_order_cancel(int order_no)
{
    int row=0;
    int order_no_read;
    char option[2], order_status[11];
    FILE *inOrders, *outOrders;

    printf("Do you want to cancel this order?\n1. Yes\n2. No\n\n");

    do
    {
        fflush(stdin);
        gets(option);
        system("cls");

        if (strcmp(option, "1") == 0)
        {
            inOrders = fopen("Orders.txt", "r");
            outOrders = fopen("Orders.txt", "r+");
            while(!feof(inOrders))
            {
                fscanf(inOrders, "%d%s", &order_no_read, order_status);
                row++;

                if (order_no == order_no_read && (strcmp(order_status, "0") == 0 ||
                                                  strcmp(order_status, "1") == 0 ||
                                                  strcmp(order_status, "2") == 0))
                {
                    if(strcmp(order_status, "0") == 0)
                    {
                        fseek(outOrders, 1L*((row-1)*47+10), 0);
                        fprintf(outOrders, "2    ");
                        break;
                    }
                    else if(strcmp(order_status, "2") == 0)
                    {
                        printf("This order has been canceled\n\n");
                        system("pause");
                        system("cls");
                        user_order();
                        break;
                    }
                    else
                    {
                        printf("This order has been paid, you cannot cancel it.\n\n");
                        system("pause");
                        system("cls");
                        user_order();
                        break;
                    }
                }
                else
                {
                    fscanf(inOrders, "%*[^\n]%*c");
                    continue;
                }
            }
            fclose(inOrders);
            fclose(outOrders);

            printf("Cancel Success.\n\n");
            break;
        }
        else if (strcmp(option, "2") == 0)
        {
            break;
        }
        else
        {
            printf("You should enter 1 or 2, please enter again.\n\n");
            system("pause");
            system("cls");
            continue;
        }
    }
    while (1);

    system("pause");
    system("cls");

    user_order();
}

void admin_option()
{
	char option[2]; //store user selection
	char company[11];
	char admin[11];
	int company_num;
	FILE *inAdmin;

	inAdmin = fopen("Admin.txt", "r");
	while (!(strcmp(id, admin) == 0))
    {
        fscanf(inAdmin, "%s%*s%s%*[^\n]%*c", admin, company);
    }
    fclose(inAdmin);

    if (strcmp(company, "Guohang") == 0)
    {
        company_num = 1;
    }
    else if (strcmp(company, "Donghang") == 0)
    {
        company_num = 2;
    }
    else if (strcmp(company, "Nanhang") == 0)
    {
        company_num = 3;
    }

	printf("Please choose the item which you need.\n\n1. Manage personal information\n"
            "2. Query flight information\n3. Flight management\n4. Query orders` information\n"
            "5. Orders` statistics\n0. Logout\n\n");

	/* input validation */
	do
	{
		fflush(stdin);
		gets(option);
        system("cls");

        if (!(strcmp(option, "1") == 0 || strcmp(option, "2") == 0 ||
              strcmp(option, "3") == 0 || strcmp(option, "4") == 0 ||
              strcmp(option, "5") == 0 || strcmp(option, "0") == 0))
        {
            printf("Please choose the item which you need.\n\n1. Manage personal information\n"
                    "2. Query flight information\n3. Flight management\n4. Query orders` information\n"
                    "5. Orders` statistics\n0. Logout\n\n");
            printf("You should choose the way by enter 1, 2, 3, 4, 5 or 0, please enter again.\n\n");
            continue;
        }
        else
        {
            break;
        }
	}
    while (1);

	switch (atoi(option)) //turn string-option to int-option
	{
		case 1:
			admin_information();
			break;
		case 2:
			admin_flight_information(company_num);
			break;
		case 3:
			admin_flight_management(company_num);
			break;
		case 4:
			admin_order_list(company_num);
			break;
		case 5:
			admin_order_statistics(company_num);
			break;
		case 0:
			select_way();
		default:
			break;
	}
}

void admin_information()
{
    int row=0;
    char option[2];
    char admin[11], company[11], name[11], email[31], password[11];
    char password_enter[11], email_enter[31];
    FILE *inFile, *outFile;

    inFile = fopen("Admin.txt", "r");
    while(!feof(inFile))
    {
        fscanf(inFile, "%s", admin);
        row++;

        if (strcmp(admin, id) == 0)
        {
            fscanf(inFile, "%s%s%s%s", password, company, name, email);
            break;
        }
        else
        {
            fscanf(inFile, "%*[^\n]%*c");
            continue;
        }
    }
    fclose(inFile);

    printf("|    ID    | Company  |        Name        |            Email             |\n"
           "|----------|----------|--------------------|------------------------------|\n"
           "|%10s|%10s|%20s|%30s|\n\n", admin, company, name, email);
    printf("Please select the information you want to change: \n"
           "1. Change my email\n2. Change my password\n0. Return menu\n\n");

    do
    {
        fflush(stdin);
        gets(option);
        system("cls");

        if (strcmp(option, "1") != 0 && strcmp(option, "2") != 0 && strcmp(option, "0") != 0)
        {
            printf("|    ID    | Company  |        Name        |            Email             |\n"
                   "|----------|----------|--------------------|------------------------------|\n"
                   "|%10s|%10s|%20s|%30s|\n\n", admin, company, name, email);
            printf("You should enter 1, 2 or 0, please enter again: \n"
                   "1. Change my email\n2. Change my password\n0. Return menu\n\n");
        }
        else
        {
            break;
        }
    }while(1);

    outFile = fopen("Admin.txt", "r+");
    switch(atoi(option))
    {
        case 1:
            printf("Please enter the email address you want to change: \n(Enter 0 to return)\n\n");

            do
            {
                fflush(stdin);
                gets(email_enter);
                system("cls");

                if (strcmp(email_enter, "0") == 0)
                {
                    admin_information();
                }
            }
            while(check_email(email_enter));

            fseek(outFile, 1L*((row-1)*82)+50, 0);
            fprintf(outFile, "%-30s", email_enter);
            printf("Change successfully\n\n");
            system("pause");
            break;
        case 2:
            printf("Please enter the password you want to change: \n(Enter 0 to return)\n\n");

            do
            {
                fflush(stdin);
                gets(password_enter);
                system("cls");

                if (strcmp(password_enter, "0") == 0)
                {
                    admin_information();
                }
                else if (strlen(password_enter) > 10 || strlen(password_enter) < 6 )
                {
                    printf("Password should have 6-10 characters, please enter again.\n(Enter 0 to return)\n\n");
                    continue;
                }
                else
                {
                    break;
                }

            }
            while(1);

            fseek(outFile, 1L*((row-1)*82)+10, 0);
            fprintf(outFile, "%-10s", password_enter);
            printf("Change successfully\n\n");
            system("pause");
            system("cls");
            break;
        case 0:
            admin_option();
            break;
     }
     fclose(outFile);

     admin_information();
}

void admin_flight_information(int company_num)
{
    struct flight_information
	{
		char flight[7];
		char departure[16];
		char arrival[16];
		char time_take[6];
		char time_fly[6];
		char model[5];
		int rate;
		int money1;
		int money2;
		int money3;
		char crew[5];
		int seat1;
		int seat2;
		int seat3;
	}message; //store flight information

    char cp[3], cp_read[3]; //store company
	FILE *inFlight;

    switch (company_num)
    {
        case 1:
            strcpy(cp, "CA");
            break;
        case 2:
            strcpy(cp, "MU");
            break;
        case 3:
            strcpy(cp, "CZ");
            break;
        default:
            break;
    }

    printf("The information of the flight that suit your demands is: \n\n"
        "|Flight|Departure|Take Off| Total | Plane |On-Time|Ticket| Prise |Remaining|\n"
        "|Number|& Arrival|  Time  | Time  | Model |  Rate | Type | (RMB) |  seats  |\n"
        "|------|---------|--------|-------|-------|-------|------|-------|---------|\n");

    inFlight = fopen("Flight.txt", "r");
	while (!feof(inFlight))
    {
        fscanf(inFlight, "%s", message.flight);
        sscanf(message.flight, "%2s", cp_read);

        if (strcmp(cp_read, cp) == 0) //find the row which has same flight number
        {
            fscanf(inFlight, "%s%s%s%s%s%d%d%d%d%s%d%d%d%*d\n",
                message.departure, message.arrival,
                message.time_take, message.time_fly, message.model, &message.rate,
                &message.money1, &message.money2, &message.money3, message.crew,
                &message.seat1, &message.seat2, &message.seat3); //read the information about this flight

            printf("|      |%9s|        |       |       |       |FIRST |%6d |%6d   |\n"
                "|%6s|         |%7s |%6s |%6s |%4d%%  |  BUS |%6d |%6d   |\n"
                "|      |%9s|        |       |       |       | ECON |%6d |%6d   |\n"
                "|--------------------------------------------------------------------------|\n",
                message.departure, message.money1, message.seat1, message.flight,
                message.time_take, message.time_fly,
                message.model, message.rate, message.money2, message.seat2,
                message.arrival, message.money3, message.seat3); //display the information
            continue;
        }
        else
        {
            fscanf(inFlight, "%*[^\n]%*c");
            continue;
        }
    }
    fclose(inFlight);
    system("pause");
    system("cls");

    admin_option();
}

void admin_flight_management(int company_num)
{
	FILE *rfile,*rec;
	struct place pl;
	struct plane fl[220],fl2[12];
	struct plane leave[7], arrive[7];
	char flight[3];
	int a=0,b=0,c=0,d=0,e=0,f=0,g=0,h=0;
	int pilot;
	int time1,time2,time3,time4;
	char time5[6];
	char hour[3], min[3];
	int sure;
    int addflight;
	char dir[3];
	char flight2[3];

	switch (company_num)
    {
        case 1:
            strcpy(flight, "CA");
            break;
        case 2:
            strcpy(flight, "MU");
            break;
        case 3:
            strcpy(flight, "CZ");
            break;
        default:
            break;
    }

	for(f=0;f<=0;e++)
	{
        printf("Please enter the place where the plane takes off:\n");//let the user enter the take off site
        fflush(stdin);
        scanf("%s",&pl.off);

        for(e=0;e<=0;e--)  //check the format of the place
        {
            pl.off[0]=toupper(pl.off[0]);

            for(a=1;a<16;a++)
            {
			pl.off[a]=tolower(pl.off[a]);
            }

            if(strcmp(pl.off, "Shanghai")==0 || strcmp(pl.off, "Beijing")==0|| strcmp(pl.off,"Guangzhou")==0)
            {
                break;
            }
            else
            {
                printf("Please enter the place again: ");
                fflush(stdin);
                scanf("%s", pl.off);
            }
        }

        printf("Please enter the place where the plane landing:\n");//let the user enter the place of arrival
        fflush(stdin);
        scanf("%s",&pl.land);

        for(e=0;e<=0;e--)   //check the format of the place
        {
            pl.land[0]=toupper(pl.land[0]);
            if(strcmp(pl.land, "Shanghai")==0 ||strcmp(pl.land, "Beijing")==0 || strcmp(pl.land,"Guangzhou") ==0&& strcmp(pl.land, pl.off)!=0)
            {
                break;
            }
            else
            {
                printf("Please enter the place again: ");
                fflush(stdin);
                scanf("%s", pl.land);

                pl.land[0]=toupper(pl.land[0]);

                for(a=1;a<10;a++)
                {
				pl.land[a]=tolower(pl.land[a]);
                }
            }
        }

        printf("The information of the flight that suit your demands is/are: \n"        //printf the result of the search
            "(If no flight are shown, we are sorry that no flight suit your demands.)\n\n"
            "NO|Flight|Take Off and|Take Off|total |Plane|Accuracy| Prise |Aircrew|Remain|\n"
            "  |Number|  Land In   | Time   |time  |Model|        | (RMB) |  NO.  | seat |\n");

        rfile=fopen("flight.txt","r");
        for(a=0;a<216;a++)
        {
            b=0;
            while((fscanf(rfile,
						"%s\t%s%s%d:%d\t%d:%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
                	    &fl[a].pl,&fl[a].off,&fl[a].land,&fl[a].th,&fl[a].tm,&fl[a].fh,&fl[a].fm,
            	        &fl[a].ty,&fl[a].ac,&fl[a].pr1,&fl[a].pr2,&fl[a].pr3,&fl[a].group,
        	            &fl[a].left1,&fl[a].left2,&fl[a].left3,&fl[a].type)!=EOF))
            {
                if(strcmp(fl[a].off,pl.off)==0)
                {
                    if(strcmp(fl[a].land,pl.land)==0)
                    {
                        if(strncmp(flight,fl[a].pl,2)==0)
                        {
                            printf("  |-------------------------------------------------------------------------|\n"
                                    "  |      |%-12s|        |      |     |        |F:%-5d|       |F:%-4d|\n"
                                    "%2d|%s|to          |%2d:%-2d   |%dh%dm |%s |%d%%     |C:%-5d|NO.%-4d|C:%-4d|\n"
                                    "  |      |%-12s|        |      |     |        |Y:%-5d|       |Y:%-4d|\n",
                                    fl[a].off,fl[a].pr1,fl[a].left1,b+1,fl[a].pl,fl[a].th,fl[a].tm,fl[a].fh,fl[a].fm,fl[a].ty,
                                    fl[a].ac,fl[a].pr2,fl[a].group,fl[a].left2,fl[a].land,fl[a].pr3,fl[a].left3);
                            fl2[b]=fl[a];
                            b++;
                        }
                    }
                }
            }
        }

        printf("\nPlease enter the number of the aircrew you want: ");
        fflush(stdin);
        scanf("%d", &pilot);
        for(e=0;e>=0;e++)     //check the aircrew
        {
            if(pilot>=1 && pilot<=12)
            {
                break;
            }
            else
            {
                fflush(stdin);
                printf("\nPlease enter the number of the list you want again: ");
                scanf("%d", &pilot);
            }
        }

        c=0;
        d=0;
        g=fl2[pilot-1].type;
        for(a=0;a<12;a++)
        {
            if (g==1)
            {
                leave[c]=fl2[a];
                c++;
            }
            else
            {
                arrive[d]=fl2[a];
                d++;
            }
        }

        switch (fl2[pilot-1].type)     //decide the time quantum the user plane can be used
        {
            case 1:
                time1=leave[pilot-1].th+8;
                if(time1>=24)
                time1=time1-24;
                if(time1<0)
                time1=time1+24;
                time3=leave[pilot-1].th-4;
                if(time3>=24)
                time3=time3-24;
                if(time3<0)
                time3=time3+24;
                time2=leave[pilot-1].tm;
                time4=leave[pilot-1].fm;
                if(time1<time3)
                printf("\nPlease choose the take off time between %d:%d and %d:%d", time1,time2,time3,time4);
                else printf("\nPlease choose the take off time between %d:%d and %d:%d(the next day)", time1,time2,time3,time2);
                break;
            case 2:
                time1=arrive[pilot-1].th+4;
                if(time1>=24)
                time1=time1-24;
                if(time1<0)
                time1=time1+24;
                time3=arrive[pilot-1].th-8;
                if(time3>=24)
                time3=time3-24;
                if(time3<0)
                time3=time3+24;
                time2=arrive[pilot-1].tm;
                time4=arrive[pilot-1].fm;
                if(time1<time3)
                printf("\nPlease choose the take off time between %d:%d and %d:%d", time1,time2,time3,time2);
                else printf("\nPlease choose the take off time between %d:%d and %d:%d(the next day)", time1,time2,time3,time2);
                break;
            default:
                break;
        }

        for(e=0;e>=0;e++)     //check the time user enterd
        {
            printf("\nPlease enter the time you want the plane to take off: ");
            fflush(stdin);
            scanf("%s",time5);
            if(time5[2]==':')
            {
                if(strlen(time5)==5)
                {
                    fflush(stdin);
                    sscanf(time5, "%2s:%2s", hour, min);
                    if(atoi(hour)>=0 &&  atoi(hour)<=23)
                    {
                        if(atoi(min)>=0 && atoi(min)<=59)
                        {
                            break;
                        }
                    }
                }
            }
        }

        for(e=0;e>=0;e++)
		{
			printf("\nPlease enter the number of this flight(a double-digit number): ");
       		scanf("%d", &addflight);
       		if(addflight>46)
       		{
       			break;
			   }
		}
		if(strcmp(pl.off, "Beijing")==0)
		{
			dir[0]='1';
		}
			else
			{
				if(strcmp(pl.off, "Shanghai")==0)
				{
					dir[0]='2';
					}
					else dir[0]='3';
			}

		if(strcmp(pl.land, "Beijing")==0)
		{
			dir[1]='1';
		}
			else
			{
				if(strcmp(pl.land, "Shanghai")==0)
				{
					dir[1]='2';
					}
					else dir[1]='3';
			}
		dir[2]='\0';
		sprintf(flight2, "%d", addflight);
        strcat(dir,flight2);

        fflush(stdin);
        printf("\nAre you sure to add this flight?\n1.Yes\n2.No ");
        scanf("%d", &sure);

        if(sure==1)
        {
        break;
        }
    }

	rec=fopen("flight.txt","a");              //add the information into the database
	fprintf(rec,"\n%s%s\t%-15s%-15s%s\t%d:%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t1",
            flight,dir,fl2[pilot-1].off,fl2[pilot-1].land,
            time5,fl2[pilot-1].fh,fl2[pilot-1].fm,fl2[pilot-1].ty,fl2[pilot-1].ac,fl2[pilot-1].pr1,fl2[pilot-1].pr2,
            fl2[pilot-1].pr3,fl2[pilot-1].group,fl2[pilot-1].left1,fl2[pilot-1].left2,fl2[pilot-1].left3);
	fclose(rfile);
	fclose(rec);

	system("pause");
	system("cls");
	admin_option();
}

void admin_order_list(int company_num)
{
    char option[2];
    char user[11], flight[11], date[11], type[11], status[11], order_status[11], cp[3], cp_read[3];
    int order_no, ticket_num, ticket_type;
    FILE *inOrders;

    switch (company_num)
    {
        case 1:
            strcpy(cp, "CA");
            break;
        case 2:
            strcpy(cp, "MU");
            break;
        case 3:
            strcpy(cp, "CZ");
            break;
        default:
            break;
    }

    printf("| Order No. |    Date    |   Flight   |Ticket Type| Quantity |  Status  |\n"
           "|-----------|------------|------------|-----------|----------|----------|\n");

    inOrders = fopen("Orders.txt", "r");
    while(!feof(inOrders))
    {
        fscanf(inOrders, "%d%s%s%s%*[^\n]%*c", &order_no, order_status, user, date);
        fscanf(inOrders, "%d%s", &ticket_num, flight);

        sscanf(flight, "%2s", cp_read);

        if (strcmp(cp_read, cp) == 0)
        {
            fscanf(inOrders, "%d%s%*[^\n]%*c", &ticket_type, date);

            switch (ticket_type)
            {
                case 1:
                    strcpy(type, "First");
                    break;
                case 2:
                    strcpy(type, "Business");
                    break;
                case 3:
                    strcpy(type, "Economy");
                    break;
                default:
                    break;

            }
            switch (atoi(order_status))
            {
                case 0:
                    strcpy(status, "Reserved");
                    break;
                case 1:
                    strcpy(status, "Bought");
                    break;
                case 2:
                    strcpy(status, "Cancelled");
                    break;
                default:
                    break;
            }

            printf("|%-9d  |%-11s |%-11s |%-9s  |%-5d     |%-9s |\n", order_no, date, flight, type, ticket_num, status);

            while (!feof(inOrders))
            {
                fscanf(inOrders, "%*d%s", flight);

                if (strcmp(flight, "0") == 0 || strcmp(flight, "1") == 0 || strcmp(flight, "2") == 0)
                {
                    fseek(inOrders, -11L, 1);
                    break;
                }
                else
                {
                    sscanf(flight, "%2s", cp_read);

                    if (strcmp(cp_read, cp) == 0)
                    {
                        fseek(inOrders, -11L, 1);
                        fscanf(inOrders, "%d%*s%d%s%*[^\n]%*c", &ticket_num, &ticket_type, date);

                        switch (ticket_type)
                        {
                            case 1:
                                strcpy(type, "First");
                                break;
                            case 2:
                                strcpy(type, "Business");
                                break;
                            case 3:
                                strcpy(type, "Economy");
                                break;
                            default:
                                break;
                        }

                        switch (atoi(order_status))
                        {
                            case 0:
                                strcpy(status, "Reserve");
                                break;
                            case 1:
                                strcpy(status, "Bought");
                                break;
                            case 2:
                                strcpy(status, "Cancel");
                                break;
                            default:
                                break;
                        }
                    }
                    else
                    {
                        fscanf(inOrders, "%*[^\n]%*c");
                        continue;
                    }

                printf("|           |%-11s |%-11s |%-9s  |%-5d     |          |\n", date, flight, type, ticket_num);
                }
            }

            printf("|-----------|------------|------------|-----------|----------|----------|\n");
            continue;
        }
        else
        {
            fscanf(inOrders, "%*[^\n]%*c");
            continue;
        }
    }
    fclose(inOrders);

    system("pause");
    system("cls");
    admin_option();
}

void admin_order_statistics(int company_num)
{
    int order_num,order_status,flight_num, num=0,way=0,a;
    char cp[3], cp_read[3];
    order_num = row2();
    flight_num=row();
    struct order_information info[order_num];
    struct plane fl[flight_num];
    FILE *inOrders,*rfile;


    rfile=fopen("flight.txt","r");
    for(a=0;a<flight_num;a++)
    {
        fscanf(rfile,       //gain the flight information
               "%s\t%s%s%d:%d\t%d:%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d%*d\n",
               &fl[a].pl,&fl[a].off,&fl[a].land,&fl[a].th,&fl[a].tm,&fl[a].fh,&fl[a].fm,
               &fl[a].ty,&fl[a].ac,&fl[a].pr1,&fl[a].pr2,&fl[a].pr3,&fl[a].group,
               &fl[a].left1,&fl[a].left2,&fl[a].left3);
    }
    switch (company_num)
    {
        case 1:
            strcpy(cp, "CA");
            break;
        case 2:
            strcpy(cp, "MU");
            break;
        case 3:
            strcpy(cp, "CZ");
            break;
        default:
            break;
    }
    inOrders = fopen("Orders.txt", "r");
    while (!feof(inOrders))
    {
        fscanf(inOrders, "%*d%d", &order_status);

        if (order_status == 0 || order_status == 1)
        {
            fscanf(inOrders, "%*[^\n]%*c");

            while (!feof(inOrders))
            {
                fscanf(inOrders, "%*d%s", info[num].flight);
                sscanf(info[num].flight, "%2s", cp_read);

                if (strcmp(cp, cp_read) == 0)
                {
                    fseek(inOrders, -11L, 1);
                    fscanf(inOrders, "%d%*s%d%s\n", &info[num].ticket_num, &info[num].ticket_type, info[num].date);
                    fscanf(inOrders, "%*[^\n]%*c");
                    num++;

                    continue;
                }
                else
                {
                    fseek(inOrders, -11L, 1);
                    break;
                }
            }
        }
        else
        {
            fscanf(inOrders, "%*[^\n]%*c");
            continue;
        }
    }
    fclose(inOrders);
    printf("Please choose the way you want to count the order:\n"
           "(Only show the flight that have order, enter other number to exit)\n"
           "1.Print all flight that not taking off ,the more passengers,the earlier it show\n"
           "2.Print all flight in on board rate\n(separate by day, only show flight that take off the day before today)\n"
           "3.Print all flight in each flight's income\n(separate by day, only show flight that take off the day before today)\n");
    fflush(stdin);
    scanf("%d",&way);
    switch(way)
    {
    case 1:
        print_order_1(info,order_num,cp);
        break;
    case 2:
        print_order_2(info,fl,order_num,flight_num,cp);
        break;
    case 3:
        print_order_3(info,fl,order_num,flight_num,cp);
        break;
    default:
        break;
    }

    system("pause");
    system("cls");
    admin_option();
}

void print_order_1(struct order_information info[],int order_num,char cp[])
{
    time_t time1;
    struct tm * time0;
    int day,month;
    time ( &time1 );
    time0 = localtime ( &time1 );
    day=time0->tm_mday;
    month=(time0->tm_mon)+1;

    int a,a1,e,e2;
    char infoday[3],infomonth[3];
    struct order_information info0,info1[order_num];

    printf("The fligh has been list as follow:\n"
           "|   time   |Flight number|Order Number|\n");
    for(a=0;a<order_num;a++)        //rearrange the flight
    {
        for(a1=(a+1);a1<order_num;a1++)
        {
            if(strcmp(info[a].date,info[a1].date)==0&&strcmp(info[a].flight,info[a1].flight)==0&&info[a1].flight[0]!='0')
            {
                info[a].ticket_num+=info[a1].ticket_num;
                info[a1].flight[0]='0';
            }
        }
    }

    e=0;
    for(a=0;a<order_num;a++)
    {
        if(info[a].flight[0]!='0'&&strncmp(info[a].flight,cp,2)==0)
        {
            info1[e]=info[a];
            e++;
        }
    }
    for(a=0;a<e;a++)
    {
        info0.ticket_num=0;
        for(a1=0;a1<e;a1++)
        {
            if(info0.ticket_num<info1[a1].ticket_num)
            {
                info0.ticket_num=info1[a1].ticket_num;
                e2=a1;
            }
        }
        printf("|%s|   %s    |    %-3d     |\n",
               info1[e2].date,info1[e2].flight,info1[e2].ticket_num);
        info1[e2].ticket_num=0;
    }
}

void print_order_2(struct order_information info[],struct plane fl[],int order_num,int flight_num,char cp[])
{
    int a,a1,b,c,e,seat;
    struct order_information info0,info1[order_num];

    printf("The fligh has been list as follow:\n"
           "|   time   |Flight number|Order Number|On Board Rate|\n");
    for(a=0;a<order_num;a++)        //combine the flight with  same flight number, same take off time
    {
        for(a1=(a+1);a1<order_num;a1++)
        {
            if(strcmp(info[a].date,info[a1].date)==0&&strcmp(info[a].flight,info[a1].flight)==0)
            {
                info[a].ticket_num+=info[a1].ticket_num;
                info[a1].flight[0]='0';
            }
        }
    }
    e=0;
    for(a=0;a<order_num;a++)
    {
        if(info[a].flight[0]!='0'&&strncmp(info[a].flight,cp,2)==0)
        {
            info1[e]=info[a];
            e++;
        }
    }
    for(a=0;a<e;a++)        //rearrange the flight
    {
        info0.ticket_num=0;
        a1=0;
        for(b=0;b<e;b++)
        {
            if(strncmp(info1[a].flight,cp,2)==0)
            {
                for(c=0;c<flight_num;c++)
                {
                    seat=0;
                    if(strcmp(info1[b].flight,fl[c].pl)==0)
                    {
                        break;
                    }
                }
                seat=(float)(fl[c].left1+fl[c].left2+fl[c].left3);
                if(((float)info0.ticket_num/seat)<((float)info1[b].ticket_num/seat))
                {
                    info0.ticket_num=info1[b].ticket_num;
                    a1=b;
                }
            }
        }
        if(strncmp(info1[a1].flight,cp,2)==0&&(info1[a1].flight[0]!='0'))
        {
            printf("|%s|   %s    |    %-3d     |    %5.2f    |\n",
                   info1[a1].date,info1[a1].flight,info1[a1].ticket_num,(float)info0.ticket_num/seat);
            info1[a1].ticket_num=0;
        }
    }
}

void print_order_3(struct order_information info[],struct plane fl[],int order_num,int flight_num,char cp[])
{
    int a,a1,b,e,e2,compincome=0;
    struct money income[order_num], income2;

    for(a=0;a<order_num;a++)
    {
    	income[a].num1=0;
    	income[a].num2=0;
    	income[a].num3=0;
	}

    printf("The flight has been list as follow:\n"
           "|   time   |Flight number|  Income  |\n");
    for(a=0;a<order_num;a++)        //combine the flight with  same flight number, same take off time with same ticket type
    {
        for(a1=(a+1);a1<order_num;a1++)
        {
            if(strcmp(info[a].date,info[a1].date)==0
               &&strcmp(info[a].flight,info[a1].flight)==0
               &&info[a].ticket_type==info[a1].ticket_type)
            {
                info[a].ticket_num+=info[a1].ticket_num;
                info[a1].flight[0]='0';
            }
        }
    }
    e=0;
    for(a=0;a<order_num;a++)        //calculate the income of each class of each flight,put them in to a new struct
    {
        for(b=0;b<a;b++)
        {
            if(strcmp(info[b].flight,info[a].flight)==0
               &&strcmp(info[b].date,info[a].date)==0)
            {
                a++;
                break;
            }
        }
        if(info[a].flight[0]!='0'&&strncmp(info[a].flight,cp,2)==0)
        {
            strcpy(income[e].date,info[a].date);
            strcpy(income[e].flight,info[a].flight);
            if(info[a].ticket_type==1)
            {
                income[e].num1=info[a].ticket_num;
            }else if(info[a].ticket_type==2)
            {
                income[e].num2=info[a].ticket_num;
            }else if(info[a].ticket_type==3)
            {
                income[e].num3=info[a].ticket_num;
            }
            for(a1=a+1;a1<order_num;a1++)
            {

                if(strcmp(info[a].date,info[a1].date)==0
                   &&strcmp(info[a].flight,info[a1].flight)==0)
                   {
                       if(info[a1].ticket_type==1)
                        {
                            income[e].num1+=info[a1].ticket_num;
                        }else if(info[a1].ticket_type==2)
                        {
                            income[e].num2+=info[a1].ticket_num;
                        }else if(info[a1].ticket_type==3)
                        {
                            income[e].num3+=info[a1].ticket_num;
                        }
                   }
            }
            e++;
        }
    }
    for(a=0;a<e;a++)        //calculate the total in come of each flight,and rearrange them
    {
        for(a1=0;a1<flight_num;a1++)
        {
            if(strcmp(income[a].flight,fl[a1].pl)==0)
            {
                income[a].mon1=fl[a1].pr1;
                income[a].mon2=fl[a1].pr2;
                income[a].mon3=fl[a1].pr3;
            }
            income[a].toincome=income[a].mon1*income[a].num1+income[a].mon2*income[a].num2+income[a].mon3*income[a].num3;
        }
    }
    for(a=0;a<e;a++)
    {
        income2.toincome=10;
        for(a1=0;a1<e;a1++)
        {
            if(income2.toincome<income[a1].toincome)
            {
                income2.toincome=income[a1].toincome;
                e2=a1;
            }
        }
        printf("|%s|   %s    |%10d|\n",
               income[e2].date,income[e2].flight,income[e2].toincome);
        compincome+=income[e2].toincome;
        income[e2].toincome=0;
    }
    printf("The total income are: %d RMB\n",compincome);
}

int check_id(char ID[])
{
	int i;
	int num_digit=0;
	int num_letter=0;

	/* check if ID has 6-10 chars */
	if (strlen(ID) < 6 || strlen(ID) > 10)
	{
		printf("ID should have 6-10 characters, please enter again.\n(Enter 0 to reselect login way)\n\n");
		return 1;
	}

	/* check if ID has digit and char */
	for (i=0; i < strlen(ID); i++)
	{
		if (ID[i] >= 48 && ID[i] <=57) //caculate number
		{
			num_digit++;
		}
		else if ((ID[i] >= 65 && ID[i] <= 90) || (ID[i] >= 97 && ID[i] <= 122)) //caculate letter
		{
			num_letter++;
		}
	}

	if (num_digit == 0 || num_letter == 0) //has not digit or letter
	{
		printf("The ID should be consist of numbers and letters entirely, please enter again.\n(Enter 0 to reselect login way)\n\n");
		return 1;
	}
	else if (num_digit + num_letter != strlen(ID)) //not be consist of number and letter entirely
	{
		printf("The ID should be consist of numbers and letters entirely, please enter again.\n(Enter 0 to reselect login way)\n\n");
		return 1;
	}
	else //correct ID
	{
		return 0;
	}
}

int check_email(char email[])
{
	int i; //use in circulation
	int at, dot, before_at, between_atdot, behind_dot; //rule the number of char of email

	if (strlen(email) > 30) //if char of email under 30
	{
		printf("The email cannot be more than 30 character, please enter again.\n(Enter 0 to return)\n\n");
		return 1;
	}
	else
	{
		for (i=0, at=0, dot=0; i<strlen(email); i++) //caculate the number of char of email
		{
			if (email[i] == '@')
			{
				if (dot != 0)
				{
					break;
				}
				else
				{
					at++;
					before_at = i;
				}
			}
			else if (email[i] == '.')
			{
				dot++;
				between_atdot = i - before_at - 1;
			}
		}

		behind_dot = i - before_at - between_atdot - 2;

		if (at != 1 || dot != 1 || before_at == 0 || between_atdot == 0 || behind_dot == 0)
		{
			printf("The format of email should like this ***@***.***, please enter again.\n(Enter 0 to return)\n\n");
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

int check_flight_num(char flight_enter[])
{
    int i; // use for circulation
    int letter_num, digit_num; //caculate letter and digit

    if (strlen(flight_enter) != 6)
    {
        printf("The format of Flight number should like this 'AA1111', please enter again.\n(Enter 0 to return menu.)\n\n");
        return 1;
    }
    else
    {
        for (i=0, letter_num=0; i<2; i++)
        {
            if ((flight_enter[i] >= 'A' && flight_enter[i] <= 'Z'))
            {
                letter_num++;
            }
        }

        if (letter_num != 2)
        {
            printf("The format of Flight number should like this 'AA1111', please enter again.\n(Enter 0 to return menu.)\n\n");
            return 1;
        }
        else
        {
            for (i=2, digit_num=0; i<6; i++)
            {
                if (flight_enter[i] >= '0' && flight_enter[i] <= '9')
                {
                    digit_num++;
                }
            }

            if (digit_num != 4)
            {
                printf("The format of Flight number should like this 'AA1111', please enter again.\n(Enter 0 to return menu.)\n\n");
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
}

int check_phone(char phone_enter[])
{
    int i, number;

    for (i=0, number=0; i<strlen(phone_enter); i++)
	{
        if (phone_enter[i] >= 48 && phone_enter[i] <=57)
        {
        number++;
        }
    }

    if (number != 11 || strlen(phone_enter) != 11)
    {
        printf("Phone number should be consist of 11 numbers.\n(Enter 0 to cancel register)\n\n");
        return 1;
    }
    else
    {
        return 0;
    }
}

int check_date(char date_enter[])
{
    int date[3]; //respect 1-year, 2-month, 3-day
    int day[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int i, number;

    /* input validation, check format */
    if (strlen(date_enter) != 10)
    {
        printf("The format of date should like '2016-09-01', please enter again.\n(Enter 0 to return)\n\n");
        return 1;
    }
    else
    {
        if (date_enter[4] != '-' || date_enter[7] != '-')
        {
            printf("The format of date should like '2016-09-01', please enter again.\n(Enter 0 to return)\n\n");
            return 1;
        }
        else
        {
            for (i=0, number=0; i<10; i++)
            {
                if (date_enter[i] >= '0' && date_enter[i] <= '9')
                {
                    number++;
                }
            }
        }
    }

    if (number != 8)
    {
        printf("The format of date should like '2016-09-01', please enter again.\n(Enter 0 to return)\n\n");
        return 1;
    }
    else
    {
        /* cut off date_enter to get year, month and day respectively */
        sscanf(date_enter, "%d-%d-%d", &date[0], &date[1], &date[2]);

        /* date validation */
        if ((date[0] % 4 == 0 && date[0]% 100 != 0) || date[0] % 400 == 0) //if leap-year
        {
            day[1] = 29;
        }

        if (date[1] <= 12 && date[1] >= 1 && date[2] >=1 && date[2] <= day[date[1] - 1])
        {
            return 0;
        }
        else
        {
            printf("Invalid Date, please enter again.(Enter 0 to return)\n\n");
            return 1;
        }
    }
}

int check_order_no(char order_no[])
{
    int i;
    char user[11], order_no_read[11];
    FILE *inOrders;

    for (i=0; i<strlen(order_no); i++)
    {
        if (order_no[i] > '9' || order_no[i] < '0')
        {
            printf("Please enter correct order No. consisted of 2 number, please enter again.\n(Enter 0 to return)\n\n");
            return 1;
        }
    }


    inOrders = fopen("Orders.txt", "r");
    while (!feof(inOrders))
    {
        fscanf(inOrders, "%s%*s%s", order_no_read, user);

        if (strcmp(user, id) == 0 && strcmp(order_no_read, order_no) == 0)
        {
            return 0;
        }
        else
        {
            fscanf(inOrders, "%*[^\n]%*c");
            continue;
        }
    }
    fclose(inOrders);

    printf("Please enter your order No.\n\n");
    return 1;
}

int row()
{
    int num=0;
    FILE *inFlight;

    inFlight = fopen("flight.txt", "r");
    while(!feof(inFlight))
    {
        fscanf(inFlight, "%*[^\n]%*c");
        num++;
    }
    fclose(inFlight);
    return num;
}

int row2()
{
    int num=0;
    FILE *inOrders;

    inOrders = fopen("Orders.txt", "r");
    while(!feof(inOrders))
    {
        fscanf(inOrders, "%*[^\n]%*c");
        num++;
    }
    fclose(inOrders);

    return num;
}

int get_order_no()
{
    int order_no=0;
    char read[7];
    FILE *inOrders;

    inOrders = fopen("Orders.txt", "r");
    while(!feof(inOrders))
    {
        fscanf(inOrders, "%*d%s", read);

        if (strcmp(read, "0") == 0 || strcmp(read, "1") == 0 || strcmp(read, "2") == 0)
        {
            fscanf(inOrders, "%*[^\n]%*c");
            order_no++;
            continue;
        }
        else
        {
            fscanf(inOrders, "%*[^\n]%*c");
            continue;
        }
    }
    fclose(inOrders);

    return order_no;
}

int ticket_can_buy()
{
    int num, seat_type, order_status, ticket_num=0;
    char user[11], time_now[11], flight[11], date[20];
    time_t timep;
    struct tm *timeinfo;
    FILE *inOrders;

    timep = time(NULL);
    timeinfo = localtime(&timep);
    strftime(time_now, sizeof(time_now), "%Y-%m-%d", timeinfo);

    inOrders = fopen("Orders.txt", "r");
    while (!feof(inOrders))
    {
        fscanf(inOrders, "%*d%d%s%10s", &order_status, user, date);

        if ((order_status == 0 || order_status == 1) && strcmp(user, id) == 0 &&
            strcmp(time_now, date) == 0)
        {
            fscanf(inOrders, "%*[^\n]%*c");

            while (!feof(inOrders))
            {
                fscanf(inOrders, "%d%s", &num, flight);

                if (strcmp(flight, "0") == 0 || strcmp(flight, "1") == 0 || strcmp(flight, "2") == 0)
                {
                    fseek(inOrders, -11L, 1);
                    break;
                }
                else
                {
                    fscanf(inOrders, "%*[^\n]%*c");
                    ticket_num += num;
                    continue;
                }
            }
        }
        else
        {
            fscanf(inOrders, "%*[^\n]%*c");
            continue;
        }
    }
    fclose(inOrders);

    return 5 - ticket_num;
}
