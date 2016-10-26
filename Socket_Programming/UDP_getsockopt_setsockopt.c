/*
 * Name - Khushbu Lohiya
 * File - UDP getsockoption() and setsockoption()
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
#include<pthread.h>

int new_socket, send_buffer_size, receive_buffer_size, size;
int receive_bytes = 0;

void modify_receive_buffer_size_display();
void modify_send_buffer_size_display();
void display_all_other_socket_options();

void modify_send_buffer_size_display()
{
	size = sizeof(send_buffer_size);
	receive_bytes = getsockopt(new_socket, SOL_SOCKET, SO_SNDBUF, &send_buffer_size, (socklen_t*) &size);

	if(receive_bytes == -1)
	{
		printf("Error setting getsockoption.\n");
		exit(-1);
	}
	else
	{
		printf("The default set value of the UDP send buffer is %d bytes\n", send_buffer_size);
	}

	printf("\nEnter the new value you want to set for UDP send buffer: ");
	scanf("%d",&send_buffer_size);
	printf("The new requested value to set of the UDP send buffer is %d bytes\n", send_buffer_size);

	receive_bytes = setsockopt(new_socket, SOL_SOCKET, SO_SNDBUF, &send_buffer_size, sizeof(send_buffer_size));
	printf("By using setsockopt() setting size the new value of UDP send buffer.\n");
	if(receive_bytes == -1)
	{
		printf("Error setting getsockoption.\n");
		exit(-1);
	}
	else
	{
		size = sizeof(send_buffer_size);
		receive_bytes = getsockopt(new_socket, SOL_SOCKET, SO_SNDBUF, &send_buffer_size, (socklen_t *) &size);
		if(receive_bytes == -1)
		{
			printf("Error setting getsockoption.\n");
			exit(-1);
		}
		else
		{
			printf("\nThe new set value of the UDP send buffer is %d bytes\n\n", send_buffer_size);
		}
	}
}

void modify_receive_buffer_size_display()
{
	size = sizeof(receive_buffer_size);
	receive_bytes = getsockopt(new_socket, SOL_SOCKET, SO_RCVBUF, &receive_buffer_size, (socklen_t *) &size);
	if(receive_bytes == -1)
	{
		printf("Error setting setsockoption.\n");
		exit(-1);
	}
	else
	{
		printf("The default set value of the UDP receive buffer is %d bytes\n", receive_buffer_size);
	}
	printf("\nEnter the new value you want to set for UDP receive buffer: ");
	scanf("%d",&receive_buffer_size);
	printf("The new requested value to set of the UDP receive buffer is %d bytes\n", send_buffer_size);

	receive_bytes = setsockopt(new_socket, SOL_SOCKET, SO_RCVBUF, &receive_buffer_size, sizeof(receive_buffer_size));
	printf("By using setsockopt() setting size the new value of UDP receive buffer.\n");
	if(receive_bytes == -1)
	{
		printf("Error setting setsockoption.\n");
		exit(-1);
	}
	else
	{
		size = sizeof(send_buffer_size);
		receive_bytes = getsockopt(new_socket, SOL_SOCKET, SO_RCVBUF, &receive_buffer_size,(socklen_t*) &size);
		if(receive_bytes == -1)
		{
			printf("Error setting getsockoption.\n");
			exit(-1);
		}
		else
		{
			printf("\nThe new set value of the UDP receive buffer is %d bytes\n\n", receive_buffer_size);
		}
	}
}

void display_all_other_socket_options()
{
	int debug, reuseaddr, keepalive, type, sndtimeo, rcvtimeo, oobinline, acceptconn;

	printf("All the default socket options values are as follows:\n");

	size = sizeof(receive_buffer_size);
	getsockopt(new_socket, SOL_SOCKET, SO_RCVBUF, &receive_buffer_size, (socklen_t *) &size);
	printf ("The default set value of UDP receive buffer is %d bytes\n", receive_buffer_size);

	size = sizeof(send_buffer_size);
	getsockopt(new_socket, SOL_SOCKET, SO_SNDBUF, &send_buffer_size, (socklen_t *) &size);
	printf ("The default set value of UDP send buffer is %d bytes\n", send_buffer_size);

	size = sizeof(debug);
	getsockopt(new_socket, SOL_SOCKET, SO_DEBUG, &debug, (socklen_t*) &size);
	printf ("The default set value of UDP debug is %d\n", debug);

	size = sizeof(reuseaddr);
	getsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, (socklen_t*) &size);
	printf ("The default set value of UDP reuseaddr is %d\n", reuseaddr);

	size = sizeof(keepalive);
	getsockopt(new_socket, SOL_SOCKET, SO_KEEPALIVE, &keepalive, (socklen_t*) &size);
	printf ("The default set value of UDP keeplive is %d\n", keepalive);

	size = sizeof(type);
	getsockopt(new_socket, SOL_SOCKET, SO_TYPE, &type, (socklen_t*) &size);
	printf ("The default set value of UDP type is %d\n", type);

	size = sizeof(sndtimeo);
	getsockopt(new_socket, SOL_SOCKET, SO_SNDTIMEO, &sndtimeo, (socklen_t*) &size);
	printf ("The default set value of UDP sndtimeo is %d\n", sndtimeo);

	size = sizeof(rcvtimeo);
	getsockopt(new_socket, SOL_SOCKET, SO_RCVTIMEO, &rcvtimeo, (socklen_t*) &size);
	printf ("The default set value of UDP rcvtimeo is %d\n", rcvtimeo);

	size = sizeof(oobinline);
	getsockopt(new_socket, SOL_SOCKET, SO_OOBINLINE, &oobinline,(socklen_t*) &size);
	printf ("The default set value of UDP oobinline is %d\n", oobinline);

	size = sizeof(acceptconn);
	getsockopt(new_socket, SOL_SOCKET, SO_ACCEPTCONN, &acceptconn, (socklen_t*) &size);
	printf ("The default set value of UDP acceptconn is %d\n", acceptconn);
}
int main(int argc, char *argv[])
{
	int choice;
	new_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(new_socket == -1)
	{
		printf("Failed to create socket");
		return -1;
	}
	while(1)
	{
		printf("\n--------- MENU ------------\n");
		printf("1. Send_buffer_size_display\n");
		printf("2. Receive_buffer_size_display\n");
		printf("3. All other socket options\n");
		printf("\nEnter your option from menu:");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1: modify_send_buffer_size_display();
					break;

			case 2: modify_receive_buffer_size_display();
					break;

			case 3: display_all_other_socket_options();
					break;

			default: printf("Please enter the valid option\n");

		}
	}
}
