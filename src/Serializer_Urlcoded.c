/*
	cwb
	File:/src/Serailize_Urlcoded.c
	Date:2021.07.20
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cwb/Dstr.h"
#include"cwb/Buffer.h"
#include"cwb/Coder.h"
#include"cwb/Serializer.h"

#define HEX2NUM(ch) ((ch)>57?(ch)-55:(ch)-48)

Cwb_Dstr *cwb_serialize_urlcoded(Cwb_Dstr *output,Cwb_Serialize_Data *data)
{
	if (!output) {
		output = cwb_dstr_new();
		if (!output)
			return NULL;
	}

	Cwb_Serialize_Value *tmp = cwb_serialize_first(data);
	for (Cwb_Serialize_Value *value = cwb_serialize_first(data);
	     value;
	     value = tmp) {

		char const *key = cwb_serialize_getkey(data,value);
		if (!cwb_encode_uri(output,key,strlen(key)))
			return NULL;
		if (!cwb_dstr_appendc(output,'='))
			return NULL;
		
		Cwb_Serialize_Type type = cwb_serialize_gettype(data,value);
		if (type == CWB_SERIALIZE_STRING) {
			char const *src = cwb_serialize_gets(data,value);
			if (!cwb_encode_uri(output,src,strlen(src)))
				return NULL;
		} else {
			abort();
		}

		tmp = cwb_serialize_next(data,tmp);
		if (tmp) {
			if (!cwb_dstr_appendc(output,'&'))
				return NULL;
		}
	}

	return output;
}

Cwb_Serialize_Data *cwb_deserialize_urlcoded(Cwb_Serialize_Data *data,
					     char const *code)
{
	if (!data) {
		data = cwb_serialize_new();
		if (!data)
			return NULL;
	}

	Cwb_Dstr *str = cwb_dstr_new();
	if (!str)
		return NULL;

	for (char const *p = code;*p;) {
		while (*p != '=') {
			if (*p == '%') {
				char tmp = 0;
				tmp = (HEX2NUM(p[1]) << 4) +
				      (HEX2NUM(p[2]));
				if (!cwb_dstr_appendc(str,tmp))
					return NULL;
				p += 3;
			} else {
				cwb_dstr_appendc(str,*p);
				p++;
			}
		}
		p++;
		char *key = cwb_dstr_convert(str,NULL,0);

		cwb_dstr_clear(str);

		while (*p != '&' && *p) {
			if (*p == '%') {
				char tmp = 0;
				tmp = (HEX2NUM(p[1]) << 4) +
				      (HEX2NUM(p[2]));
				if (!cwb_dstr_appendc(str,tmp))
					return NULL;
				p += 3;
			} else {
				cwb_dstr_appendc(str,*p);
				p++;
			}
		}
		char *value = cwb_dstr_convert(str,NULL,0);
		cwb_serialize_adds(data,key,value);
		free(key);
		free(value);
		cwb_dstr_clear(str);

		p = *p ? p + 1 : p;
	}
	cwb_dstr_destroy(str);

	return data;
}
