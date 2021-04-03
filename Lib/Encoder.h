/*
	cwb
	File:/Lib/Encoder.h
	Date:2021.04.03
	By LGPL v3.0 and Anti-996 License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#ifndef CWB_EMCODER_H_INC
#define CWB_ENCODER_H_INC

#include<stdlib.h>

char *cwb_encode_base64(const void *data,size_t dataSize,
			char *output,size_t bufSize);

#endif
