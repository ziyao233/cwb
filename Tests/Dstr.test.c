/*
	cwb
	File:/Tests/Dstr.test.c
	Date:2021.05.04
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

	Cwb_Dstr *s = cwb_dstr_new();
	assert(s);
	
	assert(!cwb_dstr_assign(s,temp));

	cwb_dstr_destroy(s);

	return 0;
}
