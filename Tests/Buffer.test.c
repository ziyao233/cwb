/*
	cwb
	File:/Test/Buffer.test.c
	Date:2021.06.14
	By LGPL v3.0 and Anti-996 License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<assert.h>
#include<time.h>

#include<cwb/Buffer.h>

#include"Test.h"

#define TEST_TARGET 100

int main(void)
{
	test_init();

	TEST_LOOP(TEST_TARGET) {
		Cwb_Buffer *buffer = cwb_buffer_new();
		assert(buffer);

		size_t size = (random()+1)%512;
		void *data = test_random_data(size);
		size_t tmp = size;
		void *tmpData = data;

		while (size) {
			size_t step = random();
			step = step > size ? size : step;
			assert(cwb_buffer_appends(buffer,data,step));
			data = (void*)((uint8_t*)data + step);
			size -= step;
		}

		size = tmp;

		void *result = cwb_buffer_convert(buffer,NULL,0);
		assert(result);

		assert(!memcmp(result,tmpData,size));
		
		free(result);
		free(tmpData);
		cwb_buffer_destroy(buffer);
	}

	return 0;
}
