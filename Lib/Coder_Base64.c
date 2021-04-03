/*
	cwb
	File:/Lib/Coder_Base64.c
	Date:2021.04.03
	By LGPL v3.0 and Anti-996 License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include"Encoder.h"
#include"Decoder.h"

static const char *base64ChangeTable=
				  "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
				  "abcdefghijklmnopqrstuvwxyz" \
				  "0123456789+/";

static const uint8_t base64UnchangeTable[]=
					   {
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x3F, 
						0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
						0x3C, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
						0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 
						0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
						0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
						0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 
						0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
						0x31, 0x32, 0x33
					};

char *cwb_encode_base64(const void *data,size_t size,
			char *output,size_t bufSize) {
	size_t filled=(3-size%3)==3?0:(3-size%3);
	size_t resSize=(size/3+(filled?1:0))*4;

	if(output&&bufSize<resSize)
		return NULL;

	char *code=output?output:(char*)malloc(sizeof(char)*(resSize+1));
	if(!code)
		return NULL;

	uint8_t *p=(uint8_t*)data;

	size_t dataCount=0,count;
	for(count=0;count<resSize;count+=4) {
		code[count]=base64ChangeTable[p[dataCount]>>2];
		code[count+1]=base64ChangeTable[((p[dataCount]&0x03)<<4) | ((p[dataCount+1])>>4)];
		code[count+2]=base64ChangeTable[(p[dataCount+1]&0x0f)<<2 | (p[dataCount+2]>>6)];
		code[count+3]=base64ChangeTable[p[dataCount+2]&0x3f];
		dataCount+=3;
	}

	for(count=0;count<filled;count++) {
		code[resSize-count-1]='=';
	}
	code[resSize]='\0';

	return code;
}

void *cwb_decode_base64(const char *code,size_t *size,
			void *output,size_t bufSize) {
	size_t dataSize=strlen(code);
	uint8_t *data=output?output:(uint8_t*)malloc(sizeof(uint8_t)*dataSize);

	if(output && bufSize<(dataSize/3+1))
		return NULL;

	if(!data)
		return NULL;

	size_t count=0,dataCount=0;
	uint8_t *base64=(uint8_t*)code;
	for(count=0;count<dataSize;count+=4) {
		data[dataCount]=(base64UnchangeTable[base64[count]]<<2) |
						((base64UnchangeTable[base64[count+1]]>>4)&0x3);
		data[dataCount+1]=((base64UnchangeTable[base64[count+1]]&0x0f)<<4) |
						  ((base64UnchangeTable[base64[count+2]]>>2)&0x0f);
		data[dataCount+2]=(base64UnchangeTable[base64[count+2]]&0x03)<<6 |
						  (base64UnchangeTable[base64[count+3]]);
		dataCount+=3;
	}

	if(base64[dataSize-1]=='=')
		dataCount--;
	
	if(base64[dataSize-2]=='=')	
		dataCount--;
	
	
	*size=dataCount;
	return data;
}
