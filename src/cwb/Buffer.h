/*
	cwb
	File:/src/cwb/Buffer.h
	Date:2021.08.19
	By MIT License.
	Copyright (c) 2021 cwb developers.All rights reserved.
*/

#ifndef _CWB_BUFFER_H_INC
#define _CWB_BUFFER_H_INC

#include<stdlib.h>
#include<stdint.h>

struct __Cwb_Buffer_Part{
	uint8_t *data;
	size_t size;
	size_t used;
	struct __Cwb_Buffer_Part *next;
};

typedef struct {
	struct __Cwb_Buffer_Part *part,*last;
	size_t size;
}Cwb_Buffer;

#define cwb_buffer_size(buffer) ((buffer)->size)

Cwb_Buffer *cwb_buffer_new(void);
void cwb_buffer_destroy(Cwb_Buffer *buffer);

Cwb_Buffer *cwb_buffer_appends(Cwb_Buffer *buffer,const void *src,size_t size);

void *cwb_buffer_convert(Cwb_Buffer *buffer,void *data,size_t size);

#endif
