/*
	cwb
	File:/src/Time.c
	Date:2021.02.16
	By MIT License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<time.h>

#include<unistd.h>
#include<sys/time.h>

#include"cwb/Time.h"

Cwb_Time_MicroSecond cwb_time_get(void)
{
	struct timeval res;
	if(gettimeofday(&res,NULL))
	{
		return 0;
	}

	return (Cwb_Time_MicroSecond)((uint64_t)res.tv_sec*1000+res.tv_usec);
}
