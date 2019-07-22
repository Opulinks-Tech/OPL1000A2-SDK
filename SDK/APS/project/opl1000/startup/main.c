/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "boot_sequence.h"
#include "cmsis_os.h"
#include "sys_init.h"
#include "hal_patch.h"
#include "msg.h"

void Main_AppRun_impl(void);

typedef void (*T_Main_AppRun_fp)(void);

RET_DATA T_Main_AppRun_fp Main_AppRun;

void Main_Fun_Init(void)
{
    Main_AppRun = Main_AppRun_impl;
}

void Main_AppRun_impl(void)
{
    osKernelInitialize();

    Sys_DriverInit();

    osKernelRestart();

    Sys_ServiceInit();

    Test_ForSwPatch();

    Sys_AppInit();

    Sys_PostInit();

#if defined  (__RTL_SIMULATION__)
    reg_write(DEBUG_ADDR, 40);
#endif

    osKernelStart();

    while(1);
}

/*
 * main: initialize and start the system
 */
int main (void)
{
    Boot_Sequence();

    Main_AppRun();
}

//ToDo: move to another file: ex: panic.c
#if( configCHECK_FOR_STACK_OVERFLOW > 0 )
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    Sys_StackOverflowHook(xTask, pcTaskName);
}
#endif

#if( configUSE_IDLE_HOOK > 0)
void vApplicationIdleHook( void )
{
    // Idle task hook...
    Sys_IdleHook();
}
#endif

