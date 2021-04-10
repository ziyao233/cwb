/*
	cwb
	File:/Lib/Coder_URI.c
	Date:2021.04.10
	By LGPL v3.0 and Anti-996 License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<stdint.h>

#include"Encoder.h"
#include"Decoder.h"

#define HEX2NUM(ch) ((ch)>57?(ch)-55:(ch)-48)

static inline int uri_needcode(uint8_t ch)
{
	return isalnum((char)ch) ||
		   ch=='.' || ch=='-' || ch=='_' || ch=='~';
}

char *cwb_encode_uri(const void *data,size_t dataSize,
		     char *output,size_t bufSize)
{
	char *result=output?output:(char*)malloc(sizeof(char)*dataSize*3+1);
	if(!result)
		return NULL;
	if(output && (bufSize<dataSize*3))
		return NULL;

	char *dest=result;
	static const char *changeTable="0123456789ABCDEF";
	uint8_t *p=(uint8_t*)data;
	for(size_t i=0;i<dataSize;i++)
	{
		if(!uri_needcode(*p))
		{
			*dest='%';
			dest[1]=changeTable[*p/16];
			dest[2]=changeTable[*p%16];
			dest+=3;
		}
		else
		{
			*dest=*p;
			dest++;
		}
		p++;
	}

	*dest='\0';

	return result;
}

void *cwb_decode_uri(const char *data,size_t *size,
		     void *output,size_t bufSize) {
	size_t dataSize=strlen(data);
	void *result=output?output:malloc(sizeof(uint8_t)*dataSize*3);
	if(!result)
		return NULL;
	
	if(output && bufSize<dataSize)
		return NULL;

	uint8_t *dest=(uint8_t*)result;
	*size=0;
	for(const char *p=data;*p;dest++) {
		/*	Coded	*/
		if(*p=='%') {
			*dest=HEX2NUM(p[1])<<4 | HEX2NUM(p[2]);
			p+=3;
		} else {
			*dest=*p;
			p++;
		}
		(*size)++;
	}

	return result;
}
