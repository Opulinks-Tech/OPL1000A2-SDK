/******************************************************************************
*  Copyright 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
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
*  util_func.h
*
*  Project:
*  --------
*  OPL1000 Project - the utility functionality file
*
*  Description:
*  ------------
*  This include file is the utility functionality file
*
*  Author:
*  -------
*  Vincent Tai
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _UTIL_FUNC_H_
#define _UTIL_FUNC_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file

// Sec 1: Include File
#include <stdint.h>


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype
typedef void (*util_get_current_time_fp_t)(uint32_t *s, uint32_t *ms);

extern util_get_current_time_fp_t util_get_current_time;

void Util_FuncPreInit(void);

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype



#ifdef __cplusplus
}
#endif /* #ifdef __cplusplus */

#endif /* #ifndef _UTIL_FUNC_H_ */
