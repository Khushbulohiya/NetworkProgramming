/*
 * Name - Khushbu Lohiya
 * File - Client TCP program
 */
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include <fcntl.h>
#include<sys/types.h>

#define PORT 6667

int main(int argc, char *argv[])
{
	int client_socket;
	char *message_to_server;
	struct sockaddr_in server;
	char reply_from_server[100];

	// 1. Create a TCP/IP socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(client_socket == -1)
	{
		printf("Error creating socket.\n");
		return -1;
	}

	// 2. Structure of sockaddr_in
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

	// 3. Connect to server
	if(connect(client_socket, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		printf("cannot connect to server.\n");
		return -1;
	}
	else
	{
		printf("Connected to remote server.\n");
	}

	// 4. Sending message to server
	message_to_server = "Hello Server.\0";
	if(send(client_socket, message_to_server, strlen(message_to_server), 0) < 0)
	{
		printf("Message sending failed.\n");
		return -1;
	}
	printf("\nTo Server: %s\n", message_to_server);

	bzero(reply_from_server, sizeof(char) * 100);
	// 5. Reply message from server
	if(recv(client_socket, reply_from_server, 100, 0) < 0)
	{
		printf("Message receiving failed.\n");
	}
	printf("\nFrom Server: %s\n", reply_from_server);

	return 0;
}




