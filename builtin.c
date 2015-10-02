#include <stdlib.h>
#include <string.h>
#include "print.h"


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
			
	} else {
		printError();
	}	

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
