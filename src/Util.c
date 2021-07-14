/*
	cwb
	File:/src/Util.c
	Date:2021.07.14
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char *cwb_util_str_copys(char const *src,size_t size)
{
	char *result = (char*)malloc(size+1);
	if (!result)
		return NULL;
	return strcpy(result,src);
}

char *cwb_util_str_copy(char const *src)
{
	return cwb_util_str_copys(src,strlen(src));
}
