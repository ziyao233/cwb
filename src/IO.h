/*
	cwb
	File:/src/IO.h
	Date:2021.03.05
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef CWB_IO_H_INC
#define CWB_IO_H_INC

#include<errno.h>
#include<sys/select.h>

//	Check errno to find out the reason of error
#define CWB_IO_WOULDBLOCK (errno==EAGAIN || errno==EWOULDBLOCK)
#define CWB_IO_OK (!errno)
#define CWB_IO_TRUEERROR (!CWB_IO_WOULDBLOCK && errno!=EINTR && !CWB_IO_OK)
#define CWB_IO_WATCHER_TIMEOUT (!CWB_IO_WOULDBLOCK && !CWB_IO_TRUEERROR)

typedef struct
{
	unsigned int maxNum;
	int maxFd;
	fd_set rSet,wSet;
}Cwb_IO_Watcher;

/*
	Pass to cwb_net_listener(flag)
*/
//	Unused.(For fun and a cool lock...)
#define CWB_IO_WATCHER_NULL 0x00
//	For read event
#define CWB_IO_WATCHER_READ 0x01
//	For write event
#define CWB_IO_WATCHER_WRITE 0x02

Cwb_IO_Watcher *cwb_io_watcher_new(unsigned int maxNum);
int cwb_io_watcher_watch(Cwb_IO_Watcher *watcher,int fd,uint8_t flag);
int *cwb_io_watcher_wait(Cwb_IO_Watcher *watcher,int *readyList,
			 size_t maxNum,Cwb_Time_MicroSecond *timeout);
int cwb_io_watcher_unwatch(Cwb_IO_Watcher *watcher,int fd);
void cwb_io_watcher_destroy(Cwb_IO_Watcher *watcher);
int cwb_io_watcher_resize(Cwb_IO_Watcher *watcher,unsigned int maxNum);

#endif
