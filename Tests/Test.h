/*
	cwb
	File:/Tests/Test.h
	Date:2021.05.23
	By LGPL v3.0 and Anti-996 License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef TEST_ERROR_H_INC
#define TEST_ERROR_H_INC

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

static void test_init(void)
{
	srand(time(NULL));
	return;
}

static void *test_random_data(size_t length)
{
	uint8_t *data = malloc(length);
	assert(data);

	for (size_t count = 0;count < length;count++)
		data[count] = (uint8_t)random();
	
	return data;
}

#define TEST_LOOP(target) for (int count = 0;count < (target);count++)

#endif
