#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int MAX_CHAR = 256;

typedef struct patient* Patient;

typedef struct user* User;

typedef struct session* Session;

struct session
{
	User currentUser;
	time_t loginTime;
};

struct user
{
	char *name;
	int type;
};

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

/* PROTOTYPES */
void writeLogs(User currentUser, char *purpose);
void readLogs();
void addNewPatient();
void findPatient();
Patient createPatient(char*, char*, char*, char*, int, int, char, char, char, char, char);
void setAllergyInfo(int);
void getAllergyInfo(int);
void setPrescriptionInfo(int);
void getPrescriptionInfo(int);
char *patientGetFirstName(Patient);
char *patientGetLastName(Patient);
char *patientGetDOB(Patient);
char *patientGetSocial(Patient);
int patientGetHeight(Patient);
int patientGetWeight(Patient);
int patientIsSmoker(Patient);
int patientHadSurgeries(Patient);
int patientMentalIllness(Patient);
int patientHasAllergies(Patient);
int patientOnPrescriptions(Patient);
void filteredSearch();
void deletePatient(User currentDoctor);
void drawMenu(User);
void drawLogin();
void drawPatientSearch(FILE *fp);
void drawFilteredSearch(FILE *fp);
void drawPatientInfo();
void drawPatientNew();
void drawAppointment();
void drawAppointmentList(FILE *fp);
void drawLogs();
void drawViewUsers();
void drawAddUser();
void drawPassword();
void drawExit();
void createAppointment();
void viewAppointments();
char *encrypt(char *);
char *decrypt(char *);
User getUser(char *, int);
Session createNewSession(User, unsigned long int);
User sessionGetUser(Session);
time_t sessionGetLoginTime(Session);
char *userGetName(User);
int userGetType(User);
Session  authenticate();
int hash(char *);
int getUserCount(FILE *fp);
char *getLine(FILE *fp, int);
User createNewUser(char *, int);
void addUser();
void viewUsers();
char *sread(int size);
char *wspace(int size);
void changepass(User);
void pressEnterKey();
int verify(User);
void deleteUser(User);
char *createPassword();

////////////////
/* MAIN FUNCTION */
int main()
{
	Session newSession = authenticate();

	if(newSession == NULL)
	{
		printf("You are unable to be authenticated.\n");
		exit(1);
	}
	else if(newSession != NULL)
	{
		/* should we keep running? */
		int active = 0;
		User currentUser = sessionGetUser(newSession);
		writeLogs(currentUser, "Login");
		
		int menuchoice;
		int type = userGetType(currentUser);
		char *menustring = malloc(sizeof(char) * 2);

		printf("\033[2J");

		while(active == 0)
		{
			/* user authenticated, draw the menu */
			drawMenu(currentUser);
		
			if(type >= 0 && type < 5)
			{
				/* now we need to get the user input */
				printf("Please enter a choice and press ENTER -> ");
				strcpy(menustring, sread(1));
				menuchoice = atoi(menustring);
				free(menustring);
				
				if(menuchoice > 6)
				{
					printf("\nInvalid choice. Please try again.\n");
				}
			}
			else active = 1;

			/* doctor */
			if(type == 0)
			{
				if(menuchoice == 1)
				{
					/* search patients */
					writeLogs(currentUser, "Menu -> Find Patient");
					findPatient();						
				}
				else if(menuchoice == 2)
				{
					/* create new patient */
					if(verify(currentUser))
					{					
						writeLogs(currentUser, "Menu -> Filtered Patient Search");
						filteredSearch();
					}
				}
				else if(menuchoice == 3)
				{
					/* create new patient */
					if(verify(currentUser))
					{					
						writeLogs(currentUser, "Menu -> Create New Patient");
						addNewPatient();
					}
				}
				else if(menuchoice == 4)
				{
					/* create new patient */
					if(verify(currentUser))
					{					
						writeLogs(currentUser, "Menu -> Delete Patient");
						deletePatient(currentUser);
					}
				}
				else if(menuchoice == 5)
				{
					/* change password */		
					writeLogs(currentUser, "Menu -> Change Password");
					changepass(currentUser);
				}
				else if(menuchoice == 6)
				{
					/* log off */
					active = 1; //set flag
				}
				else continue;
			}
			else if(type == 1)
			{
				if(menuchoice == 1)
				{
					/* search patients */
					writeLogs(currentUser, "Menu -> Find Patient");
					findPatient();						
				}
				else if(menuchoice == 2)
				{
					/* filtered patient search */
					if(verify(currentUser))
					{					
						writeLogs(currentUser, "Menu -> Filtered Patient Search");
						filteredSearch();
					}
				}
				else if(menuchoice == 3)
				{
					/* create new patient */
					if(verify(currentUser))
					{					
						writeLogs(currentUser, "Menu -> Create New Patient");
						addNewPatient();
					}
				}
				else if(menuchoice == 4)
				{
					/* change password */		
					writeLogs(currentUser, "Menu -> Change Password");
					changepass(currentUser);
				}
				else if(menuchoice == 5)
				{
					/* log off */
					active = 1; //set flag
				}
				else continue;
			}
			/* help desk */
			else if(type == 2)
			{
				if(menuchoice == 1)
				{
					/* view appointments */
					if(verify(currentUser))
					{					
						writeLogs(currentUser, "Menu -> View Appointments");
						viewAppointments();
					}
				}
				else if(menuchoice == 2)
				{
					/* create new appointment */
					writeLogs(currentUser, "Menu -> Create New Appointment");
					createAppointment();
				}
				else if(menuchoice == 3)
				{
					/* change password */
					writeLogs(currentUser, "Menu -> Change Password");
					changepass(currentUser);
				}
				else if(menuchoice == 4)
				{
					/* log off */
					active = 1;
				}
				else continue;
			}
			/* auditor */
			else if(type == 3)
			{
				if(menuchoice == 1)
				{
					/* view logs */
					writeLogs(currentUser, "Menu -> View Logs");
					readLogs();
				}
				else if(menuchoice == 2)
				{
					/* change password */
					writeLogs(currentUser, "Menu -> Change Password");
					changepass(currentUser);
				}
				else if(menuchoice == 3)
				{
					/* log off */
					active = 1;
				}
				else continue;
			}
			/* admin */
			else if(type == 4)
			{
				if(menuchoice == 1)
				{
					/* view users */
					writeLogs(currentUser, "Menu -> View Users");
					viewUsers();
				}
				else if(menuchoice == 2)
				{
					/* create user */
					if(verify(currentUser))
					{
						writeLogs(currentUser, "Menu -> Create New User");
						addUser();
					}
				}
				else if(menuchoice == 3)
				{
					/* delete user */
					if(verify(currentUser))
					{					
						writeLogs(currentUser, "Menu -> Delete User");
						deleteUser(currentUser);
					}
				}
				else if(menuchoice == 4)
				{
					/* change password */
					writeLogs(currentUser, "Menu -> Change Password");
					changepass(currentUser);
				}
				else if(menuchoice == 5)
				{
					/* log off */
					active = 1;
				}
				else continue;
			}
		}
		
		time_t departure = time(NULL);
		
		/* show exit screen */
		drawExit();
		
		/* get departure time and format user's duration */
		char *string = malloc(sizeof(char*) * 256);
		char *time = malloc(sizeof(char*) * 128);;
		
		snprintf(time, 128, "%.2f", ((float )departure - (float )sessionGetLoginTime(newSession)));
		strcpy(string, "Log Off -> Time duration of ");
		strcat(string, time);
		strcat(string, " seconds");
		
		/* write to logs */
		writeLogs(currentUser, string);
		
		/* free */
		free(time);
		free(string);
		free(currentUser);
		free(menustring);
	}

	free(newSession);
	sleep(2);
	printf("\033[2J");
	return 0;
}
///////////////////

/* LOGIN INFORMATION */
/* gets the user input, checks it against
 * the data file, and if user exists,
 * returns new session, else fails and returns NULL
 */
Session authenticate()
{
	char *username = malloc(sizeof(char*) * MAX_CHAR);
	char *password = malloc(sizeof(char*) * MAX_CHAR);
	time_t logintime = time(NULL);

	drawLogin();

	printf("To access the program, please enter your username and password\n\n");
	
	printf("Username: ");
	strcpy(username, sread(MAX_CHAR));
	
	if(strlen(username) > 256)
	{
		printf("Invalid username length!\n");
		return NULL;
	}
	
	printf("Password: ");
	strcpy(password, sread(MAX_CHAR));

	if(strlen(password) > 256)
	{
		printf("Invalid password length!\n");
		return NULL;
	}

	/* get the hash value of password */
	int hashValue = hash(password);
	/* before freeing memory, fill the address with whitespace */
	strcpy(password, wspace(strlen(password)));
	free(password);

	User newUser = getUser(username, hashValue);
		
	if(newUser != NULL)
	{
		Session newSession = createNewSession(newUser, logintime);
		return newSession;
	}
	else
	{
		free(newUser);
		return NULL;
	}
}

/* returns type User if exists in a data file */
User getUser(char *username, int password)
{
	FILE *fp = fopen("./userdata.bin", "r");

	if(fp == NULL)
	{
		printf("Error! Could not locate \"userdata.bin\" in the directory.\n");
		pressEnterKey();
		return NULL;
	}
	else
	{
		char *temp = malloc(sizeof(char*) * MAX_CHAR);
		char buffer[MAX_CHAR];

		int length = getUserCount(fp);
		int i;
		
		for(i = 0; i < length; i++)
		{
			strncpy(buffer, decrypt(getLine(fp, i)), MAX_CHAR);

			/* tokenize the line */
			temp = strtok(buffer, ",");

			/* check if the username matches */
			if(strncmp(temp, username, MAX_CHAR) == 0)
			{
				temp = strtok(NULL, ",");
			
				/* password match? */
				if(password == atoi(temp))
				{
					temp = strtok(NULL, ",");
					int type = atoi(temp);

					User newUser = createNewUser(username, type);
					
					fclose(fp);

					return newUser;
				}
				else
				{
					printf("\nInvalid username or password!\n");
					writeLogs(createNewUser(username, -1), "Authentication failure - Invalid username/password");
					fclose(fp);
					return NULL;
				}
			}
		}
		
		printf("\nInvalid username or password!\n");
		pressEnterKey();
		fclose(fp);
		return NULL;
	}
}

/* returns a new login session for the specified user */
Session createNewSession(User currentUser, unsigned long int logintime)
{
	Session new = malloc(sizeof(*new));

	new->currentUser = currentUser;
	new->loginTime = logintime;

	return new;
}

User sessionGetUser(Session currentSession)
{
	return currentSession->currentUser;
}

time_t sessionGetLoginTime(Session currentSession)
{
	return currentSession->loginTime;
}

int userGetType(User currentUser)
{
	return currentUser->type;
}

char *userGetName(User currentUser)
{
	return (char*)currentUser->name;
}

/* returns a new user in memory */
User createNewUser(char *name, int type)
{
	User newu = malloc(sizeof(*newu));

	newu->name = malloc(sizeof(char*) * MAX_CHAR);
	newu->name = name;

	newu->type = type;

	return newu;
}

/* takes in a string and returns it encrypted */
char *encrypt(char *string)
{
	unsigned char *encryptBuff = malloc(sizeof(char*) * MAX_CHAR);
	unsigned char c;
	unsigned char new;
	int length = strnlen(string, MAX_CHAR);
	int i;

	for(i = 0; i < length; ++i)
	{
		c = string[i];

		if(c > 99)
		{
			new = c;
			new -= (c / 100) * 100;
			new = ((new % 10) * 10) + (new / 10) + 16;
			new += (c / 100) * 100;
		}
		else
		{
			new = ((c % 10) * 10) + (c / 10) + 16;
		}

		encryptBuff[i] = new;
	}
	
	return (char *)encryptBuff;
}

/* takes in an encrypted string and returns it decrypted */
char *decrypt(char *string)
{
	unsigned char *decryptBuff = malloc(sizeof(char*) * MAX_CHAR);
	unsigned char c;
	unsigned char new;
	int length = strnlen(string, MAX_CHAR);
	int i;

	for(i = 0; i < length; ++i)
	{
		c = string[i];
		c -= 16;

		if(c > 99)
		{
			new = c;
			new -= (c / 100) * 100;
			new = ((new % 10) * 10) + (new / 10);
			new += (c / 100) * 100;
		}
		else
		{
			new = ((c % 10) * 10) + (c / 10);
		}
		
		decryptBuff[i] = new;
	}

	return (char *)decryptBuff;
}

/* returns the number of users saved in the data file */
int getUserCount(FILE *fp)
{
	int i = 0;
	char c;

	/* make sure we are at the beginning of the file */
	fseek(fp, 0, SEEK_SET);

	while(c != EOF)
	{
		c = fgetc(fp);

		if(c == '\n')
		{
			i++;
		}
	}

	return i;
}

/* returns the full line as a string */
char *getLine(FILE *fp, int line)
{
	char *buffer = malloc(sizeof(char*) * MAX_CHAR);
	char *string = malloc(sizeof(char*) * MAX_CHAR);
	int i = 0;

	fseek(fp, 0, SEEK_SET);

	while(i <= line)
	{
		/* get the line */
		fgets(buffer, MAX_CHAR, fp);
	
		if(i == line)
		{
			/* double check not the end */
			if(*buffer == EOF)
			{
				break;
			}

			buffer[strnlen(buffer, MAX_CHAR)] = '\0';
			strncpy(string, buffer, strnlen(buffer, MAX_CHAR));
			
			return (char *)string;
		}

		i++;
	}

	return NULL;
}

/* returns an int hash value of a passed in string */
int hash(char *string)
{
	int i;
	int sum = 0;

	for(i = 0; string[i] != '\0'; i++)
	{
		sum += string[i] * (i+1);
	}

	return sum;
}

/* add a new user to the file */
void addUser()
{
	/* file set to 'a'ppend */
	FILE *fp = fopen("./userdata.bin", "a");

	if(fp == NULL)
	{
		printf("Error: Could not open \"userdata.bin\" in the directory!\n");
		pressEnterKey();
		return;
	}
	else
	{
		int hashvalue;
		int type;
		
		char *username = malloc(sizeof(char) * 64);
		char *password = malloc(sizeof(char) * 16);	
		char *string = malloc(sizeof(char*) * MAX_CHAR * 2);
		char *buffer = malloc(sizeof(char*) * MAX_CHAR);
		
		drawAddUser();
		
		printf("Username: ");
		strcpy(username, sread(64));
		
		printf("\nAll passwords must contain at least one capital, one number, and one special character !@#$* \nLength must be between 8 and 16 characters\n");
		printf("Password: ");
		strcpy(password, createPassword());
		
		/* if password creation fails */
		if(password == NULL)
		{
			printf("\nPassword entered does not meet the requirements!\n");
			pressEnterKey();
			return;
		}
		
		hashvalue = hash(password);
		strcpy(password, wspace(strlen(password)));
		free(password);
		
		printf("\nPlease select a user type (0 - doctor, 1 - nurse, 2 - helpdesk, 3 - auditor, 4 - administrator\n");
		printf("User Type: ");
		type = atoi(sread(1));
		
		string[0] = '\0';

		strncat(string, username, MAX_CHAR);
		strcat(string, ",");
		snprintf(buffer, MAX_CHAR, "%d", hashvalue);
		strncat(string, buffer, MAX_CHAR);
		strcat(string, ",");
		snprintf(buffer, MAX_CHAR, "%d", type);
		strncat(string, buffer, MAX_CHAR);

		strncpy(string, encrypt(string), MAX_CHAR);

		strcat(string, "\n");
		fprintf(fp, "%s", string);
		
		free(string);
		free(buffer);
		free(username);
	}
	
	fclose(fp);
}

void viewUsers()
{
	FILE *fp = fopen("./userdata.bin", "r");
	
	if(fp == NULL)
	{
		printf("Error: Could not open \"userdata.bin\" file!\n");
		pressEnterKey();
		return;
	}
	else
	{
		char *username = malloc(sizeof(char*) * MAX_CHAR);
		char *type = malloc(sizeof(char*) * MAX_CHAR);
		char *temp = malloc(sizeof(char*) * MAX_CHAR);
		
		char buff[256];
		int i = 1;
		drawViewUsers();
		
		while(1)
		{
			fgets(buff, 255, (FILE*)fp);
			if(feof(fp)) break;
			temp = strtok((char*)decrypt(buff), ",");
			strcpy(username, temp);
			temp = strtok(NULL, ",");
			temp = strtok(NULL, ",");
			strncpy(type, temp, strlen(temp)-1);
			printf("%3d.) %s (%s)\n", i, username, type);
			i++;
		}
		
		free(type);
		free(username);
		fclose(fp);
		
		pressEnterKey();
	}
}

/* reads a string with whitespace until ENTER key */
char *sread(int size)
{
	int i = 0;
	char ch;
	
	char *temp = malloc(sizeof(char*) * size+1);
	char *string = malloc(sizeof(char*) * size+1);

	if(string == NULL && temp == NULL)
	{
		printf("Error: Could not allocate memory\n");
		pressEnterKey();
		return NULL;
	}
	
	while((ch = getchar()) != '\n')
	{
		temp[i] = ch;
		if(i < size)
		{
			i++;
		}
	}

	if(i <= size)
	{
		strncpy(string, temp, i);
	}
	else
	{
		strncpy(string, temp, size);
	}
	
	string[size] = '\0';

	fflush(stdin);

	return string;
}

char *wspace(int size)
{
	int i = 0;
	char *string = malloc(sizeof(char*) * size);

	if(string == NULL)
	{
		printf("Error: Could not allocate memory\n");
		pressEnterKey();
		return NULL;
	}
	
	for(i = 0; i < size; i++)
	{
		string[i] = ' ';
	}
	
	string[i] = '\0';
	
	return string;
}

void changepass(User currentUser)
{
	FILE *fp = fopen("./userdata.bin", "r");
	FILE *nfp = fopen("./temp", "a");

	if(fp == NULL)
	{
		printf("Error: Could not locate \"userdata.bin\" in the directory.\n");
		pressEnterKey();
		return;
	}
	else
	{
		drawPassword();
		
		char *temp = malloc(sizeof(char*) * MAX_CHAR);
		char *line = malloc(sizeof(char*) * MAX_CHAR);
		char buffer[MAX_CHAR];

		int length = getUserCount(fp);
		int i;
		
		fseek(fp, 0, SEEK_SET);
		fseek(nfp, 0, SEEK_SET);
		
		for(i = 0; i < length; i++)
		{
			fgets(line, MAX_CHAR, fp);
			strncpy(buffer, decrypt(line), strlen(line));

			/* tokenize the line */
			temp = strtok(buffer, ",");
			
			/* check if the username matches */
			if(strncmp(temp, userGetName(currentUser), MAX_CHAR) == 0)
			{
				temp = strtok(NULL, ",");
				
				char *string = malloc(sizeof(char*) * 256);
				char *pass = malloc(sizeof(char*) * 16);
				char *pass2 = malloc(sizeof(char*) * 16);
				char *buff = malloc(sizeof(char*) * 10);
				/* type in old password for verification */
				printf("Old Password: ");
				strcpy(pass, sread(16));

				/* does it match? */
				if(hash(pass) == atoi(temp))
				{
					printf("New Password: ");
					strcpy(pass, wspace(16));
					strcpy(pass, createPassword());

					/* verify it was valid */
					printf("Reenter New Password: ");
					strcpy(pass2, createPassword());
					
					if(strcmp(pass, pass2) == 0)
					{
						/* get the type again */
						temp = strtok(NULL, ",");

						strcpy(string, userGetName(currentUser));
						strcat(string, ",");
					
						snprintf(buff, 10, "%d", hash(pass));
						strcat(string, buff);
					
						strcat(string, ",");
						strncat(string, temp, 1);
					
						strcpy(string, encrypt(string));
						strcat(string, "\n");
				
						strcpy(pass, wspace(strlen(pass)));
						strcpy(pass2, wspace(strlen(pass2)));
						strcpy(buffer, wspace(strlen(buffer)));
					
						fprintf(nfp, "%s", string);
					
						strcpy(string, wspace(strlen(string)));
						
						writeLogs(currentUser, "Password change success");
					}
					else
					{
						printf("\nNew password mismatch! Password was not changed.\n");
						writeLogs(currentUser, "Password change failure - password mismatch");
						pressEnterKey();
						fprintf(nfp, "%s", line);
					}
				}
				else
				{
					printf("\nOld password is incorrect!\n");
					writeLogs(currentUser, "Password change failure - incorrect password");
					pressEnterKey();
					fprintf(nfp, "%s", line);
				}
			}
			else
			{
				/* not the one we need to change, so add to temp */
				fprintf(nfp, "%s", line);
			}
		}

		fclose(fp);
		remove("./userdata.bin");
		rename("./temp", "./userdata.bin");
		fclose(nfp);
		
		return;
	}
}

/* press any key to continue */
void pressEnterKey()
{
	fflush(stdin);
	printf("\nPress [ENTER] To Continue...");
	char *null = malloc(sizeof(char)*3);
	strcpy(null, wspace(3));
	strcpy(null, sread(2));
	free(null);
}

/* before executing some function, verify the user's password for confirmation
 * @ returns 1 if verification is successful
 * @ returns 0 on failure
 */
int verify(User currentUser)
{
	FILE *fp = fopen("./userdata.bin", "r");

	if(fp == NULL)
	{
		printf("Error: Could not locate \"userdata.bin\" in the directory.\n");
		pressEnterKey();
		return 0;
	}
	else
	{
		char *temp = malloc(sizeof(char*) * MAX_CHAR);
		char buffer[MAX_CHAR];

		int length = getUserCount(fp);
		int i;
		
		char *username = malloc(sizeof(char*) * MAX_CHAR);
		char *password = malloc(sizeof(char*) * MAX_CHAR);
		int hashpass;
		
		/* get the username */
		strcpy(username, userGetName(currentUser));
		
		for(i = 0; i < length; i++)
		{
			strncpy(buffer, decrypt(getLine(fp, i)), MAX_CHAR);

			/* tokenize the line */
			temp = strtok(buffer, ",");
			
			/* check if the username matches */
			if(strncmp(temp, username, MAX_CHAR) == 0)
			{
				/* prompt for the password */
				printf("\nPassword Verification: ");
				strcpy(password, sread(MAX_CHAR));
				
				temp = strtok(NULL, ",");
			
				hashpass = hash(password);
				strcpy(password, wspace(MAX_CHAR));
				
				/* password match? */
				if(hashpass == atoi(temp))
				{
					fclose(fp);
					return 1;
				}
				else
				{
					printf("Invalid password!\n");
					writeLogs(currentUser, "Password verification failure");
					pressEnterKey();
					fclose(fp);
					return 0;
				}
			}
		}
		
		printf("Could not find user in the data file.\n");
		pressEnterKey();
		fclose(fp);
		return 0;
	}
}

void deleteUser(User currentAdmin)
{
	FILE *fp = fopen("./userdata.bin", "r");
	FILE *nfp = fopen("./temp", "a");

	if(fp == NULL)
	{
		printf("Error: Could not locate \"userdata.bin\" in the directory.\n");
		pressEnterKey();
		return;
	}
	else
	{
		printf("\033[2J");
		printf("\n-------------[ DELETE USER ]-------------\n");
		
		char *temp = malloc(sizeof(char*) * MAX_CHAR);
		char *line = malloc(sizeof(char*) * MAX_CHAR);
		char *username = malloc(sizeof(char*) * MAX_CHAR);
		
		char buffer[MAX_CHAR];

		/* get the user to delete */
		printf("\nUsername to delete: ");
		strcpy(username, sread(MAX_CHAR));
		
		if(strncmp(username, userGetName(currentAdmin), MAX_CHAR) == 0)
		{
			printf("\nCannot delete self! Deletion halted.\n");
			fclose(fp);
			fclose(nfp);
			pressEnterKey();
			return;
		}
		
		int length = getUserCount(fp);
		int i;
		int flag = 0;
		int found = 0;
		
		fseek(fp, 0, SEEK_SET);
		fseek(nfp, 0, SEEK_SET);
		
		/* go through userdata.bin until we find the user */
		for(i = 0; i < length; i++)
		{
			fgets(line, MAX_CHAR, fp);
			strncpy(buffer, decrypt(line), strlen(line));

			/* tokenize the line */
			temp = strtok(buffer, ",");
			
			/* check if the username matches */
			if(strncmp(temp, username, MAX_CHAR) == 0)
			{
				found = 1;
				printf("You are about to delete user %s from the system. This action cannot be undone.\n", username);
				printf("Proceed? (Y\\N) ");

				char *c = malloc(sizeof(char*) * 3);
				strcpy(c, sread(3));
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
					pressEnterKey();
					return;
				}

				/* if admin says yes, then proceed */
				if(flag == 1)
				{
					/* verify admin's password */
					if(verify(currentAdmin))
					{
						/* we aren't going to edit the data as much as we won't copy, so continue */
						char *string = malloc(sizeof(char*) * MAX_CHAR);
						
						strcpy(string, "Administrator deleted user ");
						strcat(string, username);
						strcat(string, " from the system");
						
						printf("\nUser %s successfully deleted from the system.\n", username);
						pressEnterKey();
						writeLogs(currentAdmin, string);
						free(string);
					}
				}
				else if(flag == 0)
				{
					printf("\nUser deletion cancelled. The file has not been changed.\n");
					pressEnterKey();
					
					writeLogs(currentAdmin, "User delete cancelled");
					fclose(fp);
					fclose(nfp);
					remove("./temp");
					return;
				}
			}
			else
			{
				/* copy over the users who are not affected */
				fprintf(nfp, "%s", line);
			}
		}

		fclose(fp);
		fclose(nfp);
		
		if(found == 0)
		{
			printf("\nCould not find the specified user in the file.\n");
			pressEnterKey();
		}
		else
		{		
			remove("./userdata.bin");
			rename("./temp", "./userdata.bin");		
		}

	}
}

char *createPassword()
{
	/* passwords must be between 8-16 characters,
	 * must contain at least one CAPITAL, number, and special character
	 */
	 
	char *password = malloc(sizeof(char*) * 16);
	strcpy(password, sread(16));
	
	if(strlen(password) >= 8 && strlen(password) <= 16)
	{
		int capflag = 0;
		int numflag = 0;
		int spcflag = 0;
		int i;
		
		for(i = 0; i < strlen(password); i++)
		{
			/* check if it is a CAPITAL */
			if(password[i] >=65 && password[i] <= 90)
			{
				capflag = 1;
			}
			/* check if it is a NUMBER */
			else if(password[i] >= 48 && password[i] <= 57)
			{
				numflag = 1;
			}
			else if(password[i] == '!' || password[i] == '@' || password[i] == '#'
					|| password[i] == '$' || password[i] == '*')
			{
				spcflag = 1;
			}
		}
		
		/* check if the password satisfies all */
		if(capflag == 1 && numflag == 1 && spcflag == 1)
		{
			return password;
		}
		else return NULL;
	}
	else return NULL;
}
//////////////////////////////

/* DATA */
void addNewPatient()
{ 
   int MAX_CHAR = 256;
   
   // open file
   FILE *fp;
   fp = fopen("./patients.bin","a");
   
   fseek(fp, 0, SEEK_SET);
   
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
		fprintf(fp, "%s", string);
	  
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
		fprintf(fp, "%s", string);
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
		fprintf(fp, "%s", string);
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
		
		printf("\033[2J");
		
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
		printf("\033[2J");
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
		
		fseek(fp, 0, SEEK_SET);
		fseek(nfp, 0, SEEK_SET);
		
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
					else
					{
						printf("Invalid password!\n");
						pressEnterKey();
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
				fprintf(nfp, "%s", line);
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
///////////////////////////////////////

/* AUDIT LOGS */
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
	strcpy(string, encrypt(string));
	strcat(string, "\n");
	
	fprintf(fp, "%s", string);
	fclose(fp);
	free(string);
}

void readLogs()
{
	
	FILE *fp = fopen("logs.bin", "r");
	
	char buff[255];
	
	printf("\033[2J");

	drawLogs();

	while(1)
	{
		fgets(buff, 255, (FILE*)fp);
		if(feof(fp))
			break;
		strcpy(buff, decrypt(buff));
		strcat(buff, "\n");
		printf("%s", buff);
	}

	fclose(fp);
	pressEnterKey();
}
/////////////////////////////

/* HELPDESK */
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
	
	fprintf(fp, "%s", string);
	
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
/////////////////////////////////////////////////////

/* DRAWING */
/* login banner */
void drawLogin()
{
	/* clear the screen */
	printf("\033[2J");
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
	printf("\033[2J");
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
	printf("\033[2J");
	printf("\n-------------[ PATIENT SEARCH ]-------------\n");
	printf("Please enter the social security number of the patient.\n");
	printf("Patients in file: %d\n", count);
	printf("\n");
}

void drawFilteredSearch(FILE *fp)
{
	int count = getUserCount(fp);
	printf("\033[2J");
	printf("\n-------------[ FILTERED SEARCH ]-------------\n");
	printf("Here you can view all patients that meet a specific criteria.\n");
	printf("For more detailed information, please use Search Patient.\n");
	printf("Patients in file: %d\n", count);
	printf("\n");
}

void drawPatientInfo()
{
	printf("\033[2J");
	printf("\n-------------[ PATIENT INFO ]-------------\n");
	printf("\n");
}

void drawPatientNew()
{
	printf("\033[2J");
	printf("\n-------------[ CREATE NEW PATIENT ]-------------\n");
	printf("\n");
}

void drawAppointment()
{
	printf("\033[2J");
	printf("\n------------[ NEW APPOINTMENT ]------------\n");
	printf("\n");
}

void drawAppointmentList(FILE *fp)
{
	printf("\033[2J");
	printf("\n------------[ APPOINTMENT LIST ]------------\n");
	printf("\n");
}

void drawLogs()
{
	printf("\033[2J");
	printf("\n------------[ LOGS ]------------\n");
	printf("\n");
}

void drawViewUsers()
{
	printf("\033[2J");
	printf("\n------------[ USER LIST ]------------\n");
	printf("\n");	
}

void drawAddUser()
{
	printf("\033[2J");
	printf("\n------------[ ADD USER ]------------\n");
	printf("\n");
}

void drawPassword()
{
	printf("\033[2J");
	printf("\n------------[ CHANGE PASSWORD ]------------\n");
	printf("\n");
}

void drawExit()
{
	printf("\033[2J");
	printf("'':. You have been successfully logged off! .:''\n");
	printf("  ':.: The program will close momentarily :.:'\n");
	printf("\n");		
}
/////////////////////////////////////////////