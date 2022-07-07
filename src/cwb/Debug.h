/*
 *	cwb
 *	File:/src/cwb/Debug.h
 *	Date:2022.07.07
 *	By MIT License.
 *	Copyright (c) 2022 Ziyao.All rights reserved.
 */

#ifndef __CWB_DEBUG_H_INC__

#ifndef CWB_CONF_RELEASE

#include<assert.h>

#define cwb_assert(x) assert(x)
#define cwb_dang(x) (x) = NULL

#else

#define cwb_assert(x) 
#define cwb_dang(x)

#endif	// CWB_CONF_RELEASE

#endif	// __CWB_DEBUG_H_INC__
