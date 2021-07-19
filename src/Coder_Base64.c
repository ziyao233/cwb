/*
	cwb
	File:/src/Coder_Base64.c
	Date:2021.07.19
	By MIT License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include"cwb/Dstr.h"
#include"cwb/Buffer.h"
#include"cwb/Coder.h"

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

Cwb_Dstr *cwb_encode_base64(Cwb_Dstr *output,const void *data,size_t size)
{
	if (!output) {
		output = cwb_dstr_new();
		if (!output)
			return NULL;
	}

	size_t filled=(3-size%3)==3?0:(3-size%3);
	size_t resSize=(size/3+(filled?1:0))*4;

	uint8_t *p=(uint8_t*)data;

	char code[5] = {0};
	size_t dataCount=0,count;
	for(count=0;count<resSize;count+=4) {
		code[0]=base64ChangeTable[p[dataCount]>>2];
		code[1]=base64ChangeTable[((p[dataCount]&0x03)<<4) | ((p[dataCount+1])>>4)];
		code[2]=base64ChangeTable[(p[dataCount+1]&0x0f)<<2 | (p[dataCount+2]>>6)];
		code[3]=base64ChangeTable[p[dataCount+2]&0x3f];
		dataCount+=3;
		if (!cwb_dstr_appends(output,code))
			return NULL;
	}

	for(count=0;count<filled;count++) {
		cwb_dstr_appendc(output,'=');
	}

	return output;
}

static size_t real_size(uint8_t *data)
{
	size_t size = 0;
	while (*data != '=') {
		data++;
		size++;
	}
	return size;
}

Cwb_Buffer *cwb_decode_base64(Cwb_Buffer *output,char const *code)
{
	if (!output) {
		output = cwb_buffer_new();
		if (!output)
			return NULL;
	}

	size_t dataSize=strlen(code);

	uint8_t data[4];
	size_t count=0,dataCount=0;
	uint8_t *base64=(uint8_t*)code;
	for(count=0;count<dataSize;count+=4) {
		data[dataCount]=(base64UnchangeTable[base64[count]]<<2) |
						((base64UnchangeTable[base64[count+1]]>>4)&0x3);
		data[dataCount+1]=((base64UnchangeTable[base64[count+1]]&0x0f)<<4) |
						  ((base64UnchangeTable[base64[count+2]]>>2)&0x0f);
		data[dataCount+2]=(base64UnchangeTable[base64[count+2]]&0x03)<<6 |
						  (base64UnchangeTable[base64[count+3]]);
		if (count > dataSize-3) {
			cwb_buffer_appends(output,data,real_size(base64)%3);
		} else {
			cwb_buffer_appends(output,data,3);
		}
	}

	return output;
}
