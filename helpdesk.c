#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "login.h"
#include "helpdesk.h"
#include "draw.h"
#include "data.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
void createAppointment()
{
	FILE *fp;
	fflush(stdin);
	drawAppointment();

	int i = 0;
	char *string = malloc(sizeof(char) * 1025);
	char *fname = malloc(sizeof(char*) * 256);
	char *lname = malloc(sizeof(char*) * 256);
	char *purpose = malloc(sizeof(char*) * 256);
	char *date = malloc(sizeof(char*) * 10);

	
	printf("Patient's First Name: ");
	strcpy(fname, sread(256));
	if(strlen(fname) > 255)
	{
		printf("Invalid length!\n");
		return;
	}
	
	printf("Patient's Last Name: ");
	strcpy(lname, sread(256));
	if(strlen(lname) > 255)
	{
		printf("Invalid length!\n");
		return;
	}
	
	printf("Date of Appointment: ");
	strcpy(date, sread(10));
	if(strlen(date) > 10)
	{
		printf("Invalid length!\n");
		return;
	}	
	
	printf("Purpose of Appointment: ");
	strcpy(purpose, sread(256));
	if(strlen(purpose) > 256)
	{
		printf("Invalid length!\n");
		return;
	}	
	
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
	strcpy(string, encrypt(string));
	strcat(string, "\n");
	
	fprintf(fp, string);
	
	strcpy(string, wspace(strlen(string)));
	free(string);
	
	strcpy(fname, wspace(strlen(fname)));
	free(fname);
	
	strcpy(lname, wspace(strlen(lname)));
	free(lname);
	
	strcpy(date, wspace(strlen(date)));
	free(date);
	
	strcpy(purpose, wspace(strlen(purpose)));
	free(purpose);
	
	fclose(fp);
}

void viewAppointments()
{
	FILE *fp = fopen("./appointments.bin", "r");
	
	if(fp == NULL)
	{
		printf("Error: Could not open \"appointments.bin\" file\n");
		pressEnterKey();
		return;
	}
	else
	{
		fflush(stdin);
		drawAppointmentList(fp);

		char buff[255];

		while(1)
		{
			fgets(buff, 255, (FILE*)fp);
			if(feof(fp)) break;
			printf("%s\n", decrypt(buff));
		}
		
		fclose(fp);
	}
}