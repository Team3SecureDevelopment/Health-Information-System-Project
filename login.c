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
 * returns 1, else fails and returns 0
 */
int authenticate()
{
	return 0;
}

/* returns type User if exists in a data file */
User getUser(char *username, int pass)
{
	return NULL;
}

/* returns a new login session for the specified user */
Session createNewSession(User currentUser)
{
	return NULL;
}

/* takes in a string and returns it encrypted */
char *encrypt(char *string)
{
	char encryptBuff[MAX_CHAR];
	char c;
	char new;
	int length = strnlen(string, MAX_CHAR);
	int i;

	printf("String length is %d\n", length);
	printf("String: %s\n", string);
	for(i = 0; i < length; ++i)
	{
		c = string[i];

		if(c > 99)
		{
			new = ((c / 100) * 100) + ((c % 10) * 10) + (c / 10) + 16;
		}
		else
		{
			new = ((c % 10) * 10) + (c / 10) + 16;
		}

		printf("C: \'%c\' %d || N: \'%c\' %d\n\n", c, c, new, new);
		encryptBuff[i] = new;
	}
	printf("Encrypted: %s\n", encryptBuff);
	return *encryptBuff;
}

/* takes in an encrypted string and returns it decrypted */
char *decrypt(char *string)
{
	char decryptBuff[MAX_CHAR];
	char c;
	char new;
	int length = strnlen(string, MAX_CHAR);
	int i;

	printf("Beginning decryption\n");
	printf("Encrypted Text: %s\n", string); 
	for(i = 0; i < length; ++i)
	{
		c = string[i];

		printf("C: \'%c\' %d ||", c, c);

		/* ASCII is a signed 8bit, so we'll need to add 256 if negative */
		if(c < 0)
		{
			c += 256;
		}

		c -= 16;

		if(c > 99)
		{
			new = ((c / 100) * 100) + ((c % 10) * 10) + (c / 10);
		}
		else
		{
			new = ((c % 10) * 10) + (c / 10);
		}
		printf(" N: \'%c\' %d\n\n", new, new);		
		decryptBuff[i] = new;
	}
	printf("Decrypted: %s\n", decryptBuff);
	return *decryptBuff;
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
	char *string = "The quick brown fox jumped over the lazy dog!";
	decrypt(encrypt(string));
	return 0;
}
