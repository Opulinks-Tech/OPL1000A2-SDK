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
*  mw_ota_def.h
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
#ifndef _MW_OTA_DEF_H_
#define _MW_OTA_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define MW_OTA_OK                   1
#define MW_OTA_FAIL                 0

#define MW_OTA_SIGNATURE            0x4F544173  // OTAs

// the total size is 476 KB from 0x00000000 to 0x00077000.
    // 0x00000000 ~ 0x00003000 : 12 KB, the 2nd boot agent
    // 0x00003000 ~ 0x00004000 : 4 KB, the 1st OTA header
    // 0x00004000 ~ 0x00005000 : 4 KB, the 2nd OTA header
    // 0x00005000 ~ 0x0003E000 : 228 KB, the 1st OTA image
    // 0x0003E000 ~ 0x00077000 : 228 KB, the 2nd OTA image
#define MW_OTA_HEADER_ADDR_1        0x00003000
#define MW_OTA_HEADER_ADDR_2        0x00004000
#define MW_OTA_IMAGE_ADDR_1         0x00005000
#define MW_OTA_IMAGE_ADDR_2         0x0003E000
#define MW_OTA_IMAGE_SIZE           0x00039000  // 228 KB

#define MW_OTA_SEQ_INVALID  0x0000      // the invalid sequence number
#define MW_OTA_SEQ_START    0x0001      // the start sequence number
#define MW_OTA_SEQ_END      0xFFFE      // the last sequence number


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
// the information of OTA Header in the flash (24 bytes)
typedef struct
{
    uint32_t ulSignature;               // the signature of header
    uint16_t uwProjectId;               // the project ID
    uint16_t uwChipId;                  // the chip ID
    uint16_t uwFirmwareId;              // the firmware ID
    uint16_t uwSeqNum;                  // the sequence number
    uint32_t ulImageSize;               // the image size
    uint32_t ulImageSum;                // the check sum of image
    uint32_t ulHeaderSum;               // the check sum of header
} T_MwOtaFlashHeader;


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

#endif // _MW_OTA_DEF_H_
