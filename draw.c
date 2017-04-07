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
	printf("| ######### |           Version 1.2\n");
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

	printf("\n-------------[ MAIN MENU ]-------------\n");
	printf(" Hello, %s\n", name);
	printf("---------------------------------------\n");
	printf("\n");
	
	/* if doctor/nurse */
	if(type == 0 || type == 1)
	{
		printf(" 1 | Search Patients\n");
		printf(" 2 | Create New Patient\n");
		printf(" 3 | Change Password\n");
		printf(" 4 | Log Off\n");
		printf("\n");
	}
	/* if help desk */
	else if(type == 2)
	{
		printf(" 1 | View Appointments\n");
		printf(" 2 | Create New Appointment\n");
		printf(" 3 | Delete Appointment\n");
		printf(" 4 | Change Password\n");
		printf(" 5 | Log Off\n");
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
		sleep(4);
	}
}

void drawPatientSearch(FILE *fp)
{
	int count = getUserCount(fp);
	
	printf("\n-------------[ PATIENT SEARCH ]-------------\n");
	printf("Please enter the social security number of the patient.\n");
	printf("Patients in file: %d\n", count);
	printf("\n");
}

void drawPatientInfo()
{
	printf("\n-------------[ PATIENT INFO ]-------------\n");
	printf("\n");
}

void drawAppointment()
{
	printf("\n------------[ NEW APPOINTMENT ]------------\n");
	printf("\n");
}

void drawAppointmentList(FILE *fp)
{
	printf("\n------------[ APPOINTMENT LIST ]------------\n");
	printf("\n");
}

void drawLogs()
{
	printf("\n------------[ LOGS ]------------\n");
	printf("\n");
}

void drawViewUsers()
{
	printf("\n------------[ USER LIST ]------------\n");
	printf("\n");	
}

void drawAddUser()
{
	printf("\n------------[ ADD USER ]------------\n");
	printf("\n");
}

void drawPassword()
{
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
