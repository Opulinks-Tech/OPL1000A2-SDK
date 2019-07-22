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

/***********************
Head Block of The File
***********************/
#ifndef _MW_FIM_DEFAULT_GROUP06_EXTERNSION_H_
#define _MW_FIM_DEFAULT_GROUP06_EXTERNSION_H_

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
    MW_FIM_GP06_START = 0x00060000,  /*  00 Zone, 06 Group, File ID */
    MW_FIM_GP06_SAMPLE_MODE,
    MW_FIM_GP06_SAMPLE_DATA,
    MW_FIM_GP06_MAX
} E_MwFimIdxGroup06_Extension;

#define MW_FIM_SAMPLE_MODE_SIZE    sizeof(sample_mode_t)
#define MW_FIM_SAMPLE_MODE_NUM     1

#define MW_FIM_SAMPLE_DATA_CFG_SIZE   sizeof(sample_data_t)
#define MW_FIM_SAMPLE_DATA_CFG_NUM    10


// the group version number
#define MW_FIM_VER06_Extension        0x04

/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
typedef struct sample_data_tag {
    int number;
}sample_data_t;

typedef struct sample_mode_tag{
    uint8_t mode;
} sample_mode_t;

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern const sample_data_t g_MwFimDefaultSampleCfg;

extern const T_MwFimFileInfo g_taMwFimGroupTable06_Extension[];

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _MW_FIM_DEFAULT_GROUP06_PATCH_H_
