/*
	cwb
	File:/test/Serailize.test.c
	Date:2021.07.20
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cwb/Dstr.h"
#include"cwb/Serializer.h"

int main(void)
{
	Cwb_Serialize_Data *data = cwb_serialize_new();
	assert(data);

	assert(cwb_serialize_adds(data,"q","How to build a website"));
	assert(cwb_serialize_adds(data,"user","foo"));

	Cwb_Dstr *coded = cwb_serialize_urlcoded(NULL,data);
	assert(coded);
	char *result = cwb_dstr_convert(coded,NULL,0);
	assert(result);
	puts(result);
	cwb_dstr_destroy(coded);

	cwb_serialize_destroy(data);
	data = cwb_deserialize_urlcoded(NULL,result);

	for (Cwb_Serialize_Value *value = cwb_serialize_first(data);
	     value;
	     value = cwb_serialize_next(data,value)) {
		printf("%s:%s\n",cwb_serialize_getkey(data,value),
				 cwb_serialize_gets(data,value));
	}

	cwb_serialize_destroy(data);
	
	return 0;
}
