/*
	cwb
	File:/Tests/Coders.test.c
	Date:2021.04.03
	By LGPL v3.0 and Anti-996 License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<stdint.h>

#include"cwb/Encoder.h"
#include"cwb/Decoder.h"

int main(void)
{
	puts("Preparing data...");
	srand(time(NULL)*1000);
	size_t dataSize=rand()%65536+1;
	uint8_t *data=(uint8_t*)malloc(sizeof(uint8_t)*dataSize);
	if(!data) {
		fputs("Error",stderr);
		return -1;
	}
	printf("%lu byte(s)\n",dataSize);
	
	for(size_t count=0;count<dataSize;count++)
		data[count]=(uint8_t)rand();

	{
		puts("Encoding...");
		char *encodedData=cwb_encode_base64(data,dataSize,NULL,0);
		if(!encodedData) {
			fputs("Error",stderr);
			return -1;
		}

		size_t decodedSize=0;
		void *decodedData=cwb_decode_base64(encodedData,&decodedSize,NULL,0);
		if(!decodedData) {
			puts("Decoding...");
			return -1;
		}

		puts("Comparing...");
		if(dataSize!=decodedSize || memcmp(decodedData,data,dataSize)) {
			fputs("Error",stderr);
			return -1;
		}
		free(encodedData);
		free(decodedData);
	}

	puts("Done");

	free(data);
	
	return 0;
}
