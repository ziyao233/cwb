/*
	cwb
	File:/Tests/Coders.test.c
	Date:2021.06.14
	By LGPL v3.0 and Anti-996 License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<stdint.h>

#include<cwb/Dstr.h>
#include<cwb/Buffer.h>
#include<cwb/Encoder.h>
#include<cwb/Decoder.h>

void error_report(void) {
	fputs("Error\n",stderr);
	return;
}

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
		puts("Encoding(Base64)...");
		Cwb_Dstr *dEncodedData = cwb_encode_base64(data,dataSize,NULL);
		assert(dEncodedData);
		char *encodedData=cwb_dstr_convert(dEncodedData,NULL,0);
		assert(encodedData);

		puts("Decoding...");
		Cwb_Buffer *dDecodedData = cwb_decode_base64(encodedData,NULL);
		assert(dDecodedData);
		void *decodedData=cwb_buffer_convert(dDecodedData,NULL,0);
		assert(decodedData);

		puts("Comparing...");
		assert(!memcmp(decodedData,data,dataSize));

		free(encodedData);
		free(decodedData);

		cwb_dstr_destroy(dEncodedData);
		cwb_buffer_destroy(dDecodedData);
	}

	/*
	{
		puts("Encoding(URI)...");
		char *encodedData=cwb_encode_uri(data,dataSize,NULL,0);
		if(!encodedData) {
			error_report();
			return -1;
		}

		size_t decodedSize=0;
		puts("Decoding...");
		void *decodedData=cwb_decode_uri(encodedData,&decodedSize,NULL,0);
		if(!decodedData) {
			error_report();
			return -1;
		}
		
		puts("Comparing...");
		if(dataSize!=decodedSize || memcmp(decodedData,data,dataSize)) {
			error_report();
			return -1;
		}
		free(encodedData);
		free(decodedData);
	}
	*/

	puts("Done");

	free(data);
	
	return 0;
}
