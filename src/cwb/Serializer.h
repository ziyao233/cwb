/*
	cwb
	File:/src/cwb/Serailizer.h
	Date:2021.07.20
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef __CWB_SERIALIZER_H_INC
#define __CWB_SERIALIZER_H_INC

#include<stdint.h>

#include"cwb/Dstr.h"
#include"cwb/Ds.h"

typedef Cwb_Ds Cwb_Serialize_Data;
typedef Cwb_Ds_Pair Cwb_Serialize_Value;

typedef enum {
	CWB_SERIALIZE_INTEGER,		// int
	CWB_SERIALIZE_STRING		// char *
}Cwb_Serialize_Type;

/*	Basic functions	*/
Cwb_Serialize_Data *cwb_serialize_new(void);
void cwb_serialize_destroy(Cwb_Serialize_Data *data);
Cwb_Serialize_Value *cwb_serialize_search(Cwb_Serialize_Data *data,
					  char const *key);
char const *cwb_serialize_getkey(Cwb_Serialize_Data *data,
				 Cwb_Serialize_Value *value);
Cwb_Serialize_Value *cwb_serialize_adds(Cwb_Serialize_Data *data,
					char const *key,
				        char const *str);
Cwb_Serialize_Value *cwb_serialize_first(Cwb_Serialize_Data *data);
Cwb_Serialize_Value *cwb_serialize_next(Cwb_Serialize_Data *data,
				        Cwb_Serialize_Value *pair);
char const *cwb_serialize_gets(Cwb_Serialize_Data *data,
			       Cwb_Serialize_Value *value);
Cwb_Serialize_Type cwb_serialize_gettype(Cwb_Serialize_Data *data,
					 Cwb_Serialize_Value *value);

/*	Urlcoded	*/
Cwb_Dstr *cwb_serialize_urlcoded(Cwb_Dstr *buff,Cwb_Serialize_Data *data);
Cwb_Serialize_Data *cwb_deserialize_urlcoded(Cwb_Serialize_Data *data,char const *code);

#endif
