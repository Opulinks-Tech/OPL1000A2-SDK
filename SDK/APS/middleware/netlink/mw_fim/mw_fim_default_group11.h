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
*  mw_fim_default_group11.h
*
*  Project:
*  --------
*  OPL1000 Project - the Flash Item Management (FIM) definition file
*
*  Description:
*  ------------
*  This include file is the Flash Item Management (FIM) definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _MW_FIM_DEFAULT_GROUP11_H_
#define _MW_FIM_DEFAULT_GROUP11_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include "mw_fim.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// the file ID
// xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx
// ^^^^ ^^^^ Zone (0~3)
//           ^^^^ ^^^^ Group (0~8), 0 is reserved for swap
//                     ^^^^ ^^^^ ^^^^ ^^^^ File ID, start from 0
typedef enum
{
    MW_FIM_IDX_GP11_START = 0x01010000,             // the start IDX of group 11
    MW_FIM_IDX_GP11_TST_A,
    MW_FIM_IDX_GP11_TST_B,
    MW_FIM_IDX_GP11_TST_C,
    
    MW_FIM_IDX_GP11_MAX
} E_MwFimIdxGroup11;

// the information of test A
typedef struct
{
    uint32_t ulData32;
    uint16_t uwData16;
    uint8_t ubData80;
    uint8_t ubData81;
} T_MwFimTestA;
#define MW_FIM_TEST_A_SIZE      sizeof(T_MwFimTestA)
#define MW_FIM_TEST_A_NUM       1

// the information of test B
typedef struct
{
    uint8_t ubData80;
    uint8_t ubData81;
    uint16_t uwData16;
    uint32_t ulData32;
} T_MwFimTestB;
#define MW_FIM_TEST_B_SIZE      sizeof(T_MwFimTestB)
#define MW_FIM_TEST_B_NUM       4

// the information of test C
typedef struct
{
    uint16_t uwData16;
    uint8_t ubData80;
    uint8_t ubData81;
    uint32_t ulData32;
} T_MwFimTestC;
#define MW_FIM_TEST_C_SIZE      sizeof(T_MwFimTestC)
#define MW_FIM_TEST_C_NUM       4


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern const T_MwFimFileInfo g_taMwFimGroupTable11[];

extern const T_MwFimTestA g_tMwFimDefaultTestA;
extern const T_MwFimTestB g_tMwFimDefaultTestB;

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _MW_FIM_DEFAULT_GROUP11_H_
