/*
	cwb
	File:/src/cwb/Serailizer.h
	Date:2021.08.02
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef __CWB_SERIALIZER_H_INC
#define __CWB_SERIALIZER_H_INC

#include<stdint.h>

#include"cwb/Dstr.h"
#include"cwb/Ds.h"

typedef enum {
	CWB_SERIALIZE_INTEGER,		// int
	CWB_SERIALIZE_STRING,		// char *
	CWB_SERIALIZE_DS		// Cwb_Ds *
}Cwb_Serialize_Type;

typedef struct {
	Cwb_Serialize_Type type;
	union {
		char	*string;
		int	*integer;
		Cwb_Ds	*ds;
	}value;
}Cwb_Serialize_Value;

/*	Basic		*/
Cwb_Serialize_Value *cwb_serialize_new(Cwb_Serialize_Type type);
void cwb_serialize_destroy(Cwb_Serialize_Value *value);

#define cwb_serialize_get(p) ((p)->value)
#define cwb_serialize_set(p) ((p)->value)
#define cwb_serialize_type(p) ((p)->type)

/*	Urlcoded	*/
Cwb_Dstr *cwb_serialize_urlcoded(Cwb_Dstr *buff,Cwb_Serialize_Value *value);
Cwb_Serialize_Value *cwb_deserialize_urlcoded(Cwb_Serialize_Value *value,char const *code);

#endif
