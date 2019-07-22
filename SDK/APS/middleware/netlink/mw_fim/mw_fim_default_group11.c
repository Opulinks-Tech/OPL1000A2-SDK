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
*  mw_fim_default_group11.c
*
*  Project:
*  --------
*  OPL1000 Project - the Flash Item Management (FIM) implement file
*
*  Description:
*  ------------
*  This implement file is include the Flash Item Management (FIM) function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "mw_fim_default_group11.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
// the default value of test A
const T_MwFimTestA g_tMwFimDefaultTestA =
{
    0x32,
    0x16,
    0x80,
    0x81
};
// the address buffer of test A
RET_DATA uint32_t g_ulaMwFimAddrBufferTestA[MW_FIM_TEST_A_NUM];

// the default value of test B
const T_MwFimTestB g_tMwFimDefaultTestB =
{
    0x64,
    0x32,
    0x40,
    0x41
};
// the address buffer of test B
RET_DATA uint32_t g_ulaMwFimAddrBufferTestB[MW_FIM_TEST_B_NUM];

// the address buffer of test C
RET_DATA uint32_t g_ulaMwFimAddrBufferTestC[MW_FIM_TEST_C_NUM];

// the information table of group 11
const T_MwFimFileInfo g_taMwFimGroupTable11[] =
{
    {MW_FIM_IDX_GP11_TST_A, MW_FIM_TEST_A_NUM, MW_FIM_TEST_A_SIZE, (uint8_t*)&g_tMwFimDefaultTestA, g_ulaMwFimAddrBufferTestA},
    {MW_FIM_IDX_GP11_TST_B, MW_FIM_TEST_B_NUM, MW_FIM_TEST_B_SIZE, (uint8_t*)&g_tMwFimDefaultTestB, g_ulaMwFimAddrBufferTestB},
    {MW_FIM_IDX_GP11_TST_C, MW_FIM_TEST_C_NUM, MW_FIM_TEST_C_SIZE, NULL,                            g_ulaMwFimAddrBufferTestC},
    
    // the end, don't modify and remove it
    {0xFFFFFFFF,            0x00,              0x00,               NULL,                            NULL}
};


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
