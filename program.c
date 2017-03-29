#include<stdio.h>
#include<stdlib.h>
#include<strings.h>

#include "login.h"
#include "draw.h"
#include "data.h"

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
		//addPatient();
		
		User currentUser = sessionGetUser(newSession);
		int menuchoice;
		int type = userGetType(currentUser);
		
		/* user authenticated, draw the menu */
		drawMenu(currentUser);
		
		/* now we need to get the user input */
		printf("Please enter a choice and press ENTER -> ");
		scanf("%d", &menuchoice);
		
		/* doctor/nurse */
		if(type == 0 || type == 1)
		{
			if(menuchoice == 1)
			{
				/* search patients */
			}
			else if(menuchoice == 2)
			{
				/* create new patient */
			}
			else if(menuchoice == 3)
			{
				/* change password */
			}
			else if(menuchoice == 4)
			{
				/* log off */
			}
		}
		/* help desk */
		else if(type == 2)
		{
			
		}
		
		
	}

	return 0;
}
