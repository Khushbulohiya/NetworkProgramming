/*
 * Name - Khushbu Lohiya
 * File - TCPdaytime_Client
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

int main(int argc, char* argv[])
{
	struct sockaddr_in server;
	int client_socket;
	char buffer[1024];

	char *service = "13"; /* service name or port number */
	switch(argc)
	{
		case 3: // ./client 127.0.0.1 5001
			service = argv[2];
			break;
		case 2: // ./client 127.0.0.1
			service = "13";
			break;
		default:
			fprintf(stderr, "usage %s hostname port", argv[0]);
			return -1;
	}

	//Create TCP socket for communication with server
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(client_socket == -1)
	{
		printf("Error creating socket.\n");
		return -1;
	}

	//struct sockaddr_in
	bzero(&server, sizeof(server));
	int port = atoi(service);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(port);

	//Send connection request to the server
	if(connect(client_socket, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("Connection failed.\n");
		return -1;
	}
	printf("Connection done.\n");
	while(1)
	{
		bzero(buffer, 1024);

		//Read response from the server
		if(read(client_socket, buffer, 1023) < 0)
		{
			printf("Message reading failed.\n");
			return -1;
		}
		printf("From Server: %s\n",buffer);
		return 0;
	}
}
