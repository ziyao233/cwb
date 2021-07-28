/*
	cwb
	File:/src/cwb/Httpd.h
	Date:2021.07.28
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef __CWB_HTTPD_H_INC
#define __CWB_HTTPD_H_INC

#include<stdio.h>

#include"cwb/Event.h"
#include"cwb/Coroutine.h"

typedef enum {
	CWB_HTTPD_CONN_GET,
	CWB_HTTPD_CONN_POST,
	CWB_HTTPD_CONN_NULL
}Cwb_Httpd_Conn_Method;

typedef struct Cwb_Httpd_Conn Cwb_Httpd_Conn;
typedef struct Cwb_Httpd Cwb_Httpd;

typedef int (*Cwb_Httpd_Router_Rule)(char const *path);

typedef int (*Cwb_Httpd_Router_Handler)(Cwb_Httpd_Conn *conn);

typedef struct {
	struct{
		int port;
		int backlog;
	}network;
	struct {
		ssize_t maxHeaderSize;
		size_t maxPathSize;
	}http;
}Cwb_Httpd_Conf;

typedef struct Cwb_Httpd_Conn{
	Cwb_Httpd *httpd;
	Cwb_Coroutine *co;
	char *nativeHeader;
	void *buffer;
	ssize_t count;
	char *path;
	Cwb_Httpd_Conn_Method method;
	char *arg;
}Cwb_Httpd_Conn;

typedef struct Cwb_Httpd{
	Cwb_Event_Base *eventBase;
	Cwb_Httpd_Conf conf;
	int socketFd;
	struct {
		int num;
		Cwb_Httpd_Router_Rule		*rule;
		Cwb_Httpd_Router_Handler	*handler;
	}router;
	Cwb_Httpd_Conn *conn;
	char *reqPattern;
}Cwb_Httpd;

Cwb_Httpd *cwb_httpd_new(void);
void cwb_httpd_destroy(Cwb_Httpd *httpd);
int cwb_httpd_start(Cwb_Httpd *httpd);

int cwb_httpd_router_add(Cwb_Httpd *httpd,Cwb_Httpd_Router_Rule rule,
			 Cwb_Httpd_Router_Handler handler);

/*	Request		*/
Cwb_Ds *cwb_httpd_req_header(Cwb_Httpd_Conn *conn);

/*	Response	*/
int cwb_httpd_res_writen(Cwb_Httpd_Conn *conn,void *buffer,size_t size);
int cwb_httpd_res_status(Cwb_Httpd_Conn *conn,int status,char const *info);
int cwb_httpd_res_header(Cwb_Httpd_Conn *conn,char const *key,char const *value);
int cwb_httpd_res_endheader(Cwb_Httpd_Conn *conn);

#endif
