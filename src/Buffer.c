/*
	cwb
	File:/src/Buffer.c
	Date:2021.08.02
	By MIT License.
	Copyright (c) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include"cwb/Conf.h"
#include"cwb/Buffer.h"

#define create_part() ((Buffer_Part*)malloc(sizeof(Buffer_Part)))

typedef struct __Cwb_Buffer_Part Buffer_Part;

static Buffer_Part *create_guard(void)
{
	Buffer_Part *part = create_part();
	if (!part)
		return NULL;
	
	*part = (Buffer_Part) {
				.next = NULL,
				.data = (uint8_t*)malloc(1),
				.size = 0,
				.used = 0
			      };

	return part;
}

Cwb_Buffer *cwb_buffer_new(void)
{
	Cwb_Buffer *buffer = (Cwb_Buffer*)malloc(sizeof(Cwb_Buffer));
	if (!buffer)
		return NULL;

	*buffer = (Cwb_Buffer) {
				.size = 0,
				.part = create_guard()
			       };
	
	if (!buffer->part)
		return NULL;
	buffer->last = buffer->part;

	return buffer;
}

void cwb_buffer_destroy(Cwb_Buffer *buffer)
{
	Buffer_Part *last = buffer->part;

	for (Buffer_Part *part = last->next;
	     part;
	     part = part->next) {
		free(last->data);
		free(last);
		last = part;
	}

	free(last->data);
	free(last);

	free(buffer);

	return;
}

static int extend_part(Cwb_Buffer *buffer)
{
	Buffer_Part *part = create_part();

	if (part) {
		*part = (Buffer_Part) {
					.data = (uint8_t*)malloc(CWB_CONF_BUFFER_PARTSIZE),
					.used = 0,
					.size = CWB_CONF_BUFFER_PARTSIZE,
					.next = NULL
				      };
		if (!part->data)
			return -1;

		buffer->last->next	= part;
		buffer->last		= part;
	}

	return !part;
}

Cwb_Buffer *cwb_buffer_appends(Cwb_Buffer *buffer,const void *src,size_t size)
{
	Buffer_Part *part = buffer->last;

	if ((part->used + size) > part->size) {
		if (extend_part(buffer))
			return NULL;
		part = buffer->last;
	}

	memcpy((void*)(part->data + part->used),src,size);

	part->used   += size;
	buffer->size += size;

	return buffer;
}

void *cwb_buffer_convert(Cwb_Buffer *buffer,void *output,size_t size)
{
	if (output) {
		if (buffer->size > size)
			return NULL;
	} else {
		output = malloc(buffer->size);
		if (!output)
			return NULL;
	}

	uint8_t *p = (uint8_t*)output;
	
	for (Buffer_Part *part = buffer->part;
	     part;
	     part = part->next) {
		memcpy(p,part->data,part->used);
		p += part->used;
	}

	return output;
}
