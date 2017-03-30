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
   fp = fopen("patients.bin","a");
   
   rewind(fp);
   
   if(fp == NULL)
   {
     	printf("File could not be opened\n");
      	exit(1);
   }
   else
   {
	char *social = malloc(sizeof(char) * 10);
	char *fname = malloc(sizeof(char) * MAX_CHAR);
	char *lname = malloc(sizeof(char) * MAX_CHAR);
	char *dob = malloc(sizeof(char) * 11);
   
      int height;
      int weight;
      int allergies;
      int smoker;
      int surgeries;
      int mental;

		char c = 0;
		
      char *string = malloc(sizeof(char) * (MAX_CHAR*2 + 10) + sizeof(int) * 6);
      char *buffer = malloc(sizeof(char) * (MAX_CHAR));
   
	  string[0] = '\0';
	  
      // read data
	printf("Patient First Name:");
      scanf("%s", fname);
	  if(strlen(fname) > MAX_CHAR)
	  {
		  printf("Invalid input length!\n");
		  exit(2);
	  }
   
      printf("Patient Last Name:");
      scanf("%s", lname);
      if(strlen(lname) > MAX_CHAR)
	  {
		  printf("Invalid input length!\n");
		  exit(2);
	  }
	  
      printf("Patient DOB: (mm/dd/yyyy):");
      scanf("%s", dob);
	  if(strlen(dob) > 10)
	  {
		  printf("Invalid input length!\n");
		  exit(2);
	  }
	  
      printf("Social Security: ");
	  scanf("%s", social);
	  if(strlen(social) > 9)
	  {
		  printf("Invalid input length!\n");
		  exit(2);
	  }
	  
      printf("Patient Height(cm):");
      scanf("%d", &height);
	  if(height > 300)
	  {
		  printf("Invalid input length!\n");
		  exit(2);
	  }
	  
      printf("Patient Weight(lbs): ");
      scanf("%d", &weight);
	  
   	  if(weight > 999)
	  {
		  printf("Invalid input length!\n");
		  exit(2);
	  }
	  
      printf("Do you have any known allergies?(y/n) ");
      scanf(" %c", &c);

	  if(c == 'y' || c == 'Y')
	  {
		  printf("Got a y!\n");
		  allergies = 0;
	  }
	  else if(c == 'n' || c == 'N')
	  {
		  printf("Got a n!\n");
		  allergies = 1;
	  }
	  else
	  {
		  printf("Invalid character!\n");
		  exit(3);
	  }
	  
      printf("Do you smoke?(y/n) ");
      scanf(" %c", &c);

	  if(c == 'y' || c == 'Y')
	  {
		  printf("Got a y!\n");
		  smoker = 0;
	  }
	  else if(c == 'n' || c == 'N')
	  {
		  printf("Got a n!\n");
		  smoker = 1;
	  }
	  else
	  {
		  printf("Invalid character!\n");
		  exit(3);
	  }
	  
      printf("Have you ever had any surgeries?(y/n) ");
      scanf(" %c", &c);

	  if(c == 'y' || c == 'Y')
	  {
		  printf("Got a y!\n");
		  surgeries = 0;
	  }
	  else if(c == 'n' || c == 'N')
	  {
		  printf("Got a n!\n");
		  surgeries = 1;
	  }
	  else
	  {
		  printf("Invalid character!\n");
		  exit(3);
	  }
	  
      printf("Have you ever been diagnosed for any mental illnesses?(y/n) ");
      scanf(" %c", &c);

	  if(c == 'y' || c == 'Y')
	  {
		  printf("Got a y!\n");
		  mental = 0;
	  }
	  else if(c == 'n' || c == 'N')
	  {
		  printf("Got a n!\n");
		  mental = 1;
	  }
	  else
	  {
		  printf("Invalid character!\n");
		  exit(3);
	  }
	  
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
	  
	  printf("String: %s\n", string);
	  
	  strncpy(string, encrypt(string), MAX_CHAR);
	  
	  printf("Encrypted: %s\n", string);

	  strcat(string, "\n");
	  fprintf(fp, string);
	  
	  printf("Decrypted: %s\n", decrypt(string));
	  
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
		char *ssn = malloc(sizeof(char) * 9);
		int hashvalue;
		int count = getUserCount(fp); //number of patients in file
		int i;
		
		/* draw the tui */
		drawPatientSearch(fp);
		
		/* set the cursor */
		printf("\x1b[0;0H]");
		printf("\x1b[26C"); //right 32
		printf("\x1b[13B"); //down 17
		scanf("%9s", ssn);
		
		hashvalue = hash(ssn);
		printf("Hash value is %d\n", hashvalue);
		
		/* we got the hash, so zero out the char */
		strcpy(ssn, "         ");
		printf("SSN string is now \"%s\"\n", ssn);
		//free(ssn);
		
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
				drawPatientInfo(newPatient);
			}
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
	char c = currentPatient->allergies;
	
	if(c == 'Y' || c == 'y')
	{
		return 0;
	}
	else return 1;
}

int patientHadSurgeries(Patient currentPatient)
{
	char c = currentPatient->surgeries;
	
	if(c == 'Y' || c == 'y')
	{
		return 0;
	}
	else return 1;
}

int patientIsSmoker(Patient currentPatient)
{
	char c = currentPatient->smoker;
	
	if(c == 'Y' || c == 'y')
	{
		return 0;
	}
	else return 1;
}

int patientMentalIllness(Patient currentPatient)
{
	char c = currentPatient->mental;
	
	if(c == 'Y' || c == 'y')
	{
		return 0;
	}
	else return 1;
}
