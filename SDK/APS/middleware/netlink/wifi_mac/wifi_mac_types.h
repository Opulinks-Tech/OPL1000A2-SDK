/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#ifndef _WIFI_MAC_TYPES_H_
#define _WIFI_MAC_TYPES_H_

//#include "basetype.h"

#if defined(GCC)
    #include <stdint.h>
    typedef int8_t   s8;
    typedef int16_t  s16;
    typedef int32_t  s32;
    typedef int64_t  s64;
    typedef uint8_t  u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;
#else
    typedef signed char     s8;
    typedef signed short    s16;
    typedef signed int      s32;
    typedef signed long long    s64;
    typedef unsigned char   u8;
    typedef unsigned short  u16;
    typedef unsigned int    u32;
    typedef unsigned long long  u64;
#endif

//-----------------------------------------------
// Generic function pointer
//
//typedef int (*function_ptr_t)();

#endif /* _WIFI_MAC_TYPES_H_ */

