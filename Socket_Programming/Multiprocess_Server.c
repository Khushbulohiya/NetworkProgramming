/*
 * Name - Khushbu Lohiya
 * File - Multi-process_Server
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
#include<errno.h>

#define QLEN 5
#define BUFSIZE 4096
struct sockaddr_in servaddr;

// used by system calls to set error
// and so declared as extern
extern int errno;

int TCPechod(int fd);
int errexit(const char *format, ...);
int passiveTCP(const char *service, int qlen);


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
	if (bind(new_socket ,(struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("Error in binding.\n");
		return -1;
	}
	printf("Binding done.\n");

	listen(new_socket, QLEN);

	return(new_socket);
}

int TCPechod(int fd)
{
	char buf[BUFSIZ];
	int cc = 1;
	while (cc)
	{
		cc = read(fd, buf, sizeof(buf));
		if (cc < 0)
		{
			errexit("echo read: %s\n", strerror(errno));
		}
		else if (cc > 0) {
			printf("From Client: %s\n", buf);
			if (write(fd, buf, cc) < 0)
			{
				errexit("echo write: %s\n", strerror(errno));
			}
			printf("To Client: %s\n", buf);
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	char *service = "echo"; /* service name or port number */ struct sockaddr_in fsin; /* the address of a client */
	int alen;
	int msock;
	int ssock;

	switch (argc)
	{
		case 1:
			break;
		case 2:
			/* length of client's address */
			/* master server socket */
			/* slave server socket */
			service = argv[1];
			break;
		default:
			errexit("usage: %s [port]\n", argv[1]);
	}

	msock = passiveTCP(service, QLEN);

	while (1)
	{
		alen = sizeof(fsin);
		ssock = accept(msock, (struct sockaddr *)&fsin, (socklen_t *)&alen);
		if (ssock < 0) {
			if (errno == EINTR)
			{
				continue;
			}
			errexit("accept: %s\n", strerror(errno));
		}
		switch (fork())
		{
			case 0:     /* child */
				(void) close(msock);
				exit(TCPechod(ssock));
			default: /* parent */
				(void) close(ssock);
				break;
			case -1:
				errexit("fork: %s\n", strerror(errno));
		}
	}
	return 0;
}



