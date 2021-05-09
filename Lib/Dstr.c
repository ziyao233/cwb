/*
	cwb
	File:/Lib/Dstr.c
	Date:2021.05.09
	By LGPL v3.0 and Anti-996 License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdint.h>

#include"Conf.h"
#include"Dstr.h"

typedef struct __Cwb_Dstr_Part Dstr_Part;

#define create_part() ((Dstr_Part*)malloc(sizeof(Dstr_Part)))

Cwb_Dstr *cwb_dstr_new(void)
{
	Cwb_Dstr *dstr = malloc(sizeof(Cwb_Dstr));
	if (!dstr)
		return NULL;
	
	*dstr = (Cwb_Dstr){
				.length=0,
				.part=NULL,
				.last=NULL
			  };

	return dstr;
}

void cwb_dstr_clear(Cwb_Dstr *dstr)
{
	Dstr_Part *last=(Dstr_Part*)(dstr->part);

	for (Dstr_Part *part = last ? last->next : NULL;
	     part;
	     part = part->next) {
		free(last->partStr);
		free(last);
		last=part;
	}

	if (last) {
		free(last->partStr);
		free(last);
	}

	dstr->length = 0;
	dstr->part   = NULL;

	return;
}

void cwb_dstr_destroy(Cwb_Dstr *dstr)
{
	cwb_dstr_clear(dstr);

	free(dstr);

	return;
}

Cwb_Dstr *cwb_dstr_assignd(Cwb_Dstr *dstr,char **p)
{
	cwb_dstr_clear(dstr);

	dstr->part = create_part();
	if (!(dstr->part)) {
		return NULL;
	}
	
	char *src = *p;
	size_t length = strlen(src);
	*(dstr->part) = (Dstr_Part) {
					.partStr = src,
					.length	 = length,
					.used	 = length,
					.next	 = NULL
				    };
	*p = NULL;
	dstr->last = dstr->part;
	return dstr;
}

static char *copy_str(const char *src)
{
	char *result = (char*)malloc(strlen(src));
	if (result)
		strcpy(result,src);
	return result;
}

Cwb_Dstr *cwb_dstr_assign(Cwb_Dstr *dstr,const char *src)
{
	char *copy = copy_str(src);
	if (!copy)
		return NULL;
	
	return cwb_dstr_assignd(dstr,&copy);
}

char *cwb_dstr_convert(Cwb_Dstr *dstr,char *buffer,size_t length)
{
	if (buffer) {
		if (dstr->length+1 > length) {
			return NULL;
		}
	} else {
		buffer = (char*)malloc(dstr->length+1);
		if (!buffer) {
			return NULL;
		}
	}

	*buffer='\0';	// For strcat().It needs a string ends with '\0'
	for (Dstr_Part *part=dstr->part;
	     part;
	     part = part->next) 
		strcat(buffer,part->partStr);

	return buffer;
}

static int extend_part(Cwb_Dstr *dstr)
{
	Dstr_Part *part = create_part();
	if (part) {
		char *partStr = (char*)malloc(CWB_CONF_DSTR_PARTSIZE+1);
		if (partStr) {
			dstr->last->next   = part;
			dstr->last	   = part;
			*part = (Dstr_Part) {
					     .length = CWB_CONF_DSTR_PARTSIZE,
					     .next   = NULL,
					     .used   = 0,
					     .partStr= partStr,
					    };
			*partStr = '\0';
		} else {
			free(part);
			part = NULL;
		}
	}
	return !part;
}

Cwb_Dstr *cwb_dstr_appendc(Cwb_Dstr *dstr,char c)
{
	if (dstr->last->used == dstr->last->length) {
		if (extend_part(dstr))
			return NULL;
	}

	dstr->last->partStr[dstr->last->used]=c;
	dstr->last->used++;
	dstr->length++;
	return dstr;
}

Cwb_Dstr *cwb_dstr_copy(Cwb_Dstr *dstr)
{
	Cwb_Dstr *copy = cwb_dstr_new();
	if (!copy)
		goto endCreateStr;
	
	char *nativeStr = cwb_dstr_convert(dstr,NULL,0);

	if (!nativeStr)
		goto endGetNativeStr;
	
	if (!cwb_dstr_assignd(copy,&nativeStr))
		goto endAssign;

	return copy;

endAssign:
	free(nativeStr);
endGetNativeStr:
	cwb_dstr_destroy(copy);
endCreateStr:
	return NULL;
}

Cwb_Dstr *cwb_dstr_appendd(Cwb_Dstr *dstr,char **p)
{
	char *src = *p;
	size_t length = strlen(src);

	dstr->last->next = create_part();
	if (!(dstr->last->next)) 
		return NULL;
	
	dstr->last = dstr->last->next;
	*(dstr->last) = (Dstr_Part) {
					.next	= NULL,
					.partStr= src,
					.length = length,
					.used	= length
				    };
	
	dstr->length += length;
	
	*p = NULL;
	
	return dstr;
}

Cwb_Dstr *cwb_dstr_appends(Cwb_Dstr *dstr,const char *src)
{
	size_t length = strlen(src);
	if (dstr->last->used+length > dstr->last->length) {
		if (extend_part(dstr)) {
			return NULL;
		}
	}

	strcat(dstr->last->partStr,src);
	dstr->length += length;

	return dstr;
}
