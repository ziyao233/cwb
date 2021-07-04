/*
	cwb
	File:/Tests/Time.test.c
	Date:2021.02.15
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdint.h>

#include<cwb/Time.h>

int main(void)
{
	printf("%ji",(uintmax_t)cwb_time_get());
	return 0;
}
