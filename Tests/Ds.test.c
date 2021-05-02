/*
	Cwb
	File:/Tests/Ds.test.c
	Date:2021.05.02
	By LGPL v3.0 and Anti-996 License
	Copyright(C) 2021 Cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#include<cwb/Ds.h>

#include"Test.h"

int main(void)
{
	Cwb_Ds *dic=cwb_ds_new(CWB_DS_DICTIONARY,CWB_DS_SKEY);
	if (!dic) {
		err_log("Error:Create the data structure(CWB_DS_DICTIONARY)");
		return -1;
	}

	cwb_ds_set(dic,"I","Suote127");
	cwb_ds_set(dic,"you","User");
	cwb_ds_set(dic,"he","Unknow");

	puts(cwb_ds_get(dic,"he"));
	puts(cwb_ds_get(dic,"you"));
	puts(cwb_ds_get(dic,"I"));
	
	cwb_ds_set(dic,"I","The programmer");

	puts(cwb_ds_get(dic,"I"));

	cwb_ds_set(dic,"you",CWB_DS_UNDEFINED);
	assert(cwb_ds_get(dic,"you")==CWB_DS_UNDEFINED);

	cwb_ds_destroy(dic);
	
	return 0;
}
