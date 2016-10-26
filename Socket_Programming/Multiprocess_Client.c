/*
 * Name - Khushbu Lohiya
 * File - Multi-process_Client
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<time.h>
#include <sys/time.h>


int main(int argc, char* argv[])
{
	struct sockaddr_in server;
	int client_socket;
	char buffer[1024];

	if(argc != 3)
	{
		fprintf(stderr, "usage %s hostname port", argv[0]);
		return -1;
	}

	struct timeval start, end;
	gettimeofday(&start, NULL);

	int i = 0;
	while (i < 500) {
		i++;

		client_socket = socket(AF_INET, SOCK_STREAM, 0);
		if(client_socket == -1)
		{
			printf("Error creating socket.\n");
			return -1;
		}

		//struct sockaddr_in
		bzero(&server, sizeof(server));
		int port = atoi(argv[2]);
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = inet_addr(argv[1]);
		server.sin_port = htons(port);

		if(connect(client_socket, (struct sockaddr *)&server, sizeof(server)) < 0)
		{
			printf("Connection failed.\n");
			return -1;
		}

		printf("Connection done.\n");
		strcpy(buffer, "Sample client test message\0");

		printf("Sending message to server.\n");
		if(write(client_socket, buffer, strlen(buffer)) < 0)
		{
			printf("Message sending failed.\n");
			return -1;
		}
		printf("To Server from client %d: %s\n",client_socket,buffer);

		bzero(buffer, 1024);
		if(read(client_socket, buffer, 1000) < 0)
		{
			printf("Message reading failed.\n");
			return -1;
		}
		printf("From Server to client %d: %s\n",client_socket,buffer);

		gettimeofday(&end, NULL);
		printf("Time elapsed to connect %f sec\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000000.0);

		close(client_socket);
	}

	return 0;
}
