/*
	cwb
	File:/src/cwb/Net.h
	Date:2021.03.05
	By MIT License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef CWB_NET_H_INC
#define CWB_NET_H_INC

#include<stdint.h>

#include<unistd.h>
#include<errno.h>

#include"cwb/Time.h"

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

#endif
