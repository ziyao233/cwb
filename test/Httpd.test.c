/*
	cwb
	File:/test/Httpd.test.c
	Date:2021.07.20
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cwb/Httpd.h"

int main(void)
{
	Cwb_Httpd *httpd = cwb_httpd_new();
	assert(httpd);

	httpd->conf.network.port = 10000;

	cwb_httpd_start(httpd);

	cwb_httpd_destroy(httpd);

	return 0;
}
