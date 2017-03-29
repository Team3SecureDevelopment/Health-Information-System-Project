#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "login.h"
#include "draw.h"

/* Commonly used drawing characters
 *
 * ░ ▒ ▓ █ ▄ ▌ ▐ ▀
 * │ ┤ ╡ ╢ ╖ ╕ ╣ ║ ╗ ╝ ╜ ╛ ┐ └ ┴ ┬ ├ ─ ┼ ╞ ╟ ╚ ╔ ╩ ╦ ╠ ═ ╬ ╧ ╨ ╤ ╥ ╙ ╘ ╒ ╓ ╫ ╪ ┘ ┌
 */
 
 
/* login banner */
void drawLogin()
{
	/* clear the screen */
	system("clear");
	
	printf("\n\n");
	printf("     \x1b[36;1;44m╔═════════════════════╗\x1b[0m\n");
	printf("     \x1b[36;1;44m║    Health Record    ║\x1b[0m\n");
	printf("     \x1b[36;1;44m║     System 2000     ║\x1b[0m\n");
	printf("     \x1b[36;1;44m║        v 1.4        ║\x1b[0m\n");
	printf("     \x1b[36;1;44m╚═════════════════════╝\x1b[0m\n");
	printf("\n");
	printf("             Welcome!\n");
}

/* draw the main menu depending on user */
void drawMenu(User currentUser)
{
	/* clear the screen */
	system("clear");
	
	char *name = userGetName(currentUser);
	int type = userGetType(currentUser);

		printf("\n\n");
		printf("\x1b[36;1;44m╔═════════════════════════════════\x1b[34;1;46;22m MAIN MENU \x1b[36;1;44m══════════════════════════════════╗\x1b[0m\n");
		printf("\x1b[36;1;44m║ Hello, \x1b[33;1;44m%-69s\x1b[36;1;44m ║\x1b[0m\n", name);
		printf("\x1b[36;1;44m╟──────────────────────────────────────────────────────────────────────────────╢\x1b[0m\n");
	/* if doctor/nurse */
	if(type == 0 || type == 1)
	{
		printf("\x1b[36;1;44m║                                                                              ║\x1b[0m\n");
		printf("\x1b[36;1;44m║                                                                              ║\x1b[0m\n");
		printf("\x1b[36;1;44m║                                                                              ║\x1b[0m\n");
		printf("\x1b[36;1;44m║                                                                              ║\x1b[0m\n");
		printf("\x1b[36;1;44m║                          ┌───┬────────────────────┐                          ║\x1b[0m\n");
		printf("\x1b[36;1;44m║                          │ \x1b[31m1\x1b[36;1;44m │    Search Patients │                          ║\x1b[0m\n");
		printf("\x1b[36;1;44m║                          │ \x1b[31m2\x1b[36;1;44m │ Create New Patient │                          ║\x1b[0m\n");
		printf("\x1b[36;1;44m║                          ├───┼────────────────────┤                          ║\x1b[0m\n");
		printf("\x1b[36;1;44m║                          │ \x1b[31m3\x1b[36;1;44m │    Change Password │                          ║\x1b[0m\n");
		printf("\x1b[36;1;44m║                          │ \x1b[31m4\x1b[36;1;44m │            Log Off │                          ║\x1b[0m\n");
		printf("\x1b[36;1;44m║                          └───┴────────────────────┘                          ║\x1b[0m\n");
		printf("\x1b[36;1;44m║                                                                              ║\x1b[0m\n");
		printf("\x1b[36;1;44m║                                                                              ║\x1b[0m\n");
		printf("\x1b[36;1;44m║                                                                              ║\x1b[0m\n");
		printf("\x1b[36;1;44m║                                                                              ║\x1b[0m\n");
		printf("\x1b[36;1;44m║                                                                              ║\x1b[0m\n");
		printf("\x1b[36;1;44m╚══════════════════════════════════════════════════════════════════════════════╝\x1b[0m\n");
		printf("\n");
	}
	/* if help desk */
	else if(type == 2)
	{
		printf("║       ┌───┬────────────────────────┐       ║\n");
		printf("║       │ 1 │      View Appointments │       ║\n");
		printf("║       │ 2 │ Create New Appointment │       ║\n");
		printf("║       │ 3 │     Delete Appointment │       ║\n");
		printf("║       ├───┼────────────────────────┤       ║\n");
		printf("║       │ 4 │        Change Password │       ║\n");
		printf("║       │ 5 │                Log Off │       ║\n");
		printf("║       └───┴────────────────────────┘       ║\n");
		printf("╚════════════════════════════════════════════╝\n");
		printf("\n");
	}
	/* if auditor */
	else if(type == 3)
	{
		printf("║          ┌───┬──────────────────┐          ║\n");
		printf("║          │ 1 │ View System Logs │          ║\n");
		printf("║          ├───┼──────────────────┤          ║\n");
		printf("║          │ 2 │  Change Password │          ║\n");
		printf("║          │ 3 │          Log Off │          ║\n");
		printf("║          └───┴──────────────────┘          ║\n");
		printf("╚════════════════════════════════════════════╝\n");
		printf("\n");
	}
	/* if system admin */
	else if(type == 4)
	{
		printf("║           ┌───┬─────────────────┐          ║\n");
		printf("║           │ 1 │      View Users │          ║\n");
		printf("║           │ 2 │ Create New User │          ║\n");
		printf("║           │ 3 │     Delete User │          ║\n");
		printf("║           ├───┼─────────────────┤          ║\n");
		printf("║           │ 4 │ Change Password │          ║\n");
		printf("║           │ 5 │         Log Off │          ║\n");
		printf("║           └───┴─────────────────┘          ║\n");
		printf("╚════════════════════════════════════════════╝\n");
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