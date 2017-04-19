#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "login.h"
#include "data.h"
#include "auditLogs.h"
#include "helpdesk.h"
#include "draw.h"

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
		char *menustring = malloc(sizeof(char*)*3);
		int type = userGetType(currentUser);
		
		system("clear");
		
		while(active == 0)
		{
			/* user authenticated, draw the menu */
			drawMenu(currentUser);
		
			if(type >= 0 && type < 5)
			{
				/* now we need to get the user input */
				printf("Please enter a choice and press ENTER -> ");
				strcpy(menustring, sread(2));

				if(strlen(menustring) > 1)
				{
					printf("\nInvalid choice. Please try again.\n");
				}
				else
				{
					menuchoice = atoi(menustring);
				}
				
			}
			else active = 1;
			
			/* doctor/nurse */
			if(type == 0 || type == 1)
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
						writeLogs(currentUser, "Menu -> Create New Patient");
						addNewPatient();
					}
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
					/* delete appointment */
					if(verify(currentUser))
					{
						writeLogs(currentUser, "Menu -> Deleted Appointment");
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
	system("clear");
	return 0;
}