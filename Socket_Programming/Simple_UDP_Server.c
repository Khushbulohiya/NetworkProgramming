
 /*
 * Name - Khushbu Lohiya
 * File - Server UDP program
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
#define IP_ADDR "127.0.0.1"

int main(void)
{
	struct sockaddr_in server_me, server_other;
	int server_socket;
	char *message_to_client;
	char reply_from_client[100];
	unsigned int server_length = sizeof(server_other);

	// 1. Create a UDP socket
	server_socket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(server_socket == -1)
	{
		printf("Binding of socket failed.\n");
		return -1;
	}

	// 2. setting of memory
	memset((char *) &server_me, 0, sizeof(server_me));
	server_me.sin_family = AF_INET;
	server_me.sin_port = htons(PORT);
	server_me.sin_addr.s_addr = inet_addr(IP_ADDR);

	// 3. Binding of socket
	if(bind(server_socket, (struct sockaddr*)&server_me, sizeof(server_me)) == -1)
	{
		printf("Binding of socket failed.\n");
		return -1;
	}
	printf("Binding done.\n");

	// make udp socket non blocking
	fcntl(server_socket, F_SETFL, O_NONBLOCK);

	// 4. Listening for message from client
	printf("Waiting for message from client.....\n");

	memset(reply_from_client, '\0', 100);
	while(1){

		// 5. trying to receive message
		if((recvfrom(server_socket, reply_from_client, 100, 0, (struct sockaddr *) &server_other,
				&server_length)) < 1)
		{
			continue;
		}
		printf("\nFrom Client: %s\n",reply_from_client);

		// 6. Reply from Server to Client
		message_to_client = "Hello Client Welcome, I have received your message\n";
		if(sendto(server_socket, message_to_client, strlen(message_to_client), 0, (struct sockaddr*)
				&server_other, server_length) == -1)
		{
			printf("Sending message failed.\n");
			continue;
		}
		printf("\nTo Client: %s\n", message_to_client);
	}
	close(server_socket);
	return 0;
}


