/*
	cwb
	File:/src/Ds.c
	Date:2021.10.24
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<stdint.h>

#include"cwb/Ds.h"

#include"Ds_HashTable.c"

typedef struct {
	void *realDs;
	Cwb_Ds_Type type;
}Ds;

typedef struct {
	void *(*new)(va_list argList);
	void (*destroy)(void *ds);
	Cwb_Ds_Iter *(*get)(void *ds,va_list argList);
	Cwb_Ds_Iter *(*insert)(void *ds,va_list);
	int (*set)(void *ds,Cwb_Ds_Iter *iter,void *data);
	void *(*value)(void *ds,Cwb_Ds_Iter *iter);
	int (*delete)(void *ds,Cwb_Ds_Iter *iter);
	Cwb_Ds_Iter *(*first)(void *ds);
	Cwb_Ds_Iter *(*next)(void *ds,Cwb_Ds_Iter *iter);
	void (*freefunc)(void *ds,Cwb_Ds_FreeFunc func);
	intptr_t (*key)(void *ds,Cwb_Ds_Iter *iter);
}Ds_Prototype;

static const Ds_Prototype prototype[]={
					[CWB_DS_HASHTABLE] = {
						.new		= hashtable_new,
						.destroy	= hashtable_destroy,
						.get		= hashtable_get,
						.insert		= hashtable_insert,
						.set		= hashtable_set,
						.value		= hashtable_value,
						.delete		= hashtable_delete,
						.first		= hashtable_first,
						.next		= hashtable_next,
						.freefunc	= hashtable_freefunc,
						.key		= hashtable_key
					}
				      };

#define DS_PROTOTYPE(t) (prototype[(t)])

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
	free(in);
	return;
}

Cwb_Ds_Iter *cwb_ds_get(Cwb_Ds *in,...)
{
	va_list argList;
	va_start(argList,in);
	Ds *ds = (Ds*)in;

	Cwb_Ds_Iter *iter = DS_PROTOTYPE(ds->type).get(ds->realDs,argList);
	va_end(argList);

	return iter;
}

int cwb_ds_set(Cwb_Ds *in,Cwb_Ds_Iter *iter,void *data)
{
	Ds *ds = (Ds*)in;
	
	int ret = DS_PROTOTYPE(ds->type).set(ds->realDs,iter,data);
	return ret;
}

void *cwb_ds_value(Cwb_Ds *in,Cwb_Ds_Iter *iter)
{
	Ds *ds = (Ds*)in;

	void *value = DS_PROTOTYPE(ds->type).value(ds->realDs,iter);

	return value;
}

int cwb_ds_delete(Cwb_Ds *in,Cwb_Ds_Iter *iter)
{
	Ds *ds = (Ds*)in;

	return DS_PROTOTYPE(ds->type).delete(ds->realDs,iter);
}

Cwb_Ds_Iter *cwb_ds_insert(Cwb_Ds *in,...)
{
	Ds *ds = (Ds*)in;

	va_list argList;
	va_start(argList,in);

	Cwb_Ds_Iter *iter = DS_PROTOTYPE(ds->type).insert(ds->realDs,argList);
	
	va_end(argList);

	return iter;
}

Cwb_Ds_Iter *cwb_ds_first(Cwb_Ds *in)
{
	Ds *ds = (Ds*)in;

	return DS_PROTOTYPE(ds->type).first(ds->realDs);
}

Cwb_Ds_Iter *cwb_ds_next(Cwb_Ds *in,Cwb_Ds_Iter *iter)
{
	Ds *ds = (Ds*)in;
	
	return DS_PROTOTYPE(ds->type).next(ds->realDs,iter);
}

void cwb_ds_freefunc(Cwb_Ds *in,Cwb_Ds_FreeFunc freeFunc)
{
	Ds *ds = (Ds*)in;

	DS_PROTOTYPE(ds->type).freefunc(ds->realDs,freeFunc);
	return;
}

intptr_t cwb_ds_key(Cwb_Ds *in,Cwb_Ds_Iter *iter)
{
	Ds *ds = (Ds*)in;

	return DS_PROTOTYPE(ds->type).key(ds->realDs,iter);
}
