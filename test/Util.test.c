/*
	cwb
	File:/test/Util.test.c
	Date:2021.10.15
	By MIT License.
	Copyright (c) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>

#include<unistd.h>
#include<cwb/Util.h>

int main(void)
{
	cwb_util_daemon("/");

	Cwb_Util_PidFile *pidFile = cwb_util_pidfile_new("/home/st127/Util.test.pid");

	sleep(10);

	cwb_util_pidfile_destroy(pidFile);

	return 0;
}
