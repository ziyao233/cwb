/*
	cwb
	File:/src/cwb/Httpd.h
	Date:2021.07.20
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef __CWB_HTTPD_H_INC
#define __CWB_HTTPD_H_INC

#include<stdio.h>

#include"cwb/Event.h"

typedef struct {
	struct{
		int port;
		int backlog;
	}network;
}Cwb_Httpd_Conf;

typedef struct {
	Cwb_Event_Base *eventBase;
	Cwb_Httpd_Conf conf;
	int socketFd;
}Cwb_Httpd;

Cwb_Httpd *cwb_httpd_new(void);
void cwb_httpd_destroy(Cwb_Httpd *httpd);
int cwb_httpd_start(Cwb_Httpd *httpd);

#endif
