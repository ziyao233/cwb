/*
	cwb
	File:/Lib/Time.h
	Date:2021.02.16
	By LGPL v3.0 and Anti-996 License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef CWB_TIME_H_INC
#define CWB_TIME_H_INC

#include<stdint.h>

typedef uint64_t Cwb_Time_MicroSecond;

Cwb_Time_MicroSecond cwb_time_get(void);

#endif
