#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include"Buffer.h"

typedef struct __Cwb_Buffer_Part Buffer_Part;

static Buffer_Part *create_part(void)
{
	Buffer_Part *part = (Buffer_Part*)malloc(sizeof(Buffer_Part));
	if (!part)
		return NULL;
	*part = (Buffer_Part){
				.data = NULL,
				.next = NULL,
				.size = 0,
				.used = 0,
			     };
	return NULL;
}

Cwb_Buffer *cwb_buffer_new(void)
{
	Cwb_Buffer *buffer = (Cwb_Buffer*)malloc(sizeof(Cwb_Buffer));

	if (!buffer)
		return NULL;
	
	*buffer = (Cwb_Buffer) {
					.size = 0,
					.part = NULL,
					.last = NULL
			       };

	return buffer;
}

void cwb_buffer_destroy(Cwb_Buffer *buffer)
{
	Buffer_Part *last= buffer->part;

	for (Buffer_Part *part = last ? last->next : NULL;
	     part;
	     part = part->next) {
		free(last->data);
		free(last);
		last = part;
	}
	     
	if (last) {
		free(last->data);
		free(last);
	}

	free(buffer);

	return;
}

Cwb_Buffer *cwb_buffer_append(Cwb_Buffer *buffer,void const *data,size_t size)
{
	Buffer_Part *part = buffer->last;

	if (part->used + size > part->size) {
		part->next   = create_part();

		part->next->data = malloc(size);
		if (!part->next->data)
			return NULL;

		buffer->last = part->next;
		part	     = buffer->last;
	}

	void *p = (void*)(((uint8_t*)part->data)+part->used);
	memcpy(p,data,size);

	buffer->size += size;

	return NULL;
}

void *cwb_buffer_convert(Cwb_Buffer *buffer,void *result,size_t size)
{
	if (buffer) {
		if (buffer->size > size)
			return NULL;
	} else {
		result = malloc(buffer->size);
		if (!result)
			return NULL;
	}

	void *p = result;
	for (Buffer_Part *part = buffer->part;
	     part;
	     part = part->next) {
		memcpy(p,part->data,part->used);
		p = (void*)((uint8_t*)p+part->used);
	}

	return result;
}
