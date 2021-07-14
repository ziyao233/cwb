/*
	cwb
	File:/src/cwb/Coder.h
	Date:2021.07.14
	By MIT License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef __CWB_CODER_H_INC
#define __CWB_CODER_H_INC

#include<stdlib.h>

#include"cwb/Dstr.h"
#include"cwb/Buffer.h"

/*	Decoders	*/
Cwb_Buffer *cwb_decode_base64(char const *code,Cwb_Buffer *output);
Cwb_Buffer *cwb_decode_uri(char const *code,Cwb_Buffer *output);

/*	Encoders	*/
Cwb_Dstr *cwb_encode_base64(void const *data,size_t size,Cwb_Dstr *output);
Cwb_Dstr *cwb_encode_uri(void const *data,size_t size,Cwb_Dstr *output);

#endif	// __CWB_CODER_H_INC in Coder.h
