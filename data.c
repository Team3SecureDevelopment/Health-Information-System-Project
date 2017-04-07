#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "login.h"
#include "data.h"
#include "draw.h"

struct patient
{
	char *fname;
	char *lname;
	char *dob;
	char *social;
	
	int height;
	int weight;
	
	int allergies;
	int surgeries;
	int smoker;
	int mental;
};

void addNewPatient()
{ 
   int MAX_CHAR = 256;
   
   // open file
   FILE *fp;
   fp = fopen("./patients.bin","a");
   
   rewind(fp);
   
   if(fp == NULL)
   {
     	printf("File could not be opened\n");
      	exit(1);
   }
   else
   {
		drawPatientNew();
		
		int height;
		int weight;
		int allergies;
		int smoker;
		int surgeries;
		int mental;

		char *c = malloc(sizeof(char*) * 3);
		
		char *social = malloc(sizeof(char*) * 10);
		char *dob = malloc(sizeof(char*) * 11);
		char *fname = malloc(sizeof(char*) * MAX_CHAR);
		char *lname = malloc(sizeof(char*) * MAX_CHAR);
		char *buffer = malloc(sizeof(char*) * (MAX_CHAR));
		char *string = malloc(sizeof(char*) * (MAX_CHAR*2 + 10) + sizeof(int) * 6);
		string[0] = '\0';
	  
		// read data
		printf("Patient First Name: ");
		strcpy(fname, sread(MAX_CHAR));
		
		if(strlen(fname) > MAX_CHAR)
		{
			printf("Invalid input length!\n");
			return;
		}
   
		printf("Patient Last Name: ");
		strcpy(lname, sread(MAX_CHAR));
		if(strlen(lname) > MAX_CHAR)
		{
			printf("Invalid input length!\n");
			return;
		}
	  
		printf("Patient DOB: (mm/dd/yyyy): ");
		strcpy(dob, sread(10));
		
		if(strlen(dob) > 10)
		{
			printf("Invalid input length!\n");
			return;
		}
	  
		printf("Social Security: ");
		strcpy(social, sread(9));
		if(strlen(social) > 9)
		{
			printf("Invalid input length!\n");
			return;
		}
	  
		printf("Patient Height(cm): ");
		height = atoi(sread(3));
		if(height > 300)
		{
			printf("Invalid input length!\n");
			return;
		}
	  
		printf("Patient Weight(lbs): ");
		weight = atoi(sread(3));
		if(weight > 999)
		{
			printf("Invalid input length!\n");
			return;
		}
	  
		printf("Do you have any known allergies?(y/n) ");
		strcpy(c, sread(3));
		if(c[0] == 'y' || c[0] == 'Y')
		{
			allergies = 1;
		}
		else if(c[0] == 'n' || c[0] == 'N')
		{
			allergies = 0;
		}
		else
		{
			printf("Invalid character!\n");
			return;
		}
	  
		printf("Do you smoke?(y/n) ");
		strcpy(c, sread(3));
		if(c[0] == 'y' || c[0] == 'Y')
		{
			smoker = 1;
		}
		else if(c[0] == 'n' || c[0] == 'N')
		{
			smoker = 0;
		}
		else
		{
			printf("Invalid character!\n");
			return;
		}
	  
		printf("Have you ever had any surgeries?(y/n) ");
		strcpy(c, sread(3));
		if(c[0] == 'y' || c[0] == 'Y')
		{
			surgeries = 1;
		}
		else if(c[0] == 'n' || c[0] == 'N')
		{
			surgeries = 0;
		}
		else
		{
			printf("Invalid character!\n");
			return;
		}
	  
		printf("Have you ever been diagnosed for any mental illnesses?(y/n) ");
		strcpy(c, sread(3));
		if(c[0] == 'y' || c[0] == 'Y')
		{
			mental = 1;
		}
		else if(c[0] == 'n' || c[0] == 'N')
		{
			mental = 0;
		}
		else
		{
			printf("Invalid character!\n");
			return;
		}
	  
		fflush(stdin);
		
		/* lets not store the SSN in plain text */
		snprintf(buffer, 9, "%d", hash(social));
		strncat(string, buffer, 9);
		strcat(string, ",");
	  
		strncat(string, lname, MAX_CHAR);
		strcat(string, ",");
	  
		strncat(string, fname, MAX_CHAR);
		strcat(string, ",");
	  
		strncat(string, dob, 10);
		strcat(string, ",");
	  
		snprintf(buffer, 12, "%d", height);
		strncat(string, buffer, 12);
		strcat(string, ",");
	  
		snprintf(buffer, 12, "%d", weight);
		strncat(string, buffer, 12);
		strcat(string, ",");
	  
		snprintf(buffer, 4, "%d", allergies);
		strncat(string, buffer, 4);
		strcat(string, ",");
	  
		snprintf(buffer, 4, "%d", smoker);
		strncat(string, buffer, 4);
		strcat(string, ",");
	  
		snprintf(buffer, 4, "%d", surgeries);
		strncat(string, buffer, 4);
		strcat(string, ",");
	  
		snprintf(buffer, 4, "%d", mental);
		strncat(string, buffer, 4);
	  
		strncpy(string, encrypt(string), MAX_CHAR);
	
		strcat(string, "\n");
		fprintf(fp, string);
	  
		fclose(fp);
   }
}

void findPatient()
{
	int MAX_CHAR = 256;
	
	FILE *fp = fopen("./patients.bin", "r");
	
	if(fp == NULL)
	{
		printf("Could not find \"patients.bin\" in the directory!\n");
		exit(1);
	}
	else
	{
		int hashvalue;
		int count = getUserCount(fp); //number of patients in file
		int i;
		int found = 0;
		/* draw the tui */
		drawPatientSearch(fp);
		
		printf("SSN to search: ");
		hashvalue = hash(sread(9));

		for(i = 0; i < count; ++i)
		{
			char *temp = malloc(sizeof(char*) * MAX_CHAR);
			char buffer[MAX_CHAR];
		
			strncpy(buffer, decrypt(getLine(fp, i)), MAX_CHAR);
			
			/* tokenize the line */
			temp = strtok(buffer, ",");
			
			/* hash match? */
			if(hashvalue == atoi(temp))
			{
				found = 1;
				char *lname = strtok(NULL, ",");
				char *fname = strtok(NULL, ",");
				char *dob = strtok(NULL, ",");
				int h = atoi(strtok(NULL,","));
				int w = atoi(strtok(NULL,","));
				int a = atoi(strtok(NULL,","));
				int su = atoi(strtok(NULL,","));
				int sm = atoi(strtok(NULL,","));
				int m = atoi(strtok(NULL,","));
				
				Patient newPatient = createPatient(temp,lname,fname,dob,h,w,a,su,sm,m);
				drawPatientInfo();
				
				printf("@             First Name: %s\n", patientGetFirstName(newPatient));
				printf("@              Last Name: %s\n", patientGetLastName(newPatient));
				printf("@          Date of Birth: %s\n", patientGetDOB(newPatient));
				printf("@            Height (cm): %d\n", patientGetHeight(newPatient));
				printf("@            Weight (lb): %d\n", patientGetWeight(newPatient));
				printf("@          Has allergies? %d\n", patientHasAllergies(newPatient));
				printf("@        Is/was a smoker? %d\n", patientIsSmoker(newPatient));
				printf("@     Previous surgeries? %d\n", patientHadSurgeries(newPatient));
				printf("@     Has mental illness? %d\n", patientMentalIllness(newPatient));
				printf("\n");
				
				fclose(fp);
				return;
			}
		}
		
		if(found == 0)
		{
			printf("\nCould not find patient under the social security number.\n");
		}
		
		fclose(fp);
	}
}

Patient createPatient(char *social, char *lastname, char *firstname, char *dob, int height, int weight, char allergies, char surgeries, char smoker, char mental)
{
	Patient newPatient = malloc(sizeof(*newPatient));
	
	newPatient->fname = malloc(sizeof(char) * 256);
	newPatient->fname = firstname;
	
	newPatient->lname = malloc(sizeof(char)*256);
	newPatient->lname = lastname;
	
	newPatient->dob = malloc(sizeof(char)*10);
	newPatient->dob = dob;
	
	newPatient->social = malloc(sizeof(char)*9);
	newPatient->social = social;
	
	newPatient->height = height;
	newPatient->weight = weight;
	
	newPatient->allergies = allergies;
	newPatient->smoker = smoker;
	newPatient->surgeries = surgeries;
	newPatient->mental = mental;
	
	return newPatient;
}

char *patientGetFirstName(Patient currentPatient)
{
	return currentPatient->fname;
}

char *patientGetLastName(Patient currentPatient)
{
	return currentPatient->lname;
}

char *patientGetDOB(Patient currentPatient)
{
	return currentPatient->dob;
}

char *patientGetSocial(Patient currentPatient)
{
	return currentPatient->social;
}

int patientGetHeight(Patient currentPatient)
{
	return currentPatient->height;
}

int patientGetWeight(Patient currentPatient)
{
	return currentPatient->weight;
}

int patientHasAllergies(Patient currentPatient)
{
	return currentPatient->allergies;
}

int patientHadSurgeries(Patient currentPatient)
{
	return currentPatient->surgeries;
}

int patientIsSmoker(Patient currentPatient)
{
	return currentPatient->smoker;
}

int patientMentalIllness(Patient currentPatient)
{
	return currentPatient->mental;
}
