/*
	cwb
	File:/Lib/Encoder.h
	Date:2021.06.14
	By LGPL v3.0 and Anti-996 License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef CWB_EMCODER_H_INC
#define CWB_ENCODER_H_INC

#include<stdlib.h>

#include"Dstr.h"
#include"Buffer.h"

Cwb_Dstr *cwb_encode_base64(void const *data,size_t size,Cwb_Dstr *output);
Cwb_Dstr *cwb_encode_uri(void const *data,size_t size,Cwb_Dstr *output);

#endif
