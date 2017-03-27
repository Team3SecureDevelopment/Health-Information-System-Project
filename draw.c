#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "login.h"

/* Commonly used drawing characters
 *
 * ░ ▒ ▓ █ ▄ ▌ ▐ ▀
 * │ ┤ ╡ ╢ ╖ ╕ ╣ ║ ╗ ╝ ╜ ╛ ┐ └ ┴ ┬ ├ ─ ┼ ╞ ╟ ╚ ╔ ╩ ╦ ╠ ═ ╬ ╧ ╨ ╤ ╥ ╙ ╘ ╒ ╓ ╫ ╪ ┘ ┌
 */
 
 
/* login banner */
void drawLogin()
{
	/* clear the screen */
	//system(clear);
	
	printf("\n\n");
	printf("     .---------------------.\n");
	printf("     |    Health Record    |\n");
	printf("     |     System 2000     |\n");
	printf("     |                     |\n");
	printf("     |        v 1.4        |\n");
	printf("     '---------------------'\n");
	printf("\n");
	printf("             Welcome!\n");
}

/* draw the main menu depending on user */
void drawMenu(User currentUser)
{
	/* clear the screen */
	//system(clear);
	
	printf("\n");
	printf("---> Hello, %s\n", currentUser->name);
	printf("\n\n");
	printf("              .-----------.\n");
	printf("              | Main Menu |\n");
	printf("--------------'-----------'------------------\n\n");
	
	/* if doctor/nurse */
	if(currentUser->type == 0 || currentUser->type == 1)
	{
		printf("     1 |    Search Patients\n");
		printf("     2 | Create New Patient\n");
		printf("     ----------------------\n");
		printf("     4 |    Change Password\n");
		printf("     5 |            Log Off\n");
		printf("\n");
	}
	/* if help desk */
	else if(currentUser->type == 2)
	{
		printf("     1 |      View Appointments\n");
		printf("     2 | Create New Appointment\n");
		printf("     3 |     Delete Appointment\n");
		printf("     --------------------------\n");
		printf("     4 |        Change Password\n");
		printf("     5 |                Log Off\n");
		printf("\n");
	}
	/* if auditor */
	else if(currentUser->type == 3)
	{
		printf("     1 | View System Logs\n");
		printf("     --------------------\n");
		printf("     2 |  Change Password\n");
		printf("     3 |             Exit\n");
		printf("\n");
	}
	/* if system admin */
	else if(currentUser->type == 4)
	{
		printf("     1 |      View Users\n");
		printf("     2 | Create New User\n");
		printf("     3 |     Delete User\n");
		printf("     -------------------\n");
		printf("     4 | Change Password\n");
		printf("     5 |         Log Off\n");
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
	}
}

/* debug driver */
int main()
{
	drawLogin();
	
	return 0;
}
