/* ex: set tabstop=4 noexpandtab: */
#include <unistd.h>
#include <string.h>

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

void display_command(char * command)
{
	write(STDOUT_FILENO, command, strlen(command));
}
