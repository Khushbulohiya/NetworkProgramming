 /*
 * Name - Khushbu Lohiya
 * File - Server_select
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
	struct sockaddr_in servaddr, cliaddr;
	int size, client_socket;
	char buffer[1024], buffer1[1000];
	fd_set fd_no;
	fd_set read_fds;

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
	int one = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

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

	FD_ZERO(&fd_no);
	FD_SET(server_socket, &fd_no);

	printf("Waiting for connection.....\n");
	while(1)
	{
		//Block until input or onr or more active socket arrives
		read_fds = fd_no;
		select(FD_SETSIZE, &read_fds, NULL, NULL, NULL);

		//provide service all the sockets with pending inputs
		for(int i = 0; i < FD_SETSIZE; i++)
		{
			if(FD_ISSET(i, &read_fds))
			{
				// if server socket there is a client request for connection
				if(i == server_socket)
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
					printf("port: %d for client socket %d\n", cliaddr.sin_port, client_socket);

					//Print system time
					time_t current_time=time(NULL);
					snprintf(buffer1,sizeof(buffer1),"%s\n",ctime(&current_time));
					write(client_socket,buffer1,500);
					printf("%s",buffer1);
					FD_SET(client_socket, &fd_no);
				}
				else // some client socket data is pending to be read
				{
					bzero(buffer, 1024);

					// block on read to read content sent by client
					if(read(i, buffer, 1023) > 0)
					{
						printf("From Client %d: %s\n", i, buffer);
						bzero(buffer, 1024);
					}
					printf("Enter message for client:");
					fgets(buffer, 1024, stdin);

					// write message taken from stdin to the client
					if(write(i, buffer, strlen(buffer)) <= 0)
					{
						printf("Message sending failed.\n");
					}
					else
					{
						printf("To Client %d: %s\n",i, buffer);
					}
				} // end of else

			} // end of if

		} // end of for
	}
	return 0;
}




