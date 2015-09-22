#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<fcntl.h>


int main(int argc, char *argv[])
{	
	while(1)
	{
		int childPid;
		
		// print command line

		// read input from command line or batch file

		// parse input to get command

		// record command for history

		// if built in command, execute command

		// else system call

		childPid = fork();
		if(childPid > 0) {
			// call execvp
		} else {
			waitpid(childPid, NULL, 0);
		}
	}
	return 0;
}
