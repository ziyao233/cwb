/*
	cwb
	File:/src/Serializer.c
	Date:2021.07.20
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cwb/Ds.h"
#include"cwb/Util.h"
#include"cwb/Dstr.h"
#include"cwb/Buffer.h"
#include"cwb/Serializer.h"

#define create_dsdata() ((Ds_Data*)malloc(sizeof(Ds_Data)))

typedef struct {
	char *string;
}Value;

typedef struct {
	Cwb_Serialize_Type type;
	Value value;
}Ds_Data;

void data_free(void *data)
{
	Ds_Data *dsData = (Ds_Data*)data;
	if (dsData->type == CWB_SERIALIZE_STRING)
		free(dsData->value.string);
	
	free(data);
	return;
}

Cwb_Serialize_Data *cwb_serialize_new(void)
{
	Cwb_Serialize_Data *data = (Cwb_Serialize_Data*)
		cwb_ds_new(CWB_DS_HASHTABLE,CWB_DS_SKEY);
	if (!data)
		return NULL;
	
	cwb_ds_freefunc((Cwb_Ds*)data,(Cwb_Ds_FreeFunc)data_free);
	
	return data;
}

void cwb_serialize_destroy(Cwb_Serialize_Data *data)
{
	cwb_ds_destroy((Cwb_Ds*)data);
	return;
}

Cwb_Serialize_Value *cwb_serialize_search(Cwb_Serialize_Data *data,
					  char const *key)
{
	return (Cwb_Serialize_Value*)cwb_ds_search((Cwb_Ds*)data,key);
}

char const *cwb_serialize_getkey(Cwb_Serialize_Data *data,
				 Cwb_Serialize_Value *value)
{
	return (char const*)cwb_ds_getkey((Cwb_Ds*)data,(Cwb_Ds_Pair*)value);
}

Cwb_Serialize_Value *cwb_serialize_adds(Cwb_Serialize_Data *data,
					char const *key,
					char const *str)
{
	char *strCopy = cwb_util_str_copy(str);
	if (!strCopy)
		return NULL;
	
	Ds_Data *dsData = create_dsdata();
	if (!dsData)
		return NULL;
	
	dsData->type		= CWB_SERIALIZE_STRING;
	dsData->value.string	= strCopy;

	return (Cwb_Serialize_Value*)
		cwb_ds_insert((Cwb_Ds*)data,key,(void*)dsData);
}

Cwb_Serialize_Value *cwb_serialize_first(Cwb_Serialize_Data *data)
{
	return (Cwb_Serialize_Value*)cwb_ds_first((Cwb_Ds*)data);
}

Cwb_Serialize_Value *cwb_serialize_next(Cwb_Serialize_Data *data,
					Cwb_Serialize_Value *value)
{
	return (Cwb_Serialize_Value*)
		cwb_ds_next((Cwb_Ds*)data,(Cwb_Ds_Pair*)value);
}

char const *cwb_serialize_gets(Cwb_Serialize_Data *data,
			       Cwb_Serialize_Value *value)
{
	return ((Ds_Data*)cwb_ds_get((Cwb_Ds*)data,(Cwb_Ds_Pair*)value))
		->value.string;
	
}

Cwb_Serialize_Type cwb_serialize_gettype(Cwb_Serialize_Data *data,
					 Cwb_Serialize_Value *value)
{
	return ((Ds_Data*)cwb_ds_get((Cwb_Ds*)data,(Cwb_Ds_Pair*)value))
		->type;
}
