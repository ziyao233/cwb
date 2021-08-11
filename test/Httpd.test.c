/*
	cwb
	File:/test/Httpd.test.c
	Date:2021.08.11
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cwb/Ds.h"
#include"cwb/Serializer.h"
#include"cwb/Httpd.h"

static int urltest_rule(const char *path)
{
	puts(path);
	return !strcmp(path,"/urltest");
}

static int rule(const char *path)
{
	(void)path;
	return 1;
}

static int post_rule(const char *path)
{
	return !strcmp(path,"/post");
}

static int post_handler(Cwb_Httpd_Conn *conn)
{
	long int size = cwb_httpd_req_loadlen(conn);
	printf("POST Data Length:%ld\n",size);

	char *data = (char*)malloc(sizeof(char) * size + 1);
	if (!data)
		return -1;
	data[size] = '\0';
	
	if (cwb_httpd_req_readn(conn,(void*)data,size))
		return -1;
	
	puts(data);

	return 0;
}

static const char *text = "<html><head><title>Test</title></head><body><h1>Test</h1></body></html>";

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
	cwb_ds_destroy(cookie);

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

static int urltest_handler(Cwb_Httpd_Conn *conn)
{
	const char *nativeArg = cwb_httpd_req_arg(conn);
	if (!nativeArg) {
		const char *msg = "Got no arguments";
		if (cwb_httpd_res_writen(conn,(void*)msg,strlen(msg)))
			return -1;
		return 0;
	}

	if (cwb_httpd_res_status(conn,200,"OK"))
		return -1;
	if (cwb_httpd_res_endheader(conn))
		return -1;

	Cwb_Serialize_Value *arg = cwb_deserialize_urlcoded(NULL,nativeArg);
	if (!arg)
		return -1;
	Cwb_Ds *ds = cwb_serialize_get(arg).ds;
	Cwb_Ds_Pair *user = cwb_ds_search(ds,"user");
	if (!user)
		return -1;
	
	char temp[64] = {0};
	Cwb_Serialize_Value *value = (Cwb_Serialize_Value*)cwb_ds_get(ds,user);
	snprintf(temp,64,"Username is %s",cwb_serialize_get(value).string);

	if (cwb_httpd_res_writen(conn,(void*)temp,strlen(temp)))
		return -1;

	cwb_serialize_destroy(arg);
	
	return 0;
}

int main(void)
{
	Cwb_Httpd *httpd = cwb_httpd_new();
	assert(httpd);

	httpd->conf.network.port = 10000;

	assert(!cwb_httpd_router_add(httpd,(Cwb_Httpd_Router_Rule)post_rule,
				     (Cwb_Httpd_Router_Handler)post_handler));
	assert(!cwb_httpd_router_add(httpd,(Cwb_Httpd_Router_Rule)urltest_rule,
				     (Cwb_Httpd_Router_Handler)urltest_handler));
	assert(!cwb_httpd_router_add(httpd,(Cwb_Httpd_Router_Rule)rule,
				     (Cwb_Httpd_Router_Handler)handler));

	assert(!cwb_httpd_start(httpd));

	cwb_httpd_destroy(httpd);

	return 0;
}
