/* ex: set tabstop=4 noexpandtab: */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "print.h"
#include "readline.h"

#define MAX_LENGTH 514
#define TOKEN_DELIM " \t\n\r"

//check if input string is more than 512 characters
static inline int overflow(char * str)
{
	return (strlen(str) == MAX_LENGTH - 1 &&
			str[MAX_LENGTH - 2] != '\n') ? 1 : 0;
}	

READ_STATUS readInput(FILE * input, char **cmdLine)
{
	*cmdLine = malloc(MAX_LENGTH * sizeof(char));

	//TODO: check if we need to handle null return case
	if (NULL == fgets(*cmdLine, MAX_LENGTH, input))
		goto out;

	//input string is more than 512 characters
	if (overflow(*cmdLine)) {
		printError();

		do {
			fgets(*cmdLine, MAX_LENGTH, input);
		} while(overflow(*cmdLine));

		return INPUT_READ_OVERFLOW;
	}

out:
	if (feof(input)) {
		return INPUT_READ_EOF;
	} else if (ferror(input)) {
		return INPUT_READ_ERROR;
	} else {
		return INPUT_READ_OK;
	}
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
