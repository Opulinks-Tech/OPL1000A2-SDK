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
*  mw_fim_default_group01.h
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
#ifndef _MW_FIM_DEFAULT_GROUP01_H_
#define _MW_FIM_DEFAULT_GROUP01_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include "mw_fim.h"
#include "hal_uart.h"
#include "sys_cfg.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// the file ID
// xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx
// ^^^^ ^^^^ Zone (0~3)
//           ^^^^ ^^^^ Group (0~8), 0 is reserved for swap
//                     ^^^^ ^^^^ ^^^^ ^^^^ File ID, start from 0
typedef enum
{
    MW_FIM_IDX_GP01_START = 0x00010000,             // the start IDX of group 01
    MW_FIM_IDX_GP01_UART_CFG,
    
    MW_FIM_IDX_GP01_TRACER_CFG,
    MW_FIM_IDX_GP01_TRACER_INT_TASK_INFO,
    MW_FIM_IDX_GP01_TRACER_EXT_TASK_INFO,
    
    MW_FIM_IDX_GP01_RF_CFG,
    
    MW_FIM_IDX_GP01_MAX
} E_MwFimIdxGroup01;


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
// the information of Uart config
#define MW_FIM_UART_CFG_SIZE    sizeof(T_HalUartConfig)
#define MW_FIM_UART_CFG_NUM     UART_IDX_MAX

#define MW_FIM_TRACER_CFG_SIZE              sizeof(T_TracerCfg)
#define MW_FIM_TRACER_CFG_NUM               1

#define MW_FIM_TRACER_INT_TASK_INFO_SIZE    sizeof(T_TracerTaskInfo)
#define MW_FIM_TRACER_INT_TASK_INFO_NUM     TRACER_INT_TASK_NUM_MAX

#define MW_FIM_TRACER_EXT_TASK_INFO_SIZE    sizeof(T_TracerTaskInfo)
#define MW_FIM_TRACER_EXT_TASK_INFO_NUM     TRACER_EXT_TASK_NUM_MAX

#define MW_FIM_RF_CFG_SIZE                  sizeof(T_RfCfg)
#define MW_FIM_RF_CFG_NUM                   1

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern const T_MwFimFileInfo g_taMwFimGroupTable01[];

extern const T_HalUartConfig g_tMwFimDefaultUartConfig;
extern const T_RfCfg g_tMwFimDefaultRfConfig;


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _MW_FIM_DEFAULT_GROUP01_H_
