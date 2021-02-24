/*
	cwb
	File:/Lib/Event.h
	Date:2021.02.24
	By LGPL v3.0 and Anti-996 License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef CWB_EVENT_H_INC
#define CWB_EVENT_H_INC

#include"Net.h"

#define CWB_EVENT_ONCE 0x40

typedef struct
{
	unsigned long int maxConnection;
	unsigned long int countConnection;
	struct Cwb_Event_Connection *connections;
	Cwb_Net_Listener *connListener;
	uint8_t run;
}Cwb_Event_Base;

typedef int (*Cwb_Event_Connection_Handler)(Cwb_Event_Base *base,int fd,void *userData);

typedef struct Cwb_Event_Connection
{
	void *userData;
	Cwb_Event_Connection_Handler handler;
	uint8_t type;
}Cwb_Event_Connection;

Cwb_Event_Base *cwb_event_new(unsigned long int maxFd);
int cwb_event_listen_connection(Cwb_Event_Base *base,int connection,
				uint8_t flag,
				Cwb_Event_Connection_Handler handler,
				void *userData);
int cwb_event_unlisten_connection(Cwb_Event_Base *base,int connection);
void cwb_event_exit(Cwb_Event_Base *base);
int cwb_event_loop(Cwb_Event_Base *base);
void cwb_event_destroy(Cwb_Event_Base *base);

#endif
