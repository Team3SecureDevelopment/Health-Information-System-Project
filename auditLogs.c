#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	
	char *username, *purpose;
	
	username = "Kaldiron";
	purpose = "Logging in as patient";
	
	writeLogs(username, purpose);
	
	readLogs();
	
	return 0;
}

void writeLogs(char *username, char *purpose)
{
	
	FILE *fp;
	time_t current_time;
	char* c_time_string;
	int i = 0;
	
	if(access("Logs.txt", F_OK) != -1)
	{
		fp = fopen("Logs.txt", "r+");
		while((i = fgetc(fp)) != EOF)
		{
			i--;
		}
	}
	else
	{
		fp = fopen("Logs.txt", "w");
	}
	
	current_time = time(NULL);
	
	c_time_string = ctime(&current_time);
	
	fprintf(fp, c_time_string);
	fprintf(fp, username);
	fprintf(fp, " ");
	fprintf(fp, purpose);
	fprintf(fp, "\n");
	
	fclose(fp);
}

void readLogs()
{
	FILE *fp;
	
	int i = 0;
	char buff[255];
	
	fp = fopen("Logs.txt", "r");
	
	while(1)
	{
		fgets(buff, 255, (FILE*)fp);
		if(feof(fp))
			break;
		printf("%s", buff);
	}

	fclose(fp);
}
