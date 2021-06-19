/*
	cwb
	File:/Tests/Dstr.test.c
	Date:2021.06.14
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<stdint.h>

#include"Test.h"
#include<cwb/Dstr.h>

#define TARGET 100

int main(void)
{
	(void)test_random_data;
	test_init();

	TEST_LOOP(TARGET) {
		Cwb_Dstr *dstr = cwb_dstr_new();
		assert(dstr);

		size_t length = random()%65536+1;
		
		char *str = test_random_str(length);

		size_t tmpLength = length;
		char *p = str;
		while (tmpLength) {
			size_t step = random()%512+1;
			step = step > tmpLength ? tmpLength : step;
			
			char *subStr = (char*)malloc(step+2);
			assert(subStr);
			subStr[step] = '\0';
			strncpy(subStr,p,step);
			p += step;

			assert(cwb_dstr_appends(dstr,subStr));
			tmpLength -= step;
			free(subStr);
		}

		char *result = cwb_dstr_convert(dstr,NULL,0);
		assert(!strcmp(result,str));

		free(str);
		cwb_dstr_destroy(dstr);
	}

	return 0;
}
