#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "login.h"
#include "data.h"
#include "auditLogs.h"
#include "draw.h"

/* global variables */
int MAX_CHAR = 256;

/* struct definitions */
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
		printf("\nPress any key to continue...\n");
		getchar();
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
					printf("\nInvalid password!\n");
					writeLogs(createNewUser(username, -1), "Invalid password login");
					fclose(fp);
					return NULL;
				}
			}
		}
		
		printf("Could not find user in the data file.\n");
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
	rewind(fp);

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

	rewind(fp);

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
		printf("Error! Could not open \"userdata.bin\" in the directory!\n");
		printf("\nPress any key to continue...\n");
		getchar();
		exit(1);
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
		
		printf("Password: ");
		strcpy(password, sread(16));
		
		hashvalue = hash(password);
		strcpy(password, wspace(strlen(password)));
		free(password);
		
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

		fprintf(fp, string);
		
		free(string);
		free(buffer);
		free(username);
	}
	
	fclose(fp);
}

void deleteUser()
{
		drawDeleteUser();
		printf("Username of User to Delete: ");
		getchar();
}

void viewUsers()
{
	FILE *fp = fopen("./userdata.bin", "r");
	
	if(fp == NULL)
	{
		printf("Could not open file\n");
		printf("\nPress any key to continue...\n");
		getchar();
		return;
	}
	else
	{
		char buff[255];

		drawViewUsers();
		
		while(1)
		{
			fgets(buff, 255, (FILE*)fp);
			if(feof(fp)) break;
			printf("%s", decrypt(buff));
			printf("\n");
		}
		
		fclose(fp);
	}
}

/* reads a string with whitespace until ENTER key */
char *sread(int size)
{
	int i = 0;
	char ch;
	char *string = malloc(sizeof(char*) * size);
	
	if(string == NULL)
	{
		printf("Could not allocate memory\n");
		printf("\nPress any key to continue...\n");
		getchar();
		return NULL;
	}
	
	while((ch = getchar()) != '\n')
	{
		string[i] = ch;
		i++;
	}
	
	string[i++] = '\0';
	
	return string;
}

char *wspace(int size)
{
	int i = 0;
	char *string = malloc(sizeof(char*) * size);

	if(string == NULL)
	{
		printf("Could not allocate memory\n");
		printf("\nPress any key to continue...\n");
		getchar();
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
		printf("Error! Could not locate \"userdata.bin\" in the directory.\n");
		printf("\nPress any key to continue...\n");
		getchar();
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
		
		rewind(fp);
		rewind(nfp);
		
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
					strcpy(pass, sread(16));

					/* verify it was valid */
					printf("Reenter New Password: ");
					strcpy(pass2, sread(16));
					
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
					
						fprintf(nfp, string);
					
						strcpy(string, wspace(strlen(string)));
						
						writeLogs(currentUser, "Password change success");
					}
					else
					{
						printf("\nNew password mismatch! Password was not changed.\n");
						writeLogs(currentUser, "Password change failure - password mismatch");
						fprintf(nfp, line);
					}
				}
				else
				{
					printf("\nOld password is incorrect!\n");
					writeLogs(currentUser, "Password change failure - incorrect password");
					fprintf(nfp, line);
				}
			}
			else
			{
				/* not the one we need to change, so add to temp */
				fprintf(nfp, line);
			}
		}

		fclose(fp);
		remove("./userdata.bin");
		rename("./temp", "./userdata.bin");
		fclose(nfp);
		
		return;
	}
}