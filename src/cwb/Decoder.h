/*
	cwb
	File:/src/cwb/Decoder.h
	Date:2021.06.14
	By MIT License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef CWB_DECODER_H_INC
#define CWB_DECODER_H_INC

#include<stdlib.h>

#include"cwb/Dstr.h"
#include"cwb/Buffer.h"

Cwb_Buffer *cwb_decode_base64(char const *code,Cwb_Buffer *output);
Cwb_Buffer *cwb_decode_uri(char const *code,Cwb_Buffer *output);

#endif
