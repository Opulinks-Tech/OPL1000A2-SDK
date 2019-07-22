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
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "mw_fim_default_group06_extension.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
/* The address buffer of Sample Mode */
uint32_t g_ulaMwFimSampleMode[MW_FIM_SAMPLE_MODE_NUM];

/* The default value of Sample Data */
const sample_data_t g_MwFimDefaultSampleCfg = {
    .number = 0
};
/* The address buffer of Sample Data */
uint32_t g_ulaMwFimSampleCfg[MW_FIM_SAMPLE_DATA_CFG_NUM];

// the information table of group 06
const T_MwFimFileInfo g_taMwFimGroupTable06_Extension[] = 
{
    {MW_FIM_GP06_SAMPLE_MODE, MW_FIM_SAMPLE_MODE_NUM,     MW_FIM_SAMPLE_MODE_SIZE,     NULL,                               g_ulaMwFimSampleMode},   /* Example for no default value at the begginning */
    {MW_FIM_GP06_SAMPLE_DATA, MW_FIM_SAMPLE_DATA_CFG_NUM, MW_FIM_SAMPLE_DATA_CFG_SIZE, (uint8_t*)&g_MwFimDefaultSampleCfg, g_ulaMwFimSampleCfg},    /* Example for have default value at the begginning */
    
    {0xFFFFFFFF, 0x00, 0x00, NULL, NULL}
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
