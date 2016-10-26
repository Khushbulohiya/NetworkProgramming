 /*
 * Name - Khushbu Lohiya
 * File - Nonblocking_client
 */

#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>


int main(int argc, char *argv[])
{
	int client_socket;
	char *message_to_server;
	struct sockaddr_in server;
	char reply_from_server[100];

	if(argc != 3)
	{
		fprintf(stderr, "usage %s hostname port", argv[0]);
		return -1;
	}

	// Create a TCP socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(client_socket == -1)
	{
		printf("Error creating socket.\n");
		return -1;
	}

	// Structure of sockaddr_in
	bzero(&server, sizeof(server));
	int port = atoi(argv[2]);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(port);

	// Connect to server
	if(connect(client_socket, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		printf("cannot connect to server.\n");
		return -1;
	}
	else
	{
		printf("Connected to remote server.\n");
	}

	// Sending message to server
	message_to_server = "Hello Server.\0";
	if(send(client_socket, message_to_server, strlen(message_to_server), 0) < 0)
	{
		printf("Message sending failed.\n");
		return -1;
	}
	printf("\nTo Server: %s\n", message_to_server);

	bzero(reply_from_server, sizeof(char) * 100);

	// Reply message from server
	if(recv(client_socket, reply_from_server, 100, 0) < 0)
	{
		printf("Message receiving failed.\n");
	}
	printf("\nFrom Server: %s\n", reply_from_server);

	close(client_socket);
	return 0;
}







