/*
 * Name - Khushbu Lohiya
 * File - TCPdaytime_client
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
#include <netdb.h>


#define server_name "localhost"
#define size 128

int main(int argc, char *argv[])
{
	struct addrinfo serverinfo, *result = NULL;
	int client_socket = -1, info_received;
	char   buffer[size];
	char   serverin[50];
	char   server_port[] = "5000";
	struct in6_addr servaddr;

	if (argc > 1) {
		strcpy(serverin, argv[1]);
	} else {
		strcpy(serverin, server_name);
	}

	memset(&serverinfo, 0x00, sizeof(serverinfo));

	//Address can be IPv4 or IPv6
	serverinfo.ai_family   = AF_UNSPEC;
	serverinfo.ai_flags    = AI_NUMERICSERV;
	serverinfo.ai_socktype = SOCK_STREAM;

	//To convert address to binary
	info_received = inet_pton(AF_INET, serverin, &servaddr);
	if (info_received == 1)
	{
		printf("\nIPv4 address\n");
		serverinfo.ai_family = AF_INET;
		serverinfo.ai_flags |= AI_NUMERICHOST;
	}
	else
	{
		info_received = inet_pton(AF_INET6, serverin, &servaddr);
		if (info_received == 1)
		{
			printf("\nIPv6 address\n");
			serverinfo.ai_family = AF_INET6;
			serverinfo.ai_flags |= AI_NUMERICHOST;
		}
	}

	
	info_received = getaddrinfo(serverin, server_port, &serverinfo, &result);
	if (info_received != 0)
	{
		printf("Host not found:%s\n", gai_strerror(info_received));
		if (info_received == EAI_SYSTEM)
			printf("Unable to get server address info\n");
		exit(0);
	}
	client_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (client_socket < 0)
	{
		printf("Failed to create socket.\n");
		exit(0);
	}
	info_received = connect(client_socket, result->ai_addr, result->ai_addrlen);
	if (info_received < 0)
	{
		printf("Failed to connect to server\n");
		exit(0);
	}
	printf("Connection done\n");
	printf("Date and time:");


	int bytes_received = 0;
	while((bytes_received = recv(client_socket, buffer, size, 0)) > 0)
	{
		buffer[bytes_received]='\0';
		fputs(buffer, stdout);
		bytes_received = 0;
	}

	printf("\n");
	close(client_socket);
	if (result != NULL)
	{
		freeaddrinfo(result);
	}

}




