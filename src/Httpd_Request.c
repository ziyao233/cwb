/*
	cwb
	File:/src/Httpd_Request.c
	Date:2021.08.05
	By MIT License.
	Copyright (c) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cwb/Util.h"
#include"cwb/Dstr.h"
#include"cwb/Ds.h"
#include"cwb/Serializer.h"
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
		while (*p !=':') {
			if (!*p)
				return NULL;
			cwb_dstr_appendc(str,*p);
			p++;
		}
		p += 2;		/*	Skip ':' and the space	*/
		if (!cwb_dstr_convert(str,key,64))
			return NULL;

		cwb_dstr_clear(str);

		while (*p !='\r') {
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
		p += 2;		/*	Skip '\r' and '\n'	*/
	}

	conn->header = header;
	
	return header;
}

Cwb_Ds *cwb_httpd_req_cookie(Cwb_Httpd_Conn *conn)
{
	if (!conn->header) {
		if (!cwb_httpd_req_header(conn))
			return NULL;
	}

	Cwb_Ds_Pair *pair = cwb_ds_search(conn->header,"Cookie");
	if (!pair)
		return cwb_ds_new(CWB_DS_HASHTABLE,CWB_DS_SKEY);
	const char *src = (const char*)cwb_ds_get(conn->header,pair);

	Cwb_Serialize_Value *cookie = cwb_deserialize_urlcoded(NULL,src);
	if (!cookie)
		return NULL;
	Cwb_Ds *ds = cwb_serialize_get(cookie).ds;

	Cwb_Ds *result = cwb_ds_new(CWB_DS_HASHTABLE,CWB_DS_SKEY);
	if (!result)
		return NULL;

	for (Cwb_Ds_Pair *pair = cwb_ds_first(ds);
	     pair;
	     pair = cwb_ds_next(ds,pair)) {
		Cwb_Serialize_Value *value = (Cwb_Serialize_Value*)
			cwb_ds_get(ds,pair);
		const char *key = (const char*)cwb_ds_getkey(ds,pair);
		char *copy = cwb_util_str_copy(cwb_serialize_get(value).string);
		if (!copy)
			return NULL;

		if (!cwb_ds_insert(result,key,(void*)copy))
			return NULL;
	}

	cwb_serialize_destroy(cookie);
	
	return result;
}
