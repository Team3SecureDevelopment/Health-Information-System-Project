#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "login.h"

/* global variables */
int MAX_CHAR = 256;

/* struct definitions */
struct session
{
	User currentUser;
	unsigned long int loginTime;
	unsigned long int departureTime;
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

	unsigned long int logintime = 0;

	printf("Welcome!\n\n");
	printf("Please type in your username: ");
	scanf("%s", username);

	printf("Please type in your password: ");
	scanf("%s", password);

	int hashValue = hash(password);
	free(password);

	if(username != NULL)
	{
		User newUser = getUser(username, hashValue);
		
		if(newUser != NULL)
		{
			Session newSession = createNewSession(newUser, logintime);

			return newSession;
		}

		return NULL;
	}
	else return NULL;
}

/* returns type User if exists in a data file */
User getUser(char *username, int password)
{
	FILE *fp = fopen("./userdata.bin", "r");

	if(fp == NULL)
	{
		printf("Error! Could not locate \"userdata.bin\" in the directory.\n");
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
				printf("Found username in the file!\n");
				
				temp = strtok(NULL, ",");
			
				/* password match? */
				if(password == atoi(temp))
				{
					printf("Password match!\n");
					temp = strtok(NULL, ",");
					int type = atoi(temp);

					printf("User type is %d\n", type);

					User newUser = createNewUser(username, type);

					return newUser;
				}
				else
				{
					printf("Invalid password!\n");
					return NULL;
				}
			}
			else
			{
				printf("Still looking for %s\n", username);
			}
		}
		
		printf("Could not find user in the data file.\n");
		return NULL;
	}
}

/* returns a new login session for the specified user */
Session createNewSession(User currentUser, unsigned long int logintime)
{
	return NULL;
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

	printf("String length is %d\n", length);
	printf("String: %s\n", string);
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

		printf("C: \'%c\' %d || N: \'%c\' %d\n\n", c, c, new, new);
		encryptBuff[i] = new;
	}
	printf("Encrypted: %s\n", encryptBuff);
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

	printf("Beginning decryption\n");
	printf("Encrypted Text: %s\n", string); 
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
	printf("Decrypted: %s\n", decryptBuff);
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
	char buffer[MAX_CHAR];
	int i;

	char *string = malloc(sizeof(char*) * MAX_CHAR);

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

/* debug driver */
int main()
{
	Session newSession = authenticate();

	if(newSession == NULL)
	{
		printf("Could not authenticate your session.\n");
		exit(1);
	}
	else
	{
		printf("You have been successfully authenticated!\n");
	}

	return 0;
}
