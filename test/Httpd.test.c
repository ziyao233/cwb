/*
	cwb
	File:/test/Httpd.test.c
	Date:2021.07.23
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cwb/Httpd.h"

static int rule(char const *path)
{
	(void)path;
	return 1;
}

char const *data = "HTTP/1.1 200 OK\r\n\r\n<h1>Hello</h1>";

static int handler(Cwb_Httpd_Conn *conn)
{
	if (cwb_httpd_conn_writen(conn,(void*)data,strlen(data)))
		return -1;
	return 0;
}

int main(void)
{
	Cwb_Httpd *httpd = cwb_httpd_new();
	assert(httpd);

	httpd->conf.network.port = 12345;

	assert(!cwb_httpd_router_add(httpd,(Cwb_Httpd_Router_Rule)rule,
				     (Cwb_Httpd_Router_Handler)handler));

	assert(!cwb_httpd_start(httpd));

	cwb_httpd_destroy(httpd);

	return 0;
}
