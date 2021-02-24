/*
	cwb
	File:/Lib/Event.c
	Date:2021.02.24
	By LGPL v3.0 and Anti-996 License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<unistd.h>

#include"Time.h"
#include"Net.h"
#include"Event.h"

#define EVENT_NULL 0x00
#define EVENT_LISTENED 0x80
#define EVENT_FREE 0x00

Cwb_Event_Base *cwb_event_new(unsigned long int maxFd)
{
	Cwb_Event_Base *base=(Cwb_Event_Base*)malloc(sizeof(Cwb_Event_Base));
	if(!base)
		goto endMallocBase;

	base->connListener=cwb_net_listener_new(maxFd);
	if(!(base->connListener))
		goto endNewNetListener;

	base->connections=(Cwb_Event_Connection*)malloc((maxFd+1)*
			sizeof(Cwb_Event_Connection));
	if(!(base->connections))
		goto endMallocConnections;

	base->maxConnection=maxFd;
	base->countConnection=0;
	for(unsigned long int i=0;i<=maxFd;i++)
		base->connections[i].type=EVENT_NULL|
					   EVENT_FREE;

	return base;

endMallocConnections:
	cwb_net_listener_destroy(base->connListener);
endNewNetListener:
	free(base);
endMallocBase:
	return NULL;
}

int cwb_event_listen_connection(Cwb_Event_Base *base,int connection,
				uint8_t flag,
				Cwb_Event_Connection_Handler handler,
				void *userData)
{
	int success=-1;

	if(base->countConnection==base->maxConnection)
		goto endTooManyConnections;

	if(base->connections[connection].type&EVENT_LISTENED)
		goto endAlreadyListened;
	base->connections[connection].type&=~EVENT_FREE;
	base->connections[connection].type|=EVENT_LISTENED;
	if(flag&CWB_EVENT_ONCE)
	{
		base->connections[connection].type|=CWB_EVENT_ONCE;
	}
	base->connections[connection].handler=handler;
	base->connections[connection].userData=userData;

	if(cwb_net_listener_listen(base->connListener,connection,flag))
		goto endListen;

	base->countConnection++;
	success=0;

endListen:
endAlreadyListened:
endTooManyConnections:

	return success;
}

int cwb_event_unlisten_connection(Cwb_Event_Base *base,int connection)
{
	if((unsigned long int)connection>base->maxConnection)
		return -1;

	if(base->connections[connection].type==EVENT_FREE)
		return -1;

	if(cwb_net_listener_unlisten(base->connListener,connection))
		return -1;
	
	base->connections[connection].type=EVENT_FREE;

	base->countConnection--;

	return 0;
}

void cwb_event_exit(Cwb_Event_Base *base)
{
	base->run=0;
	return;
}

int cwb_event_loop(Cwb_Event_Base *base)
{
	int success=-1;
	int *activeList=NULL;

	base->run=1;

	while(base->run)
	{
		/*	Get and process all active connections	*/
		activeList=cwb_net_listener_wait(base->connListener,NULL,
						 base->countConnection,
						 NULL);
		if(!activeList)
			goto endConnectionWait;

		int tmp=-1;
		Cwb_Event_Connection *connection=NULL;
		for(int i=0;activeList[i]>0;i++)
		{
			tmp=activeList[i];
			connection=(base->connections)+tmp;

			if(connection->type&EVENT_FREE)
				goto endProcess;
			connection->handler(base,tmp,connection->userData);
			if(connection->type&CWB_EVENT_ONCE)
			{
				cwb_event_unlisten_connection(base,tmp);
			}
		}
		free(activeList);
	}
	success=0;

endProcess:
endConnectionWait:

	return success;
}

void cwb_event_destroy(Cwb_Event_Base *base)
{
	cwb_net_listener_destroy(base->connListener);
	free(base->connections);
	free(base);
	return;
}
