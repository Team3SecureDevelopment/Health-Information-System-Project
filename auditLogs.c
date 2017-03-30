#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "login.h"
#include "auditLogs.h"


/* run this program using the console pauser or add your own getch, system("pause") or input loop */
void writeLogs(User currentUser, char *purpose)
{
	FILE *fp;
	time_t current_time;
	char* c_time_string;
	int i = 0;
	
	char *string = malloc(sizeof(char) * 256);
	char *userType = malloc(sizeof(char) * 128);
	
	strcpy(string, "");
	
	if(userGetType(currentUser) == 0)
	{
		strcpy(userType, "Doctor");
	}
	else if(userGetType(currentUser) == 1)
	{
		strcpy(userType, "Nurse");
	}
	else if(userGetType(currentUser) == 2)
	{
		strcpy(userType, "HelpDesk");
	}
	else if(userGetType(currentUser) == 3)
	{
		strcpy(userType, "Auditor");
	}
	else if(userGetType(currentUser) == 4)
	{
		strcpy(userType, "Admin");
	}
	else strcpy(userType, "Unknown");
	
	if(access("logs.bin", F_OK) != -1)
	{
		fp = fopen("logs.bin", "r+");
		while((i = fgetc(fp)) != EOF)
		{
			i--;
		}
	}
	else
	{
		fp = fopen("logs.bin", "w");
	}
	
	current_time = time(NULL);
	
	c_time_string = ctime(&current_time);
	
	strncat(string, c_time_string, strlen(c_time_string)-1);
	strcat(string, " | ");
	strcat(string, userGetName(currentUser));
	strcat(string, " | ");
	strcat(string, userType);
	strcat(string, " | ");
	strcat(string, purpose);
	strcat(string, "\n");
	
	fprintf(fp, string);
	fclose(fp);
	
	free(string);
}

void readLogs()
{
	FILE *fp = fopen("logs.bin", "r");
	
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
