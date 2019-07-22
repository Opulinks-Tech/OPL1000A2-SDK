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
*  sys_os_config.h
*
*  Project:
*  --------
*  OPL1000 Project - the system os configuration file
*
*  Description:
*  ------------
*  This include file is the system os configuration file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _SYS_OS_CONFIG_H_
#define _SYS_OS_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include "cmsis_os.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// Task - Priority, the type of cmsis_os priority
#define OS_TASK_PRIORITY_PS             osPriorityRealtime
#define OS_TASK_PRIORITY_TIMER          osPriorityRealtime

#define OS_TASK_PRIORITY_CONTROLLER     osPriorityHigh
#define OS_TASK_PRIORITY_LE             osPriorityHigh
#define OS_TASK_PRIORITY_SUPPLICANT     osPriorityHigh
#define OS_TASK_PRIORITY_WIFI_MAC       osPriorityHigh
#define OS_TASK_PRIORITY_SETUP_IF       osPriorityHigh
#define OS_TASK_PRIORITY_AGENT          osPriorityHigh

#define OS_TASK_PRIORITY_APP            osPriorityNormal
#define OS_TASK_PRIORITY_DIAG           osPriorityNormal
#define OS_TASK_PRIORITY_AT             osPriorityNormal
#define OS_TASK_PRIORITY_LE_TEST        osPriorityNormal

#define OS_TASK_PRIORITY_EVENT_LOOP     OS_TASK_PRIORITY_APP
#define OS_TASK_PRIORITY_AT_WIFI_APP    OS_TASK_PRIORITY_APP
#define OS_TASK_PRIORITY_AT_TX_DATA     OS_TASK_PRIORITY_APP

#define OS_TASK_PRIORITY_TRACER         osPriorityLow
#define OS_TASK_PRIORITY_IDLE           osPriorityIdle              // must be osPriorityIdle, 
                                                                    // it will be used for the translation between cmsis and FreeRTOS

// For xTaskCreate in LwIP
#define OS_TASK_PRIORITY_TCPIP          (osPriorityAboveNormal - osPriorityIdle)

// Task - Stack Size, the count of 4 bytes
#define OS_TASK_STACK_SIZE_REFINED

#ifdef OS_TASK_STACK_SIZE_REFINED

    #define OS_TASK_STACK_SIZE_PS           (128)
    #define OS_TASK_STACK_SIZE_TIMER        (128)
    
    #define OS_TASK_STACK_SIZE_CONTROLLER   (264)
    #define OS_TASK_STACK_SIZE_LE           (512)
    #define OS_TASK_STACK_SIZE_SUPPLICANT   (412)
    #define OS_TASK_STACK_SIZE_WIFI_MAC     (220)
    #define OS_TASK_STACK_SIZE_SETUP_IF     (256)
    #define OS_TASK_STACK_SIZE_TCPIP        (484)
        
    #define OS_TASK_STACK_SIZE_APP          (512)
    #define OS_TASK_STACK_SIZE_DIAG         (444)
    #define OS_TASK_STACK_SIZE_AT           (512)
    #define OS_TASK_STACK_SIZE_LE_TEST      (512)
    #define OS_TASK_STACK_SIZE_EVENT_LOOP   (372)
    #define OS_TASK_STACK_SIZE_AT_WIFI_APP  (512)
    #define OS_TASK_STACK_SIZE_AT_TX_DATA   (452)
    
    #define OS_TASK_STACK_SIZE_AGENT        (128)
    #define OS_TASK_STACK_SIZE_TRACER       (128)
    #define OS_TASK_STACK_SIZE_IDLE         (128)

#else

    #define OS_TASK_STACK_SIZE_PS           (128)
    #define OS_TASK_STACK_SIZE_TIMER        (128)
    
    #define OS_TASK_STACK_SIZE_CONTROLLER   (512)
    #define OS_TASK_STACK_SIZE_LE           (1024)
    #define OS_TASK_STACK_SIZE_SUPPLICANT   (512)
    #define OS_TASK_STACK_SIZE_WIFI_MAC     (1024)
    #define OS_TASK_STACK_SIZE_SETUP_IF     (256)
    #define OS_TASK_STACK_SIZE_TCPIP        (512)
        
    #define OS_TASK_STACK_SIZE_APP          (512)
    #define OS_TASK_STACK_SIZE_DIAG         (512)
    #define OS_TASK_STACK_SIZE_AT           (512)
    #define OS_TASK_STACK_SIZE_LE_TEST      (512)
    #define OS_TASK_STACK_SIZE_EVENT_LOOP   (512)
    #define OS_TASK_STACK_SIZE_AT_WIFI_APP  (512)
    #define OS_TASK_STACK_SIZE_AT_TX_DATA   (512)
    
    #define OS_TASK_STACK_SIZE_AGENT        (128)
    #define OS_TASK_STACK_SIZE_TRACER       (128)
    #define OS_TASK_STACK_SIZE_IDLE         (128)

#endif


// Task - Name (max length is 15 bytes (not including '\0'))
#define OS_TASK_NAME_DIAG               "opl_diag"
#define OS_TASK_NAME_WIFI_MAC           "opl_wifi_mac"
#define OS_TASK_NAME_SUPPLICANT         "opl_supplicant"
#define OS_TASK_NAME_CONTROLLER         "opl_controller"
#define OS_TASK_NAME_LE                 "opl_le"
#define OS_TASK_NAME_EVENT_LOOP         "opl_event_loop"
#define OS_TASK_NAME_TCPIP              "opl_tcpip"
#define OS_TASK_NAME_PING               "opl_ping"
#define OS_TASK_NAME_AGENT              "opl_agent"
#define OS_TASK_NAME_AT_WIFI_APP        "opl_at_wifi_app"
#define OS_TASK_NAME_AT                 "opl_at"
#define OS_TASK_NAME_AT_TX_DATA         "opl_at_tx_data"
#define OS_TASK_NAME_AT_SOCKET_CLIENT   "opl_at_sock_"
#define OS_TASK_NAME_AT_SOCKET_SERVER   "opl_at_sockserv"
#define OS_TASK_NAME_TRACER             "opl_tracer"


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

#endif // _SYS_OS_CONFIG_H_
