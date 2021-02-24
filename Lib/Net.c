/*
	cwb
	File:/Lib/Net.c
	Date:2021.02.23
	By LGPL v3.0 and Anti-996 License
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

#include"Net.h"
#include"Time.h"

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

/*
	funcName:cwb_net_listener_new
	input:
		maxNum:The maxium number of the handlers that could be listened.
	output:
		The handler of the listener.
	brief:
		This function creates a listener and return it.If failed,NULL will be returned.
		The listener should be destroyed by cwb_net_listener_destroy
*/
Cwb_Net_Listener *cwb_net_listener_new(unsigned int maxNum)
{
	/*	Select can only save FD_SETSIZE fds.So check it	*/
	if(maxNum>FD_SETSIZE)
	{
		return NULL;
	}

	Cwb_Net_Listener *listener=(Cwb_Net_Listener*)malloc(sizeof(Cwb_Net_Listener));
	if(!listener)
	{
		return NULL;
	}

	FD_ZERO(&(listener->rSet));
	FD_ZERO(&(listener->wSet));

	return listener;
}

/*
	funcName:cwb_net_listener_listen
	input:
		listener: Just the listener.I will NOT repeat it.
		fd:The file describer that will be listened
		flag:What event will be reported.
	return:
		0 on success and non-zero when fails
	brief:
		This function lets the listener listen a file describer
*/
int cwb_net_listener_listen(Cwb_Net_Listener *listener,int fd,
							uint8_t flag)
{
	if(fd>(FD_SETSIZE-1))
	{
		return -1;
	}

	/*	CWB_NET_LISTENER_NULL should not be used	*/
	if(!flag)
	{
		return -1;
	}

	/*	The file describer has already been listened	*/
	if(FD_ISSET(fd,&(listener->rSet)) ||
	   FD_ISSET(fd,&(listener->rSet)))
	{
		return -1;
	}

	if(flag&CWB_NET_LISTENER_READ)
	{
		FD_SET(fd,&(listener->rSet));
	}

	if(flag&CWB_NET_LISTENER_WRITE)
	{
		FD_SET(fd,&(listener->wSet));
	}

	return 0;
}

/*
	funcName:cwb_net_listener_wait
	input:
		listener:I will NOT repeat again.
		readyList:The list to store the ready file describers.
		maxNum:The maxium number of the handlers.
	return:
		NULL when fails.
		readyList on success.
		The address of a list filled with ready file describers.
	brief:
		This function will be blocked until any fd begins.
		The maxNum should bigger than the length of the readyList.
		The list is end with an element less than 0
		If readyList is NULL,this functions will allocate a list.
			And this list should be free(just by free())
*/
int *cwb_net_listener_wait(Cwb_Net_Listener *listener,int *readyList,
						   size_t maxNum,Cwb_Time_MicroSecond *timeout)
{
	int *list=readyList?readyList:malloc(sizeof(int)*(maxNum+1));

	/*	readyList is NULL and cannot allocate memory	*/
	if(!list)
	{
		return NULL;
	}

	//	The backup of the two sets
	fd_set wSet,rSet;

	/*	Backup	*/
	memcpy((void*)&wSet,(void*)(&(listener->wSet)),sizeof(fd_set));
	memcpy((void*)&rSet,(void*)(&(listener->rSet)),sizeof(fd_set));


	struct timeval timeVal,*timeoutPointer=NULL;
	if(timeout)
	{
		timeoutPointer=&timeVal;
		timeVal=(struct timeval){
									.tv_sec=(time_t)(*timeout/1000),
									.tv_usec=(suseconds_t)(*timeout%1000)
								};
	}
	int readyNum=select(FD_SETSIZE,&rSet,&wSet,NULL,timeoutPointer);
	
	/*	Timeout	or something wrong	*/
	if(readyNum<=0)
	{
		return NULL;
	}


	unsigned int listIndex=0;
	for(int testFd=0;
		testFd<FD_SETSIZE &&
		listIndex<(unsigned int)readyNum &&
		listIndex<maxNum;
		testFd++)
	{
		if(FD_ISSET(testFd,&rSet) || FD_ISSET(testFd,&wSet))
		{
			list[listIndex]=testFd;
			listIndex++;
		}
	}

	list[listIndex]=-1;

	return list;
}

int cwb_net_listener_unlisten(Cwb_Net_Listener *listener,int fd)
{
	if(FD_ISSET(fd,&(listener->wSet))||
	   FD_ISSET(fd,&(listener->rSet)))
	{
		FD_CLR(fd,&(listener->wSet));
		FD_CLR(fd,&(listener->rSet));
		return 0;
	}
	return -1;
}

void cwb_net_listener_destroy(Cwb_Net_Listener *listener)
{
	free(listener);
	return;
}
