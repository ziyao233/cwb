/*
	cwb
	File:/Lib/Coder_URI.c
	Date:2021.06.14
	By MIT and Anti-996 License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<stdint.h>

#include"Dstr.h"
#include"Buffer.h"
#include"Encoder.h"
#include"Decoder.h"

#define HEX2NUM(ch) ((ch)>57?(ch)-55:(ch)-48)

static inline int uri_needcode(uint8_t ch)
{
	return isalnum((char)ch) ||
		   ch=='.' || ch=='-' || ch=='_' || ch=='~';
}

Cwb_Dstr *cwb_encode_uri(void const *data,size_t size,Cwb_Dstr *output)
{
	if (!output) {
		output = cwb_dstr_new();
		if (!output)
			return NULL;
	}

	static const char *changeTable="0123456789ABCDEF";
	uint8_t *p=(uint8_t*)data;
	char temp[4] = "%\0\0\0";
	for(size_t i=0;i<size;i++)
	{
		if(!uri_needcode(*p))
		{
			temp[1]=changeTable[*p/16];
			temp[2]=changeTable[*p%16];
			cwb_dstr_appends(output,temp);
		}
		else
		{
			cwb_dstr_appendc(output,*p);
		}
		p++;
	}

	return output;
}

Cwb_Buffer *cwb_decode_uri(char const *code,Cwb_Buffer *output)
{
	if (!output) {
		output = cwb_buffer_new();
		if (!output)
			return NULL;
	}

	for(const char *p = code;*p;) {
		char temp;
		if (*p == '%') {
			temp = HEX2NUM(p[1]) << 4 | HEX2NUM(p[2]);
			p    += 3;
			cwb_buffer_appends(output,(void*)&temp,1);
		} else {
			cwb_buffer_appends(output,(void*)p,1);
			p++;
		}
	}

	return output;
}
