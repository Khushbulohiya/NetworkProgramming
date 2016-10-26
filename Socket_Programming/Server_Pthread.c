/*
 * Name - Khushbu Lohiya
 * File - Server_pthread
 */


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<time.h>

int success = 0;
int server_socket;
int flag = 1;

void signal_handler(int sig_type)
{
	char  c;
	signal(sig_type, SIG_IGN);
	printf("Do you really want to quit? [y/n] ");

	c = getchar();
	if (c == 'y' || c == 'Y')
	{
		close(server_socket);
	    flag = 0;
	}
	else
	{
		signal(SIGINT, signal_handler);
	}
}

void handle_control(void * socket_1)
{
	while(1)
	{
		int socket = *((int *)socket_1);
		char buffer[1024];
		bzero(buffer, 1024);

		// block on read to read content sent by client
		if(read(socket, buffer, 1023) <= 0)
		{
			printf("Client socket closed.\n");
			break;
		}
		printf("From Client %d: %s\n",socket, buffer);
		bzero(buffer, 1024);
		printf("Enter message for client:");
		fgets(buffer, 1024, stdin);

		// write message taken from stdin to the client
		if(write(socket, buffer, strlen(buffer)) <= 0)
		{
			printf("Message sending failed.\n");
			break;
		}
		printf("To Client %d: %s\n",socket, buffer);
	}

	//exit thread once communication is done with return status
	pthread_exit(&success);
}

int main(int argc, char* argv[])
{
	struct sockaddr_in servaddr, cliaddr;
	int size, client_socket;
	char buffer1[1000];
	time_t current_time;


	if(argc != 2)
	{
		fprintf(stderr, "usage %s port\n",argv[0]);
		return -1;
	}

	//Create TCP socket to accept connection from clients
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
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
		close(server_socket);
		return -1;
	}
	printf("Binding done.\n");

	//socket in passive mode to listen connection from client
	listen(server_socket, 3);

	printf("Waiting for connection.....\n");

	// register signal handler to handle when control-C is pressed
	signal(SIGINT, signal_handler);

	int new_sockets[1000];
	int new_socket_index = 0;
	pthread_t thread_id[1000];
	int index = 0;

	while(flag)
	{
		size = sizeof(struct sockaddr_in);

		//Accept connection from client
		client_socket = accept(server_socket, (struct sockaddr *)&cliaddr, (socklen_t *)&size);
		if (flag == 0)
		{
			printf("Shutting down server!!\n");
			break;
		}
		if(client_socket < 0)
		{
			printf("Connection failed.\n");
			return -1;
		}
		printf("Connection done.\n");
		// store client sockets in array, to take action on when
		// SIGINT happens
		new_sockets[new_socket_index] = client_socket;
		new_socket_index++;

		//Print system time
		current_time=time(NULL);
		snprintf(buffer1,sizeof(buffer1),"%s",ctime(&current_time));
		write(client_socket,buffer1,500);
		printf("%s",buffer1);
		printf("Connected to client %d at socket number.\n",client_socket);
		printf("port: %d for client socket %d\n\n", cliaddr.sin_port, client_socket);

		//Use pthread_create to create new thread to perform communication with client
		if(pthread_create(&thread_id[index] , NULL , (void *)&handle_control , (void*) &client_socket) < 0)
		{
			printf("Unable to create socket.\n");
			return -1;
		}
		index++;
	} // end of while

	close(server_socket);

	for (int i=0; i < new_socket_index; i++)
	{
		close(new_sockets[i]); // close all client sockets
	}// end of for

	// once all client sockets are closed
	// wait for the child threads to exit before ending the code
	for (int i=0; i < index; i++)
	{
		int status;
		pthread_join(thread_id[index], (void *)&status);
	}// end of for

	return 0;
}


