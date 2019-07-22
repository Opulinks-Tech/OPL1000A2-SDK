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
*  mw_fim_default_group04.h
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
#ifndef _MW_FIM_DEFAULT_GROUP04_H_
#define _MW_FIM_DEFAULT_GROUP04_H_

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
    MW_FIM_IDX_GP04_START = 0x00040000,             // the start IDX of group 04
    
	// BLE HOST
	MW_FIM_IDX_LE_STORE0,
	MW_FIM_IDX_LE_STORE1,
	MW_FIM_IDX_LE_STORE2,
	MW_FIM_IDX_LE_STORE3,
	
    MW_FIM_IDX_GP04_MAX
} E_MwFimIdxGroup04;

// BLE HOST
#define MW_FIM_LE_STORE0_NUM     	4
#define MW_FIM_LE_STORE1_NUM     	1
#define MW_FIM_LE_STORE2_NUM     	8
#define MW_FIM_LE_STORE3_NUM     	8

#define MW_FIM_LE_STORE0_SIZE    	20
#define MW_FIM_LE_STORE1_SIZE    	100
#define MW_FIM_LE_STORE2_SIZE    	164
#define MW_FIM_LE_STORE3_SIZE    	12


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern const T_MwFimFileInfo g_taMwFimGroupTable04[];


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _MW_FIM_DEFAULT_GROUP03_H_
