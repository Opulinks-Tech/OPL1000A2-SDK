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
*  mw_fim_default.h
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
#ifndef _MW_FIM_DEFAULT_H_
#define _MW_FIM_DEFAULT_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include "mw_fim.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// the max of zone and group
#define MW_FIM_ZONE_MAX     4
#define MW_FIM_GROUP_MAX    9

// the version of group
#define MW_FIM_VER00        0x01    // reserve for swap
#define MW_FIM_VER01        0x09    // system & driver
#define MW_FIM_VER02        0x09    // for WIFI & TCPIP
#define MW_FIM_VER03        0x06    // calibration data
#define MW_FIM_VER04        0x05    // for LE Controller and Host
#define MW_FIM_VER05        0x01
#define MW_FIM_VER06        0x01
#define MW_FIM_VER07        0x01
#define MW_FIM_VER08        0x01

#define MW_FIM_VER10        0x01    // reserve for swap
#define MW_FIM_VER11        0x01    // example
#define MW_FIM_VER12        0x01
#define MW_FIM_VER13        0x01
#define MW_FIM_VER14        0x01
#define MW_FIM_VER15        0x01
#define MW_FIM_VER16        0x01
#define MW_FIM_VER17        0x01
#define MW_FIM_VER18        0x01

#define MW_FIM_VER20        0x01    // reserve for swap
#define MW_FIM_VER21        0x01
#define MW_FIM_VER22        0x01
#define MW_FIM_VER23        0x01
#define MW_FIM_VER24        0x01
#define MW_FIM_VER25        0x01
#define MW_FIM_VER26        0x01
#define MW_FIM_VER27        0x01
#define MW_FIM_VER28        0x01

#define MW_FIM_VER30        0x01    // reserve for swap
#define MW_FIM_VER31        0x01
#define MW_FIM_VER32        0x01
#define MW_FIM_VER33        0x01
#define MW_FIM_VER34        0x01
#define MW_FIM_VER35        0x01
#define MW_FIM_VER36        0x01
#define MW_FIM_VER37        0x01
#define MW_FIM_VER38        0x01

// the information of zone
#define MW_FIM_ZONE0_BASE_ADDR      0x00077000
#define MW_FIM_ZONE0_BLOCK_SIZE     0x1000
#define MW_FIM_ZONE0_BLOCK_NUM      9               // swap + used

#define MW_FIM_ZONE1_BASE_ADDR      0x00080000
#define MW_FIM_ZONE1_BLOCK_SIZE     0x1000
#define MW_FIM_ZONE1_BLOCK_NUM      0               // swap + used

#define MW_FIM_ZONE2_BASE_ADDR      0x00081000
#define MW_FIM_ZONE2_BLOCK_SIZE     0x1000
#define MW_FIM_ZONE2_BLOCK_NUM      0               // swap + used

#define MW_FIM_ZONE3_BASE_ADDR      0x00082000
#define MW_FIM_ZONE3_BLOCK_SIZE     0x1000
#define MW_FIM_ZONE3_BLOCK_NUM      0               // swap + used


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern uint8_t g_ubaMwFimVersionTable[MW_FIM_ZONE_MAX][MW_FIM_GROUP_MAX];
extern T_MwFimZoneInfo g_taMwFimZoneInfoTable[MW_FIM_ZONE_MAX];
extern T_MwFimFileInfo* g_ptaMwFimGroupInfoTable[MW_FIM_ZONE_MAX][MW_FIM_GROUP_MAX];


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _MW_FIM_DEFAULT_H_
