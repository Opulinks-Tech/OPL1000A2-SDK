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

#ifndef _BLE_TYPE_DEFINE_H_
#define _BLE_TYPE_DEFINE_H_

#include <stdint.h>


typedef uint8_t		UINT8;
typedef uint16_t	UINT16;
typedef uint32_t	UINT32;
typedef uint64_t	UINT64;

typedef int8_t		INT8;
typedef int16_t		INT16;
typedef int32_t		INT32;
typedef int64_t		INT64;

typedef UINT8		BOOL;

#ifndef TRUE
#define TRUE        ((BOOL)1)
#endif

#ifndef FALSE
#define FALSE       ((BOOL)0)
#endif

#ifndef NULL
#define NULL		((void *)0)
#endif

#endif
