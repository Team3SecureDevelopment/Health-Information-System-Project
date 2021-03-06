#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

/* printf("\033[2J\033[;H"); <------------------ ANSI sequence for clear screen, reset cursor */

size_t MAX_CHAR = 256;

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

/* PROTOTYPES */
void writeLogs(User currentUser, char *purpose);
void readLogs();
void addNewPatient();
void findPatient();
void setAllergyInfo(int);
void getAllergyInfo(int);
void setPrescriptionInfo(int);
void getPrescriptionInfo(int);
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
	const Session newSession = authenticate();

	if(newSession == NULL)
	{
		printf("You are unable to be authenticated.\n");
		exit(1);
	}
	else if(newSession != NULL)
	{
		/* should we keep running? */
		int active = 0;
		User currentUser;
		
		if(newSession->currentUser != NULL)
		{
			currentUser = sessionGetUser(newSession);
		}
		
		writeLogs(currentUser, "Login");
		
		const int type = userGetType(currentUser);
		
		long int menuchoice;
		
		printf("\033[2J\033[;H");

		while(active == 0)
		{
			char *menustring = (char*)calloc(2, sizeof(char));
			
			if(NULL == menustring)
			{
				free(menustring);
				menustring = NULL;
				printf("Unable to allocate memory\n");
				exit(1);
			}
			/* user authenticated, draw the menu */
			drawMenu(currentUser);
		
			if(type >= 0 && type < 5)
			{
				/* now we need to get the user input */
				printf("Please enter a choice and press ENTER -> ");
				strcpy(menustring, sread(1));
				
				if(menustring != NULL)
				{
					menuchoice = strtol(menustring, NULL, 10);
					menustring = NULL;
				}
				else
				{
					printf("System Error: string pointer is NULL!\n");
					exit(1);
				}

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
		
		const time_t departure = time(NULL);
		
		/* show exit screen */
		drawExit();
		
		/* get departure time and format user's duration */
		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *string = (char*)calloc(MAX_CHAR, sizeof(char));

		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *timetotal = (char*)calloc(MAX_CHAR, sizeof(char));
		
		if(NULL == string)
		{
			free(string);
			string = NULL;
			printf("Unable to allocate memory\n");
			exit(1);
		}
		
		if(NULL == timetotal)
		{
			free(timetotal);
			timetotal = NULL;
			printf("Unable to allocate memory\n");
			exit(1);
		}
		
		snprintf(timetotal, 128, "%.2f", ((float )departure - (float )sessionGetLoginTime(newSession)));
		strcpy(string, "Log Off -> Time duration of ");
		strcat(string, timetotal);
		strcat(string, " seconds");
		
		/* write to logs */
		writeLogs(currentUser, string);
		
		/* free */
		if(timetotal != NULL)
		{
			timetotal = NULL;
		}
		
		if(string != NULL)
		{
			string = NULL;
		}
		
		if(currentUser != NULL)
		{
			currentUser = NULL;
		}
	}

	const int n = sleep(2);
	if(n != 0)
	{
		printf("Sleep interrupted\n");
	}
	
	printf("\033[2J\033[;H");
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
	if(MAX_CHAR > SIZE_MAX/sizeof(char))
	{
		printf("Not enough memory!\n");
		exit(1);
	}
	char *username = (char*)calloc(MAX_CHAR, sizeof(char));

	if(MAX_CHAR > SIZE_MAX/sizeof(char))
	{
		printf("Not enough memory!\n");
		exit(1);
	}
	char *password = (char*)calloc(MAX_CHAR, sizeof(char));
	
	if(NULL == username)
	{
		free(username);
		username = NULL;
		printf("Memory allocation error\n");
		exit(1);
	}
	
	if(NULL == password)
	{
		free(password);
		password = NULL;
		printf("Memory allocation error\n");
		exit(1);
	}
	
	const time_t logintime = time(NULL);
	
	int hashvalue = 0;
	
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
	
	if(password != NULL)
	{
		if(strlen(password) > 256)
		{
			printf("Password is too long!\n");
		}
		else
		{
			/* if the password is valid, get its hash */
			hashvalue = hash(password);
		}
		
		/* overwrite the password with blank spaces */
		strcpy(password, wspace(strlen(password)));
		
		/* get rid of it */
		if(password != NULL)
		{
			password = NULL;
		}
	}
	
	const User newUser = getUser(username, hashvalue);
		
	if(newUser != NULL)
	{
		const Session newSession = createNewSession(newUser, logintime);
		
		if(newSession != NULL)
		{
			return newSession;
		}
	}
	
	return NULL;
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
		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *temp = (char*)calloc(MAX_CHAR, sizeof(char));
		
		if(NULL == temp)
		{
			free(temp);
			temp = NULL;
			printf("Memory allocation error\n");
			exit(1);
		}
		
		char buffer[MAX_CHAR];

		const int length = getUserCount(fp);
		int i;
		
		char *token;
		
		if(fseek(fp, 0, SEEK_SET) == 0)
		{
			for(i = 0; i < length; i++)
			{
				if(fgets(buffer, MAX_CHAR, fp) != NULL)
				{
					strncpy(buffer, decrypt(buffer), MAX_CHAR);
					
					/* tokenize the line */
					strcpy(temp, buffer);
					
					token = strtok(temp, ",");
					
					/* check if the username matches */
					if(strncmp(token, username, MAX_CHAR) == 0)
					{
						token = strtok(NULL, ",");
						
						/* password match? */
						if(password == (int)strtol(token, NULL, 10))
						{
							token = strtok(NULL, ",");
							const long int type = strtol(token, NULL, 10);

							const User newUser = createNewUser(username, type);
							
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
	if(NULL == new)
	{
		free(new);
		new = NULL;
	}
	
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
	
	if(NULL == newu)
	{
		free(newu);
		newu = NULL;
		printf("Memory allocation error\n");
		exit(1);
	}

	if(NULL == newu->name)
	{
		free(newu->name);
		newu->name = NULL;
		printf("Memory allocation error\n");
		exit(1);
	}
	
	newu->name = name;
	newu->type = type;


	return newu;
}

/* takes in a string and returns it encrypted */
char *encrypt(char *string)
{
	if(MAX_CHAR > SIZE_MAX/sizeof(char))
	{
		printf("Not enough memory!\n");
		exit(1);
	}
	unsigned char *encryptBuff = (unsigned char*)calloc(MAX_CHAR, sizeof(char));
	
	if(NULL == encryptBuff)
	{
		free(encryptBuff);
		encryptBuff = NULL;
		printf("Memory allocation error\n");
		exit(1);
	}
	
	unsigned char c;
	unsigned char new;
	const int length = strnlen(string, MAX_CHAR);
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
	if(MAX_CHAR > SIZE_MAX/sizeof(char))
	{
		printf("Not enough memory!\n");
		exit(1);
	}	
	unsigned char *decryptBuff = (unsigned char*)calloc(MAX_CHAR, sizeof(char));
	
	if(NULL == decryptBuff)
	{
		free(decryptBuff);
		decryptBuff = NULL;
		printf("Memory allocation error\n");
		exit(1);
	}
	unsigned char c;
	unsigned char new;
	const int length = strnlen(string, MAX_CHAR);
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
	signed int c = 0;

	/* make sure we are at the beginning of the file */
	if(fseek(fp, 0, SEEK_SET) == 0)
	{
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
	else return 0;
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
		
		if(64 > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *username = (char*)calloc(64, sizeof(char));
		
		if(16 > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *password = (char*)calloc(16, sizeof(char));
		
		if(MAX_CHAR*2 > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *string = (char*)calloc(MAX_CHAR*2, sizeof(char));
		
		if(MAX_CHAR*2 > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *buffer = (char*)calloc(MAX_CHAR*2, sizeof(char));
		
		if(NULL == username)
		{
			free(username);
			username = NULL;
			printf("Memory allocation error\n");
			exit(1);
		}
		
		if(NULL == password)
		{
			free(password);
			password = NULL;
			printf("Memory allocation error\n");
			exit(1);
		}
		
		if(NULL == string)
		{
			free(string);
			string = NULL;
			printf("Memory allocation error\n");
			exit(1);
		}
		
		if(NULL == buffer)
		{
			free(buffer);
			buffer = NULL;
			printf("Memory allocation error\n");
			exit(1);
		}
		
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
		
		if(password != NULL)
		{
			password = NULL;
		}
		
		printf("\nPlease select a user type (0 - doctor, 1 - nurse, 2 - helpdesk, 3 - auditor, 4 - administrator\n");
		printf("User Type: ");
		type = (int)strtol(sread(1), NULL, 10);
		
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
		
		if(string != NULL)
		{
			string = NULL;
		}
		
		if(buffer != NULL)
		{
			buffer = NULL;
		}
		
		if(username != NULL)
		{
			username = NULL;
		}
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
		const int length = getUserCount(fp);
		if(fseek(fp, 0, SEEK_SET) == 0);
		
		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *username = (char*)calloc(MAX_CHAR, sizeof(char));
		
		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *type = (char*)calloc(MAX_CHAR, sizeof(char));
		
		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *temp = (char*)calloc(MAX_CHAR, sizeof(char));

		if(NULL == username)
		{
			free(username);
			username = NULL;
			printf("Memory allocation error\n");
			exit(1);
		}

		if(NULL == type)
		{
			free(type);
			type = NULL;
			printf("Memory allocation error\n");
			exit(1);
		}

		if(NULL == temp)
		{
			free(temp);
			temp = NULL;
			printf("Memory allocation error\n");
			exit(1);
		}
		
		char buff[256];
		int i;

		drawViewUsers();
		char *token;
		
		for(i = 0; i < length; i++)
		{
			if(fgets(buff, 255, fp) != NULL)
			{
				strncpy(buff, decrypt(buff), MAX_CHAR);
				strcpy(temp, buff);
				token = temp;
				
				if(feof(fp))
				{
					break;
				}

				token = strtok(temp, ",");
				strcpy(username, token);
				token = strtok(NULL, ",");
				token = strtok(NULL, ",");
				strncpy(type, token, 1);
				printf("%3d.) %s (%s)\n", i+1, username, type);
			}
		}

		token = strtok(NULL, ",");
		
		if(type != NULL)
		{
			type = NULL;
		}
		
		if(username != NULL)
		{
			username = NULL;
		}

		if(temp == NULL)
		{
			temp = NULL;
		}
		
		fclose(fp);
		pressEnterKey();
	}
}

/* reads a string with whitespace until ENTER key */
char *sread(int size)
{
	fflush(stdin);
	int i = 0;
	unsigned int ch = ' ';
	
	if(size+1 > SIZE_MAX/sizeof(char))
	{
		printf("Not enough memory!\n");
		exit(1);
	}
	unsigned char *temp = (unsigned char*)calloc(size+1, sizeof(char*));
	
	if(NULL == temp)
	{
		free(temp);
		temp = NULL;
		printf("Memory allocation error\n");
		exit(1);
	}

	if(size+1 > SIZE_MAX/sizeof(char))
	{
		printf("Not enough memory!\n");
		exit(1);
	}
	unsigned char *string = (unsigned char*)calloc(size+1, sizeof(char*));
	if(NULL == string)
	{
		free(string);
		string = NULL;
		printf("Memory allocation error\n");
		exit(1);
	}
	
	while((ch = getchar()) != '\n')
	{
		temp[i] = ch;
		if(i < size)
		{
			i++;
		}
	}

	if(i > size)
	{
		strncpy((char*)string, (char*)temp, size);
	}
	else
	{
		strncpy((char*)string, (char*)temp, i);
	}

	if(temp != NULL)
	{
		temp = NULL;
	}

	return (char*)string;
}

char *wspace(int size)
{
	int i = 0;
	
	if(size+1 > SIZE_MAX/sizeof(char))
	{
		printf("Not enough memory!\n");
		exit(1);
	}
	char *string = (char*)calloc(size+1, sizeof(char));

	if(NULL == string)
	{
		free(string);
		string = NULL;
		printf("Memory allocation error\n");
		exit(1);
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
	/* before we start, make sure there isn't already a temp file */
	if(remove("./temp") == 0);
	
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
		
		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *temp = (char*)calloc(MAX_CHAR, sizeof(char));
		
		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *line = (char*)calloc(MAX_CHAR, sizeof(char));
		
		if(NULL == temp)
		{
			free(temp);
			temp = NULL;
			printf("memory allocation error\n");
			exit(1);
		}
		
		if(NULL == line)
		{
			free(line);
			line = NULL;
			printf("memory allocation error\n");
			exit(1);
		}
		
		char buffer[MAX_CHAR];
		char *token;
		const int length = getUserCount(fp);
		int change_flag = 0;
		int i;
		
		if(fseek(fp, 0, SEEK_SET) != 0)
		{
			return;
		}

		if(fseek(nfp, 0, SEEK_SET) != 0)
		{
			return;
		}
		
		for(i = 0; i < length; i++)
		{
			if(fgets(line, MAX_CHAR, fp) != NULL)
			{
				strncpy(buffer, decrypt(line), strlen(line));
				strcpy(temp, buffer);
				
				/* tokenize the line */
				token = strtok(temp, ",");
				
				/* check if the username matches */
				if(strncmp(token, userGetName(currentUser), MAX_CHAR) == 0)
				{
					token = strtok(NULL, ",");

					if(MAX_CHAR > SIZE_MAX/sizeof(char))
					{
						printf("Not enough memory!\n");
						exit(1);
					}
					char *string = (char*)calloc(MAX_CHAR, sizeof(char));
					
					if(NULL == string)
					{
						free(string);
						string = NULL;
						printf("memory allocation error\n");
						exit(1);
					}

					if(16 > SIZE_MAX/sizeof(char))
					{
						printf("Not enough memory!\n");
						exit(1);
					}
					char *pass = (char*)calloc(16, sizeof(char));
					
					if(NULL == pass)
					{
						free(pass);
						pass = NULL;
						printf("memory allocation error\n");
						exit(1);
					}
					
					if(16 > SIZE_MAX/sizeof(char))
					{
						printf("Not enough memory!\n");
						exit(1);
					}

					char *pass2 = (char*)calloc(16, sizeof(char));
					if(NULL == pass2)
					{
						free(pass2);
						pass2 = NULL;
						printf("memory allocation error\n");
						exit(1);
					}

					if(10 > SIZE_MAX/sizeof(char))
					{
						printf("Not enough memory!\n");
						exit(1);
					}
					char *buff = (char*)calloc(10, sizeof(char));
					if(NULL == buff)
					{
						free(buff);
						buff = NULL;
						printf("memory allocation error\n");
						exit(1);
					}
					
					/* type in old password for verification */
					printf("Old Password: ");
					strcpy(pass, sread(16));

					/* does it match? */
					if(hash(pass) == (int)strtol(token, NULL, 10))
					{
						printf("New Password: ");
						strcpy(pass, wspace(16));
						strcpy(pass, createPassword());
						
						if(strcmp(pass, "null") != 0)
						{
							/* verify it was valid */
							printf("Reenter New Password: ");
							strcpy(pass2, createPassword());
							
							if(strcmp(pass, pass2) == 0)
							{
								/* get the type again */
								token = strtok(NULL, ",");

								strcpy(string, userGetName(currentUser));
								strcat(string, ",");
							
								snprintf(buff, 10, "%d", hash(pass2));
								strcat(string, buff);
							
								strcat(string, ",");
								strncat(string, token, 1);

								strcpy(string, encrypt(string));
								strcat(string, "\n");
						
								strncpy(pass, wspace(strlen(pass)), 16);
								strncpy(pass2, wspace(strlen(pass2)), 16);
								strncpy(buffer, wspace(strlen(buffer)), 16);
							
								fprintf(nfp, "%s", string);
							
								strcpy(string, wspace(strlen(string)));
								
								writeLogs(currentUser, "Password change success");
								change_flag = 1;
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
							printf("New password does not meet requirements!\n");
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
					
					if(buff != NULL)
					{
						buff = NULL;
					}
					if(string != NULL)
					{
						string = NULL;
					}
					if(pass != NULL)
					{
						pass = NULL;
					}
					if(pass2 != NULL)
					{
						pass2 = NULL;
					}
				}
				else
				{
					/* not the one we need to change, so add to temp */
					fprintf(nfp, "%s", line);
				}
			}
		}

		if(line != NULL)
		{
			line = NULL;
		}

		if(temp != NULL)
		{
			temp = NULL;
		}

		
		fclose(fp);
		fclose(nfp);
		
		/* if we made changes, we need to remove the old file */
		if(change_flag == 1)
		{
			if(remove("./userdata.bin") == 0)
			{
				if(rename("./temp", "./userdata.bin") == 0)
				{
					printf("Password changed successfully!\n");
				}
			}
		}
	}
}

/* press any key to continue */
void pressEnterKey()
{
	fflush(stdin);
	printf("\nPress [ENTER] To Continue...");
	unsigned int ch = getchar();
	while((ch != '\n') && (ch != '\r'))
	{
		ch = getchar();
	}
	fflush(stdin);
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
		if(MAX_CHAR > SIZE_MAX/sizeof(char*))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *temp = (char*)calloc(MAX_CHAR, sizeof(char*));
		if(NULL == temp)
		{
			free(temp);
			temp = NULL;
			printf("memory allocation error\n");
			exit(1);
		}
		
		if(MAX_CHAR > SIZE_MAX/sizeof(char*))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *username = (char*)calloc(MAX_CHAR, sizeof(char*));
		if(NULL == username)
		{
			free(username);
			username = NULL;
			printf("memory allocation error\n");
			exit(1);
		}
		
		if(16 > SIZE_MAX/sizeof(char*))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *password = (char*)calloc(16, sizeof(char*));
		if(NULL == password)
		{
			free(password);
			password = NULL;
			printf("memory allocation error\n");
			exit(1);
		}
		
		int hashpass;
		char buffer[MAX_CHAR];
		char *token;
		const int length = getUserCount(fp);
		int i;
		
		/* get the username */
		strncpy(username, userGetName(currentUser), MAX_CHAR);
		
		if(fseek(fp, 0, SEEK_SET) == 0);
		
		for(i = 0; i < length; i++)
		{
			if(fgets(buffer, 255, fp) != NULL)
			{
				strncpy(buffer, decrypt(buffer), MAX_CHAR);
				strcpy(temp, buffer);
				
				/* tokenize the line */
				token = strtok(temp, ",");
				
				/* check if the username matches */
				if(strncmp(token, username, MAX_CHAR) == 0)
				{
					/* prompt for the password */
					printf("\nPassword Verification: ");
					strncpy(password, sread(16), 16);
					
					token = strtok(NULL, ",");
				
					hashpass = hash(password);
					strcpy(password, wspace(MAX_CHAR));
					
					/* password match? */
					if(hashpass == (int)strtol(token, NULL, 10))
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
		printf("\033[2J\033[;H");
		printf("\n-------------[ DELETE USER ]-------------\n");
		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *temp = (char*)calloc(MAX_CHAR, sizeof(char));
		if(NULL == temp)
		{
			free(temp);
			temp = NULL;
		}
		
		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *line = (char*)calloc(MAX_CHAR, sizeof(char));
		if(NULL == line)
		{
			free(line);
			line = NULL;
		}

		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}		
		char *username = (char*)calloc(MAX_CHAR, sizeof(char));
		if(NULL == username)
		{
			free(username);
			username = NULL;
		}
		
		fflush(stdin);
		char buffer[MAX_CHAR];

		/* get the user to delete */
		printf("Username to delete: ");
		strcpy(username, sread(MAX_CHAR));
		
		if(strncmp(username, userGetName(currentAdmin), MAX_CHAR) == 0)
		{
			printf("\nCannot delete self! Deletion halted.\n");
			fclose(fp);
			fclose(nfp);
			if(remove("./temp") == 0)
			{
				pressEnterKey();
				return;
			}
		}
		
		const int length = getUserCount(fp);
		int i;
		int flag = 0;
		int found = 0;
		int delete = 0;
		char *token;
		
		if(fseek(fp, 0, SEEK_SET) == 0);
		if(fseek(nfp, 0, SEEK_SET) == 0);
		
		/* go through userdata.bin until
		 * we find the user */
		for(i = 0; i < length; i++)
		{
			if(fgets(line, MAX_CHAR, fp) != NULL)
			{
				strncpy(buffer, decrypt(line), strlen(line));
				strcpy(temp, buffer);
				
				/* tokenize the line */
				token = strtok(temp, ",");
				
				/* check if the username matches */
				if(strncmp(token, username, MAX_CHAR) == 0)
				{
					found = 1;
					printf("You are about to delete user %s from the system. This action cannot be undone.\n", username);
					printf("Proceed? (Y\\N) ");

					if(3 > SIZE_MAX/sizeof(char))
					{
						printf("Not enough memory!\n");
						exit(1);
					}
					char *c = (char*)calloc(3, sizeof(char));
					if(NULL == c)
					{
						free(c);
						c = NULL;
					}
					
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
						c = NULL;
						return;
					}

					if(c != NULL)
					{
						c = NULL;
					}
					
					/* if admin says yes, then proceed */
					if(flag == 1)
					{
						/* verify admin's password */
						if(verify(currentAdmin))
						{
							/* we aren't going to edit the data as much as we won't copy, so continue */
							if(MAX_CHAR > SIZE_MAX/sizeof(char))
							{
								printf("Not enough memory!\n");
								exit(1);
							}
							char *string = (char*)calloc(MAX_CHAR, sizeof(char));
							if(NULL == string)
							{
								free(string);
								string = NULL;
							}
							
							strcpy(string, "Administrator deleted user ");
							strcat(string, username);
							strcat(string, " from the system");
							
							printf("\nUser %s successfully deleted from the system.\n", username);
							pressEnterKey();
							writeLogs(currentAdmin, string);
							
							delete = 1;
							if(string != NULL)
							{
								string = NULL;
							}
						}
					}
					else if(flag == 0)
					{

						printf("User deletion cancelled\n");
						pressEnterKey();
						writeLogs(currentAdmin, "User delete cancelled");
					}
				}
				else
				{
					fprintf(nfp, "%s", line);
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
			if(remove("./temp") == 0)
			{
				return;
			}
		}
		else if(delete == 1)
		{		
			if(remove("./userdata.bin") == 0)
			{
				if(rename("./temp", "./userdata.bin") == 0)
				{
					printf("User deleted successfully!\n");
				}					
			}	
		}
		else
		{
			if(remove("./temp") == 0)
			{
				printf("Removed temp file\n");
			}
		}

	}
}

char *createPassword()
{
	/* passwords must be between 8-16 characters,
	 * must contain at least one CAPITAL, number, and special character
	 */
	if(16 > SIZE_MAX/sizeof(char))
	{
		printf("Not enough memory!\n");
		exit(1);
	}
	char *password = (char*)calloc(16, sizeof(char));
	if(NULL == password)
	{
		free(password);
		password = NULL;
	}
	
	strcpy(password, sread(16));
	
	if(strlen(password) >= 8 && strlen(password) <= 16)
	{
		int capflag = 0;
		int numflag = 0;
		int spcflag = 0;
		//int i;
		size_t i;
		
		//size_t returns an int value for the size of the object
		const size_t passLength = strlen(password);
		
		
		for(i = 0; i < passLength; i++)
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
		else
		{
			printf("Password does not contain a capital, number, or special character!\n");
			pressEnterKey();
			if(password != NULL)
			{
				password = NULL;
			}
			
			return "null";
		}
	}
	else
	{
		printf("Password is not between 8 and 16 characters!\n");
		pressEnterKey();
		if(password != NULL)
		{
			password = NULL;
		}
		return "null";
	}
}
//////////////////////////////

/* DATA */
void addNewPatient()
{ 
   // open file
   FILE *fp = fopen("./patients.bin","a");

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

		if(3 > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *c = (char*)calloc(3, sizeof(char));
		if(NULL == c)
		{
			free(c);
			c = NULL;
		}

		if(10 > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *social = (char*)calloc(10, sizeof(char));
		if(NULL == social)
		{
			free(social);
			social = NULL;
		}

		if(11 > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *dob = (char*)calloc(11, sizeof(char));
		if(NULL == dob)
		{
			free(dob);
			dob = NULL;
		}
		
		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *fname = (char*)calloc(MAX_CHAR, sizeof(char));
		if(NULL == fname)
		{
			free(fname);
			fname = NULL;
		}

		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}		
		char *lname = (char*)calloc(MAX_CHAR, sizeof(char));
		if(NULL == lname)
		{
			free(lname);
			lname = NULL;
		}

		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}		
		char *buffer = (char*)calloc(MAX_CHAR, sizeof(char));
		if(NULL == buffer)
		{
			free(buffer);
			buffer = NULL;
		}

		if(MAX_CHAR*4 > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}		
		char *string = (char*)calloc(MAX_CHAR*4, sizeof(char));
		if(NULL == string)
		{
			free(string);
			string = NULL;
		}
		
		string[0] = '\0';
	  
		// read data
		printf("Patient First Name: ");
		strcpy(fname, sread(MAX_CHAR));
		
		const size_t fnameLength = strlen(fname);
		
		if(fnameLength > SIZE_MAX)
		{
			printf("Invalid input length!\n");
			return;
		}
   
		printf("Patient Last Name: ");
		strcpy(lname, sread(MAX_CHAR));
		const size_t lnameLength = strlen(lname);
		if(lnameLength > SIZE_MAX)
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
		const size_t socialLength = strlen(social);
		if(socialLength > 9)
		{
			printf("Invalid input length!\n");
			return;
		}
	  
		printf("Patient Height(cm): ");
		height = (int)strtol(sread(3), NULL, 10);
		if(height > 300)
		{
			printf("Invalid input length!\n");
			return;
		}
	  
		printf("Patient Weight(lbs): ");
		weight = (int)strtol(sread(3), NULL, 10);
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

		if(c != NULL)
		{
			c = NULL;
		}

		if(social != NULL)
		{
			social = NULL;
		}
		
		if(dob != NULL)
		{
			dob = NULL;
		}

		if(fname != NULL)
		{
			fname = NULL;
		}
		
		if(lname != NULL)
		{
			lname = NULL;
		}

		if(buffer != NULL)
		{
			buffer = NULL;
		}

		if(string != NULL)
		{
			string = NULL;
		}
   }
}

void setAllergyInfo(int ssnhash)
{
	FILE *fpa = fopen("./allergy.bin", "a");
	
	if(fpa == NULL)
	{
		printf("Error: Could not find or open \"allergy.bin\" in the directory!\n");
		pressEnterKey();
		fclose(fpa);
		return;
	}
	else
	{	
		if(MAX_CHAR*10 > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *string = (char*)calloc(MAX_CHAR*10, sizeof(char));
		if(NULL == string)
		{
			free(string);
			string = NULL;
		}

		if(MAX_CHAR*10 > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}		
		char *allergies = (char*)calloc(MAX_CHAR*10, sizeof(char));
		if(NULL == allergies)
		{
			free(allergies);
			allergies = NULL;
		}
		
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
		fprintf(fpa, "%s", string);

		if(string != NULL)
		{
			string = NULL;
		}
		
		if(allergies != NULL)
		{
			allergies = NULL;
		}
	}
	
	fclose(fpa);
}

void getAllergyInfo(int ssnhash)
{
	FILE *fpa = fopen("./allergy.bin", "r");
	
	if(fpa == NULL)
	{
		printf("Could not find or open \"allergy.bin\" in the directory!\n");
		pressEnterKey();
		fclose(fpa);
		return;
	}
	else
	{
		const int count = getUserCount(fpa); //number of patients in file
		int found = 0;
		int i;
		char *token;
		
		if(MAX_CHAR*10 > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *temp = (char*)calloc(MAX_CHAR*10, sizeof(char));
		if(NULL == temp)
		{
			free(temp);
			temp = NULL;
		}
		
		if(fseek(fpa, 0, SEEK_SET) == 0);
		
		for(i = 0; i < count; ++i)
		{
			char buffer[MAX_CHAR*11];
			
			if(found == 0)
			{
				if(fgets(buffer, MAX_CHAR*11, fpa) != NULL)
				{
					strncpy(buffer, decrypt(buffer), MAX_CHAR*11);
					strcpy(temp, buffer);
					
					/* tokenize the line */
					token = strtok(temp, "|");
					
					/* hash match? */
					if(ssnhash == (int)strtol(token, NULL, 10))
					{
						printf("Patient Allergies\n");
						printf("-----------------\n");
						
						/* get the number of commas for counting purposes */
						int comma = 0;
						int j = 0;
						signed int c = 0;
						
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
							token = strtok(NULL, ",");
							printf("%d.) %s\n", i+1, token);
						}
						
						found = 1;
					}
				}
				else break;
			}
			else break;
		}
		
		if(found == 0)
		{
			printf("\nCould not find allergy information for the specified patient.\n");
			pressEnterKey();
		}
		
		fclose(fpa);
		
		if(temp != NULL)
		{
			temp = NULL;
		}
	}	
}

void setPrescriptionInfo(int ssnhash)
{
	FILE *fpp = fopen("./prescriptions.bin", "a");
	
	if(fpp == NULL)
	{
		printf("Could not find or open \"prescriptions.bin\" in the directory!\n");
		fclose(fpp);
		pressEnterKey();
		return;
	}
	else
	{
		if(MAX_CHAR*10+9 > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *string = (char*)calloc(MAX_CHAR*10+9, sizeof(char));
		if(NULL == string)
		{
			free(string);
			string = NULL;
		}
		
		if(MAX_CHAR*10 > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *prescriptions = (char*)calloc(MAX_CHAR*10, sizeof(char));
		if(NULL == prescriptions)
		{
			free(prescriptions);
			prescriptions = NULL;
		}
		
		printf("Please type in a prescription and then hit ENTER to finish. For multiple prescriptions, please separate each one by a comma:\n");
		printf("--> ");
		strcpy(prescriptions, sread(MAX_CHAR*10));
		
		/* format the data */
		snprintf(string, 9, "%d", ssnhash);
		strcat(string, "|");
		strcat(string, prescriptions);
		/* encrypt */
		strncpy(string, encrypt(string), MAX_CHAR*10+9);
		strcat(string, "\n");
		
		/* write to file */
		fprintf(fpp, "%s", string);

		if(string != NULL)
		{
			string = NULL;
		}
		
		if(prescriptions != NULL)
		{
			prescriptions = NULL;
		}
	}
	
	fclose(fpp);
}

void getPrescriptionInfo(int ssnhash)
{
	FILE *fpp = fopen("./prescriptions.bin", "r");
	
	if(fpp == NULL)
	{
		printf("Could not find or open \"prescriptions.bin\" in the directory!\n");
		pressEnterKey();
		fclose(fpp);
		return;
	}
	else
	{
		const int count = getUserCount(fpp); //number of patients in file
		int found = 0;
		int i;
		char *token;
		
		if(MAX_CHAR*10 > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *temp = (char*)calloc(MAX_CHAR*10, sizeof(char));
		if(NULL == temp)
		{
			free(temp);
			temp = NULL;
		}
		
		if(fseek(fpp, 0, SEEK_SET) == 0);
		
		for(i = 0; i < count; ++i)
		{
			char buffer[MAX_CHAR*11];
			
			if(found == 0)
			{
				if(fgets(buffer, MAX_CHAR*11, fpp) != NULL)
				{
					strncpy(buffer, decrypt(buffer), MAX_CHAR*11);
					strcpy(temp, buffer);
					
					/* tokenize the line */
					token = strtok(temp, "|");
					
					/* hash match? */
					if(ssnhash == (int)strtol(token, NULL, 10))
					{
						printf("Patient Prescription(s)\n");
						printf("-----------------------\n");
						
						/* get the number of commas for counting purposes */
						int comma = 0;
						int j = 0;
						signed int c = 0;
						
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
							token = strtok(NULL, ",");
							printf("%d.) %s\n", i+1, token);
						}
						
						found = 1;
					}
				}
				else break;
			}
			else break;
		}
		
		if(found == 0)
		{
			printf("\nCould not find prescription information for the specified patient.\n");
			pressEnterKey();
		}
		
		if(temp != NULL)
		{
			temp = NULL;
		}
		
		fclose(fpp);
	}	
}

void findPatient()
{
	FILE *fp = fopen("./patients.bin", "r");
	
	if(fp == NULL)
	{
		printf("Could not find \"patients.bin\" in the directory!\n");
		pressEnterKey();
		return;
	}
	else
	{
		const int count = getUserCount(fp); //number of patients in file
		int i;
		int found = 0;

		drawPatientSearch(fp);
		fflush(stdin);
		printf("SSN to search: ");
		const int hashvalue = hash(sread(9));

		if(fseek(fp, 0, SEEK_SET) == 0);
		
		for(i = 0; i < count; ++i)
		{
			if(found == 0)
			{
				if(MAX_CHAR > SIZE_MAX/sizeof(char))
				{
					printf("Not enough memory!\n");
					exit(1);
				}
				char *temp = (char*)calloc(MAX_CHAR, sizeof(char));
				if(NULL == temp)
				{
					free(temp);
					temp = NULL;
				}
				
				char buffer[MAX_CHAR];
				char *token;
				
				if(fgets(buffer, MAX_CHAR, fp) != NULL)
				{
					strncpy(buffer, decrypt(buffer), MAX_CHAR);
					strcpy(temp, buffer);
					
					/* tokenize the line */
					token = strtok(temp, ",");
					
					printf("we have %d, looking for %d\n", hashvalue, (int)strtol(token, NULL, 10));
					/* hash match? */
					if(hashvalue == (int)strtol(token, NULL, 10))
					{
						found = 1;
						token = strtok(NULL, ",");
						const char *lname = token;
						token = strtok(NULL, ",");
						const char *fname = token;
						token = strtok(NULL, ",");
						const char *dob = token;
						token = strtok(NULL, ",");
						const int h = (int)strtol(token, NULL, 10);
						token = strtok(NULL, ",");
						const int w = (int)strtol(token, NULL, 10);
						token = strtok(NULL, ",");
						const int a = (int)strtol(token, NULL, 10);
						token = strtok(NULL, ",");
						const int su = (int)strtol(token, NULL, 10);
						token = strtok(NULL, ",");
						const int sm = (int)strtol(token, NULL, 10);
						token = strtok(NULL, ",");
						const int m = (int)strtol(token, NULL, 10);
						token = strtok(NULL, ",");
						const int dr = (int)strtol(token, NULL, 10);
						
						drawPatientInfo();
						
						printf("         First Name: %s\n", fname);
						printf("          Last Name: %s\n", lname);
						printf("      Date of Birth: %s\n", dob);
						printf("        Height (cm): %d\n", h);
						printf("        Weight (lb): %d\n", w);
						printf("      Has allergies? %d\n", a);
						printf("   On Prescriptions? %d\n", dr);				
						printf("    Is/was a smoker? %d\n", sm);
						printf(" Previous surgeries? %d\n", su);
						printf(" Has mental illness? %d\n", m);
						printf("\n");
						
						/* if they have allergies, list them */
						if(a == 1)
						{
							printf("\nPress [ENTER] to view allergy information\n");
							if(getchar())
							{
								getAllergyInfo(hashvalue);
								printf("\n");
							}
						}
						
						/* if they have allergies, list them */
						if(dr == 1)
						{
							printf("\nPress [ENTER] to view prescription information\n");
							if(getchar())
							{
								getPrescriptionInfo(hashvalue);
								printf("\n");
							}
						}

						pressEnterKey();
					}
				}
			}
			else break;
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
		const int count = getUserCount(fp); //number of patients in file
		int value;
		int i;
		
		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *input = (char*)calloc(MAX_CHAR, sizeof(char));
		if(NULL == input)
		{
			free(input);
			input = NULL;
		}
		else
		{
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
			strncpy(input, sread(1), 3);

			if((int)strlen(input) < 2)
			{
				value = (int)strtol(input, NULL, 10);
				input = NULL;
			}
			else
			{
				printf("\nInvalid search parameter!\n");
				pressEnterKey();
				return;
			}
			
			/* drawing elements */
			if(value == 1)
			{
				printf("\033[2J\033[;H");
				printf("---[ ALLERGIES ]---\n\n");
			}
			else if(value == 2)
			{
				printf("\033[2J\033[;H");
				printf("---[ PRIOR SURGERIES ]---\n\n");			
			}
			else if(value == 3)
			{
				printf("\033[2J\033[;H");
				printf("---[ SMOKERS ]---\n\n");			
			}
			else if(value == 4)
			{
				printf("\033[2J\033[;H");
				printf("---[ MENTAL ILLNESS ]---\n\n");			
			}
			else if(value == 5)
			{
				printf("\033[2J\033[;H");
				printf("---[ PRESCRIPTIONS ]---\n\n");			
			}
			else
			{
				printf("\nInvalid search parameter!\n");
				pressEnterKey();
				return;
			}
			
			char *token;
			
			if(fseek(fp, 0, SEEK_SET) == 0);
			
			for(i = 0; i < count; ++i)
			{
				if(MAX_CHAR > SIZE_MAX/sizeof(char))
				{
					printf("Not enough memory!\n");
					exit(1);
				}
				char *temp = (char*)calloc(MAX_CHAR, sizeof(char));
				if(NULL == temp)
				{
					free(temp);
					temp = NULL;
				}
				char buffer[MAX_CHAR];
			
				if(fgets(buffer, MAX_CHAR, fp) != NULL)
				{
					strncpy(buffer, decrypt(buffer), MAX_CHAR);
					strcpy(temp, buffer);
					
					/* tokenize the line */
					token = strtok(temp, ",");
					
					token = strtok(NULL, ",");
					const char *lname = token;
					token = strtok(NULL, ",");
					const char *fname = token;
					if((token = strtok(NULL, ",")));
					if((token = strtok(NULL, ",")));
					if((token = strtok(NULL, ",")));
					token = strtok(NULL, ",");
					const int a = (int)strtol(token, NULL, 10); //allergies bool
					token = strtok(NULL, ",");
					const int su = (int)strtol(token, NULL, 10); //surgeries bool
					token = strtok(NULL, ",");
					const int sm = (int)strtol(token, NULL, 10); //smoker bool
					token = strtok(NULL, ",");
					const int m = (int)strtol(token, NULL, 10); //mental bool
					token = strtok(NULL, ",");
					const int dr = (int)strtol(token, NULL, 10); //prescriptions bool
					
					if(value == 1)
					{
						if(a)
						{
							printf("%3d.) %s, %s\n", j, lname, fname);
							j++;
						}
					}
					
					if(value == 2)
					{
						if(su)
						{
							printf("%3d.) %s, %s\n", j, lname, fname);
							j++;
						}
					}
					
					if(value == 3)
					{
						if(sm)
						{
							printf("%3d.) %s, %s\n", j, lname, fname);
							j++;
						}
					}
					
					if(value == 4)
					{
						if(m)
						{
							printf("%3d.) %s, %s\n", j, lname, fname);
							j++;
						}
					}
					
					if(value == 5)
					{
						if(dr)
						{
							printf("%3d.) %s, %s\n", j, lname, fname);
							j++;
						}
					}

					if(temp != NULL)
					{
						temp = NULL;
					}
				}
				else break;
			}
			
			j--;
			
			if(j == 0)
			{
				printf("No patients match the search parameter.\n");
			}
			fclose(fp);
		}

	}
	pressEnterKey();
}

void deletePatient(User currentDoctor)
{
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
		printf("\033[2J\033[;H");
		printf("\n-------------[ DELETE PATIENT ]-------------\n");

		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *temp = (char*)calloc(MAX_CHAR, sizeof(char));
		if(NULL == temp)
		{
			free(temp);
			temp = NULL;
		}
		
		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *line = (char*)calloc(MAX_CHAR, sizeof(char));
		if(NULL == line)
		{
			free(line);
			line = NULL;
		}

		if(MAX_CHAR > SIZE_MAX/sizeof(char))
		{
			printf("Not enough memory!\n");
			exit(1);
		}
		char *social = (char*)calloc(MAX_CHAR, sizeof(char));
		if(NULL == social)
		{
			free(social);
			social = NULL;
		}
		
		char buffer[MAX_CHAR];

		/* get the user to delete */
		printf("\nSocial Security of the patient to delete: ");
		strcpy(social, sread(9));

		/* go ahead and get the hash value, overwrite the string from memory */
		const int hashsocial = hash(social);
		strncpy(social, wspace(strlen(social)), strlen(social));
		
		if(social != NULL)
		{
			social = NULL;
		}
		
		const int length = getUserCount(fp);
		int i;
		int flag = 0;
		int found = 0;
		int delete = 0;
		char *token;
		
		if(fseek(fp, 0, SEEK_SET) == 0)
		{
			if(fseek(nfp, 0, SEEK_SET) == 0)
			{
				/* go through userdata.bin until we find the user */
				for(i = 0; i < length; i++)
				{
					if(fgets(line, MAX_CHAR, fp) != NULL)
					{
						strncpy(buffer, decrypt(line), strlen(line));
						strcpy(temp, buffer);
						
						/* tokenize the line */
						token = strtok(temp, ",");
						
						/* check if the social security hash matches */
						if((int)strtol(token, NULL, 10) == hashsocial && found == 0)
						{
							found = 1;
							
							/* temporary holder for first, last name */
							if(MAX_CHAR > SIZE_MAX/sizeof(char))
							{
								printf("Not enough memory!\n");
								exit(1);
							}
							char *fname = (char*)calloc(MAX_CHAR, sizeof(char));
							if(NULL == fname)
							{
								free(fname);
								fname = NULL;
							}
							
							if(MAX_CHAR > SIZE_MAX/sizeof(char))
							{
								printf("Not enough memory!\n");
								exit(1);
							}
							char *lname = (char*)calloc(MAX_CHAR, sizeof(char));
							if(NULL == lname)
							{
								free(lname);
								lname = NULL;
							}
							
							token = strtok(NULL, ",");
							strcpy(lname, token);
							token = strtok(NULL, ",");
							strcpy(fname, token);
							
							printf("You are about to delete patient: %s, %s from the system. This action cannot be undone.\n", lname, fname);
							printf("Proceed? (Y\\N) ");
							
							if(1 > SIZE_MAX/sizeof(char))
							{
								printf("Not enough memory!\n");
								exit(1);
							}
							char *c = (char*)calloc(1, sizeof(char));
							if(NULL == c)
							{
								free(c);
								c = NULL;
							}
							
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
							}

							if(c != NULL)
							{
								c = NULL;
							}
							
							/* if admin says yes, then proceed */
							if(flag == 1)
							{
								/* verify admin's password */
								if(verify(currentDoctor))
								{
									/* we aren't going to edit the data as much as we won't copy, so continue */
									if(MAX_CHAR > SIZE_MAX/sizeof(char))
									{
										printf("Not enough memory!\n");
										exit(1);
									}
									char *string = (char*)calloc(MAX_CHAR, sizeof(char));
									if(NULL == string)
									{
										free(string);
										string = NULL;
									}
									
									strcpy(string, "Deleted patient: ");
									strcat(string, lname);
									strcat(string, ", ");
									strcat(string, fname);
									strcat(string, " from the system");
									writeLogs(currentDoctor, string);
									
									delete = 1;
									
									if(string != NULL)
									{
										string = NULL;
									}
								}
								else
								{
									printf("Invalid password!\n");
									fprintf(nfp, "%s", line);
									pressEnterKey();
								}
							}
							else if(flag == 0)
							{
								fprintf(nfp, "%s", line);
								writeLogs(currentDoctor, "Patient delete cancelled");
							}
						}
						else
						{
							/* copy over the users who are not affected */
							fprintf(nfp, "%s", line);
						}
					}
				}
			}
		}
		
		fclose(fp);
		fclose(nfp);
		
		if(found == 0)
		{
			printf("\nCould not find the specified patient in the file.\n");
			pressEnterKey();
		}
		else if(delete == 1)
		{		
			if(remove("./patients.bin") == 0)
			{
				if(rename("./temp2", "./patients.bin") == 0)
				{
					printf("Patient deleted successfully.\n");
					pressEnterKey();
				}
			}
		}
		else
		{
			if(remove("./temp2.bin") == 0)
			{
				printf("Patient deletion cancelled. The file has not been changed.\n");
				pressEnterKey();
			}
		}

	}
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

	if(MAX_CHAR > SIZE_MAX/sizeof(char))
	{
		printf("Not enough memory!\n");
		exit(1);
	}
	char *string = (char*)calloc(MAX_CHAR, sizeof(char));
	if(NULL == string)
	{
		free(string);
		string = NULL;
	}

	if(MAX_CHAR > SIZE_MAX/sizeof(char))
	{
		printf("Not enough memory!\n");
		exit(1);
	}
	char *userType = (char*)calloc(MAX_CHAR, sizeof(char));
	if(NULL == userType)
	{
		free(userType);
		userType = NULL;
	}
	
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
	
	if(string != NULL)
	{
		string = NULL;
	}
	
	if(userType != NULL)
	{
		userType = NULL;
	}
}

void readLogs()
{
	FILE *fp = fopen("logs.bin", "r");
	
	if(fp == NULL)
	{
		printf("Error: Could not open logs.bin!\n");
	}
	else
	{
		char buff[255];
		printf("\033[2J\033[;H");
		drawLogs();
		const int length = getUserCount(fp);
		int i;
		if(fseek(fp, 0, SEEK_SET) == 0);
		
		for(i = 0; i < length; i++)
		{
			if(i % 100 == 0)
			{
				printf("\n%d more logs to view:\n", length - i);
				pressEnterKey();
			}
			
			if(fgets(buff, 255, fp) != NULL)
			{
				if(feof(fp)) break;
				strncpy(buff, decrypt(buff), strlen(buff)-1);
				strcat(buff, "\n");
				printf("%s", buff);
			}
			else break;
		}
	}
	
	fclose(fp);
	pressEnterKey();
}
/////////////////////////////

/* HELPDESK */
void createAppointment()
{
	FILE *fp;
	fflush(stdin);
	drawAppointment();

	if(MAX_CHAR*4 > SIZE_MAX/sizeof(char))
	{
		printf("Not enough memory!\n");
		exit(1);
	}
	char *string = (char*)calloc(MAX_CHAR*4, sizeof(char));
	if(NULL == string)
	{
		free(string);
		string = NULL;
	}

	if(MAX_CHAR > SIZE_MAX/sizeof(char))
	{
		printf("Not enough memory!\n");
		exit(1);
	}
	char *fname = (char*)calloc(MAX_CHAR, sizeof(char));
	if(NULL == fname)
	{
		free(fname);
		fname = NULL;
	}
	
	if(MAX_CHAR > SIZE_MAX/sizeof(char))
	{
		printf("Not enough memory!\n");
		exit(1);
	}
	char *lname = (char*)calloc(MAX_CHAR, sizeof(char));
	if(NULL == lname)
	{
		free(lname);
		lname = NULL;
	}

	if(MAX_CHAR > SIZE_MAX/sizeof(char))
	{
		printf("Not enough memory!\n");
		exit(1);
	}
	char *purpose = (char*)calloc(MAX_CHAR, sizeof(char));
	if(NULL == purpose)
	{
		free(purpose);
		purpose = NULL;
	}

	if(10 > SIZE_MAX/sizeof(char))
	{
		printf("Not enough memory!\n");
		exit(1);
	}
	char *date = (char*)calloc(10, sizeof(char));
	if(NULL == date)
	{
		free(date);
		date = NULL;
	}
	
	signed int i = 0;
	
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
	strcat(string, "\n| Patient: ");
	strcat(string, lname);
	strcat(string, ", ");
	strcat(string, fname);
	strcat(string, "\n| Reason: ");
	strcat(string, purpose);
	strcpy(string, encrypt(string));
	strcat(string, "\n");
	
	fprintf(fp, "%s", string);
	
	strcpy(string, wspace(strlen(string)));
	if(string != NULL)
	{
		string = NULL;
	}
	
	strcpy(fname, wspace(strlen(fname)));
	if(fname != NULL)
	{
		fname = NULL;
	}
	
	strcpy(lname, wspace(strlen(lname)));
	if(lname != NULL)
	{
		lname = NULL;
	}
	
	strcpy(date, wspace(strlen(date)));
	if(date != NULL)
	{
		date = NULL;
	}
	
	strcpy(purpose, wspace(strlen(purpose)));
	if(purpose != NULL)
	{
		purpose = NULL;
	}
	
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
		int i = 1;
		
		if(fseek(fp, 0, SEEK_SET) == 0);
		
		while(1)
		{
			if(fgets(buff, 255, fp) != NULL)
			{
				strncpy(buff, decrypt(buff), strlen(buff) - 1);
				if(feof(fp)) break;
				printf("%3d.) %s\n", i, buff);
				i++;
			}
			else break;
		}
		
		fclose(fp);
		pressEnterKey();
	}
}
/////////////////////////////////////////////////////

/* DRAWING */
/* login banner */
void drawLogin()
{
	/* clear the screen */
	printf("\033[2J\033[;H");
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
	const char *name = userGetName(currentUser);
	const int type = userGetType(currentUser);
	printf("\033[2J\033[;H");
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
	const int count = getUserCount(fp);
	printf("\033[2J\033[;H");
	printf("\n-------------[ PATIENT SEARCH ]-------------\n");
	printf("Please enter the social security number of the patient.\n");
	printf("Patients in file: %d\n", count);
	printf("\n");
}

void drawFilteredSearch(FILE *fp)
{
	const int count = getUserCount(fp);
	printf("\033[2J\033[;H");
	printf("\n-------------[ FILTERED SEARCH ]-------------\n");
	printf("Here you can view all patients that meet a specific criteria.\n");
	printf("For more detailed information, please use Search Patient.\n");
	printf("Patients in file: %d\n", count);
	printf("\n");
}

void drawPatientInfo()
{
	printf("\033[2J\033[;H");
	printf("\n-------------[ PATIENT INFO ]-------------\n");
	printf("\n");
}

void drawPatientNew()
{
	printf("\033[2J\033[;H");
	printf("\n-------------[ CREATE NEW PATIENT ]-------------\n");
	printf("\n");
}

void drawAppointment()
{
	printf("\033[2J\033[;H");
	printf("\n------------[ NEW APPOINTMENT ]------------\n");
	printf("\n");
}

void drawAppointmentList(FILE * fp)
{
	const int count = getUserCount(fp);
	printf("\033[2J\033[;H");
	printf("\n------------[ APPOINTMENT LIST ]------------\n");
	printf("\nThere are currently %d active appointments:\n", count);
	printf("\n");
}

void drawLogs()
{
	printf("\033[2J\033[;H");
	printf("\n------------[ LOGS ]------------\n");
	printf("\n");
}

void drawViewUsers()
{
	printf("\033[2J\033[;H");
	printf("\n------------[ USER LIST ]------------\n");
	printf("\n");	
}

void drawAddUser()
{
	printf("\033[2J\033[;H");
	printf("\n------------[ ADD USER ]------------\n");
	printf("\n");
}

void drawPassword()
{
	printf("\033[2J\033[;H");
	printf("\n------------[ CHANGE PASSWORD ]------------\n");
	printf("\n");
}

void drawExit()
{
	printf("\033[2J\033[;H");
	printf("'':. You have been successfully logged off! .:''\n");
	printf("  ':.: The program will close momentarily :.:'\n");
	printf("\n");		
}
/////////////////////////////////////////////
