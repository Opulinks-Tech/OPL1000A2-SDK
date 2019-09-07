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
#include "mw_fim_default_group12_project.h"
#include "blewifi_configuration.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
// Aliyun device
const T_MwFim_GP12_AliyunDevice g_tMwFimDefaultGp12AliyunDevice =
{
    .ulProductId           = ALI_PRODUCT_ID,
    .ubaProductKey         = ALI_PRODUCT_KEY,
    .ubaProductSecret      = ALI_PRODUCT_SECRET,
    .ubaDeviceName         = ALI_DEVICE_NAME,
    .ubaDeviceSecret       = ALI_DEVICE_SECRET
};
// the address buffer of aliyun device
uint32_t g_ulaMwFimAddrBufferGP12AliyunDevice[MW_FIM_GP12_ALIYUN_DEVICE_NUM];


// the information table of group 12
const T_MwFimFileInfo g_taMwFimGroupTable12_project[] =
{
    {MW_FIM_IDX_GP12_PROJECT_ALIYUN_DEVICE,     MW_FIM_GP12_ALIYUN_DEVICE_NUM,      MW_FIM_GP12_ALIYUN_DEVICE_SIZE,     (uint8_t*)&g_tMwFimDefaultGp12AliyunDevice,     g_ulaMwFimAddrBufferGP12AliyunDevice},

    // the end, don't modify and remove it
    {0xFFFFFFFF,        0x00,       0x00,       NULL,       NULL}
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
