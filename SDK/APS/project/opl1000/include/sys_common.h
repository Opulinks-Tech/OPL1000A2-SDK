/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  sys_common.h
*
*  Project:
*  --------
*  OPL1000 Project - the system common definition file
*
*  Description:
*  ------------
*  This include file is the system common definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _SYS_COMMON_H_
#define _SYS_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdint.h>
#include "msg.h"
#include "section.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// retention region
#if defined(GCC)
    #define RET_DATA    __attribute__((section("RET_REGION"), used))
    #define RET_STATIC  __attribute__((section("RET_STATIC_REGION"), used))
    #define __forceinline inline
#else
    #define RET_DATA    __attribute__((section("RET_REGION"), used, zero_init))
    #define RET_STATIC  __attribute__((section("RET_STATIC_REGION"), used, zero_init))
#endif

// memory read/write
#define MEM_READ_DW(addr)		(*(volatile uint32_t *)(addr))
#define MEM_WRITE_DW(addr, val)	(*(volatile uint32_t *)(addr) = (val))

// reg read/write
#define reg_read(addr)          MEM_READ_DW(addr)
#define reg_write(addr, val)    MEM_WRITE_DW(addr, val)

// default Oscillator frequency
#define  XTAL            (22000000UL)
#define  XTAL_PER_US     (22)               // XTAL_PER_US = XTAL / 1000000


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _SYS_COMMON_H_
