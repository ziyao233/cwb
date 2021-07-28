/*
	cwb
	File:/src/Httpd_Request.c
	Date:2021.07.28
	By MIT License.
	Copyright (c) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cwb/Dstr.h"
#include"cwb/Ds.h"
#include"cwb/Httpd.h"

Cwb_Ds *cwb_httpd_req_header(Cwb_Httpd_Conn *conn)
{
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
	
	return header;
}
