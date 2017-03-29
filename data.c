#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "login.h"
#include "data.h"



void addPatient()
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
	char *social = malloc(sizeof(char) * 9);
	char *fname = malloc(sizeof(char) * MAX_CHAR);
	char *lname = malloc(sizeof(char) * MAX_CHAR);
	char *dob = malloc(sizeof(char) * 10);
   
      char *height;
      char *weight;
      char allergies;
      char smoker;
      char history;
      char mental;

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
      scanf("%s", height);
	  if(strlen(height) > 3)
	  {
		  printf("Invalid input length!\n");
		  exit(2);
	  }
	  
      printf("Patient Weight(lbs):");
      scanf("%s", weight);
   	  if(strlen(weight) > 3)
	  {
		  printf("Invalid input length!\n");
		  exit(2);
	  }
	  
      printf("Do you have any known allergies?(y/n)");
      scanf("%c", &allergies);
	  if(allergies != 'y' || allergies != 'Y' || allergies != 'n' || allergies != 'N')
	  {
		  printf("Invalid input length!\n");
		  exit(2);
	  }
	  
      printf("Do you smoke?(y/n)");
      scanf("%c", &smoker);
 	  if(smoker != 'y' || smoker != 'Y' || smoker != 'n' || smoker != 'N')
	  {
		  printf("Invalid input length!\n");
		  exit(2);
	  }  
	  
      printf("Have you ever had any surgeries?(y/n)");
      scanf("%c", &history);
      if(history != 'y' || history != 'Y' || history != 'n' || history != 'N')
	  {
		  printf("Invalid input length!\n");
		  exit(2);
	  }
	  
      printf("Have you ever been diagnosed for any mental illnesses?(y/n)");
      scanf("%c", &mental);
 	  if(mental != 'y' || mental != 'Y' || mental != 'n' || mental != 'N')
	  {
		  printf("Invalid input length!\n");
		  exit(2);
	  }
	  
	  strncat(string, social, 9);
	  strcat(string, ",");
	  
	  strncat(string, lname, MAX_CHAR);
      strcat(string, ",");
	  
	  strncat(string, fname, MAX_CHAR);
      strcat(string, ",");
	  
	  strncat(string, dob, 10);
      strcat(string, ",");
	  
	  snprintf(buffer, 4, "%d", height);
	  strncat(string, buffer, 4);
      strcat(string, ",");
	  
	  snprintf(buffer, 4, "%d", weight);
	  strncat(string, buffer, 4);
      strcat(string, ",");
	  
	  snprintf(buffer, 4, "%d", allergies);
	  strncat(string, buffer, 4);
      strcat(string, ",");
	  
	  snprintf(buffer, 4, "%d", smoker);
	  strncat(string, buffer, 4);
      strcat(string, ",");
	  
	  snprintf(buffer, 4, "%d", history);
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
	  
	  //free(string);
	  //free(buffer);
	  //free(lname);
	  //free(fname);
	  //free(dob);
   }
}
