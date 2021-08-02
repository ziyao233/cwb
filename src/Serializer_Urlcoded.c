/*
	cwb
	File:/src/Serailize_Urlcoded.c
	Date:2021.08.02
	By MIT License.
	Copyright (c) 2021 cwb developers.All rights reserved.
*/

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cwb/Dstr.h"
#include"cwb/Buffer.h"
#include"cwb/Coder.h"
#include"cwb/Serializer.h"

#define HEX2NUM(ch) ((ch)>57?(ch)-55:(ch)-48)

Cwb_Dstr *cwb_serialize_urlcoded(Cwb_Dstr *output,Cwb_Serialize_Value *value)
{
	if (!output) {
		output = cwb_dstr_new();
		if (!output)
			return NULL;
	}

	assert(cwb_serialize_type(value) == CWB_SERIALIZE_DS);
	Cwb_Ds *ds = cwb_serialize_get(value).ds;
	Cwb_Ds_Pair *tmp = cwb_ds_first(ds);

	for (Cwb_Ds_Pair *pair = (Cwb_Ds_Pair*)cwb_ds_first(ds);
	     pair;
	     pair = tmp) {
		const char *key = (const char*)cwb_ds_getkey(ds,pair);
		if (!cwb_encode_uri(output,key,strlen(key)))
			return NULL;
		if (!cwb_dstr_appendc(output,'='))
			return NULL;
		
		Cwb_Serialize_Value *value = cwb_ds_get(ds,pair);
		Cwb_Serialize_Type type = cwb_serialize_type(value);
		if (type == CWB_SERIALIZE_STRING) {
			const char *src = cwb_serialize_get(value).string;
			if (!cwb_encode_uri(output,src,strlen(src)))
				return NULL;
		} else {
			abort();
		}

		tmp = cwb_ds_next(ds,tmp);
		if (tmp) {
			if (!cwb_dstr_appendc(output,'&'))
				return NULL;
		}
	}

	return output;
}

Cwb_Serialize_Value *cwb_deserialize_urlcoded(Cwb_Serialize_Value *value,
					      const char *code)
{
	if (!value) {
		value = cwb_serialize_new(CWB_SERIALIZE_DS);
		if (!value)
			return NULL;
		cwb_serialize_set(value).ds =
			cwb_ds_new(CWB_DS_HASHTABLE,CWB_DS_SKEY);
		if (!cwb_serialize_get(value).ds)
			return NULL;
	}

	Cwb_Dstr *str = cwb_dstr_new();
	if (!str)
		return NULL;

	for (const char *p = code;*p;) {
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
		char *v = cwb_dstr_convert(str,NULL,0);
		Cwb_Serialize_Value *term = cwb_serialize_new(CWB_SERIALIZE_STRING);
		if (!term)
			return NULL;
		cwb_serialize_set(term).string = v;
		cwb_ds_insert(cwb_serialize_get(value).ds,key,(void*)term);
		free(key);
		cwb_dstr_clear(str);

		p = *p ? p + 1 : p;
	}
	cwb_dstr_destroy(str);

	return value;
}
