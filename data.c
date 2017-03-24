#include <stdio.h>
#include <string.h>

int main()
{ 
   char fname[50];
   char lname[50];
   char dob[50];
   char height[50];
   char weight[50];
   char allergies[50];
   char smoker[50];
   char history[50];
   char mental[50];

   // open file
   FILE *fileptr;
   fileptr = fopen("data.txt","w");
   if(!fileptr)
   {
      printf("File could not be opened\n");
      return -1;
   }

   // read data
   printf("Patient First Name:");
   scanf(" %s", fname);
   printf("Patient Last Name:");
   scanf(" %s", lname);
   printf("Patient DOB: (mm/dd/yyyy):");
   scanf(" %s", dob);
   printf("Patient Height(cm):");
   scanf(" %s", height);
   printf("Patient Weight(lbs):");
   scanf(" %s", weight);
   printf("Do you have any known allergies?(y/n)");
   scanf(" %s", allergies);
   printf("Do you smoke?(y/n)");
   scanf(" %s", smoker);
   printf("Have you ever had any surgeries?(y/n)");
   scanf(" %s", history);
   printf("Have you ever been diagnosed for any mental illnesses?(y/n)");
   scanf(" %s", mental);

   // save data
   fprintf(fileptr, "%s,%s,%s,%s,%s,%s,%s,%s,%s,", fname, lname, dob, height, weight, allergies, smoker, history, mental);

   // close file
   fclose(fileptr);
   
   return 0;
}
