/*
	cwb
	File:/src/cwb/Coder.h
	Date:2021.08.03
	By MIT License
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef __CWB_CODER_H_INC
#define __CWB_CODER_H_INC

#include<stdlib.h>

#include"cwb/Dstr.h"
#include"cwb/Buffer.h"

/*	Decoders	*/
Cwb_Buffer *cwb_decode_base64(Cwb_Buffer *output,const char *code);
Cwb_Buffer *cwb_decode_uri(Cwb_Buffer *output,const char *code);

/*	Encoders	*/
Cwb_Dstr *cwb_encode_base64(Cwb_Dstr *output,const void *data,size_t size);
Cwb_Dstr *cwb_encode_uri(Cwb_Dstr *output,const void *data,size_t size);

#endif	// __CWB_CODER_H_INC in Coder.h
