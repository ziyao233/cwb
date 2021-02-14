/*
	cwb
	File:/Lib/Net.h
	Date:2021.02.14
	By LGPL v3.0
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef CWB_NET_H_INC
#define CWB_NET_H_INC

#include<stdint.h>

#include<unistd.h>
#include<sys/select.h>
#include<errno.h>

//	Check errno to find out the reason of error
#define CWB_NET_WOULDBLOCK (errno==EAGAIN || errno==EWOULDBLOCK)
#define CWB_NET_TRUEERROR (!CWB_NET_WOULDBLOCK && errno!=EINTR)

/*
	Pass to cwb_net_listener(flag)
*/
//	Unused.(For fun and a cool lock...)
#define CWB_NET_LISTENER_NULL 0x00
//	For read event
#define CWB_NET_LISTENER_READ 0x01
//	For write event
#define CWB_NET_LISTENER_WRITE 0x02

typedef struct
{
	unsigned int maxNum;
	fd_set rSet,wSet;
}Cwb_Net_Listener;

int cwb_net_socket(void);
int cwb_net_bind(int sock,int port);
int cwb_net_listen(int sock,int backLog);
int cwb_net_accept(int sock);
ssize_t cwb_net_write(int sock,void *buf,size_t size);
ssize_t cwb_net_read(int sock,void *buf,size_t size);
int cwb_net_close(int sock);

uint16_t cwb_net_tonet16(uint16_t data);
uint32_t cwb_net_tonet32(uint32_t data);
uint16_t cwb_net_tohost16(uint16_t data);
uint32_t cwb_net_tohost32(uint32_t data);

Cwb_Net_Listener *cwb_net_listener_new(unsigned int maxNum);
int cwb_net_listener_listen(Cwb_Net_Listener *listener,int fd,uint8_t flag);
int *cwb_net_listener_wait(Cwb_Net_Listener *listener,int *readyList,size_t maxNum);
int cwb_net_listener_unlisten(Cwb_Net_Listener *listener,int fd);
void cwb_net_listener_destroy(Cwb_Net_Listener *listener);

#endif
