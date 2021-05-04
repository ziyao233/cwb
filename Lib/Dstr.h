/*
	cwb
	File:/Lib/Dstr.h
	Date:2021.05.04
	By LGPL v3.0 and Anti-996 License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef CWB_DSTR_H_INC
#define CWB_DSTR_H_INC

#include<stdlib.h>
#include<stdint.h>

typedef struct {
	size_t length;
	void *part;
	void *last;
}Cwb_Dstr;

Cwb_Dstr *cwb_dstr_new(void);
void cwb_dstr_clear(Cwb_Dstr *dstr);
int cwb_dstr_assign(Cwb_Dstr *dstr,const char *src);
void cwb_dstr_destroy(Cwb_Dstr *dstr);

#endif
