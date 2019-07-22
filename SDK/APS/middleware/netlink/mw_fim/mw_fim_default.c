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
*  mw_fim_default.c
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
#include "mw_fim_default.h"
#include "mw_fim_default_group01.h"
#include "mw_fim_default_group02.h"
#include "mw_fim_default_group03.h"
#include "mw_fim_default_group04.h"
#include "mw_fim_default_group05.h"
#include "mw_fim_default_group06.h"
#include "mw_fim_default_group07.h"
#include "mw_fim_default_group08.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
// the version table
uint8_t g_ubaMwFimVersionTable[MW_FIM_ZONE_MAX][MW_FIM_GROUP_MAX] =
{
    {
        MW_FIM_VER00,
        MW_FIM_VER01,
        MW_FIM_VER02,
        MW_FIM_VER03,
        MW_FIM_VER04,
        MW_FIM_VER05,
        MW_FIM_VER06,
        MW_FIM_VER07,
        MW_FIM_VER08
    },
    {
        MW_FIM_VER10,
        MW_FIM_VER11,
        MW_FIM_VER12,
        MW_FIM_VER13,
        MW_FIM_VER14,
        MW_FIM_VER15,
        MW_FIM_VER16,
        MW_FIM_VER17,
        MW_FIM_VER18
    },
    {
        MW_FIM_VER20,
        MW_FIM_VER21,
        MW_FIM_VER22,
        MW_FIM_VER23,
        MW_FIM_VER24,
        MW_FIM_VER25,
        MW_FIM_VER26,
        MW_FIM_VER27,
        MW_FIM_VER28
    },
    {
        MW_FIM_VER30,
        MW_FIM_VER31,
        MW_FIM_VER32,
        MW_FIM_VER33,
        MW_FIM_VER34,
        MW_FIM_VER35,
        MW_FIM_VER36,
        MW_FIM_VER37,
        MW_FIM_VER38
    }
};

// the information table of all zone
T_MwFimZoneInfo g_taMwFimZoneInfoTable[MW_FIM_ZONE_MAX] =
{
    {
        MW_FIM_ZONE0_BASE_ADDR,
        MW_FIM_ZONE0_BLOCK_SIZE,
        MW_FIM_ZONE0_BLOCK_NUM,
        g_ubaMwFimVersionTable[0]
    },
    {
        MW_FIM_ZONE1_BASE_ADDR,
        MW_FIM_ZONE1_BLOCK_SIZE,
        MW_FIM_ZONE1_BLOCK_NUM,
        g_ubaMwFimVersionTable[1]
    },
    {
        MW_FIM_ZONE2_BASE_ADDR,
        MW_FIM_ZONE2_BLOCK_SIZE,
        MW_FIM_ZONE2_BLOCK_NUM,
        g_ubaMwFimVersionTable[2]
    },
    {
        MW_FIM_ZONE3_BASE_ADDR,
        MW_FIM_ZONE3_BLOCK_SIZE,
        MW_FIM_ZONE3_BLOCK_NUM,
        g_ubaMwFimVersionTable[3]
    }
};

// the NULL information table of group
const T_MwFimFileInfo g_taMwFimGroupTableNull[] =
{
    // the end, don't modify and remove it
    {0xFFFFFFFF,            0x00,              0x00,               NULL,                            NULL}
};

// the information table of all group
T_MwFimFileInfo* g_ptaMwFimGroupInfoTable[MW_FIM_ZONE_MAX][MW_FIM_GROUP_MAX] =
{
    // zone 0
    {
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,      // reserve for swap
        (T_MwFimFileInfo*)g_taMwFimGroupTable01,
        (T_MwFimFileInfo*)g_taMwFimGroupTable02,
        (T_MwFimFileInfo*)g_taMwFimGroupTable03,
        (T_MwFimFileInfo*)g_taMwFimGroupTable04,
        (T_MwFimFileInfo*)g_taMwFimGroupTable05,
        (T_MwFimFileInfo*)g_taMwFimGroupTable06,
        (T_MwFimFileInfo*)g_taMwFimGroupTable07,
        (T_MwFimFileInfo*)g_taMwFimGroupTable08
    },
    // zone 1
    {
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,      // reserve for swap
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull
    },
    // zone 2
    {
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,      // reserve for swap
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull
    },
    // zone 3
    {
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,      // reserve for swap
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull,
        (T_MwFimFileInfo*)g_taMwFimGroupTableNull
    }
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
