/*
	cwb
	File:/Lib/Dstr.c
	Date:2021.05.04
	By LGPL v3.0 and Anti-996 License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdint.h>

#include"Dstr.h"

typedef struct Dstr_Part {
	size_t used;
	size_t length;
	char *partStr;
	struct Dstr_Part *next;
}Dstr_Part;

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

int cwb_dstr_assign(Cwb_Dstr *dstr,const char *src)
{
	cwb_dstr_clear(dstr);

	size_t length = strlen(src);

	Dstr_Part *part = create_part();
	if (!part) 
		return -1;
	
	*part = (Dstr_Part){
				.length = length+1,
				.used	= length+1,
				.partStr= (char*)malloc(length+1),
				.next	= NULL
			   };
	
	if (!(part->partStr)) {
		free(part);
		return -1;
	}

	strcpy(part->partStr,src);

	*dstr = (Cwb_Dstr){
				.part	= (void*)part,
				.last	= (void*)part,
				.length = length
			   };

	return 0;
}
