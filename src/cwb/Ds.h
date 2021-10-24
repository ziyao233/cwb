/*
	cwb
	File:/src/cwb/Ds.h
	Date:2021.10.24
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/
#ifndef CWB_DS_H_INC
#define CWB_DS_H_INC

#include<stdint.h>

typedef void* Cwb_Ds;
typedef void* Cwb_Ds_Iterator;
typedef Cwb_Ds_Iterator Cwb_Ds_Iter;

typedef void (*Cwb_Ds_FreeFunc)(void *data);

typedef enum {
	CWB_DS_HASHTABLE
}Cwb_Ds_Type;

#define CWB_DS_SKEY ((int)0)
#define CWB_DS_IKEY ((int)1)

extern int ___cwb___ds___dummy___undefined___;
#define CWB_DS_UNDEFINED ((void*)(&___cwb___ds___dummy___undefined___))

Cwb_Ds *cwb_ds_new(Cwb_Ds_Type type,...);
void cwb_ds_destroy(Cwb_Ds *in);
Cwb_Ds_Iter *cwb_ds_get(Cwb_Ds *in,...);
int cwb_ds_set(Cwb_Ds *in,Cwb_Ds_Iter *iter,void *data);
void *cwb_ds_value(Cwb_Ds *in,Cwb_Ds_Iter *iter);
Cwb_Ds_Iter *cwb_ds_insert(Cwb_Ds *in,...);
int cwb_ds_delete(Cwb_Ds *in,Cwb_Ds_Iter *iter);
Cwb_Ds_Iter *cwb_ds_first(Cwb_Ds *in);
Cwb_Ds_Iter *cwb_ds_next(Cwb_Ds *in,Cwb_Ds_Iter *iter);
void cwb_ds_freefunc(Cwb_Ds *in,Cwb_Ds_FreeFunc func);
intptr_t cwb_ds_key(Cwb_Ds *in,Cwb_Ds_Iter *iter);

#endif
