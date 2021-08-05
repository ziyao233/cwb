/*
	cwb
	File:/test/Httpd.test.c
	Date:2021.08.05
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cwb/Ds.h"
#include"cwb/Httpd.h"

static int rule(char const *path)
{
	(void)path;
	return 1;
}

static char const *text = "<html><head><title>Test</title></head><body><h1>Test</h1></body></html>";

static int handler(Cwb_Httpd_Conn *conn)
{
	Cwb_Ds *ds = cwb_httpd_req_header(conn);
	if (!ds)
		return -1;
	
	for (Cwb_Ds_Pair *pair = cwb_ds_first(ds);
	     pair;
	     pair = cwb_ds_next(ds,pair)) {
		printf("%s: %s\n",(char*)cwb_ds_getkey(ds,pair),
		       (char*)cwb_ds_get(ds,pair));
	}

	Cwb_Ds *cookie = cwb_httpd_req_cookie(conn);
	if (!cookie)
		return -1;
	Cwb_Ds_Pair *name = cwb_ds_search(cookie,"name");
	if (name)
		printf("name: %s\n",(const char*)cwb_ds_get(cookie,name));

	if (cwb_httpd_res_status(conn,200,"OK"))
		return -1;
	Cwb_Ds *attr = cwb_ds_new(CWB_DS_HASHTABLE,CWB_DS_SKEY);
	if (!attr)
		return -1;
	if (!cwb_ds_insert(attr,"SameSite","Strict"))
		return -1;
	if (cwb_httpd_res_cookie(conn,"name","user",attr))
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
