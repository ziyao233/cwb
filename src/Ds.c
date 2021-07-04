/*
	cwb
	File:/Lib/Ds.c
	Date:2021.05.02
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>

#include"Ds.h"

#include"Ds_Dictionary.c"

typedef struct {
	void *realDs;
	Cwb_Ds_Type type;
}Ds;

typedef struct {
	void *(*new)(va_list argList);
	void (*destroy)(void *ds);
	int (*set)(void *ds,va_list argList);
	void *(*get)(void *ds,va_list argList);
}Ds_Prototype;

static const Ds_Prototype prototype[]={
					[CWB_DS_DICTIONARY]={
						.new=dictionary_new,
						.destroy=dictionary_destroy,
						.set=dictionary_set,
						.get=dictionary_get
					}
				      };

#define DS_PROTOTYPE(t) (prototype[(t)])

int ___cwb___ds___dummy___undefined___;
			

Cwb_Ds *cwb_ds_new(Cwb_Ds_Type type,...)
{
	va_list argList;
	va_start(argList,type);

	Ds *ds=(Ds*)malloc(sizeof(Ds));
	if(!ds) {
		return NULL;
	}

	ds->type=type;

	ds->realDs=DS_PROTOTYPE(type).new(argList);
	va_end(argList);
	if(!(ds->realDs)) {
		free(ds);
		return NULL;
	}

	return (Cwb_Ds*)ds;
}

void cwb_ds_destroy(Cwb_Ds *in)
{
	Ds *ds = (Ds*)in;
	DS_PROTOTYPE(ds->type).destroy(ds->realDs);
	return;
}

int cwb_ds_set(Cwb_Ds *in,...)
{
	va_list argList;
	va_start(argList,in);
	Ds *ds = (Ds*)in;
	
	int ret = DS_PROTOTYPE(ds->type).set(ds->realDs,argList);
	va_end(argList);
	return ret;
}

void *cwb_ds_get(Cwb_Ds *in,...)
{
	va_list argList;
	va_start(argList,in);
	Ds *ds = (Ds*)in;

	void *ret = DS_PROTOTYPE(ds->type).get(ds->realDs,argList);
	va_end(argList);
	return ret;
}
