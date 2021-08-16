/*
	cwb
	File:/src/Httpd.c
	Date:2021.08.16
	By MIT License.
	Copyright (c) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include"cwb/Conf.h"
#include"cwb/Coroutine.h"
#include"cwb/Net.h"
#include"cwb/Event.h"
#include"cwb/Httpd.h"

static Cwb_Httpd_Conf defaultConf = {
					.network = {
							.port	= 0,
							.backlog= CWB_CONF_HTTPD_BACKLOG
						   },
					.http	 = {
							.maxHeaderSize = 
								CWB_CONF_HTTPD_MAXHEADERSIZE,
							.maxPathSize   =
								CWB_CONF_HTTPD_MAXPATHSIZE
						   }
				    };

typedef struct {
	Cwb_Httpd_Conn *conn;
	int handler;
}Proc_Data;

Cwb_Httpd *cwb_httpd_new(void)
{
	Cwb_Httpd *httpd = (Cwb_Httpd*)malloc(sizeof(Cwb_Httpd));
	if (!httpd)
		return NULL;
	
	httpd->eventBase = cwb_event_new();
	if (!httpd->eventBase)
		return NULL;
	
	httpd->router.num	= 4;
	httpd->router.rule	= (Cwb_Httpd_Router_Rule*)
				  malloc(sizeof(Cwb_Httpd_Router_Rule) * 4);
	httpd->router.handler	= (Cwb_Httpd_Router_Handler*)
				  malloc(sizeof(Cwb_Httpd_Router_Handler) * 4);
	
	if (!httpd->router.rule || !httpd->router.handler)
		return NULL;
	memset(httpd->router.rule,0,sizeof(Cwb_Httpd_Router_Rule) * 4);
	memset(httpd->router.handler,0,sizeof(Cwb_Httpd_Router_Handler)*4);

	httpd->conn		= (Cwb_Httpd_Conn*)
				  malloc(sizeof(Cwb_Httpd_Conn)*
					 CWB_CONF_EVENT_FD_MAX_NUM);
	if (!httpd->conn)
		return NULL;
	
	httpd->conf = defaultConf;

	return httpd;
}

static Cwb_Httpd_Conn_Method get_method(const char *str)
{
	static const char *table[] = {
					[CWB_HTTPD_CONN_GET]	= "GET",
					[CWB_HTTPD_CONN_POST]	= "POST",
					[CWB_HTTPD_CONN_OPTIONS]= "OPTIONS",
					[CWB_HTTPD_CONN_PUT]	= "PUT",
					[CWB_HTTPD_CONN_DELETE] = "DELETE"
				     };
	for (Cwb_Httpd_Conn_Method method = CWB_HTTPD_CONN_GET;
	     method < CWB_HTTPD_CONN_NULL;
	     method++) {
		if (!strcmp(table[method],str))
			return method;
	}
	return CWB_HTTPD_CONN_NULL;
}

static int wake_co(Cwb_Coroutine *co)
{
	short int status = cwb_coroutine_resume(co);
	if (status == CWB_COROUTINE_DEAD) {
		cwb_coroutine_destroy(co);
	} else if(status == CWB_COROUTINE_ERROR) {
		cwb_coroutine_destroy(co);
		return -1;
	}
	return 0;
}

static int proc_body(Cwb_Coroutine *co,void *data)
{
	(void)co;
	Proc_Data *procData = (Proc_Data*)data;
	Cwb_Httpd_Conn *conn = procData->conn;
	int handler = procData->handler;
	int retVal  = conn->httpd->router.handler[handler](conn);
	int fd	    = conn - conn->httpd->conn;

	if (!(conn->status.header)) {
		if (!retVal)
			retVal = cwb_httpd_res_endheader(conn);
	}
	cwb_net_close(fd);
	
	return retVal;
}

static int read_header(Cwb_Event_Base *base,int fd,
		       void *data)
{
	Cwb_Httpd_Conn *conn = (Cwb_Httpd_Conn*)data;
	Cwb_Httpd *httpd = conn->httpd;
	
	ssize_t retVal = cwb_net_read(fd,(uint8_t*)conn->buffer + conn->count,
				      httpd->conf.http.maxHeaderSize -
				      conn->count);
	if (retVal < 0) {
		return -1;
	}

	conn->count += retVal;
	((char*)conn->buffer)[conn->count] = '\0';
	
	/*	End of the header(A blank line)	*/
	char *end = strstr((char*)conn->buffer,"\r\n\r\n");
	if (!end) {
		if (conn->count == httpd->conf.http.maxHeaderSize) {
			return -1;
		}
		return 0;
	}

	if (cwb_event_fd_unwatch(base,fd))
		return -1;

	conn->nativeHeader = (char*)malloc(httpd->conf.http.maxHeaderSize);
	if (!conn->nativeHeader)
		return -1;
	
	char reqMethod[9];
	sscanf((char*)conn->buffer,httpd->reqPattern,
	       reqMethod,conn->path);
	conn->method = get_method(reqMethod);
	
	char *arg = strstr(conn->path,"?");
	if (arg) {
		*arg = '\0';
		arg++;
		size_t size = strlen(arg);
		conn->arg   = (char*)malloc(size + 1);
		if (!conn->arg)
			return -1;
		strcpy(conn->arg,arg);
	}

	char *header = strstr((char*)conn->buffer,"\r\n") + 2;
	strncpy(conn->nativeHeader,header,end-header);

	end += 4;
	size_t bodySize = conn->count - (end - (char*)conn->buffer);
	if (bodySize) {
		memmove(conn->buffer,end,bodySize);
	} else {
		free(conn->buffer);
	}
	conn->count = bodySize;
	conn->status.readHeader = 1;

	for (int i = 0;httpd->router.rule[i];i++) {
		if (httpd->router.rule[i](conn->path)) {
			Proc_Data procData = (Proc_Data){
								.handler = i,
								.conn	 = conn
							};
			conn->co = cwb_coroutine_new(proc_body,(void*)&procData);
			if (!conn->co)
				return -1;

			return wake_co(conn->co);
		}
	}

	return -1;
}

static int socket_handler(Cwb_Event_Base *base,int fd,
				void *data)
{
	Cwb_Httpd *httpd = (Cwb_Httpd*)data;
	
	while (1) {
		int connFd = cwb_net_accept(fd);
		
		if (connFd < 0) {
			if (CWB_IO_TRUEERROR)
				return -1;
			
			break;
		}
	
		httpd->conn[connFd] = (Cwb_Httpd_Conn) {
				.httpd	= httpd,
				.buffer	= malloc(httpd->conf.http.maxHeaderSize),
				.count	= 0,
				.path	= (char*)malloc(httpd->conf.http.maxPathSize),
				.arg	= NULL,
				.header = NULL
			};

		if (!httpd->conn[connFd].buffer || !httpd->conn[connFd].path)
			return -1;

		if (cwb_event_fd_watch(base,connFd,CWB_EVENT_FD_READ,
				       (Cwb_Event_Fd_Handler)read_header,
				       (void*)(httpd->conn+connFd)))
			return -1;
	}

	return 0;
}

int cwb_httpd_start(Cwb_Httpd *httpd)
{
	httpd->socketFd = cwb_net_socket();
	if (httpd->socketFd < 0)
		return -1;
	
	if (cwb_net_bind(httpd->socketFd,httpd->conf.network.port))
		return -1;
	
	if (cwb_net_listen(httpd->socketFd,httpd->conf.network.backlog))
		return -1;
	
	if (cwb_event_fd_watch(httpd->eventBase,httpd->socketFd,CWB_EVENT_FD_READ,
			       (Cwb_Event_Fd_Handler)socket_handler,
			       (void*)httpd))
		return -1;

	httpd->reqPattern = (char*)malloc(6 + httpd->conf.http.maxPathSize);
	sprintf(httpd->reqPattern,"%%s %%s %%*s");
	
	return cwb_event_loop(httpd->eventBase);
}

void cwb_httpd_destroy(Cwb_Httpd *httpd)
{
	cwb_event_destroy(httpd->eventBase);
	free(httpd);
	return;
}

int cwb_httpd_router_add(Cwb_Httpd *httpd,Cwb_Httpd_Router_Rule rule,
			 Cwb_Httpd_Router_Handler handler)
{
	int i = 0;
	for (;i < httpd->router.num;i++) {
		if (!httpd->router.rule[i])
			break;
	}

	/*	All the slots are already used	*/
	if (i == httpd->router.num) {
		httpd->router.num += 4;
		httpd->router.rule    = (Cwb_Httpd_Router_Rule*)
					realloc(httpd->router.rule,
						sizeof(Cwb_Httpd_Router_Rule) *
						httpd->router.num);
		httpd->router.handler = (Cwb_Httpd_Router_Handler*)
					realloc(httpd->router.handler,
						sizeof(Cwb_Httpd_Router_Handler) *
						httpd->router.num);

		if (!httpd->router.rule || !httpd->router.handler)
			return -1;

		memset(httpd->router.rule + httpd->router.num - 4,0,
		       sizeof(Cwb_Httpd_Router_Rule) * 4);
		memset(httpd->router.handler + httpd->router.num - 4,0,
		       sizeof(Cwb_Httpd_Router_Handler) * 4);
	}

	httpd->router.rule[i]		= rule;
	httpd->router.handler[i]	= handler;

	return 0;
}

int CWB_HTTPD_ROUTER_RULE_ANY(const char *path)
{
	(void)path;
	return 1;
}
