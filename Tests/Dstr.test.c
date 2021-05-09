/*
	cwb
	File:/Tests/Dstr.test.c
	Date:2021.05.09
	By LGPL v3.0 and Anti-996 License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<cwb/Dstr.h>

int main(void)
{
	char temp[1024];
	fgets(temp,1024,stdin);

	temp[strlen(temp)-1]='\0';

	Cwb_Dstr *dstr = cwb_dstr_new();
	assert(dstr);
	
	assert(cwb_dstr_assign(dstr,temp));
	Cwb_Dstr *copy = cwb_dstr_copy(dstr);

	char *s = cwb_dstr_convert(dstr,NULL,0);
	puts(s);
	char *t = strdup(s);
	cwb_dstr_appendd(dstr,&t);
	assert(!t);
	cwb_dstr_appendc(dstr,':');
	cwb_dstr_appendc(dstr,')');
	free(s);
	
	s = cwb_dstr_convert(dstr,NULL,0);
	puts(s);

	cwb_dstr_destroy(dstr);
	free(s);

	cwb_dstr_appends(copy,":)");
	s = cwb_dstr_convert(copy,NULL,0);
	puts(s);
	free(s);

	return 0;
}
