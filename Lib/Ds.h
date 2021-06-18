/*
	cwb
	File:/Lib/Ds.h
	Date:2021.05.02
	By MIT and Anti-996 License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/
#ifndef CWB_DS_H_INC
#define CWB_DS_H_INC

#include<stdint.h>

typedef void Cwb_Ds;

typedef enum {
	CWB_DS_DICTIONARY
}Cwb_Ds_Type;

#define CWB_DS_SKEY ((int)0)
#define CWB_DS_IKEY ((int)1)

extern int ___cwb___ds___dummy___undefined___;
#define CWB_DS_UNDEFINED ((void*)(&___cwb___ds___dummy___undefined___))

Cwb_Ds *cwb_ds_new(Cwb_Ds_Type type,...);
void cwb_ds_destroy(Cwb_Ds *in);
int cwb_ds_set(Cwb_Ds *in,...);
void *cwb_ds_get(Cwb_Ds *in,...);

#endif
