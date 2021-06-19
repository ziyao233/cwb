/*
	cwb
	File:/Lib/Event.h
	Date:2021.03.21
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef CWB_EVENT_H_INC
#define CWB_EVENT_H_INC

#include"IO.h"

#define CWB_EVENT_FD_READ 0x01
#define CWB_EVENT_FD_WRITE 0x02

typedef struct {
	unsigned int maxFdNum;
	unsigned int fdNum;
	struct Cwb_Event_Fd *fdEvent;
	Cwb_IO_Watcher *fdWatcher;
	int running;
}Cwb_Event_Base;

typedef int (*Cwb_Event_Fd_Handler)(Cwb_Event_Base *base,int fd,void *userData);

struct Cwb_Event_Fd {
	int fd;
	void *userData;
	Cwb_Event_Fd_Handler handler;
};

Cwb_Event_Base *cwb_event_new(void);
int cwb_event_fd_watch(Cwb_Event_Base *base,int fd,
		       int flag,
		       Cwb_Event_Fd_Handler handler,
		       void *userData);
int cwb_event_fd_unwatch(Cwb_Event_Base *base,int fd);
int cwb_event_loop(Cwb_Event_Base *base);
int cwb_event_exit(Cwb_Event_Base *base);
void cwb_event_destroy(Cwb_Event_Base *base);

#endif
