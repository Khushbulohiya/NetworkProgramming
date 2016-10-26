/*
 * Name - Khushbu Lohiya
 * File - Execve
 */


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char *args[])
{

	int x, y;

	char x_[10];
	printf("\n\nPlease enter number 1: ");
	scanf("%d", &x);
	sprintf(x_, "%d", x);

	char y_[10];
	printf("Please enter number 2: ");
	scanf("%d", &y);
	sprintf(y_, "%d", y);

	pid_t pid;
	char *file = "add";
	char *argv[] = { "add", x_, y_, NULL };
	char *envp[] = { NULL };

	if((pid = fork()) == -1)
	{
		printf("Error occured while forking\n");
	}
	else if(pid == 0)
	{
		printf("Spawning %s executable from the child process to calculate the sum:\n", file);
		execve(file, argv, envp);
	}
	else
	{
		printf("The sum for the two numbers in parent process is: %d\n\n", (x + y));
		int status;
		// wait for child process to end
		waitpid(pid, &status, 0);
	}
}

