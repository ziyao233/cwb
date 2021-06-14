/*
	cwb
	File:/Lib/Decoder.h
	Date:2021.06.14
	By LGPL v3.0 and Anti-996 License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef CWB_DECODER_H_INC
#define CWB_DECODER_H_INC

#include<stdlib.h>

#include"Dstr.h"
#include"Buffer.h"

Cwb_Buffer *cwb_decode_base64(char const *code,Cwb_Buffer *output);
/*void *cwb_decode_uri(const char *data,size_t *size,
		     void *output,size_t bufSize);*/

#endif
