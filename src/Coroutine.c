/*
	cwb
	File:/src/Coroutine.c
	Date:2021.07.05
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<ucontext.h>

#include"Conf.h"
#include"Coroutine.h"

static int alloc_stack(Cwb_Coroutine *co)
{
	void *stack = malloc(CWB_CONF_COROUTINE_STKSIZE);
	if (!stack)
		return -1;
	
	co->coCtx.uc_stack.ss_sp	= stack;
	co->coCtx.uc_stack.ss_size	= CWB_CONF_COROUTINE_STKSIZE;

	return 0;
}

static void coroutine_main(Cwb_Coroutine		*co,
			   Cwb_Coroutine_Func		func,
			   void				*data)
{
	// Set the status
	co->status = CWB_COROUTINE_RUNNING;

	// Invoke the function and check for error
	if (func(co,data)) {
		co->status = CWB_COROUTINE_ERROR;
		return;
	}

	// Set the status
	co->status = CWB_COROUTINE_DEAD;
	
	return;
}

Cwb_Coroutine *cwb_coroutine_create(Cwb_Coroutine_Func		func,
				    void			*data)
{
	/*	Create a coroutine and set the context	*/
	Cwb_Coroutine *co	= (Cwb_Coroutine*)malloc(sizeof(Cwb_Coroutine));
	if (!co)
		return NULL;
	
	if (getcontext(&(co->coCtx))) {
		free(co);
		return NULL;
	}
	if (getcontext(&(co->mainCtx))) {
		free(co);
		return NULL;
	}

	co->coCtx.uc_link = &(co->mainCtx);
	if (alloc_stack(co)) {
		free(co);
		return NULL;
	}
	makecontext(&(co->coCtx),(void(*)(void))coroutine_main,3,co,func,data);

	// Set the status
	co->status = CWB_COROUTINE_SUSPEND;

	return co;
}

short int cwb_coroutine_resume(Cwb_Coroutine *co)
{
	if (swapcontext(&(co->mainCtx),&(co->coCtx)))
		return -1;
	return co->status;
}

void cwb_coroutine_yield(Cwb_Coroutine *co)
{
	co->status = CWB_COROUTINE_SUSPEND;
	if (swapcontext(&(co->coCtx),&(co->mainCtx)))
		abort();
	co->status = CWB_COROUTINE_RUNNING;
	return;
}

void cwb_coroutine_destroy(Cwb_Coroutine *co)
{
	free(co);
	return;
}

short int cwb_coroutine_status(Cwb_Coroutine *co)
{
	return co->status;
}
