/*
 * Name - Khushbu Lohiya
 * File - Mutex only
 */


#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<time.h>
#include<pthread.h>

#define MAX_COOKIES 10
#define MAX_JAR_SIZE 5

int total_cookies_baked = 0;
int cookie_jar[MAX_JAR_SIZE];
int number_of_cookies_in_jar = 0;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;

void *baker();
void *eater();

int pass = 0;

int main(int argc, char *argv[])
{
	pthread_t thread1, thread2;

	char * eater1 = "Eater 1";

	pthread_create( &thread2, NULL, &eater, eater1);
	pthread_create( &thread1, NULL, &baker, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	return 1;
}

void *baker()
{
	while(1)
	{
		pthread_mutex_lock(&count_mutex);
		// terminating condition
		if (total_cookies_baked == MAX_COOKIES)
		{
			pthread_mutex_unlock( &count_mutex );
			break;
		}
		// once there is space in jar start creating cookies in jar
		while(number_of_cookies_in_jar < MAX_JAR_SIZE && total_cookies_baked < MAX_COOKIES)
		{
			cookie_jar[number_of_cookies_in_jar++] = total_cookies_baked;
			printf("Cookie baked number %d\n", total_cookies_baked);
			total_cookies_baked++;
		}
		pthread_mutex_unlock( &count_mutex );
	}
	pthread_exit(&pass);
}

void *eater(void * eater_name){

	char * eatername = (char *)eater_name;

	while (1)
	{
		pthread_mutex_lock(&count_mutex);

		// terminating condition
		// once max cookies are baked and eaten
		if (total_cookies_baked == MAX_COOKIES && number_of_cookies_in_jar == 0)
		{
			pthread_mutex_unlock(&count_mutex);
			break;
		}

		// eat the cookie on the top of the jar
		// and consume the cookie
		number_of_cookies_in_jar--;
		int cookie = cookie_jar[number_of_cookies_in_jar];
		printf("%s ate cookie number %d\n", eatername, cookie);


		pthread_mutex_unlock(&count_mutex);
	}
	pthread_exit(&pass);
}

