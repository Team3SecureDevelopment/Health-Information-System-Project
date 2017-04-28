#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "data.h"
#include "login.h"
#include "draw.h"
 
/* login banner */
void drawLogin()
{
	/* clear the screen */
	system("clear");
	printf(".-----------.\n");
	printf("|    ###    | ':.:'  Health Information  ':.:'\n");
	printf("|    ###    |    ':. Data Record System .:'	\n");
	printf("| ######### |\n");
	printf("| ######### |           Version 1.6\n");
	printf("|    ###    |\n");
	printf("|    ###    |            By TEAM 3\n");
	printf("'-----------'\n");
	printf("\n");
}

/* draw the main menu depending on user */
void drawMenu(User currentUser)
{
	char *name = userGetName(currentUser);
	int type = userGetType(currentUser);
	system("clear");
	printf("\n-------------[ MAIN MENU ]-------------\n");
	printf(" Hello, %s\n", name);
	printf("---------------------------------------\n");
	printf("\n");
	
	/* if doctor */
	if(type == 0)
	{
		printf(" 1 | Search Patients\n");
		printf(" 2 | Search by Filter\n");
		printf(" 3 | Create New Patient\n");
		printf(" 4 | Delete Patient\n");
		printf(" 5 | Change Password\n");
		printf(" 6 | Log Off\n");
		printf("\n");
	}
	/* if nurse */
	else if(type == 1)
	{
		printf(" 1 | Search Patients\n");
		printf(" 2 | Search by Filter\n");
		printf(" 3 | Create New Patient\n");
		printf(" 4 | Change Password\n");
		printf(" 5 | Log Off\n");
		printf("\n");
	}
	/* if help desk */
	else if(type == 2)
	{
		printf(" 1 | View Appointments\n");
		printf(" 2 | Create New Appointment\n");
		printf(" 3 | Change Password\n");
		printf(" 4 | Log Off\n");
		printf("\n");
	}
	/* if auditor */
	else if(type == 3)
	{
		printf(" 1 | View System Logs\n");
		printf(" 2 | Change Password\n");
		printf(" 3 | Log Off\n");
		printf("\n");
	}
	/* if system admin */
	else if(type == 4)
	{
		printf(" 1 | View Users\n");
		printf(" 2 | Create New User\n");
		printf(" 3 | Delete User\n");
		printf(" 4 | Change Password\n");
		printf(" 5 | Log Off\n");
		printf("\n");
	}
	/* if for some reason invalid type */
	else
	{
		printf("Oops! You seem to have an invalid or unsupported user type.\n");
		printf("You will shortly be logged off the system.\n");
		printf("\n");
		printf("If you believe this to be in error, please contact the system administrator.\n");
		printf("\n");
		pressEnterKey();
	}
}

void drawPatientSearch(FILE *fp)
{
	int count = getUserCount(fp);
	system("clear");
	printf("\n-------------[ PATIENT SEARCH ]-------------\n");
	printf("Please enter the social security number of the patient.\n");
	printf("Patients in file: %d\n", count);
	printf("\n");
}

void drawFilteredSearch(FILE *fp)
{
	int count = getUserCount(fp);
	system("clear");
	printf("\n-------------[ FILTERED SEARCH ]-------------\n");
	printf("Here you can view all patients that meet a specific criteria.\n");
	printf("For more detailed information, please use Search Patient.\n");
	printf("Patients in file: %d\n", count);
	printf("\n");
}

void drawPatientInfo()
{
	system("clear");
	printf("\n-------------[ PATIENT INFO ]-------------\n");
	printf("\n");
}

void drawPatientNew()
{
	system("clear");
	printf("\n-------------[ CREATE NEW PATIENT ]-------------\n");
	printf("\n");
}

void drawAppointment()
{
	system("clear");
	printf("\n------------[ NEW APPOINTMENT ]------------\n");
	printf("\n");
}

void drawAppointmentList(FILE *fp)
{
	system("clear");
	printf("\n------------[ APPOINTMENT LIST ]------------\n");
	printf("\n");
}

void drawLogs()
{
	system("clear");
	printf("\n------------[ LOGS ]------------\n");
	printf("\n");
}

void drawViewUsers()
{
	system("clear");
	printf("\n------------[ USER LIST ]------------\n");
	printf("\n");	
}

void drawAddUser()
{
	system("clear");
	printf("\n------------[ ADD USER ]------------\n");
	printf("\n");
}

void drawPassword()
{
	system("clear");
	printf("\n------------[ CHANGE PASSWORD ]------------\n");
	printf("\n");
}
void drawExit()
{
	system("clear");
	printf("'':. You have been successfully logged off! .:''\n");
	printf("  ':.: The program will close momentarily :.:'\n");
	printf("\n");		
}