/*
	cwb
	File:/src/Util.c
	Date:2021.08.15
	By MIT License.
	Copyright (c) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#include"cwb/Util.h"

char *cwb_util_str_copys(const char *src,size_t size)
{
	char *result = (char*)malloc(size+1);
	if (!result)
		return NULL;
	return strcpy(result,src);
}

char *cwb_util_str_copy(const char *src)
{
	return cwb_util_str_copys(src,strlen(src));
}

void cwb_util_str_tolowerd(char *str)
{
	while (*str) {
		*str = tolower(*str);
		str++;
	}
	return;
}
