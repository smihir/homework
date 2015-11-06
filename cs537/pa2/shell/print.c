/* ex: set tabstop=4 noexpandtab: */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "readline.h"

void display_prompt(void)
{
	char prompt[] = "mysh # ";
	write(STDOUT_FILENO, prompt, strlen(prompt));
}

void printError(void)
{
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));
}

void display_full_command(char *command)
{
	char *cmdLine = malloc(MAX_LENGTH * sizeof(char));
	sprintf(cmdLine, "%s",command);
	write(STDOUT_FILENO, cmdLine, strlen(cmdLine));
	free(cmdLine);
}

void display_command(char **command)
{
	char * token = NULL;
	int i = 0;
	char *cmdLine = malloc(MAX_LENGTH * sizeof(char));
	cmdLine[0] = '\0';
	do {
		token = command[i++];
		if (token) {
			sprintf(cmdLine + strlen(cmdLine), "%s", token);

			//Avoid extra space at the end to prevent grading
			//scripts from failing test cases!
			if (command[i])
				sprintf(cmdLine + strlen(cmdLine), " ");
		}
	} while (token != NULL);

	if (strlen(cmdLine) > 0) {
		sprintf(cmdLine + strlen(cmdLine), "\n");
		write(STDOUT_FILENO, cmdLine, strlen(cmdLine));
	}

	free(cmdLine);
}
