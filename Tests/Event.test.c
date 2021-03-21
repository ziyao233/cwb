/*
	cwb
	File:/Tests/Event.test.c
	Date:2021.03.21
	By LGPL v3.0 and Anti-996 License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>

#include<unistd.h>
#include<cwb/Time.h>
#include<cwb/Net.h>
#include<cwb/Event.h>

#define HTTP_PORT 23333
#define SOCKET_BACKLOG 16
#define SOCKET_MAXFD 512

void error_print(const char *msg)
{
	fputs(msg,stderr);
	return;
}

void log_print(const char *log)
{
	puts(log);
	return;
}

int socket_handler(Cwb_Event_Base *base,int serverSocket,void *userData)
{
	(void)base;
	(void)userData;
	log_print("New Connection");
	cwb_net_close(cwb_net_accept(serverSocket));
	return 0;
}

int main(void)
{
	int serverSocket=cwb_net_socket();
	if(serverSocket<0){
		error_print("cwb_net_socket()");
		return -1;
	}

	if(cwb_net_bind(serverSocket,HTTP_PORT)) {
		error_print("cwb_net_bind()");
		return -1;
	}

	if(cwb_net_listen(serverSocket,SOCKET_BACKLOG)) {
		error_print("cwb_net_listen()");
		return -1;
	}

	Cwb_Event_Base *base=cwb_event_new();
	if(!base) {
		error_print("cwb_base_new()");
		return -1;
	}

	if(cwb_event_fd_watch(base,serverSocket,
			      CWB_EVENT_FD_READ,
			      socket_handler,NULL)) {
		error_print("cwb_event_fd_watch()");
		return -1;
	}
	
	cwb_event_loop(base);
	cwb_event_destroy(base);

	return 0;
}
