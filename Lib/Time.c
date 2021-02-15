/*
	cwb
	File:/Lib/Time.c
	Date:2021.02.15
	By LGPL v3.0
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<time.h>

#include<unistd.h>
#include<sys/time.h>

uint64_t cwb_time_get(void)
{
	struct timeval res;
	if(gettimeofday(&res,NULL))
	{
		return 0;
	}

	return res.tv_sec*1000+res.tv_usec;
}
