/*
 * Name Khushbu Lohiya
 * File - Fork
 */


#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

#define NUM_OF_CHILDS 3

void fork_child_work(pid_t, int);
void fork_parent_work(int);

int main()
{
	pid_t child_process_id[NUM_OF_CHILDS];

	for (int i = 0; i < NUM_OF_CHILDS; i++) {
		pid_t process_id = fork();

			// child process
		if (process_id == 0) {
			fork_child_work(i + 1, rand());
			exit(0);
		} else if (process_id > 0) {
			// parent can do some work here

			// for example book keeping of the child's process id
			// store the process id of each child process to wait before
			// parent ends
			child_process_id[i] = process_id;

			printf("\nChild %d given work by parent\n", process_id);
			// in parent the process_id is greater than zero
			// i.e. child's process id
			continue;
		} else {
			printf("Error forking and creating child process.\n");
		}
	}
	// only for parent process
	
	printf("\nParent is running here.\n");
	fork_parent_work(rand());

	// wait for each child to complete
	printf("Wait for children to end in parent.\n");
	for (int i = 0; i < NUM_OF_CHILDS; i++) {
		int status;
		waitpid(child_process_id[i], &status, 0);
		printf("Child Process %d ended with status %d\n", child_process_id[i], status);
	}
}

void fork_parent_work(int x)
{
	int sum;
	printf("\nIn parent Process\n");
	for(int i = 0; i <= x; i++)
	{
		sum = sum + i;
	}
	printf("The sum is %d in parent process\n", sum);
}


void fork_child_work(int child_no, int x)
{
	int sum;
	printf("\nIn Child Process: %d\n", child_no);
	for(int i = 0; i <= x; i++)
	{
		sum = sum + i;
	}
	printf("The sum is %d in CHILD process %d\n", sum, child_no);
}





