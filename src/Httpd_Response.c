/*
	cwb
	File:/src/Httpd_Response.c
	Date:2021.08.13
	By MIT License.
	Copyright (c) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include"cwb/Net.h"
#include"cwb/Dstr.h"
#include"cwb/Coder.h"
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

int cwb_httpd_res_writen(Cwb_Httpd_Conn *conn,const void *buffer,
			  size_t size)
{
	conn->buffer	= (void*)buffer;
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

	conn->status.status = 1;

	return 0;
}

int cwb_httpd_res_header(Cwb_Httpd_Conn *conn,const char *key,const char *value)
{
	if (!(conn->status.status)) {
		if (cwb_httpd_res_status(conn,200,"OK"))
			return -1;
	}

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
	if (!(conn->status.status)) {
		if (cwb_httpd_res_status(conn,200,"OK"))
			return -1;
	}
	conn->status.header = 1;
	conn->status.endHeader = 1;
	return cwb_httpd_res_writen(conn,(void*)"\r\n",2);
}

int cwb_httpd_res_cookie(Cwb_Httpd_Conn *conn,const char *key,
			 const char *value,Cwb_Ds *attr)
{
	if (!(conn->status.status)) {
		if (cwb_httpd_res_status(conn,200,"OK"))
			return -1;
	}
	Cwb_Dstr *cookie = cwb_dstr_new();
	if (!cookie)
		return -1;
	
	if (!cwb_encode_uri(cookie,(void*)key,strlen(key)))
		return -1;
	
	if (!cwb_dstr_appendc(cookie,'='))
		return -1;
	
	if (!cwb_encode_uri(cookie,(void*)value,strlen(value)))
		return -1;
	
	for (Cwb_Ds_Pair *pair = attr ? cwb_ds_first(attr) : NULL;
	     pair;
	     pair = cwb_ds_next(attr,pair)) {
		if (!cwb_dstr_appends(cookie,"; "))
			return -1;

		const char *key = (const char*)cwb_ds_getkey(attr,pair);
		if (!cwb_dstr_append(cookie,key))
			return -1;

		const char *value = (const char*)cwb_ds_get(attr,pair);
		if (value) {
			if (!cwb_dstr_appendc(cookie,'='))
				return -1;
			if (!cwb_dstr_append(cookie,value))
				return -1;
		}
	}
	char *buffer = cwb_dstr_convert(cookie,NULL,0);
	if (!buffer)
		return -1;
	cwb_dstr_destroy(cookie);

	int retVal = cwb_httpd_res_header(conn,"Set-Cookie",buffer);
	free(buffer);
	return retVal;
}

int cwb_httpd_res_write(Cwb_Httpd_Conn *conn,const void *buffer,size_t size)
{
	if (!(conn->status.status)) {
		if (cwb_httpd_res_status(conn,200,"OK"))
			return -1;
	}

	if (!(conn->status.endHeader)) {
		if (cwb_httpd_res_endheader(conn))
			return -1;
	}

	return cwb_httpd_res_writen(conn,buffer,size);
}

int cwb_httpd_res_writes(Cwb_Httpd_Conn *conn,const char *str)
{
	return cwb_httpd_res_write(conn,str,strlen(str));
}
