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
#ifndef _SYS_OS_CONFIG_PATCH_H_
#define _SYS_OS_CONFIG_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include "sys_os_config.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// Task - Priority, the type of cmsis_os priority
#define OS_TASK_PRIORITY_AGENT          osPriorityHigh

// Task - Stack Size, the count of 4 bytes
#define OS_TASK_STACK_SIZE_REFINED

#ifdef OS_TASK_STACK_SIZE_REFINED

    // Internal Tasks
    #define OS_TASK_STACK_SIZE_DIAG_PATCH           (444)
    #define OS_TASK_STACK_SIZE_WIFI_MAC_PATCH       (220)
    #define OS_TASK_STACK_SIZE_SUPPLICANT_PATCH     (412)
    #define OS_TASK_STACK_SIZE_CONTROLLER_PATCH     (264)
    #define OS_TASK_STACK_SIZE_LE_PATCH             (512)
    #define OS_TASK_STACK_SIZE_AT_PATCH             (256)
    #define OS_TASK_STACK_SIZE_AT_CMD_SUPPORT_PATCH (512)
    #define OS_TASK_STACK_SIZE_AT_TX_DATA_PATCH     (452)
    #define OS_TASK_STACK_SIZE_EVENT_LOOP_PATCH     (372)
    #define OS_TASK_STACK_SIZE_TCPIP_PATCH          (484)
    #define OS_TASK_STACK_SIZE_TRACER_PATCH         (128)
    #define OS_TASK_STACK_SIZE_AGENT_PATCH          (256)
#if defined(__AT_CMD_SUPPORT__)
    #define OS_TASK_STACK_SIZE_AT_WIFI_APP_PATCH    (512)
#else
    #define OS_TASK_STACK_SIZE_AT_WIFI_APP_PATCH    (256) //(180)
#endif
    // Application Tasks
    #define OS_TASK_STACK_SIZE_BLEWIFI_CTRL         (400)
    #define OS_TASK_STACK_SIZE_BLEWIFI_USER_APP     (248)
    #define OS_TASK_STACK_SIZE_SENSOR               (180)
    #define OS_TASK_STACK_SIZE_SENSOR_DATA          (920)

#else //#ifdef OS_TASK_STACK_SIZE_REFINED

    // Internal Tasks
    #define OS_TASK_STACK_SIZE_DIAG_PATCH           (512)
    #define OS_TASK_STACK_SIZE_WIFI_MAC_PATCH       (1024)
    #define OS_TASK_STACK_SIZE_SUPPLICANT_PATCH     (512)
    #define OS_TASK_STACK_SIZE_CONTROLLER_PATCH     (512)
    #define OS_TASK_STACK_SIZE_LE_PATCH             (1024)
    #define OS_TASK_STACK_SIZE_AT_PATCH             (512)
    #define OS_TASK_STACK_SIZE_AT_CMD_SUPPORT_PATCH (512)
    #define OS_TASK_STACK_SIZE_AT_TX_DATA_PATCH     (512)
    #define OS_TASK_STACK_SIZE_EVENT_LOOP_PATCH     (512)
    #define OS_TASK_STACK_SIZE_TCPIP_PATCH          (512)
    #define OS_TASK_STACK_SIZE_TRACER_PATCH         (128)
    #define OS_TASK_STACK_SIZE_AGENT_PATCH          (256)
    #define OS_TASK_STACK_SIZE_AT_WIFI_APP_PATCH    (512)
    
    // Application Tasks
    #define OS_TASK_STACK_SIZE_BLEWIFI_CTRL         (512)
    #define OS_TASK_STACK_SIZE_BLEWIFI_USER_APP     (512)
    #define OS_TASK_STACK_SIZE_SENSOR               (256)
    #define OS_TASK_STACK_SIZE_SENSOR_DATA          (1024)

#endif //#ifdef OS_TASK_STACK_SIZE_REFINED


// Task - Name (max length is 15 bytes (not including '\0'))
#define OS_TASK_NAME_AGENT              "opl_agent"


// Enable "tid" command to dump task information
//#define OS_TASK_INFO_DUMP


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list


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

#endif // _SYS_OS_CONFIG_PATCH_H_
