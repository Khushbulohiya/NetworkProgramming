 /*
 * Name - Khushbu Lohiya
 * File - Client_fork
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<time.h>


int main(int argc, char* argv[])
{
	struct sockaddr_in server;
	int client_socket;
	char buffer[1024], buffer1[1000];

	if(argc != 3)
	{
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
	int port = atoi(argv[2]);
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
	bzero(buffer1, 500);

	//Read system time
	if(read(client_socket, buffer1, 500) < 0)
	{
		printf("Message reading failed.\n");
		return -1;
	}
	printf("Date and time from Server: %s\n", buffer1);
	bzero(buffer, 1024);
	printf("Sending message to server.\n");
	while(1){
		printf("Enter message for server: ");
		bzero(buffer, 1024);
		fgets(buffer, 1000, stdin);

		//Send request to the server
		if(write(client_socket, buffer, strlen(buffer)) < 0)
		{
			printf("Message sending failed.\n");
			return -1;
		}
		printf("To Server from client : %s\n",buffer);

		bzero(buffer, 1024);

		//Read response from the server
		if(read(client_socket, buffer, 1000) < 0)
		{
			printf("Message reading failed.\n");
			return -1;
		}
		printf("From Server to client: %s\n",buffer);

	} // end of while

	//Close the socket after communication with the server is done
	close(client_socket);
	return 0;
}
