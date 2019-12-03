/******************************************************************************
*  Copyright 2017 - 2019, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2019
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  mw_fim_default_group01_patch.c
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
#include <string.h>
#include "mw_fim.h"
#include "mw_fim_default.h"
#include "mw_fim_default_group01_patch.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable

// the default value of rf cfg
extern const T_RfCfg g_tMwFimDefaultRfConfig;

// the information table of group 01
extern const T_MwFimFileInfo g_taMwFimGroupTable01[ 6 ];

T_MwFimFileInfo g_taMwFimGroupTable01_patch[ 6 ];
T_RfCfg g_tMwFimDefaultRfConfig_patch =
{
    0xB0       // u8HighPwrStatus
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
void MwFim_Group01_patch( void )
{
    memcpy(g_taMwFimGroupTable01_patch, g_taMwFimGroupTable01, sizeof(g_taMwFimGroupTable01) );
    g_taMwFimGroupTable01_patch[ 4 ].pubDefaultValue = (uint8_t*)&g_tMwFimDefaultRfConfig_patch;

    MwFim_GroupInfoUpdate(0, 1, (T_MwFimFileInfo *)g_taMwFimGroupTable01_patch);
    MwFim_GroupVersionUpdate(0, 1, MW_FIM_VER01+1);
}
