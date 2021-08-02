/*
	cwb
	File:/src/Serializer.c
	Date:2021.08.02
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

Cwb_Serialize_Value *cwb_serialize_new(Cwb_Serialize_Type type)
{
	Cwb_Serialize_Value *value =
		(Cwb_Serialize_Value*)malloc(sizeof(Cwb_Serialize_Value));
	if (!value)
		return NULL;
	value->type = type;
	
	return value;
}

static void free_ds(void *data)
{
	cwb_serialize_destroy((Cwb_Serialize_Value*)data);
	return;
}

void cwb_serialize_destroy(Cwb_Serialize_Value *value)
{
	if (value->type == CWB_SERIALIZE_STRING) {
		if (value->value.string)
			free(value->value.string);
	} else if (value->type == CWB_SERIALIZE_DS) {
		if (value->value.ds) {
			cwb_ds_freefunc(value->value.ds,free_ds);
			cwb_ds_destroy(value->value.ds);
		}
	}

	free(value);

	return;
}
