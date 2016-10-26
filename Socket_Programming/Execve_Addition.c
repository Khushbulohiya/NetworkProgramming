
 /*
 * Name - Khushbu Lohiya
 * File - Execve - Addition
 */



#include<stdio.h>
#include<stdlib.h>

int main(int argv, char *args[])
{
    int sum, x, y;
    printf("Welcome to addition program through execve.\n");

    x = atoi(args[1]);
    y = atoi(args[2]);
    sum = x + y;

    printf("The addition of two numbers %d and %d is %d in new process through execve.\n ", x, y, sum);
    printf("\nThank you!!\n\n\n\n");
    return 0;
}
