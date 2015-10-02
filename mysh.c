/* ex: set tabstop=4 noexpandtab: */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <string.h>
#include <fcntl.h>
#include "readline.h"
#include "print.h"
#include "builtin.h"
#include "history.h"

void do_execute(char **shArgs)
{
	int childPid;

	// if first argument is NULL, silently return
	// we don't want to throw errors here because
	// this condtion is encountered when user presses
	// 'Enter' on an empty command line.
	if (shArgs[0] == NULL)
		return;

	childPid = fork();
	if (childPid == 0) {
		execvp(shArgs[0], shArgs);

		printError();
		//execvp failed - Exit from the child!!!
		//otherwise child will continue after this function call...
		exit(1);
	}
	else if (childPid > 0) {
		waitpid(childPid, NULL, 0);
	}
}

int process_file(char *batch_file)
{
	FILE * batch_stream;
	char *cmdLine;
	READ_STATUS s;

	batch_stream = fopen(batch_file, "r");

	if (batch_stream == NULL) {
		printError();
		return errno;
	}

	//while (cmdLine != NULL && strlen(cmdLine) > 1) {
	for (s = readInput(batch_stream, &cmdLine);
		 (s != INPUT_READ_EOF) && (s != INPUT_READ_ERROR);
		 s = readInput(batch_stream, &cmdLine)) {

		if (s == INPUT_READ_OVERFLOW) {
			free(cmdLine);
			continue;
		}

		// parse input to get command
		char **shArgv = parseInput(cmdLine);

		display_command(shArgv);

		// if built in command, execute command

		// else system call
		if(shArgv[0] != NULL) {
			if(do_builtin(shArgv)) {
				continue;
			} else {
				do_execute(shArgv);
			}
		}

		free(cmdLine);
		free(shArgv);
	}
	fclose(batch_stream);
	return 0;
}

void run(void)
{
	READ_STATUS s;
	char *cmdLine;
	while (1) {

		// print command line
		display_prompt();

		// read input from command line
		s = readInput(stdin, &cmdLine);

		if (s == INPUT_READ_OK || s == INPUT_READ_EOF) {
			// parse input to get command
			char **shArgv = parseInput(cmdLine);
			// record command for history

			// if built in command, execute command
			if(shArgv[0] != NULL) {
				if(do_builtin(shArgv)) {
					continue;
				} else {
					do_execute(shArgv);
				}
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
