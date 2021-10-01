/*
	cwb
	File:/test/Util.test.c
	Date:2021.10.01
	By MIT License.
	Copyright (c) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>

#include<cwb/Util.h>

int main(void)
{
	Cwb_Util_PidFile *pidFile = cwb_util_pidfile_new("Util.test.pid");
	getchar();
	cwb_util_pidfile_destroy(pidFile);
	return 0;
}
