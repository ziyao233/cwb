/*
	cwb
	File:/Lib/IO.c
	Date:2021.06.15
	By LGPL v3.0 and Anti-996 License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

#include<unistd.h>
#include<sys/select.h>

#include"Time.h"
#include"IO.h"

/*
	funcName:cwb_io_watcher_new
	input:
		maxNum:The maxium number of the handlers that could be listened.
	output:
		The handler of the watcher.
	brief:
		This function creates a watcher and return it.If failed,NULL will be returned.
		The watcher should be destroyed by cwb_io_watcher_destroy
*/
Cwb_IO_Watcher *cwb_io_watcher_new(unsigned int maxNum)
{
	/*	Select can only save FD_SETSIZE fds.So check it	*/
	if(maxNum>FD_SETSIZE)
	{
		return NULL;
	}

	Cwb_IO_Watcher *watcher=(Cwb_IO_Watcher*)malloc(sizeof(Cwb_IO_Watcher));
	if(!watcher)
	{
		return NULL;
	}

	FD_ZERO(&(watcher->rSet));
	FD_ZERO(&(watcher->wSet));
	watcher->maxFd = 0;

	return watcher;
}

/*
	funcName:cwb_io_watcher_listen
	input:
		watcher: Just the watcher.I will NOT repeat it.
		fd:The file describer that will be watched
		flag:What event will be reported.
	return:
		0 on success and non-zero when fails
	brief:
		This function lets the watcher watch a file describer
*/
int cwb_io_watcher_watch(Cwb_IO_Watcher *watcher,int fd,
			 uint8_t flag)
{
	if(fd>(FD_SETSIZE-1))
	{
		return -1;
	}

	/*	CWB_IO_WATCHER_NULL should not be used	*/
	if(!flag)
	{
		return -1;
	}

	/*	The file describer has already been listened	*/
	if(FD_ISSET(fd,&(watcher->rSet)) ||
	   FD_ISSET(fd,&(watcher->rSet)))
	{
		return -1;
	}

	if(flag&CWB_IO_WATCHER_READ)
	{
		FD_SET(fd,&(watcher->rSet));
	}

	if(flag&CWB_IO_WATCHER_WRITE)
	{
		FD_SET(fd,&(watcher->wSet));
	}

	watcher->maxFd = watcher->maxFd > fd ? watcher->maxFd : fd;

	return 0;
}

/*
	funcName:cwb_io_watcher_wait
	input:
		watcher:I will NOT repeat again.
		readyList:The list to store the ready file describers.
		maxNum:The maxium number of the handlers.
	return:
		NULL when fails.
		readyList on success.
		The address of a list filled with ready file describers.
	brief:
		This function will be blocked until any fd begins.
		The maxNum should bigger than the length of the readyList.
		The list is end with an element less than 0
		If readyList is NULL,this functions will allocate a list.
			And this list should be free(just by free())
*/
int *cwb_io_watcher_wait(Cwb_IO_Watcher *watcher,int *readyList,
			  size_t maxNum,Cwb_Time_MicroSecond *timeout)
{
	int *list=readyList?readyList:malloc(sizeof(int)*(maxNum+1));

	/*	readyList is NULL and cannot allocate memory	*/
	if(!list)
	{
		return NULL;
	}

	//	The backup of the two sets
	fd_set wSet,rSet;

	/*	Backup	*/
	memcpy((void*)&wSet,(void*)(&(watcher->wSet)),sizeof(fd_set));
	memcpy((void*)&rSet,(void*)(&(watcher->rSet)),sizeof(fd_set));


	struct timeval timeVal,*timeoutPointer=NULL;
	if(timeout)
	{
		timeoutPointer=&timeVal;
		timeVal=(struct timeval){
					 .tv_sec=(time_t)(*timeout/1000),
					 .tv_usec=(suseconds_t)(*timeout%1000)
					};
	}
	int readyNum=select(watcher->maxFd+1,&rSet,&wSet,NULL,timeoutPointer);
	
	/*	Timeout	or something wrong	*/
	if(readyNum<=0)
	{
		return NULL;
	}


	unsigned int listIndex=0;
	for(int testFd=0;
		testFd<FD_SETSIZE &&
		listIndex<(unsigned int)readyNum &&
		listIndex<maxNum;
		testFd++)
	{
		if(FD_ISSET(testFd,&rSet) || FD_ISSET(testFd,&wSet))
		{
			list[listIndex]=testFd;
			listIndex++;
		}
	}

	list[listIndex]=-1;

	return list;
}

int cwb_io_watcher_unwatch(Cwb_IO_Watcher *watcher,int fd)
{
	if(FD_ISSET(fd,&(watcher->wSet))||
	   FD_ISSET(fd,&(watcher->rSet)))
	{
		FD_CLR(fd,&(watcher->wSet));
		FD_CLR(fd,&(watcher->rSet));
		if (fd == watcher->maxFd) {
			int nextMax = watcher->maxFd - 1;
			while (!FD_ISSET(nextMax,&(watcher->wSet)) &&
			       !FD_ISSET(nextMax,&(watcher->rSet)) &&
			       nextMax > 0)
				nextMax--;
			watcher->maxFd = nextMax;
		}

		return 0;
	}
	
	return -1;
}

void cwb_io_watcher_destroy(Cwb_IO_Watcher *watcher)
{
	free(watcher);
	return;
}

int cwb_io_watcher_resize(Cwb_IO_Watcher *watcher,unsigned int maxNum)
{
	/*	Just do a check	*/
	if(maxNum>FD_SETSIZE)
	{
		return -1;
	}
	watcher->maxNum=maxNum;
	return 0;
}
