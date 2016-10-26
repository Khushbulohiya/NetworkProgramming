/*
 * Name - Khushbu Lohiya
 * File - TCP getsockoption() and setsockoption()
 */

#include <sys/socket.h>
#include <stdio.h>
#include<stdlib.h>


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
		printf("The default set value of the TCP send buffer is %d bytes\n", send_buffer_size);
	}

	printf("\nEnter the new value you want to set for TCP send buffer: ");
	scanf("%d",&send_buffer_size);
	printf("The new requested value to set of the TCP send buffer is %d bytes\n", send_buffer_size);

	receive_bytes = setsockopt(new_socket, SOL_SOCKET, SO_SNDBUF, &send_buffer_size, sizeof(send_buffer_size));
	printf("By using setsockopt() setting size the new value of TCP send buffer.\n");
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
			printf("\nThe new set value of the TCP send buffer is %d bytes\n\n", send_buffer_size);
		}
	}
}

void modify_receive_buffer_size_display()
{
	size = sizeof(receive_buffer_size);
	receive_bytes = getsockopt(new_socket,SOL_SOCKET,SO_RCVBUF,&receive_buffer_size,(socklen_t *) &size);
	if(receive_bytes == -1)
	{
		printf("Error setting setsockoption.\n");
		exit(-1);
	}
	else
	{
		printf("The default set value of the TCP receive buffer is %d bytes\n", receive_buffer_size);
	}
	printf("\nEnter the new value you want to set for TCP receive buffer: ");
	scanf("%d",&receive_buffer_size);
	printf("The new requested value to set of the TCP receive buffer is %d bytes\n", receive_buffer_size);

	receive_bytes = setsockopt(new_socket, SOL_SOCKET, SO_RCVBUF, &receive_buffer_size, sizeof(receive_buffer_size));
	printf("By using setsockopt() setting size the new value of TCP receive buffer.\n");
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
			printf("\nThe new set value of the TCP receive buffer is %d bytes\n\n", receive_buffer_size);
		}
	}
}

void display_all_other_socket_options()
{

	int debug, reuseaddr, keepalive, type, sndtimeo, rcvtimeo, oobinline, acceptconn;

	printf("All the default socket options values are as follows:\n");

	size = sizeof(receive_buffer_size);
	getsockopt(new_socket, SOL_SOCKET, SO_RCVBUF, &receive_buffer_size, (socklen_t *) &size);
	printf ("The default set value of TCP receive buffer is %d bytes\n", receive_buffer_size);

	size = sizeof(send_buffer_size);
	getsockopt(new_socket, SOL_SOCKET, SO_SNDBUF, &send_buffer_size, (socklen_t *) &size);
	printf ("The default set value of TCP send buffer is %d bytes\n", send_buffer_size);

	size = sizeof(debug);
	getsockopt(new_socket, SOL_SOCKET, SO_DEBUG, &debug, (socklen_t*) &size);
	printf ("The default set value of TCP debug is %d\n", debug);

	size = sizeof(reuseaddr);
	getsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, (socklen_t*) &size);
	printf ("The default set value of TCP reuseaddr is %d\n", reuseaddr);

	size = sizeof(keepalive);
	getsockopt(new_socket, SOL_SOCKET, SO_KEEPALIVE, &keepalive, (socklen_t*) &size);
	printf ("The default set value of TCP keeplive is %d\n", keepalive);

	size = sizeof(type);
	getsockopt(new_socket, SOL_SOCKET, SO_TYPE, &type, (socklen_t*) &size);
	printf ("The default set value of TCP type is %d\n", type);

	size = sizeof(sndtimeo);
	getsockopt(new_socket, SOL_SOCKET, SO_SNDTIMEO, &sndtimeo, (socklen_t*) &size);
	printf ("The default set value of TCP sndtimeo is %d\n", sndtimeo);

	size = sizeof(rcvtimeo);
	getsockopt(new_socket, SOL_SOCKET, SO_RCVTIMEO, &rcvtimeo, (socklen_t*) &size);
	printf ("The default set value of TCP rcvtimeo is %d\n", rcvtimeo);

	size = sizeof(oobinline);
	getsockopt(new_socket, SOL_SOCKET, SO_OOBINLINE, &oobinline,(socklen_t*) &size);
	printf ("The default set value of TCP oobinline is %d\n", oobinline);

	size = sizeof(acceptconn);
	getsockopt(new_socket, SOL_SOCKET, SO_ACCEPTCONN, &acceptconn, (socklen_t*) &size);
	printf ("The default set value of TCP acceptconn is %d\n", acceptconn);

}
int main(int argc, char *argv[])
{
	int choice;
	new_socket = socket(AF_INET, SOCK_STREAM, 0);
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

			default: printf("Oops!! Please enter the valid option\n");
					return -1;
		}
	}
}
