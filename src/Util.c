/*
	cwb
	File:/src/Util.c
	Date:2021.10.01
	By MIT License.
	Copyright (c) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#include<unistd.h>

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

Cwb_Util_PidFile *cwb_util_pidfile_new(const char *path)
{
	FILE *file = fopen(path,"w");
	if (!file)
		return NULL;

	Cwb_Util_PidFile *pidFile = (Cwb_Util_PidFile*)malloc(strlen(path) + 1);
	if (!pidFile) {
		goto end;
	}
	strcpy((char*)pidFile,path);

	fprintf(file,"%lu\n",(unsigned long int)getpid());

end:
	fclose(file);
	return pidFile;
}

void cwb_util_pidfile_destroy(Cwb_Util_PidFile *pidFile)
{
	unlink((char*)pidFile);
	free(pidFile);
	return;
}
