/*
	cwb
	File:/src/cwb/Httpd.h
	Date:2021.08.16
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef __CWB_HTTPD_H_INC
#define __CWB_HTTPD_H_INC

#include<stdio.h>

#include"cwb/Ds.h"
#include"cwb/Event.h"
#include"cwb/Coroutine.h"

typedef enum {
	CWB_HTTPD_CONN_GET,
	CWB_HTTPD_CONN_POST,
	CWB_HTTPD_CONN_DELETE,
	CWB_HTTPD_CONN_OPTIONS,
	CWB_HTTPD_CONN_PUT,
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
	Cwb_Ds *header;
	void *buffer;
	ssize_t count;
	char *path;
	Cwb_Httpd_Conn_Method method;
	char *arg;
	struct {
		unsigned int readHeader:1;
		unsigned int status:1;
		unsigned int header:1;
		unsigned int endHeader:1;
		unsigned int body:1;
	}status;
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
int CWB_HTTPD_ROUTER_RULE_ANY(const char *path);

/*	Request		*/
#define cwb_httpd_req_arg(conn) ((const char*)((conn)->arg))
#define cwb_httpd_req_method(conn) ((conn)->method)

Cwb_Ds *cwb_httpd_req_header(Cwb_Httpd_Conn *conn);
Cwb_Ds *cwb_httpd_req_cookie(Cwb_Httpd_Conn *conn);
long int cwb_httpd_req_loadlen(Cwb_Httpd_Conn *conn);
int cwb_httpd_req_readn(Cwb_Httpd_Conn *conn,void *buffer,size_t size);
int cwb_httpd_req_read(Cwb_Httpd_Conn *conn,void *buffer,size_t size);

/*	Response	*/
int cwb_httpd_res_writen(Cwb_Httpd_Conn *conn,const void *buffer,size_t size);
int cwb_httpd_res_status(Cwb_Httpd_Conn *conn,int status,const char *info);
int cwb_httpd_res_header(Cwb_Httpd_Conn *conn,const char *key,const char *value);
int cwb_httpd_res_endheader(Cwb_Httpd_Conn *conn);
int cwb_httpd_res_cookie(Cwb_Httpd_Conn *conn,const char *key,const char *value,
			 Cwb_Ds *attr);
int cwb_httpd_res_write(Cwb_Httpd_Conn *conn,const void *buffer,size_t size);
int cwb_httpd_res_writes(Cwb_Httpd_Conn *conn,const char *str);

#endif
