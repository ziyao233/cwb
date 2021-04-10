/*
	cwb
	File:/Lib/Decoder.h
	Date:2021.04.10
	By LGPL v3.0 and Anti-996 License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef CWB_DECODER_H_INC
#define CWB_DECODER_H_INC

#include<stdlib.h>

void *cwb_decode_base64(const char *code,size_t *size,
			void *output,size_t bufSize);
void *cwb_decode_uri(const char *data,size_t *size,
		     void *output,size_t bufSize);

#endif
