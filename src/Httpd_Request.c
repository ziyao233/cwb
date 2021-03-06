/*
	cwb
	File:/src/Httpd_Request.c
	Date:2021.12.23
	By MIT License.
	Copyright (c) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include"cwb/Util.h"
#include"cwb/Dstr.h"
#include"cwb/Ds.h"
#include"cwb/Serializer.h"
#include"cwb/Net.h"
#include"cwb/Httpd.h"

Cwb_Ds *cwb_httpd_req_header(Cwb_Httpd_Conn *conn)
{
	if (conn->header)
		return conn->header;

	Cwb_Ds *header = cwb_ds_new(CWB_DS_HASHTABLE,CWB_DS_SKEY);
	if (!header)
		return NULL;
	
	char *p = conn->nativeHeader;
	Cwb_Dstr *str = cwb_dstr_new();
	char key[64];
	while (*p) {
		while (*p !=':' && *p) {
			if (!*p)
				return NULL;
			cwb_dstr_appendc(str,*p);
			p++;
		}
		if (!*p)
			return NULL;
		p += 2;		/*	Skip ':' and the space	*/
		if (!cwb_dstr_convert(str,key,64))
			return NULL;
		cwb_util_str_tolowerd(key);

		cwb_dstr_clear(str);

		while (*p !='\r' && *p) {
			if (!*p)
				break;
			cwb_dstr_appendc(str,*p);
			p++;
		}
		char *value = cwb_dstr_convert(str,NULL,0);
		if (!value)
			return NULL;
		if (!cwb_ds_insert(header,key,value))
			return NULL;
		cwb_dstr_clear(str);
		p = *p ? p + 2 : p;		/*	Skip '\r' and '\n'	*/
	}

	conn->header = header;
	
	return header;
}

static const char *skip_space(const char *str)
{
	while (*str == ' ')
		str++;
	return str;
}

Cwb_Ds *cwb_httpd_req_cookie(Cwb_Httpd_Conn *conn)
{
	if (!conn->header) {
		if (!cwb_httpd_req_header(conn))
			return NULL;
	}

	Cwb_Ds_Iter *iter = cwb_ds_get(conn->header,"cookie");
	if (!iter)
		return cwb_ds_new(CWB_DS_HASHTABLE,CWB_DS_SKEY);
	const char *src = (const char*)cwb_ds_get(conn->header,iter);

	Cwb_Ds *result = cwb_ds_new(CWB_DS_HASHTABLE,CWB_DS_SKEY);
	if (!result)
		return NULL;

	Cwb_Dstr *temp = cwb_dstr_new();
	if (!temp)
		return NULL;
	char key[256] = {0};
	while (*src) {
		src = skip_space(src);

		while (*src != '=') {
			if (!cwb_dstr_appendc(temp,*src))
				return NULL;
			src++;
		}
		if (!cwb_dstr_convert(temp,key,256))
			return NULL;
		cwb_dstr_clear(temp);

		src++;
		while (*src != ';' && *src) {
			if (!cwb_dstr_appendc(temp,*src))
				return NULL;
			src++;
		}
		if (!cwb_ds_insert(result,key,cwb_dstr_convert(temp,NULL,0)))
			return NULL;

		cwb_dstr_clear(temp);
		src = *src ? src + 1 : src;
	}

	return result;
}

long int cwb_httpd_req_loadlen(Cwb_Httpd_Conn *conn)
{
	Cwb_Ds *header = cwb_httpd_req_header(conn);
	if (!header)
		return -1;
	
	Cwb_Ds_Iter *length = cwb_ds_get(header,"content-length");
	if (!length)
		return -1;

	return strtol((const char*)cwb_ds_value(header,length),
		      NULL,0);
}

static inline int get_fd(Cwb_Httpd_Conn *conn)
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

static int handler_read(Cwb_Event_Base *base,int fd,void *userData)
{
	Cwb_Httpd_Conn *conn = (Cwb_Httpd_Conn*)userData;

	ssize_t size = cwb_net_read(fd,conn->buffer,conn->count);
	if (CWB_IO_TRUEERROR)
		return -1;

	conn->count -= size;
	conn->buffer = (void*)((uint8_t*)conn->buffer + size);

	if (!(conn->count)) {
		if (cwb_event_fd_unwatch(base,fd))
			return -1;

		if (wake_co(conn->co))
			return -1;
	}

	return 0;
}

int cwb_httpd_req_readn(Cwb_Httpd_Conn *conn,void *buffer,size_t size)
{
	if (conn->count) {
		if ((size_t)conn->count >= size) {
			conn->count -= size;
			memcpy(buffer,conn->buffer,size);
			memmove(conn->buffer,
				(void*)((uint8_t*)conn->buffer + size),
				size);
			
			if (!(conn->count)) {
				free(conn->buffer);
				conn->buffer = NULL;
			}
			return 0;
		} else {
			memcpy(buffer,conn->buffer,conn->count);
			buffer = (void*)((uint8_t*)buffer + conn->count);
			size  -= conn->count;
			conn->count = 0;
			free(conn->buffer);
			conn->buffer = NULL;
		}
	}

	conn->buffer = buffer;
	conn->count  = size;

	if (cwb_event_fd_watch(conn->httpd->eventBase,get_fd(conn),
			       CWB_EVENT_FD_READ,
			       (Cwb_Event_Fd_Handler)handler_read,(void*)conn))
		return -1;
	
	cwb_coroutine_yield(conn->co);

	return 0;
}

int cwb_httpd_req_read(Cwb_Httpd_Conn *conn,void *buffer,size_t size)
{
	return cwb_httpd_req_readn(conn,buffer,size);
}
