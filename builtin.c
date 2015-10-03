#include <stdlib.h>
#include <string.h>
#include "print.h"
#include "history.h"

char builtins[][10] = {"exit", "history", "!"};
char *re_exec_cmd;

void call_exit(char **shArgs)
{
	if(shArgs[1] == NULL) {
		exit(0);
	} else {
		printError();
	}
}

void call_history(char **shArgs)
{
	if(shArgs[1] == NULL) {
        display_all_cmd();
	} else {
		printError();
	}	

}

int is_builtin(char** shArgs)
{
    int i;

    if (shArgs[0] == NULL)
        return -1;

    for (i = 0; i < sizeof(builtins)/sizeof(builtins[0]); i++) {
        if (strncmp(shArgs[0], builtins[i], sizeof(builtins[i])) == 0) {
            return i;
        }
    }

    return -1;
}

int do_builtin(char** shArgs)
{
	int isBuiltIn = 0;
	if(strcmp("exit", shArgs[0]) == 0) {
		isBuiltIn = 1;
		call_exit(shArgs);
	} else if(strcmp("history", shArgs[0]) == 0) {
		isBuiltIn = 1;
		call_history(shArgs);
	}
	return isBuiltIn;
}

int check_re_exec(char **shArgs)
{
	int isValid = 0;
	if(shArgs[1] == NULL) {
		if(strcmp("!", shArgs[0]) == 0) {
			isValid = 1;
			re_exec_cmd = strdup(get_last_cmd());
		}
	} else if(shArgs[2] == NULL) {
	
	}
	return isValid;
}

