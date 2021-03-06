#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "login.h"
#include "auditLogs.h"
#include "draw.h"
#include "data.h"

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
	int drugs;
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
     	printf("Error: File \"patients.bin\" could not be opened!\n");
		pressEnterKey();
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
		int drugs;
		
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
			setAllergyInfo(hash(social));
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

		printf("Are you currently on any prescriptions?(y/n) ");
		strcpy(c, sread(3));
		if(c[0] == 'y' || c[0] == 'Y')
		{
			drugs = 1;
			setPrescriptionInfo(hash(social));
		}
		else if(c[0] == 'n' || c[0] == 'N')
		{
			drugs = 0;
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
		strcat(string, ",");
		
		snprintf(buffer, 4, "%d", drugs);
		strncat(string, buffer, 4);
		
		strncpy(string, encrypt(string), MAX_CHAR);
	
		strcat(string, "\n");
		fprintf(fp, string);
	  
		fclose(fp);
		
		printf("New patient successfully created!\n");
		pressEnterKey();
   }
}

void setAllergyInfo(int ssnhash)
{
	FILE *fp = fopen("./allergy.bin", "a");
	
	if(fp == NULL)
	{
		printf("Error: Could not find or open \"allergy.bin\" in the directory!\n");
		pressEnterKey();
		fclose(fp);
		return;
	}
	else
	{
		int MAX_CHAR = 256;
		
		char *string = malloc(sizeof(char*) * MAX_CHAR * 11);
		char *allergies = malloc(sizeof(char*) * MAX_CHAR * 10);
		
		printf("Please type in an allergy and then hit ENTER to save.\nFor multiple allergies, please separate each one by a comma:\n");
		printf("--> ");
		strcpy(allergies, sread(MAX_CHAR*10));
		
		/* format the data */
		snprintf(string, 9, "%d", ssnhash);
		strcat(string, "|");
		strcat(string, allergies);

		/* encrypt */
		strncpy(string, encrypt(string), MAX_CHAR*11+1);
		strcat(string, "\n");
		
		/* write to file */
		fprintf(fp, string);
		fclose(fp);
	}
}

void getAllergyInfo(int ssnhash)
{
	FILE *fp = fopen("./allergy.bin", "r");
	
	if(fp == NULL)
	{
		printf("Could not find or open \"allergy.bin\" in the directory!\n");
		pressEnterKey();
		fclose(fp);
		return;
	}
	else
	{
		int MAX_CHAR = 256;
		int count = getUserCount(fp); //number of patients in file
		int i;
		int found = 0;

		for(i = 0; i < count; ++i)
		{
			char *temp = malloc(sizeof(char*) * MAX_CHAR*11);
			char buffer[MAX_CHAR*11];
		
			strncpy(buffer, decrypt(getLine(fp, i)), MAX_CHAR*11);

			/* tokenize the line */
			temp = strtok(buffer, "|");
			
			/* hash match? */
			if(ssnhash == atoi(temp))
			{
				printf("Patient Allergies\n");
				printf("-----------------\n");
				
				/* get the number of commas for counting purposes */
				int comma = 0;
				int j = 0;
				char c = ' ';
				
				while(c != '\n')
				{
					c = buffer[j];
					
					if(c == ',')
					{
						comma++;
					}
					
					j++;
				}

				for(i = 0; i < comma+1; i++)
				{
					temp = strtok(NULL, ",");
					printf("%d.) %s\n", i+1, temp);
				}
				
				fclose(fp);
				return;
			}
		}
		
		if(found == 0)
		{
			printf("\nCould not find allergy information for the specified patient.\n");
			pressEnterKey();
		}
		
		fclose(fp);
	}	
}

void setPrescriptionInfo(int ssnhash)
{
	FILE *fp = fopen("./prescriptions.bin", "a");
	
	if(fp == NULL)
	{
		printf("Could not find or open \"prescriptions.bin\" in the directory!\n");
		pressEnterKey();
		fclose(fp);
		return;
	}
	else
	{
		int MAX_CHAR = 256;
		
		char *string = malloc(sizeof(char*) * MAX_CHAR * 11);
		char *prescriptions = malloc(sizeof(char*) * MAX_CHAR * 10);
		
		printf("Please type in a prescription and then hit ENTER to finish. For multiple prescriptions, please separate each one by a comma:\n");
		printf("--> ");
		strcpy(prescriptions, sread(MAX_CHAR*10));
		
		/* format the data */
		snprintf(string, 9, "%d", ssnhash);
		strcat(string, "|");
		strcat(string, prescriptions);

		/* encrypt */
		strncpy(string, encrypt(string), MAX_CHAR*11+1);
		strcat(string, "\n");
		
		/* write to file */
		fprintf(fp, string);
		fclose(fp);
	}
}

void getPrescriptionInfo(int ssnhash)
{
	FILE *fp = fopen("./prescriptions.bin", "r");
	
	if(fp == NULL)
	{
		printf("Could not find or open \"prescriptions.bin\" in the directory!\n");
		pressEnterKey();
		fclose(fp);
		return;
	}
	else
	{
		int MAX_CHAR = 256;
		int count = getUserCount(fp); //number of patients in file
		int i;
		int found = 0;

		for(i = 0; i < count; ++i)
		{
			char *temp = malloc(sizeof(char*) * MAX_CHAR*11);
			char buffer[MAX_CHAR*11];
		
			strncpy(buffer, decrypt(getLine(fp, i)), MAX_CHAR*11);

			/* tokenize the line */
			temp = strtok(buffer, "|");
			
			/* hash match? */
			if(ssnhash == atoi(temp))
			{
				printf("Patient Prescriptions\n");
				printf("---------------------\n");
				
				/* get the number of commas for counting purposes */
				int comma = 0;
				int j = 0;
				char c = ' ';
				
				while(c != '\n')
				{
					c = buffer[j];
					
					if(c == ',')
					{
						comma++;
					}
					
					j++;
				}

				for(i = 0; i < comma+1; i++)
				{
					temp = strtok(NULL, ",");
					printf("%d.) %s\n", i+1, temp);
				}
				
				fclose(fp);
				return;
			}
		}
		
		if(found == 0)
		{
			printf("\nCould not find prescription information for the specified patient.\n");
			pressEnterKey();
		}
		
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
		pressEnterKey();
		return;
	}
	else
	{
		int hashvalue;
		int count = getUserCount(fp); //number of patients in file
		int i;
		int found = 0;
		//char *
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
				int dr = atoi(strtok(NULL,","));
				
				Patient newPatient = createPatient(temp,lname,fname,dob,h,w,a,su,sm,m,dr);
				drawPatientInfo();
				
				printf("         First Name: %s\n", patientGetFirstName(newPatient));
				printf("          Last Name: %s\n", patientGetLastName(newPatient));
				printf("      Date of Birth: %s\n", patientGetDOB(newPatient));
				printf("        Height (cm): %d\n", patientGetHeight(newPatient));
				printf("        Weight (lb): %d\n", patientGetWeight(newPatient));
				printf("      Has allergies? %d\n", patientHasAllergies(newPatient));
				printf("   On Prescriptions? %d\n", patientOnPrescriptions(newPatient));				
				printf("    Is/was a smoker? %d\n", patientIsSmoker(newPatient));
				printf(" Previous surgeries? %d\n", patientHadSurgeries(newPatient));
				printf(" Has mental illness? %d\n", patientMentalIllness(newPatient));
				printf("\n");
				
				/* if they have allergies, list them */
				if(a == 1)
				{
					printf("\nPress [ENTER] to view allergy information\n");
					getchar();
					getAllergyInfo(hashvalue);
					printf("\n");
				}
				
				/* if they have allergies, list them */
				if(dr == 1)
				{
					printf("\nPress [ENTER] to view prescription information\n");
					getchar();
					getPrescriptionInfo(hashvalue);
					printf("\n");
				}
				
				pressEnterKey();
				
				fclose(fp);
				return;
			}
		}
		
		if(found == 0)
		{
			printf("\nCould not find patient under the social security number.\n");
			pressEnterKey();
		}
		
		fclose(fp);
	}
}

void filteredSearch()
{
	int MAX_CHAR = 256;
	int j = 1;
	
	FILE *fp = fopen("./patients.bin", "r");
	
	if(fp == NULL)
	{
		printf("Could not find \"patients.bin\" in the directory!\n");
		pressEnterKey();
		return;
	}
	else
	{
		int value;
		int count = getUserCount(fp); //number of patients in file
		int i;
		
		char *input = malloc(sizeof(char*) * MAX_CHAR);
		
		/* draw the tui */
		drawFilteredSearch(fp);
		
		printf("Please select search parameter:\n");
		printf(" 1 | Patients with Allergies\n");
		printf(" 2 | Patients with Prior Surgeries\n");
		printf(" 3 | Patients who are Smokers\n");
		printf(" 4 | Patients with Mental Illnesses\n");
		printf(" 5 | Patients who are currently on Prescriptions\n");
		printf("\n");
		
		printf("--> ");
		strncpy(input, sread(1), 1);

		if((int)strlen(input) < 2)
		{
			value = atoi(input);
			free(input);
		}
		else
		{
			printf("\nInvalid search parameter!\n");
			pressEnterKey();
			return;
		}

		sleep(5);
		
		system("clear");
		
		/* drawing elements */
		if(value == 1)
		{
			printf("---[ ALLERGIES ]---\n\n");
		}
		else if(value == 2)
		{
			printf("---[ PRIOR SURGERIES ]---\n\n");			
		}
		else if(value == 3)
		{
			printf("---[ SMOKERS ]---\n\n");			
		}
		else if(value == 4)
		{
			printf("---[ MENTAL ILLNESS ]---\n\n");			
		}
		else if(value == 5)
		{
			printf("---[ PRESCRIPTIONS ]---\n\n");			
		}
		else
		{
			printf("Invalid search parameter!\n");
			pressEnterKey();
			return;
		}
		
		for(i = 0; i < count; ++i)
		{
			char *temp = malloc(sizeof(char*) * MAX_CHAR);
			char buffer[MAX_CHAR];
		
			strncpy(buffer, decrypt(getLine(fp, i)), MAX_CHAR);
			
			/* tokenize the line */
			temp = strtok(buffer, ",");
			
			char *lname = strtok(NULL, ",");
			char *fname = strtok(NULL, ",");
			char *dob = strtok(NULL, ",");
			int h = atoi(strtok(NULL,","));
			int w = atoi(strtok(NULL,","));
			int a = atoi(strtok(NULL,","));
			int su = atoi(strtok(NULL,","));
			int sm = atoi(strtok(NULL,","));
			int m = atoi(strtok(NULL,","));
			int dr = atoi(strtok(NULL,","));
			
			Patient newPatient = createPatient(temp,lname,fname,dob,h,w,a,su,sm,m,dr);
			
			if(value == 1)
			{
				if(patientHasAllergies(newPatient))
				{
					printf("%3d.) %s, %s\n", j, lname, fname);
					j++;
				}
			}
			
			if(value == 2)
			{
				if(patientHadSurgeries(newPatient))
				{
					printf("%3d.) %s, %s\n", j, lname, fname);
					j++;
				}
			}
			
			if(value == 3)
			{
				if(patientIsSmoker(newPatient))
				{
					printf("%3d.) %s, %s\n", j, lname, fname);
					j++;
				}
			}
			
			if(value == 4)
			{
				if(patientMentalIllness(newPatient))
				{
					printf("%3d.) %s, %s\n", j, lname, fname);
					j++;
				}
			}
			
			if(value == 5)
			{
				if(patientOnPrescriptions(newPatient))
				{
					printf("%3d.) %s, %s\n", j, lname, fname);
					j++;
				}
			}
		}
		
		fclose(fp);
	}
	
	pressEnterKey();
}

Patient createPatient(char *social, char *lastname, char *firstname, char *dob, int height, int weight, char allergies, char surgeries, char smoker, char mental, char drugs)
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
	newPatient->drugs = drugs;
	
	return newPatient;
}

void deletePatient(User currentDoctor)
{
	int MAX_CHAR = 256;
	FILE *fp = fopen("./patients.bin", "r");
	FILE *nfp = fopen("./temp2", "a");

	if(fp == NULL)
	{
		printf("Error: Could not locate \"patients.bin\" in the directory.\n");
		pressEnterKey();
		return;
	}
	else
	{
		system("clear");
		printf("\n-------------[ DELETE PATIENT ]-------------\n");
		
		char *temp = malloc(sizeof(char*) * MAX_CHAR);
		char *line = malloc(sizeof(char*) * MAX_CHAR);
		char *social = malloc(sizeof(char*) * MAX_CHAR);
		
		char buffer[MAX_CHAR];

		/* get the user to delete */
		printf("\nSocial Security of the patient to delete: ");
		strcpy(social, sread(9));

		/* go ahead and get the hash value, overwrite the string from memory */
		int hashsocial = hash(social);
		strncpy(social, wspace(strlen(social)), strlen(social));
		free(social);
		
		int length = getUserCount(fp);
		int i;
		int flag = 0;
		int found = 0;
		
		rewind(fp);
		rewind(nfp);
		
		/* go through userdata.bin until we find the user */
		for(i = 0; i < length; i++)
		{
			fgets(line, MAX_CHAR, fp);
			strncpy(buffer, decrypt(line), strlen(line));

			/* tokenize the line */
			temp = strtok(buffer, ",");
			
			/* check if the social security hash matches */
			if(atoi(temp) == hashsocial && found == 0)
			{
				found = 1;
				
				/* temporary holder for first, last name */
				char *fname = malloc(sizeof(char*) * MAX_CHAR);
				char *lname = malloc(sizeof(char*) * MAX_CHAR);
				temp = strtok(NULL, ",");
				strcpy(lname, temp);
				temp = strtok(NULL, ",");
				strcpy(fname, temp);
				
				printf("You are about to delete patient: %s, %s from the system. This action cannot be undone.\n", lname, fname);
				printf("Proceed? (Y\\N) ");

				char *c = malloc(sizeof(char*) * 3);
				strcpy(c, sread(1));
				if(c[0] == 'y' || c[0] == 'Y')
				{
					flag = 1;
				}
				else if(c[0] == 'n' || c[0] == 'N')
				{
					flag = 0;
				}
				else
				{
					printf("Invalid character!\n");
					return;
				}

				/* if admin says yes, then proceed */
				if(flag == 1)
				{
					/* verify admin's password */
					if(verify(currentDoctor))
					{
						/* we aren't going to edit the data as much as we won't copy, so continue */
						char *string = malloc(sizeof(char*) * MAX_CHAR);
						
						strcpy(string, "Deleted patient: ");
						strcat(string, lname);
						strcat(string, ", ");
						strcat(string, fname);
						strcat(string, " from the system");
						
						printf("\nPatient: %s, %s (%d) successfully deleted from the system.\n", lname, fname, hashsocial);
						pressEnterKey();
						writeLogs(currentDoctor, string);
						free(string);
						free(lname);
						free(fname);
					}
				}
				else if(flag == 0)
				{
					printf("\nPatient deletion cancelled. The file has not been changed.\n");
					pressEnterKey();
					
					writeLogs(currentDoctor, "Patient delete cancelled");
					fclose(fp);
					fclose(nfp);
					remove("./temp2");
					return;
				}
			}
			else
			{
				/* copy over the users who are not affected */
				fprintf(nfp, line);
			}
		}

		fclose(fp);
		fclose(nfp);
		
		if(found == 0)
		{
			printf("\nCould not find the specified patient in the file.\n");
			pressEnterKey();
		}
		else
		{		
			remove("./patients.bin");
			rename("./temp2", "./patients.bin");		
		}

	}
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

int patientOnPrescriptions(Patient currentPatient)
{
	return currentPatient->drugs;
}