 /*
 * Name - Khushbu Lohiya
 * File - Server_fork
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<time.h>

/**
 * Function used by forked process to make communication with client
 * socket.
 */
void handle_control(int client_socket)
{
	while(1)
	{
		char buffer[1024];
		bzero(buffer, 1024);

		// block on read to read content sent by client
		if(read(client_socket, buffer, 1000) <= 0)
		{
			printf("Client successfully closed socket.\n");
			break;
		}
		printf("From Client : %s\n",buffer);
		bzero(buffer, 1024);
		printf("Enter message for client:");
		fgets(buffer, 1000, stdin);

		// write message taken from stdin to the client
		if(write(client_socket, buffer, strlen(buffer)) <= 0)
		{
			printf("Message sending failed.\n");
			break;
		}
		printf("To Client : %s\n",buffer);
	}
	close(client_socket);
}

int main(int argc, char* argv[])
{
	struct sockaddr_in servaddr, cliaddr;
	int size, client_socket;
	char buffer1[1000];


	if(argc != 2)
	{
		fprintf(stderr, "usage %s port",argv[0]);
		return -1;
	}

	//Create TCP socket to accept connection from clients
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket == -1)
	{
		printf("Error creating socket.\n");
		return -1;
	}

	//struct sockaddr_in
	int port = atoi(argv[1]);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	//Bind socket to a port to accept connections from client
	if(bind(server_socket, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("Bind failed.\n");
		return -1;
	}
	printf("Binding done.\n");

	//socket in passive mode to listen connection from client
	listen(server_socket, 3);

	printf("Waiting for connection.....\n");
	while(1)
	{
		size = sizeof(struct sockaddr_in);

		//Accept connection from client
		client_socket = accept(server_socket, (struct sockaddr *)&cliaddr, (socklen_t *)&size);
		if(client_socket < 0)
		{
			printf("Connection failed.\n");
			return -1;
		}
		printf("Connection done.\n");

		//Print system time
		time_t current_time=time(NULL);
		snprintf(buffer1,sizeof(buffer1),"%s",ctime(&current_time));
		write(client_socket,buffer1,500);
		printf("%s",buffer1);

		//Use fork to create new child process to perform communication with client
		int pid = fork();
		if(pid < 0)
		{
			printf("Error forking");
			close(client_socket);
			close(server_socket);
			return -1;
		}
		//New child process
		if(pid == 0)
		{
			printf("It is a child process.\nInteracting with the client.\n");
			printf("New client socket at port: %d \n\n", cliaddr.sin_port);

			// close server_socket, as child process does not use this socket to connect multiple clients
			close(server_socket);
			handle_control(client_socket);

			// once all the communication with the client is complete, close the client_socket
			close(client_socket);

			//we don't want child process to continue with while loop
			//child process work of communication with client is complete
			exit(1);
		}
		else  //close client socket, as parent process does not communicate with connected client
		{
			close(client_socket);
		}

	} // end of while
	return 0;
}
