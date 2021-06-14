/*
	cwb
	File:/Lib/Dstr.h
	Date:2021.06.14
	By LGPL v3.0 and Anti-996 License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef CWB_DSTR_H_INC
#define CWB_DSTR_H_INC

#include<stdlib.h>
#include<stdint.h>

struct __Cwb_Dstr_Part {
	size_t used;
	size_t length;
	char *partStr;
	struct __Cwb_Dstr_Part *next;
};

typedef struct {
	size_t length;
	struct __Cwb_Dstr_Part *part;
	struct __Cwb_Dstr_Part *last;
}Cwb_Dstr;

Cwb_Dstr *cwb_dstr_new(void);
void cwb_dstr_destroy(Cwb_Dstr *dstr);

Cwb_Dstr *cwb_dstr_assignd(Cwb_Dstr *dstr,char **p);
Cwb_Dstr *cwb_dstr_assign(Cwb_Dstr *dstr,const char *src);

char *cwb_dstr_convert(Cwb_Dstr *dstr,char *buffer,size_t length);

Cwb_Dstr *cwb_dstr_copy(Cwb_Dstr *dstr);

Cwb_Dstr *cwb_dstr_appendd(Cwb_Dstr *dstr,char **p);
Cwb_Dstr *cwb_dstr_appendc(Cwb_Dstr *dstr,char c);
Cwb_Dstr *cwb_dstr_appends(Cwb_Dstr *dstr,const char *src);

#endif
