/*
	cwb
	File:/src/Dstr.c
	Date:2021.08.02
	By MIT License.
	Copyright (c) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdint.h>

#include"cwb/Conf.h"
#include"cwb/Dstr.h"

typedef struct __Cwb_Dstr_Part Dstr_Part;

#define create_part() ((Dstr_Part*)malloc(sizeof(Dstr_Part)))

/*
 *	NOTE:
 *		The members that describe lengths or sizes 
 *		in the structures _DO_ _NOT_ include null-character
 */

static Dstr_Part *create_guard(void)
{
	Dstr_Part *part = create_part();
	if (!part)
		return NULL;
	*part = (Dstr_Part) {
				.length = 0,
				.used   = 0,
				.next   = NULL,
				.partStr= malloc(1)
			    };
	if (!part->partStr)
		return NULL;
	*part->partStr = '\0';

	return part;
}

Cwb_Dstr *cwb_dstr_new(void)
{
	Cwb_Dstr *dstr = (Cwb_Dstr*)malloc(sizeof(Cwb_Dstr));
	if (!dstr) 
		return NULL;
	
	*dstr = (Cwb_Dstr) {
				.length = 0,
				.part   = create_guard()
			   };
	dstr->last = dstr->part;
	
	return dstr->part ? dstr : NULL;
}

void cwb_dstr_destroy(Cwb_Dstr *dstr)
{
	Dstr_Part *last = dstr->part;

	for (Dstr_Part *part = last->next;
	     part;
	     part = part->next) {
		free(last->partStr);
		free(last);
		last = part;
	}
	free(last->partStr);
	free(last);

	free(dstr);

	return;
}

Cwb_Dstr *cwb_dstr_clear(Cwb_Dstr *dstr)
{
	Dstr_Part *last = dstr->part;
	for (Dstr_Part *part = last->next;
	     part;
	     part = part->next) {
		free(last->partStr);
		free(last);
		last = part;
	}
	if (last) {
		free(last->partStr);
		free(last);
	}

	dstr->part	= create_guard(); 
	if (!dstr->part)
		return NULL;
	dstr->last	= dstr->part;
	dstr->length	= 0;

	return dstr;
}

char *cwb_dstr_convert(Cwb_Dstr *dstr,char *buffer,size_t length)
{
	if (buffer) {
		if (length < dstr->length)
			return NULL;
	} else {
		buffer = (char*)malloc(dstr->length+1);
		if (!buffer)
			return NULL;
	}

	dstr->last->partStr[dstr->last->used] = '\0';

	*buffer = '\0';
	for (Dstr_Part *part = dstr->part->next;
	     part;
	     part = part->next)
		strcat(buffer,part->partStr);
	     
	return buffer;
}

static int extend_part(Cwb_Dstr *dstr)
{
	Dstr_Part *part = create_part();

	if (part) {
		*part = (Dstr_Part) {
					.length = CWB_CONF_DSTR_PARTSIZE,
					.used   = 0,
					.partStr= (char*)malloc(CWB_CONF_DSTR_PARTSIZE+1),
					.next   = NULL
				    };
		if (!part->partStr)
			return -1;
		*part->partStr   = '\0';

		dstr->last->next = part;
		dstr->last	 = part;
	}
	
	return !part;
}

Cwb_Dstr *cwb_dstr_appends(Cwb_Dstr *dstr,const char *src)
{
	size_t srcLength = strlen(src);

	Dstr_Part *part = dstr->last;

	if (part->used + srcLength > part->length) {
		if (extend_part(dstr))
			return NULL;
		part = dstr->last;
	}

	strcat(part->partStr,src);
	part->used  += srcLength;
	dstr->length+= srcLength;

	return dstr;
}

Cwb_Dstr *cwb_dstr_appendc(Cwb_Dstr *dstr,char c)
{
	Dstr_Part *part = dstr->last;

	if (part->used + 1 >part->length) {
		if (extend_part(dstr))
			return NULL;
		part = dstr->last;
	}

	part->partStr[part->used] = c;
	part->used++;
	part->partStr[part->used] = '\0';
	dstr->length++;
	
	return dstr;
}

Cwb_Dstr *cwb_dstr_assignd(Cwb_Dstr *dstr,char **p)
{
	cwb_dstr_clear(dstr);

	Dstr_Part *part = create_part();
	if (!part)
		return NULL;

	size_t length = strlen(*p);
	*part = (Dstr_Part) {
				.length = length,
				.used	= length,
				.next	= NULL,
				.partStr= *p
			    };
	dstr->length = length;

	dstr->part->next = part;
	dstr->last	 = part;

	*p = NULL;

	return dstr;
}

static char *copy_str(const char *src)
{
	char *copy = (char*)malloc(strlen(src)+1);

	if (!copy)
		return NULL;

	strcpy(copy,src);
	return copy;
}

Cwb_Dstr *cwb_dstr_assign(Cwb_Dstr *dstr,const char *src)
{
	char *copy = copy_str(src);
	if (!copy)
		return NULL;
	
	return cwb_dstr_assignd(dstr,&copy);
}
