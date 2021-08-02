/*
	cwb
	File:/src/Httpd_Conn.c
	Date:2021.08.02
	By MIT License.
	Copyright (c) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include"cwb/Net.h"
#include"cwb/Httpd.h"

static int get_fd(Cwb_Httpd_Conn *conn)
{
	return conn - conn->httpd->conn;
}

static int wake_co(Cwb_Coroutine *co)
{
	short int status = cwb_coroutine_resume(co);
	if (status == CWB_COROUTINE_DEAD) {
		cwb_coroutine_destroy(co);
	} else if (status == CWB_COROUTINE_ERROR) {
		cwb_coroutine_destroy(co);
		return -1;
	}
	return 0;
}

static int handler_write(Cwb_Event_Base *base,int fd,void *data)
{
	Cwb_Httpd_Conn *conn = (Cwb_Httpd_Conn*)data;

	ssize_t retVal = cwb_net_write(fd,conn->buffer,conn->count);
	if (CWB_IO_TRUEERROR)
		return -1;
	
	conn->count	-= retVal;
	conn->buffer	= (uint8_t*)conn->buffer + retVal;

	if (!(conn->count)) {
		if (cwb_event_fd_unwatch(base,fd))
			return -1;

		if (wake_co(conn->co))
			return -1;
	}
	
	return 0;
}

int cwb_httpd_res_writen(Cwb_Httpd_Conn *conn,void *buffer,
			  size_t size)
{
	conn->buffer	= buffer;
	conn->count	= size;

	if (cwb_event_fd_watch(conn->httpd->eventBase,get_fd(conn),
			       CWB_EVENT_FD_WRITE,
			       (Cwb_Event_Fd_Handler)handler_write,(void*)conn))
		return -1;
	
	cwb_coroutine_yield(conn->co);

	return 0;
}

int cwb_httpd_res_status(Cwb_Httpd_Conn *conn,int status,const char *info)
{
	const char *format = "HTTP/1.1 %03d %s\r\n";
	
	size_t size = strlen(format) + strlen(info) + 6;
	char *buffer = (char*)malloc(size);
	if (!buffer)
		return -1;

	sprintf(buffer,format,status,info);
	size = strlen(buffer);

	if (cwb_httpd_res_writen(conn,(void*)buffer,size))
		return -1;

	free(buffer);

	return 0;
}

int cwb_httpd_res_header(Cwb_Httpd_Conn *conn,const char *key,const char *value)
{
	size_t size = strlen(key) + strlen(value) + 5;
	char *buffer = (char*)malloc(size);
	
	if (!buffer)
		return -1;
	
	sprintf(buffer,"%s: %s\r\n",key,value);

	size = strlen(buffer);

	if (cwb_httpd_res_writen(conn,(void*)buffer,size))
		return -1;
	
	free(buffer);
	
	return 0;
}

int cwb_httpd_res_endheader(Cwb_Httpd_Conn *conn)
{
	return cwb_httpd_res_writen(conn,(void*)"\r\n",2);
}
