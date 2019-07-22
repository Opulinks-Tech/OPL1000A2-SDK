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

#ifndef __LE_CTRL_DBG_H__
#define __LE_CTRL_DBG_H__

#include "opulinks.h"
#include "le_ctrl_para.h"

#define le_ctrl_assert(pointer)														\
{																					\
	if (pointer == NULL)															\
	{																				\
		printf("pointer is abnormal, %s, %d\r\n", __FUNCTION__, __LINE__);			\
		printf("addr = %08X\r\n", (uint32_t) pointer);								\
		__ASM("BKPT 1");															\
	}																				\
}

#define le_ctrl_assert_fail()									\
{																\
	printf("assert fail, %s, %d\r\n", __FUNCTION__, __LINE__);	\
	__ASM("BKPT 1");											\
}

#if LE_CTRL_DBG_ENABLE
	#define DBG_CTRL_LL_MSG(msg)						\
	{													\
		printf(msg"\r\n");								\
	}
	#define DBG_CTRL_LL_VAR(name, fmt, var)				\
	{													\
		printf(name":["fmt" ]\r\n", var);				\
	}
	#define DBG_CTRL_LL_ARR(name, arr, len)				\
	{													\
		int var_idx;									\
		printf(name"(%d):[", len);						\
		for (var_idx = 0; var_idx < len; var_idx++)		\
			printf("%02X ", *((uint8_t *) arr + var_idx));				\
		printf("]\r\n");								\
	}
#else
	#define DBG_CTRL_LL_MSG(msg)
	#define DBG_CTRL_LL_VAR(name, fmt, var)
	#define DBG_CTRL_LL_ARR(name, arr, len)
#endif

#endif
