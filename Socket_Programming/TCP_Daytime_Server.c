/*
 * Name - Khushbu Lohiya
 * File - TCPdaytime_server
 */


#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<time.h>
#include<signal.h>

#define SERVER_PORT     5000
#define BUFFER_LENGTH   500
#define QLEN 			5
#define FALSE           0
#define TRUE 			1

int server_socket = -1;
int flag = TRUE;

void signal_handler(int sig_type)
{
	char  c;
	signal(sig_type, SIG_IGN);

	printf("\nOops!!Control C pressed.\nDo you really want to quit? [y/n] ");

	c = getchar();
	if (c == 'y' || c == 'Y')
	{
		close(server_socket);
		flag = FALSE;
	}
	else
	{
		signal(SIGINT, signal_handler);
	}
}
void TCPdaytimed(int fd)
{
	char    *pts;
	time_t  now;
	char    *ctime();
	/* pointer to time string */
	/* current time */
	time(&now);
	pts = ctime(&now);
	printf("Connected to new client\n");
	printf("Sending date and time to client.\n");
	printf("Date and time:%s\n",pts);
	write(fd, pts, strlen(pts));
	close(fd);
}

int main(int argc, char *argv[])
{
	
	struct sockaddr_in6 servaddr, cliaddr;
	char string[INET6_ADDRSTRLEN];

	
	if ((server_socket = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
	{
		printf("Failed to create socket\n");
		return -1;
	}
	int yes = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR,(char *)&yes, sizeof(yes)) < 0)
	{
		printf("Failed in setsockopt\n");
		return -1;
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin6_family = AF_INET6;
	servaddr.sin6_port   = htons(SERVER_PORT);
	servaddr.sin6_addr   = in6addr_any;

	if (bind(server_socket,(struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("Error in binding failed\n");
		exit(0);
	}
	printf("Binding done on port\n");

	listen(server_socket, QLEN);
	int size = sizeof(cliaddr);
	signal(SIGINT, signal_handler);
	printf("Waiting for conncetion from client.\n\n");
	do
	{
		int client_socket;
		if ((client_socket = accept(server_socket, NULL, NULL)) < 0)
		{
			if (flag == FALSE)
			{
				break;
			}
			printf("Unable to accept connection from client. Failed!!\n");
			break;
		}
		else
		{
			//to display the client address
			getpeername(client_socket, (struct sockaddr *)&cliaddr, (socklen_t*)&size);
			if(inet_ntop(AF_INET6, &cliaddr.sin6_addr, string, sizeof(string)))
			{
				printf("Address of the Client is %s", string);
				printf("\n");
			}
			TCPdaytimed(client_socket);
		}
	}while(flag);
}

