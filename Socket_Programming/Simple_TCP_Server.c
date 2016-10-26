/*
 * Name - Khushbu Lohiya
 * File - Server TCP program
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
	int socket_descriptor;
	struct sockaddr_in server, client;
	int new_server_socket, last_socket;
	char reply_from_client[100], *message_to_client;
	int size;

	// 1. Create a TCP/IP socket
	socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_descriptor == -1)
	{
		printf("Error creating socket.\n");
		return -1;
	}

	last_socket = socket_descriptor;

	// 2. Structure of sockaddr_in
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	// 3. Binding of socket
	if(bind(socket_descriptor,(struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("Binding of socket failed.\n");
		return -1;
	}
	printf("Binding done.\n");

	// 4. Listening to socket
	listen(socket_descriptor, 3);


	// 5. Server accepting connection
	printf("Waiting for connection on socket %d\n", socket_descriptor);
	size = sizeof(struct sockaddr_in);
	new_server_socket = accept(socket_descriptor, (struct sockaddr *)&client, (socklen_t*)&size);
	if(new_server_socket < 0)
	{
		printf("Connection failed.\n");
		return -1;
	}
	printf("\nConnected to client %d.\n", new_server_socket);

	fcntl(last_socket, F_SETFL, O_NONBLOCK);
	fcntl(new_server_socket, F_SETFL, O_NONBLOCK);

	last_socket = new_server_socket;

	while(1)
	{
		// start with first socket descriptor
		for(int i = socket_descriptor; i <= last_socket; i++)
		{
			// if first socket where the server is listening
			// accept new connections from the client
			if(i == socket_descriptor)
			{
				new_server_socket = accept(socket_descriptor, (struct sockaddr *)&client, (socklen_t*)&size);
				if(new_server_socket < 0)
				{
					//printf("No client connection.\n");
				} else{
					printf("Connected to client %d.\n", new_server_socket);
					fcntl(new_server_socket, F_SETFL, O_NONBLOCK); //non-blocking
					last_socket = new_server_socket;
				}

			} // end if
			else // else the socket is the socket of connected client
			{
				// 6. Receiving message from client
				int n = recv(i, reply_from_client, 2000, 0);
				if(n < 1) // if client sent nothing, as this is non blocking it will return < 1
				{
					// it means client sent nothing so n = 0
					//printf("Message receiving failed.\n");
				}
				else { // message received from client

					// print client's message
					printf("\nFrom Client[%d]: %s\n", i, reply_from_client);

					// 7. Sending reply message to client
					message_to_client = "Hello Client, I have received your message.\n\0";
					// send message to client
					if(send(i, message_to_client, strlen(message_to_client), 0) < 0)
					{
						// if client is dead it did not receive the message and we got error
						// print error
						//printf("Message sending failed.\n");
					}
					else
					{
						// else the message was successfully sent to client
						printf("\nTo Client[%d]: %s\n", i, message_to_client);
					}
				}

			} // end else

		} // end for

	} // end while

	close(socket_descriptor);
	return 0;
}



