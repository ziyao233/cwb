/*
	cwb
	File:/Lib/Event.c
	Date:2021.03.21
	By MIT and Anti-996 License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include<unistd.h>
#include<errno.h>

#include"Time.h"
#include"IO.h"
#include"Event.h"
#include"Conf.h"

Cwb_Event_Base *cwb_event_new(void) {
	Cwb_Event_Base *base=(Cwb_Event_Base*)malloc(sizeof(Cwb_Event_Base));
	if(!base)
	{
		goto endMallocBase;
	}

	base->fdEvent=malloc(sizeof(struct Cwb_Event_Fd)*CWB_CONF_EVENT_FD_MAX_NUM);
	if(!(base->fdEvent)) {
		goto endMallocFdEvent;
	}

	for(int i=0;i<CWB_CONF_EVENT_FD_MAX_NUM;i++) {
		base->fdEvent[i].fd=-1;
	}

	base->fdWatcher=cwb_io_watcher_new(CWB_CONF_EVENT_FD_MAX_NUM);
	if(!(base->fdWatcher)) {
		goto endNewFdWatcher;
	}

	base->fdNum=0;
	base->maxFdNum=CWB_CONF_EVENT_FD_MAX_NUM;
	
	return base;

endNewFdWatcher:
	free(base->fdEvent);
endMallocFdEvent:
	free(base);
endMallocBase:
	return NULL;
}

int cwb_event_fd_watch(Cwb_Event_Base *base,int fd,
		       int flag,
		       Cwb_Event_Fd_Handler handler,
		       void *userData) {
	/*	The file describer has already been watched	*/
	if(base->fdEvent[fd].fd>0) {
		return -1;
	}

	if(cwb_io_watcher_watch(base->fdWatcher,fd,
				(flag&CWB_EVENT_FD_READ?
					CWB_IO_WATCHER_READ:
					CWB_IO_WATCHER_NULL) |
				(flag&CWB_EVENT_FD_WRITE?
					CWB_IO_WATCHER_WRITE:
					CWB_IO_WATCHER_NULL))) {
		return -1;
	}

	base->fdEvent[fd]=(struct Cwb_Event_Fd){
					.fd=fd,
					.handler=handler,
					.userData=userData
				};
	
	base->fdNum++;

	return 0;
}

int cwb_event_fd_unwatch(Cwb_Event_Base *base,int fd) {
	if(base->fdEvent[fd].fd<0) {
		return -1;
	}

	if(cwb_io_watcher_unwatch(base->fdWatcher,fd)) {
		return -1;
	}

	base->fdEvent[fd].fd=-1;
	base->fdNum--;
	
	return 0;
}

int cwb_event_loop(Cwb_Event_Base *base) {
	int *readyFd=(int*)malloc(sizeof(int)*(base->maxFdNum+1));
	if(!readyFd) {
		return -1;
	}

	base->running=1;

	while(base->running) {
		int *retVal=cwb_io_watcher_wait(base->fdWatcher,
						readyFd,
						base->maxFdNum,
						NULL);
		if(!retVal) {
			return -1;
		}

		for(int i=0;readyFd[i]>0;i++) {
			int fd=readyFd[i];
			if(base->fdEvent[fd].handler(base,
						     fd,
						     base->fdEvent[fd].userData)) {
				return -1;
			}
		}
	}

	return 0;
}

int cwb_event_exit(Cwb_Event_Base *base) {
	base->running=0;
	return 0;
}

void cwb_event_destroy(Cwb_Event_Base *base) {
	cwb_io_watcher_destroy(base->fdWatcher);
	free(base->fdEvent);
	free(base);

	return;
}
