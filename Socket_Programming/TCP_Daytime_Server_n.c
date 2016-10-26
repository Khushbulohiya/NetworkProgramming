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

//extern int  errno;
struct sockaddr_in servaddr;
int msock;
#define QLEN 5
#define true 1
#define false 0

int flag = true;

void signal_handler(int sig_type)
{
	char  c;
	signal(sig_type, SIG_IGN);
	printf("Do you really want to quit? [y/n] ");

	c = getchar();
	if (c == 'y' || c == 'Y')
	{
		close(msock);
	    flag = false;
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
    write(fd, pts, strlen(pts));
}

int passiveTCP(const char *service, int qlen){

	int new_socket = socket(AF_INET,SOCK_STREAM,0);
	if(new_socket == -1)
	{
		printf("Error creating socket.\n");
		return -1;
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(service));
	if (bind(new_socket, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("Error in binding.\n");
		return -1;
	}
	printf("Binding done on port %d.\n", atoi(service));

	listen(new_socket, QLEN);

	return(new_socket);
}

int errexit(const char *format, ...)
{
	va_list args;
	//va_start(args,format);
	vfprintf(stderr,format,args);
	//va_end(args);
	exit(1);
}

int main(int argc, char *argv[])
{
	struct sockaddr_in fsin; /* the from address of a client */
	char *service = "13"; /* service name or port number */
	int ssock;
	int alen;
	char buffer1[1000];
	switch (argc)
	{
		case 1:
				break;
		case 2:
				service = argv[1];
				break;
		default:
				errexit("usage: %s [port]\n", argv[0]);
	}

	msock = passiveTCP(service, QLEN);
	alen = sizeof(fsin);
	signal(SIGINT, signal_handler);

	while (flag)
	{
		ssock = accept(msock, (struct sockaddr *)&fsin, (socklen_t*)&alen);
		if (flag == false)
		{
			printf("Shutting down server!!\n");
			break;
		}
		if(ssock < 0)
		{
			printf("Connection failed.\n");
			return -1;
		}
		printf("Connection done!\n");
		time_t current_time=time(NULL);
		snprintf(buffer1,sizeof(buffer1),"%s",ctime(&current_time));
		printf("%s",buffer1);
		int pid = fork();
		if(pid < 0)
		{
			printf("Error forking");
			return -1;
		}
		if(pid == 0)
		{
			signal(SIGINT, SIG_IGN);
			printf("It is a child process.\nInteracting with the client.\n");
			printf("New client socket at port: %d for client socket\n\n", fsin.sin_port);
			close(msock);
			TCPdaytimed(ssock);
			close(ssock);
			exit(1);
		}
		else
		{
			close(ssock);
		}
	}
	return 0;
}

