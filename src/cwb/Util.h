/*
	cwb
	File:/src/cwb/Util.h
	Date:2021.10.15
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef __CWB_UTIL_H_INC
#define __CWB_UTIL_H_INC

#include<stdlib.h>

/*	String	*/

char *cwb_util_str_copys(char const *src,size_t length);
char *cwb_util_str_copy(char const *src);
void cwb_util_str_tolowerd(char *str);

/*	Pid file	*/
typedef char Cwb_Util_PidFile; 
Cwb_Util_PidFile *cwb_util_pidfile_new(const char *path);
void cwb_util_pidfile_destroy(Cwb_Util_PidFile *pidFile);

/*	Util Macros	*/
#define cwb_util_create(type) ((type *)(malloc(sizeof(type))))

/*	Daemon	*/
int cwb_util_daemon(const char *workDir);

#endif	// __CWB_UTIL_H_INC in Util.h
