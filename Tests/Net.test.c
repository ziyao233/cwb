/*
	cwb
	File:/Tests/Net.Test.c
	Date:2021.02.16
	By LGPL v3.0
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

#include<unistd.h>

#include<cwb/Time.h>
#include<cwb/Net.h>

#define HTTP_PORT 14137
#define HTTP_BACKLOG 32
#define BUFFER_SIZE 4096

void error_print(const char *err)
{
	fputs(err,stderr);
	return;
}

typedef struct
{
	uint8_t buffer[BUFFER_SIZE];
	size_t used;
}Data_Buffer;

int main(void)
{
	int serverSocket=cwb_net_socket();
	if(serverSocket<0)
	{
		error_print("cwb_net_socket()");
		return -1;
	}

	if(cwb_net_bind(serverSocket,HTTP_PORT))
	{
		error_print("cwb_net_bind()");
		return -1;
	}

	if(cwb_net_listen(serverSocket,HTTP_BACKLOG))
	{
		error_print("cwb_net_listen()");
		return -1;
	}
		
	Cwb_Net_Listener *listener=cwb_net_listener_new(32);
	if(!listener)
	{
		error_print("cwb_net_listener_new()");
		return -1;
	}

	cwb_net_listener_listen(listener,serverSocket,CWB_NET_LISTENER_READ);

	Data_Buffer *dataBuffer=(Data_Buffer*)malloc(sizeof(Data_Buffer)*32);
	if(!dataBuffer)
	{
		error_print("Malloc for dataBuffer");
		return -1;
	}
	while(1)
	{
		Cwb_Time_MicroSecond ms=2000;
		int *activeList=cwb_net_listener_wait(listener,NULL,32,&ms);
		if(!activeList)
		{
			if(CWB_NET_TRUEERROR)
			{
				error_print("cwb_net_listener_wait()");
				cwb_net_listener_destroy(listener);
				return -1;
			}
			if(CWB_NET_LISTENER_TIMEOUT)
			{
				puts("Timeout");
				continue;
			}
		}
		
		for(int index=0;activeList[index]>=0;index++)
		{
			/*	New Connection	*/
			if(activeList[index]==serverSocket)
			{
				int connectionFd=cwb_net_accept(serverSocket);
				if(connectionFd<0)
				{
					if(CWB_NET_TRUEERROR)
					{
						error_print("cwb_net_accept()");
						return -1;
					}
					continue;
				}
				if(connectionFd>32)
				{
					cwb_net_close(connectionFd);
					continue;
				}
				cwb_net_listener_listen(listener,connectionFd,CWB_NET_LISTENER_READ);
				dataBuffer[connectionFd].used=0;
			}
			/*	Common Connection	*/
			else
			{
				int connectionFd=activeList[index];

				ssize_t size=cwb_net_read(connectionFd,
										  (void*)(dataBuffer[connectionFd].used+
											dataBuffer[connectionFd].buffer),
										  BUFFER_SIZE-dataBuffer[connectionFd].used
										 );

				/*	An error is returned	*/
				if(size<0)
				{
					if(CWB_NET_TRUEERROR)
					{
						error_print("cwb_net_read()");
						return -1;
					}
				}

				dataBuffer[connectionFd].used+=(size_t)size;
				if(dataBuffer[connectionFd].used>=BUFFER_SIZE)
				{
					cwb_net_close(connectionFd);
				}

				size_t dataLength=dataBuffer[connectionFd].used;
				char temp[5]={0};
				strncpy(temp,
						(char*)(dataBuffer[connectionFd].buffer+dataLength),4);
				/*	The end of the HTTP header	*/
				if(strcmp(temp,"\r\n\r\n"))
				{
					puts((char*)(dataBuffer[connectionFd].buffer));
					cwb_net_close(connectionFd);
					cwb_net_listener_unlisten(listener,connectionFd);
				}

			}
		}
	}
	
	return 0;
}
