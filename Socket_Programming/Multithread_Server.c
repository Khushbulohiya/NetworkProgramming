/*
 * Name - Khushbu Lohiya
 * File - Multithread_Server
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

#define BUFSIZE 4096
struct sockaddr_in servaddr;
#define QLEN 10

extern int errno;
int errexit(const char *format, ...);
int passiveTCP(const char *service, int qlen);
int echo(int fd);

int errexit(const char *format, ...)
{
	va_list args;
	vfprintf(stderr,format,args);
	exit(1);
}

int passiveTCP(const char *service, int qlen)
{

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
	printf("Binding done.\n");

	listen(new_socket, QLEN);
	return new_socket;
}

int main(int argc, char *argv[])
{
	char *service = "echo"; /* service name or port number */
	struct sockaddr_in fsin; /* the from address of a client */
	int msock;
	fd_set  rfds;
	fd_set  afds;
	int alen;
	int fd;
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
	FD_ZERO(&afds);
	FD_SET(msock, &afds);
	while (1)
	{
		memcpy(&rfds, &afds, sizeof(rfds));
		if (select(FD_SETSIZE, &rfds, NULL, NULL, NULL) < 0)
		{
			errexit("select: %d %s\n", errno, strerror(errno));
		}
		if (FD_ISSET(msock, &rfds))
		{
			int ssock;
			alen = sizeof(fsin);
			ssock = accept(msock, (struct sockaddr *)&fsin,(socklen_t*)&alen);
			if (ssock < 0)
			{
				errexit("accept: %s\n", strerror(errno));
			}
			FD_SET(ssock, &afds);
		}
		for (fd=0; fd < FD_SETSIZE; ++fd)
		{
			if (fd != msock && FD_ISSET(fd, &rfds))
			{
				if (echo(fd) == 0)
				{
					(void) close(fd);
					FD_CLR(fd, &afds);
				}
			}
		}
	}
}

int echo(int fd)
{
	char buf[BUFSIZ];
    int cc;
    cc = read(fd, buf, sizeof buf);

    if (cc < 0)
    {
    	errexit("echo read: %s\n", strerror(errno));
    }
    printf("From Client: %s\n",buf);
    if (cc && write(fd, buf, cc) < 0)
    {
    	errexit("echo write: %s\n", strerror(errno));
    }
    printf("To Client: %s\n",buf);
    return cc;
}

