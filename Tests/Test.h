/*
	cwb
	File:/Tests/Test.h
	Date:2021.06.14
	By MIT and Anti-996 License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef TEST_ERROR_H_INC
#define TEST_ERROR_H_INC

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void test_init(void)
{
	srand(time(NULL));
	return;
}

void *test_random_data(size_t length)
{
	uint8_t *data = malloc(length);
	assert(data);

	for (size_t count = 0;count < length;count++)
		data[count] = (uint8_t)random();
	
	return data;
}

char *test_random_str(size_t length)
{
	char *str = malloc(length+1);
	assert(str);

	for (size_t count = 0;count < length;count++) {
		str[count] = '\0';
		while (!str[count])
			str[count] = (char)random();
	}

	str[length] = '\0';

	return str;
}

#define TEST_LOOP(target) for (int count = 0;count < (target);count++)

#endif
