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
#include "mw_fim_default_group12_extension.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
/* The default value of sensor data number */
const sensor_data_t g_MwFimDefaultSensorDataNumber = {
    .number = 10,
};
/* The address buffer of sensor data number */
uint32_t g_ulaMwFimAddrBufferConfig[MW_FIM_SENSOR_DATA_CFG_NUM];

// the information table of group 12
const T_MwFimFileInfo g_taMwFimGroupTable12_Extension[] = 
{
    {MW_FIM_GP12_SENSOR_DATA, MW_FIM_SENSOR_DATA_CFG_NUM, MW_FIM_SENSOR_DATA_CFG_SIZE, (uint8_t*)&g_MwFimDefaultSensorDataNumber, g_ulaMwFimAddrBufferConfig},
    
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
