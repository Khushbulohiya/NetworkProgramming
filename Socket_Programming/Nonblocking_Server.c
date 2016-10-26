 /*
 * Name - Khushbu Lohiya
 * File - Nonblocking_server
 */

#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>


void handle_control (int socket)

{
	char reply_from_client[100], *message_to_client;
	while (1)
	{
		//  Receiving message from client
		bzero(reply_from_client, sizeof(char) * 100);

		// Reply message from server
		int c = 0;
		if((c = recv(socket, reply_from_client, 100, 0)) < 0)
		{
			printf("Client closed socket.\n\n");
			break;
		}
		if (c > 0)
		{
			printf("\nFrom Client: %s\n", reply_from_client);

			// Sending reply message to client
			message_to_client = "Hello Client, I have received your message.\n\0";

			// send message to client
			if(send(socket, message_to_client, strlen(message_to_client), 0) < 0)
			{
				printf("Message sending failed.\n");
				break;
			}
			else
			{
				// else the message was successfully sent to client
				printf("\nTo Client: %s\n", message_to_client);
			}
		}
	}
	close(socket);
	return;
}


int main(int argc, char *argv[])
{
	int socket_descriptor;
	struct sockaddr_in server, client;
	int client_socket, last_socket;
	int size;


	if(argc != 2)
	{
		fprintf(stderr, "usage %s port",argv[0]);
		return -1;
	}
	// Create a TCP/IP socket
	socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_descriptor == -1)
	{
		printf("Error creating socket.\n");
		return -1;
	}

	last_socket = socket_descriptor;

	// Structure of sockaddr_in
	int port = atoi(argv[1]);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);
	// 3. Binding of socket
	if(bind(socket_descriptor,(struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("Binding of socket failed.\n");
		return -1;
	}
	printf("Binding done.\n");

	// Listen to socket
	listen(socket_descriptor, 5);

	//  Server accepting connection
	printf("Waiting for connection on socket %d\n", socket_descriptor);
	size = sizeof(struct sockaddr_in);

	while(1)
	{
		client_socket = accept(socket_descriptor, (struct sockaddr *)&client, (socklen_t*)&size);
		if(client_socket < 0)
		{
			printf("Connection failed.\n");
			return -1;
		}
		printf("Connected to client %d.\n", client_socket);
		pid_t pid = fork();
		if(pid < 0)
		{
			printf("Error forking");
			close(client_socket);
			return -1;
		}
		if(pid == 0)
		{
			//non-blocking
			fcntl(client_socket, F_SETFL, O_NONBLOCK);
			printf("It is a child process.\nInteracting with the client.\n");
			printf("port: %d for new client connection \n", client.sin_port);
			close(socket_descriptor);
			handle_control(client_socket);
			close(client_socket);

			// exit from child process, as we don't want forked process to run in the while loop here
			exit(0);
		}

		else if(pid  > 0)
		{
			close(client_socket);
		}

	} // end of while
	return 0;
}







