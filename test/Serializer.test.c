/*
	cwb
	File:/test/Serailize.test.c
	Date:2021.10.24
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cwb/Util.h"
#include"cwb/Dstr.h"
#include"cwb/Serializer.h"

int main(void)
{
	Cwb_Serialize_Value *value = cwb_serialize_new(CWB_SERIALIZE_DS);
	assert(value);

	Cwb_Ds *ds = cwb_ds_new(CWB_DS_HASHTABLE,CWB_DS_SKEY);
	assert(ds);
	cwb_serialize_set(value).ds = ds;

	char *tmp = cwb_util_str_copy("How to build a website");
	assert(tmp);
	Cwb_Serialize_Value *term = cwb_serialize_new(CWB_SERIALIZE_STRING);
	assert(term);
	cwb_serialize_set(term).string = tmp;
	assert(cwb_ds_insert(ds,"q",term));
	tmp = cwb_util_str_copy("foo");
	term = cwb_serialize_new(CWB_SERIALIZE_STRING);
	assert(tmp);
	assert(term);
	cwb_serialize_set(term).string = tmp;
	assert(cwb_ds_insert(ds,"user",term));

	Cwb_Dstr *coded = cwb_serialize_urlcoded(NULL,value);
	assert(coded);
	char *result = cwb_dstr_convert(coded,NULL,0);
	assert(result);
	puts(result);
	cwb_dstr_destroy(coded);

	cwb_serialize_destroy(value);
	value = cwb_deserialize_urlcoded(NULL,result);

	ds = cwb_serialize_get(value).ds;
	for (Cwb_Ds_Iter *iter = cwb_ds_first(ds);
	     iter;
	     iter = cwb_ds_next(ds,iter)) {
		Cwb_Serialize_Value *tmp = 
			(Cwb_Serialize_Value*)cwb_ds_get(ds,iter);
		char *data = cwb_serialize_get(tmp).string;
		printf("%s:%s\n",(const char*)cwb_ds_getkey(ds,iter),
				 data);
	}

	free(result);
	cwb_serialize_destroy(value);
	
	return 0;
}
