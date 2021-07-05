/*
	cwb
	File:/test/Coroutine.test.c
	Date:2021.07.05
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<cwb/Coroutine.h>

int foo(Cwb_Coroutine *co,void *data)
{
	char const *str = (char const*)data;
	for (int i = 0;i < 512;i++) {
		puts(str);
		cwb_coroutine_yield(co);
	}
	return 0;
}

int main(void)
{
	char const *p1 = "Hello World,this is 1";
	char const *p2 = "Hello World,this is 2";

	Cwb_Coroutine *co1 = cwb_coroutine_create(foo,(void*)p1);
	assert(co1);
	Cwb_Coroutine *co2 = cwb_coroutine_create(foo,(void*)p2);
	assert(co2);
	
	assert(cwb_coroutine_status(co1) == cwb_coroutine_status(co2));
	assert(cwb_coroutine_status(co2) == CWB_COROUTINE_SUSPEND);

	while (cwb_coroutine_status(co1) != CWB_COROUTINE_DEAD &&
	       cwb_coroutine_status(co2) != CWB_COROUTINE_DEAD ) {
		if (cwb_coroutine_status(co1) != CWB_COROUTINE_DEAD) {
			cwb_coroutine_resume(co1);
		}
		if (cwb_coroutine_status(co2) != CWB_COROUTINE_DEAD) {
			cwb_coroutine_resume(co2);
		}
	}

	assert(cwb_coroutine_status(co1) == cwb_coroutine_status(co2));
	assert(cwb_coroutine_status(co1) == CWB_COROUTINE_DEAD);
	
	cwb_coroutine_destroy(co1);
	cwb_coroutine_destroy(co2);

	return 0;
}
