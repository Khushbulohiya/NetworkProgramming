/*
 * Name - Khushbu Lohiya
 * File - Semaphore
 */

#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<semaphore.h>
#include<fcntl.h>

#define MAX_COOKIES 10
#define MAX_JAR_SIZE 3

int total_cookies_baked = 0;
int cookie_jar[MAX_JAR_SIZE];
int number_of_cookies_in_jar = 0;

sem_t *empty;
sem_t *full;

void *baker();
void *eater();

int pass = 0;

int main(int argc, char *argv[])
{
	empty = sem_open("empty_4", O_CREAT, 0600, 1);
	if(empty == SEM_FAILED) {
		perror("parent sem_open");
		return -1;
	}
	full = sem_open("full_4", O_CREAT, 0600, 0);
	if(full == SEM_FAILED) {
		perror("parent sem_open");
		return -1;
	}
	pthread_t thread1, thread2, thread3;
	char *eater1 = "Eater 1";

	pthread_create( &thread2, NULL, &eater, eater1);
	pthread_create( &thread1, NULL, &baker, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	sem_unlink("empty_4");
	sem_unlink("full_4");

	sem_close(full);
	sem_close(empty);
	return 1;
}

void *baker()
{
	while(1)
	{
		// if jar is full wait for the eater to eat
		sem_wait(empty);

		// terminating condition
		if (total_cookies_baked == MAX_COOKIES)
		{
			sem_post(full);
			break;
		}
		// once there is space in jar start creating cookies in jar
		while(number_of_cookies_in_jar < MAX_JAR_SIZE && total_cookies_baked < MAX_COOKIES)
		{
			cookie_jar[number_of_cookies_in_jar++] = total_cookies_baked;
			printf("Cookie baked number %d\n", total_cookies_baked);
			total_cookies_baked++;
		}
		// once cookies are baked signal to eater to start eating
		sem_post(full);
	}
	pthread_exit(&pass);
}

void *eater(void * eater_name){

	char * eatername = (char *)eater_name;

	while (1)
	{
		// if no cookies in jar wait for baker to bake some
		sem_wait(full);

		// terminating condition
		// once max cookies are baked and eaten
		if (total_cookies_baked == MAX_COOKIES && number_of_cookies_in_jar == 0)
		{
			break;
		}
		while (number_of_cookies_in_jar > 0)
		{
			// once signal received from baker
			// eat the cookie on the top of the jar
			// and consume the cookie
			number_of_cookies_in_jar--;
			int cookie = cookie_jar[number_of_cookies_in_jar];
			printf("%s ate cookie number %d\n", eatername, cookie);
		}
		// signal the baker as there is space in jar now for the baker to
		// bake cookie and fill the jar
		sem_post(empty);
	}
	pthread_exit(&pass);
}
