/*
	cwb
	File:/test/Httpd.test.c
	Date:2021.07.26
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

static char const *text = "<html><head><title>Test</title></head><body><h1>Test</h1></body></html>";

static int handler(Cwb_Httpd_Conn *conn)
{
	if (cwb_httpd_res_status(conn,200,"OK"))
		return -1;
	if (cwb_httpd_res_header(conn,"Content-type","text/html"))
		return -1;
	if (cwb_httpd_res_endheader(conn))
		return -1;
	if (cwb_httpd_res_writen(conn,(void*)text,strlen(text)))
		return -1;
	
	return 0;
}

int main(void)
{
	Cwb_Httpd *httpd = cwb_httpd_new();
	assert(httpd);

	httpd->conf.network.port = 10000;

	assert(!cwb_httpd_router_add(httpd,(Cwb_Httpd_Router_Rule)rule,
				     (Cwb_Httpd_Router_Handler)handler));

	assert(!cwb_httpd_start(httpd));

	cwb_httpd_destroy(httpd);

	return 0;
}
