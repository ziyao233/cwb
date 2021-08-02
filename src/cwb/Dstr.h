/*
	cwb
	File:/src/cwb/Dstr.h
	Date:2021.08.02
	By MIT License.
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

#define cwb_dstr_length(dstr) ((dstr)->length)

Cwb_Dstr *cwb_dstr_new(void);
void cwb_dstr_destroy(Cwb_Dstr *dstr);

Cwb_Dstr *cwb_dstr_clear(Cwb_Dstr *dstr);

Cwb_Dstr *cwb_dstr_assignd(Cwb_Dstr *dstr,char **p);
Cwb_Dstr *cwb_dstr_assign(Cwb_Dstr *dstr,const char *src);

char *cwb_dstr_convert(Cwb_Dstr *dstr,char *buffer,size_t length);

Cwb_Dstr *cwb_dstr_copy(Cwb_Dstr *dstr);

Cwb_Dstr *cwb_dstr_appendc(Cwb_Dstr *dstr,char c);
Cwb_Dstr *cwb_dstr_appends(Cwb_Dstr *dstr,const char *src);
Cwb_Dstr *cwb_dstr_appendd(Cwb_Dstr *dstr,char **p);
Cwb_Dstr *cwb_dstr_appendy(Cwb_Dstr *str1,Cwb_Dstr *str2);

#endif
