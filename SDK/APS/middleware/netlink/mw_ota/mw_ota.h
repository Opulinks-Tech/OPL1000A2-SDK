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

/******************************************************************************
*  Filename:
*  ---------
*  mw_ota.h
*
*  Project:
*  --------
*  OPL1000 Project - the Over The Air (OTA) definition file
*
*  Description:
*  ------------
*  This include file is the Over The Air (OTA) definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _MW_OTA_H_
#define _MW_OTA_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>
#include "mw_ota_def.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define MW_OTA_IDX_INVALID      -1  // the invalid index

#define MW_OTA_PREPARE_NONE     0   // not ready
#define MW_OTA_PREPARE_READY    1   // read


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
// the information of OTA flash layout (20 bytes)
typedef struct
{
    uint32_t ulaHeaderAddr[2];  // the start address of header
    uint32_t ulaImageAddr[2];   // the start address of image
    uint32_t ulImageSize;       // the max size of image
} T_MwOtaLayoutInfo;

typedef uint8_t (*T_MwOta_Init_Fp)(T_MwOtaLayoutInfo *ptInfo, uint8_t ubBoot);
typedef uint8_t (*T_MwOta_VersionGet_Fp)(uint16_t *puwProjectId, uint16_t *puwChipId, uint16_t *puwFirmwareId);
typedef uint8_t (*T_MwOta_Prepare_Fp)(uint16_t uwProjectId, uint16_t uwChipId, uint16_t uwFirmwareId, uint32_t ulImageSize, uint32_t ulImageSum);
typedef uint8_t (*T_MwOta_DataIn_Fp)(uint8_t *pubAddr, uint32_t ulSize);
typedef uint8_t (*T_MwOta_DataFinish_Fp)(void);
typedef uint8_t (*T_MwOta_DataGiveUp_Fp)(void);

typedef uint8_t (*T_MwOta_CurrentIdxFake_Fp)(void);
typedef uint8_t (*T_MwOta_BootAddrGet_Fp)(uint32_t *pulImageAddr);

typedef uint16_t (*T_MwOta_HeaderSeqNumGet_Fp)(T_MwOtaFlashHeader *ptHeader);
typedef uint32_t (*T_MwOta_ImageCheckSumLocal_Fp)(void);
typedef uint32_t (*T_MwOta_ImageCheckSumAlloc_Fp)(void);
typedef uint32_t (*T_MwOta_ImageCheckSumCompute_Fp)(uint8_t ubaData[]);


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype
extern T_MwOta_Init_Fp MwOta_Init;
extern T_MwOta_VersionGet_Fp MwOta_VersionGet;
extern T_MwOta_Prepare_Fp MwOta_Prepare;
extern T_MwOta_DataIn_Fp MwOta_DataIn;
extern T_MwOta_DataFinish_Fp MwOta_DataFinish;
extern T_MwOta_DataGiveUp_Fp MwOta_DataGiveUp;

extern T_MwOta_CurrentIdxFake_Fp MwOta_CurrentIdxFake;              // for upgrade in the 2nd boot loader
extern T_MwOta_BootAddrGet_Fp MwOta_BootAddrGet;                    // for load image from flash in the 2nd boot loader

// internal part
extern T_MwOta_HeaderSeqNumGet_Fp MwOta_HeaderSeqNumGet;
extern T_MwOta_ImageCheckSumLocal_Fp MwOta_ImageCheckSumLocal;      // use the local buffer for 2nd boot loader
extern T_MwOta_ImageCheckSumAlloc_Fp MwOta_ImageCheckSumAlloc;      // use the alloc buffer for the normal image
extern T_MwOta_ImageCheckSumCompute_Fp MwOta_ImageCheckSumCompute;

void MwOta_PreInitCold(void);


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _MW_OTA_H_
