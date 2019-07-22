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
*  mw_fim_internal.h
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
#ifndef _MW_FIM_INTERNAL_H_
#define _MW_FIM_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>
#include "mw_fim.h"
#include "mw_fim_default.h"
#include "cmsis_os.h"


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


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
extern uint8_t g_ubMwFimInit;
extern osSemaphoreId g_tMwFimSemaphoreId;
extern T_MwFimGroupStatus g_taMwFimGroupStatusTable[MW_FIM_ZONE_MAX][MW_FIM_GROUP_MAX];


// Sec 7: declaration of static function prototype
typedef uint8_t (*T_MwFim_FileWriteDo_Fp)(uint32_t ulFileId, uint16_t uwRecIdx, uint16_t uwFileSize, uint8_t *pubFileData);
typedef void (*T_MwFim_GroupSizeCheck_Fp)(void);
typedef void (*T_MwFim_GroupStatusFill_Fp)(void);
typedef void (*T_MwFim_GroupHeaderCheck_Fp)(uint32_t ulZoneIdx, uint8_t ubaBlockUsed[MW_FIM_GROUP_MAX]);
typedef uint8_t (*T_MwFim_GroupHeaderVerify_Fp)(uint32_t ulZoneIdx, T_MwFimGroupHeader *ptGroupHeader);
typedef uint8_t (*T_MwFim_GroupCheckSumVerify_Fp)(T_MwFimGroupHeader *ptGroupHeader);
typedef void (*T_MwFim_GroupStatusRemap_Fp)(uint32_t ulZoneIdx, uint8_t ubaBlockUsed[MW_FIM_GROUP_MAX]);
typedef uint8_t (*T_MwFim_EmptyBlockFind_Fp)(uint32_t ulZoneIdx, uint8_t ubaBlockUsed[MW_FIM_GROUP_MAX], uint32_t *pulIndex);
typedef void (*T_MwFim_GroupBlockParser_Fp)(void);
typedef uint32_t (*T_MwFim_FileHeaderVerify_Fp)(T_MwFimFileInfo *ptFileTable, T_MwFimFileHeader *ptFileHeader);
typedef uint32_t (*T_MwFim_FreeOffsetVerify_Fp)(uint32_t ulZoneIdx, uint32_t ulGroupIdx, uint32_t ulDataOffset);
typedef uint8_t (*T_MwFim_FileTableSearch_Fp)(uint32_t ulFileId, T_MwFimFileInfo **pptFileTable);
typedef uint8_t (*T_MwFim_GroupSwap_Fp)(uint32_t ulZoneIdx, uint32_t ulGroupIdx);
typedef void (*T_MwFim_FileDataDefaultFill_Fp)(void);

extern T_MwFim_FileWriteDo_Fp MwFim_FileWriteDo;
extern T_MwFim_GroupSizeCheck_Fp MwFim_GroupSizeCheck;
extern T_MwFim_GroupStatusFill_Fp MwFim_GroupStatusFill;
extern T_MwFim_GroupHeaderCheck_Fp MwFim_GroupHeaderCheck;
extern T_MwFim_GroupHeaderVerify_Fp MwFim_GroupHeaderVerify;
extern T_MwFim_GroupCheckSumVerify_Fp MwFim_GroupCheckSumVerify;
extern T_MwFim_GroupStatusRemap_Fp MwFim_GroupStatusRemap;
extern T_MwFim_EmptyBlockFind_Fp MwFim_EmptyBlockFind;
extern T_MwFim_GroupBlockParser_Fp MwFim_GroupBlockParser;
extern T_MwFim_FileHeaderVerify_Fp MwFim_FileHeaderVerify;
extern T_MwFim_FreeOffsetVerify_Fp MwFim_FreeOffsetVerify;
extern T_MwFim_FileTableSearch_Fp MwFim_FileTableSearch;
extern T_MwFim_GroupSwap_Fp MwFim_GroupSwap;
extern T_MwFim_FileDataDefaultFill_Fp MwFim_FileDataDefaultFill;


#ifdef __cplusplus
}
#endif

#endif // _MW_FIM_INTERNAL_H_
