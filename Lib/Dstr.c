/*
	cwb
	File:/Lib/Dstr.c
	Date:2021.06.14
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

	*buffer = '\0';
	for (Dstr_Part *part = dstr->part;
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

Cwb_Dstr *cwb_dstr_appends(Cwb_Dstr *dstr,char const *src)
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
