#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "data.h"
#include "helpdesk.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
void createAppointment()
{
	FILE *fp;
	int i = 0;
	int in = 0;
	int ch = 0;
	getchar(); //absorbs the previous '\n' char
	fflush(stdin);

	char fname[256];
	char lname[256];
	char date[11];
	char purpose[256];
	
	printf("Patient's First Name: ");
	in = 0;
	while((ch = getchar()) != '\n')
	{
		fname[in] = ch;
		in++;
	}
	fname[in++] = '\0';
	
	if(strlen((char*)fname) > 255)
	{
		printf("Invalid length!\n");
		return;
	}
	
	printf("Patient's Last Name: ");
	in = 0;
	while((ch = getchar()) != '\n')
	{
		lname[in] = ch;
		in++;
	}
	lname[in++] = '\0';
	if(strlen((char*)lname) > 255)
	{
		printf("Invalid length!\n");
		return;
	}
	printf("Date of Appointment: ");
	in = 0;
	while((ch = getchar()) != '\n')
	{
		date[in] = ch;
		in++;
	}
	date[in++] = '\0';
	if(strlen((char*)date) > 10)
	{
		printf("Invalid length!\n");
		return;
	}	
	printf("Purpose of Appointment: ");
	in = 0;
	while((ch = getchar()) != '\n')
	{
		purpose[in] = ch;
		in++;
	}
	purpose[in++] = '\0';
	if(strlen((char*)purpose) > 255)
	{
		printf("Invalid length!\n");
		return;
	}	
	
	char *string = malloc(sizeof(char) * 1025);
	
	strcpy(string, "");

	if(access("./appointments.bin", F_OK) != -1)
	{
		fp = fopen("./appointments.bin", "r+");
		while((i = fgetc(fp)) != EOF)
		{
			i--;
		}
	}
	else
	{
		fp = fopen("./appointments.bin", "w");
	}

	strcpy(string, "");
	strcat(string, "Date of Appointment: ");
	strcat(string, date);
	strcat(string, " | Patient: ");
	strcat(string, lname);
	strcat(string, ", ");
	strcat(string, fname);
	strcat(string, " | Reason: ");
	strcat(string, purpose);
	strcat(string, "\n");
	
	fprintf(fp, string);
	fclose(fp);
	
	//free(string);
	//free(fname);
	//free(lname);
	//free(date);
	//free(purpose);
}

void viewAppointments()
{
	FILE *fp = fopen("./appointments.bin", "r");
	
	if(fp == NULL)
	{
		printf("Could not open file\n");
		exit(1);
	}
	else
	{
		char buff[255];
		
		while(1)
		{
			fgets(buff, 255, (FILE*)fp);
			if(feof(fp))
				break;
			printf("%s", buff);
		}
		
		fclose(fp);
	}
}

