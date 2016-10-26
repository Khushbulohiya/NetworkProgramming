 /*
 * Name - Khushbu Lohiya
 * File - Client UDP program
 */


#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include <fcntl.h>
#include<sys/types.h>

#define PORT 6668

int main(void)
{
	struct sockaddr_in client_other;
	int client_socket;
	unsigned int client_length = sizeof(client_other);
	char *message_to_server;
	char reply_from_server[100];

	// 1. Creating a UDP socket
	client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		printf("Error creating socket.\n");
		return -1;
	}

	// Clearing the buffer
	memset((char *) &client_other, 0, sizeof(client_other));
	client_other.sin_family = AF_INET;
	client_other.sin_port = htons(PORT);

	// 2. Sending message to client
	message_to_server = "Hello Server Hello.\n";
	if (sendto(client_socket, message_to_server, strlen(message_to_server) ,0 ,
				(struct sockaddr *) &client_other, client_length) == -1)
	{
		printf("Message sending failed.\n");
		return -1;
	}
	printf("\nTo Server: %s\n", message_to_server);

	// 3. Receiving reply message from server
	if (recvfrom(client_socket, reply_from_server, 100, 0, (struct sockaddr *)
				&client_other, &client_length) == -1)
	{
		printf("Message receiving failed.\n");
		return -1;
	}
	printf("\nFrom Server: %s\n", reply_from_server);

	int close(int client_socket);
	return 0;
}



