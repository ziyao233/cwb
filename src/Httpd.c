/*
	cwb
	File:/src/Httpd.c
	Date:2021.07.20
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cwb/Conf.h"
#include"cwb/Net.h"
#include"cwb/Event.h"
#include"cwb/Httpd.h"

static Cwb_Httpd_Conf defaultConf = {
					.network = {
							.port	= 0,
							.backlog= CWB_CONF_HTTPD_BACKLOG
						   }
				    };

Cwb_Httpd *cwb_httpd_new(void)
{
	Cwb_Httpd *httpd = (Cwb_Httpd*)malloc(sizeof(Cwb_Httpd));
	if (!httpd)
		return NULL;
	
	httpd->eventBase = cwb_event_new();
	if (!httpd->eventBase)
		return NULL;
	
	httpd->conf = defaultConf;

	return httpd;
}

static int httpd_socket_handler(Cwb_Event_Base *base,int fd,
				void *data)
{
	Cwb_Httpd *httpd = (Cwb_Httpd*)data;
	(void)httpd;
	(void)base;
	int conn = cwb_net_accept(fd);
	if (conn < 0)
		return -1;
	
	cwb_net_close(conn);

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
			       (Cwb_Event_Fd_Handler)httpd_socket_handler,
			       (void*)httpd))
		return -1;
	
	cwb_event_loop(httpd->eventBase);
	
	return 0;
}

void cwb_httpd_destroy(Cwb_Httpd *httpd)
{
	cwb_event_destroy(httpd->eventBase);
	free(httpd);
	return;
}

