/*
	cwb
	File:/src/Net.c
	Date:2021.03.05
	By MIT License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netdb.h>
#include<errno.h>
#include<fcntl.h>

#include"cwb/Net.h"
#include"cwb/Time.h"

/*
	funcName:cwb_net_socket
	input:none
	output:a file describer
	brief:Open a new socket(IPv4 & TCP)
*/
int cwb_net_socket(void)
{
	return socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,0);
}

/*
	funcName:cwb_net_bind
	input:
		sock:The file describer of the socket
		port:The port bind with the socket
	output:
		Return zero if it binds successfully.
		If not,return -1.
	brief:Bind a socket with a port.
*/
int cwb_net_bind(int sock,int port)
{
	struct sockaddr_in addr;

	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	return bind(sock,(struct sockaddr*)(&addr),sizeof(struct sockaddr));
}

/*
	funcName:cwb_net_listen
	input:
		sock:The socket file describer
		backLog:The size of backlog
	output:
		Return 0 if listen successfully.
		If not,return -1.
	brief:Listen to a socket.
*/
int cwb_net_listen(int sock,int backLog)
{
	return listen(sock,backLog);
}

/*
	funcName:cwb_net_accept
	input:
		sock:The file describer of the socket.
	output:
		The file describer of the client.
	brief:
		
*/
int cwb_net_accept(int sock)
{
	return accept(sock,NULL,NULL);
}

/*
	funcName:cwb_net_write
	input:
		fd:The file describer(maybe a client or a socket)
		buf:The data
		size:The size of data
	output:
		The size of the data that wrote successfully.
	brief:
		This function tries writing size bytes data in buf to fd.
*/
ssize_t cwb_net_write(int fd,void *buf,size_t size)
{
	return write(fd,buf,size);
}

/*
	funcName:cwb_net_read
	input:
		fd:file describer(maybe a client,a socket or even a common file :)
		buf:The data buffer
		size:The size that you expect to get
	output:
		The real size it reads.
	brief:
		This function tries to read size bytes from fd,and writes them to buf.
*/
ssize_t cwb_net_read(int fd,void *buf,size_t size)
{
	return read(fd,buf,size);
}

/*
	funcName:cwb_net_close
	input:
		fd:The file describer(Maybe a client or a socket)
	output:
		On success,zero will be returned
		If failed,it will return -1
	brief:
		Close a file describer
*/
int cwb_net_close(int fd)
{
	return close(fd);
}

/*
	funcName:cwb_net_tonet??
	input:
		data:The data with host byte order
	output:
		The data with network byte order
	brief:
		These functions change the data from host byte order to 
			network byte order.
*/
uint32_t cwb_net_tonet32(uint32_t data)
{
	return htonl(data);
}

uint16_t cwb_net_tonet16(uint16_t data)
{
	return htons(data);
}

/*
	funcName:cwb_net_tohost??
	input:
		data:The data with network byte order
	output:
		The data with host byte order
	brief:
		These functions change the data from network byte order to
			host byte order
*/
uint16_t cwb_net_tohost16(uint16_t data)
{
	return ntohs(data);
}

uint32_t cwb_net_tohost32(uint32_t data)
{
	return ntohl(data);
}
