/* ex: set tabstop=4 noexpandtab: */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<fcntl.h>

#define MAX_LENGTH 514
#define TOKEN_DELIM " \t\n\r"

void printError()
{
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));
}

char *readInput()
{
	char* cmdLine = malloc(MAX_LENGTH * sizeof(char));

	//TODO: check if we need to handle null return case
	fgets(cmdLine, MAX_LENGTH, stdin);

	//input string is more than 512 characters
	if(strlen(cmdLine) == MAX_LENGTH -1 && cmdLine[MAX_LENGTH -2] != '\n') {
		printError();
		return NULL;
	}

	return cmdLine;
}

char **parseInput(char *cmdLine)
{
    //TODO: check suitable size to allocate memory
	char **tokenList = malloc (MAX_LENGTH * sizeof(char*));
	char *token = strtok(cmdLine, TOKEN_DELIM);
	int i = 0;
	while (token != NULL) {
		tokenList[i] = token;
		i++;
		token = strtok(NULL, TOKEN_DELIM);
	}
		tokenList[i] = NULL;
	return tokenList;
}

int process_file(char * batch_file)
{
	return 0;
}

void run(void)
{
	while (1) {
		int childPid;

		// print command line
		printf("mysh # ");

		// read input from command line or batch file
		char *cmdLine = readInput();

		if (cmdLine != NULL && strlen(cmdLine) > 1) {
			// parse input to get command
			char **shArgv = parseInput(cmdLine);
			// record command for history

			// if built in command, execute command

			// else system call

			childPid = fork();
			if (childPid == 0) {
				execvp(shArgv[0], shArgv);
				printf("Error in command execution\n");
			}
			else if (childPid > 0) {
				waitpid(childPid, NULL, 0);
			}
			free(cmdLine);
			free(shArgv);
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc == 1) {
		// Run interactive shell
		run();
	} else if (argc == 2) {
		// Run batchmode processing
		return process_file(argv[1]);
	} else {
		// unsupported mode!
		printError();
		return 1;
	}
	return 0;
}
