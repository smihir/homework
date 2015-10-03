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

enum mysh_modes {
	INTERACTIVE_MODE = 0,
	BATCH_MODE = 1,
};

void do_execute(char **shArgs, int do_redir, char *file)
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

		if (do_redir) {
			close(STDOUT_FILENO);
			open(file, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
			free(file);
		}
		execvp(shArgs[0], shArgs);

		printError();
		//execvp failed - Exit from the child!!!
		//otherwise child will continue after this function call...
		exit(1);
	}
	else if (childPid > 0) {
		if (do_redir) {
			free(file);
		}
		waitpid(childPid, NULL, 0);
	}
}

void run_cmd(char *cmdLine, int mode)
{
	char *histCmd;
	int redirect_status;

start:
	histCmd = strdup(cmdLine);

	redirect_status = check_redirection(cmdLine);
	if (redirect_status == REDIR_ERROR) {

		if (mode == BATCH_MODE) {
			display_full_command(histCmd);
		}

		printError();
		free(cmdLine);
		free(histCmd);
		return;
	}

	char **shArgv = parseInput(cmdLine);

	if (mode == BATCH_MODE && shArgv[0] != NULL) {
		display_full_command(histCmd);
	}

	int builtin = is_builtin(shArgv);
	if (builtin != -1 && redirect_status == REDIR_OK_REDIR) {
		printError();
		free(cmdLine);
		free(shArgv);
		free(histCmd);
		return;
	}

	if(builtin == 2) {
		int isValid = check_re_exec(shArgv);
		if(isValid) {
			free(cmdLine);
			free(shArgv);
			free(histCmd);

			cmdLine = strdup(re_exec_cmd);

			free(re_exec_cmd);
			goto start;
		} else {
			printError();
			free(cmdLine);
			free(shArgv);
			free(histCmd);
			return;
		}
	}

	if(shArgv[0] != NULL) {

		add_cmd(histCmd);

		if(do_builtin(shArgv)) {
			free(histCmd);
			free(cmdLine);
			free(shArgv);
			return;
		} else {
			do_execute(shArgv,
						redirect_status == REDIR_OK_REDIR ?
							1 : 0, redir_file);
		}
	}
	free(cmdLine);
	free(shArgv);
	free(histCmd);
}

int process_file(char *batch_file)
{
	FILE * batch_stream;
	char *cmdLine;
	READ_STATUS s;

	batch_stream = fopen(batch_file, "r");

	if (batch_stream == NULL) {
		printError();
		return 1;
	}

	for (s = readInput(batch_stream, &cmdLine);
		 (s != INPUT_READ_EOF) && (s != INPUT_READ_ERROR);
		 s = readInput(batch_stream, &cmdLine)) {

		if (s == INPUT_READ_OVERFLOW) {
			display_full_command(cmdLine);
			free(cmdLine);
			continue;
		}
		run_cmd(cmdLine, BATCH_MODE);
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

		if (s == INPUT_READ_EOF) {
			free(cmdLine);
			exit(0);
		}

		if (s == INPUT_READ_OK) {
			run_cmd(cmdLine, INTERACTIVE_MODE);
		}
	}
}

int main(int argc, char *argv[])
{	
	init_ll();
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
