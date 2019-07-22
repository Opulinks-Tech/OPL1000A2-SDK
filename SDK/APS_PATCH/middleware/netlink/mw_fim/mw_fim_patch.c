/* *****************************************************************************
 *  Copyright 2019, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2019
 * 
 ******************************************************************************/
 
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include <string.h>
#include "mw_fim.h"
#include "mw_fim_internal.h"
#include "mw_fim_default.h"
#include "hal_flash.h"
#include "cmsis_os.h"
#include "boot_sequence.h"

#include "mw_fim_patch.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define MW_FIM_SIGNATURE_GROUP      0x67726F70  // grop
#define MW_FIM_SIGNATURE_FILE       0x46494C45  // FILE

#define MW_FIM_VER_GROUP_SEQUENCE_MIN       0x01
#define MW_FIM_VER_GROUP_SEQUENCE_MAX       0xFE
#define MW_FIM_VER_GROUP_SEQUENCE_OVER      0xFF

#define MW_FIM_DATA_SIZE_MIN        8   // used by check the offset of free space
#define MW_FIM_DATA_BUFFER_SIZE     64  // used for swap behavior

#define MW_FIM_SPACE_FULL           0x7FFFFFFF  // can't use 0xFFFFFFFF, it will be overflow in some case


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern T_MwFim_FlashLayoutUpdate_Fp MwFim_FlashLayoutUpdate;


// Sec 5: declaration of global function prototype
extern T_MwFim_Init_Fp MwFim_Init;
extern T_MwFim_FileRead_Fp MwFim_FileRead;
extern T_MwFim_FileWrite_Fp MwFim_FileWrite;
extern T_MwFim_FileWriteDefault_Fp MwFim_FileWriteDefault;
extern T_MwFim_FileDelete_Fp MwFim_FileDelete;

extern T_MwFim_ZoneInfoUpdate_Fp MwFim_ZoneInfoUpdate;
extern T_MwFim_GroupInfoUpdate_Fp MwFim_GroupInfoUpdate;
extern T_MwFim_GroupVersionUpdate_Fp MwFim_GroupVersionUpdate;


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
extern uint8_t g_ubMwFimInit;
extern osSemaphoreId g_tMwFimSemaphoreId;
extern T_MwFimGroupStatus g_taMwFimGroupStatusTable[MW_FIM_ZONE_MAX][MW_FIM_GROUP_MAX];


// Sec 7: declaration of static function prototype
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

extern void MwFim_FileDataDefaultFill_impl(void);


/***********
C Functions
***********/
// Sec 8: C Functions
void MwFim_PreInit_patch(void)
{
    MwFim_FileDataDefaultFill   = MwFim_FileDataDefaultFill_impl;
}

