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
*  main_patch.c
*
*  Project:
*  --------
*  OPL1000 Project - the main patch implement file
*
*  Description:
*  ------------
*  This implement file is include the main patch function and api.
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
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "sys_init.h"
#include "sys_init_patch.h"
#include "mw_fim.h"
#include "cmsis_os.h"
#include "sys_os_config.h"

extern void AppInit(void);


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// the number of elements in the message queue
#define APP_MESSAGE_Q_SIZE      16
#define MAX_NUM_MEM_POOL        8

#define OS_TIMER_TASK_STACK_DEPTH  512





/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list
typedef struct os_memory_def
{
    uint32_t ulBlockSize;
    uint32_t ulBlockNum;
} osMemoryDef_t;

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern uint8_t* g_ucaMemPartAddr;
extern uint32_t g_ulMemPartTotalSize;
extern osMemoryDef_t g_xaMemoryTable[MAX_NUM_MEM_POOL];
extern StackType_t* g_taTimerTaskStackBufferAddr;
extern uint32_t g_ulTimerTaskStackSize;

static StackType_t g_taTimerTaskStackBufferBody_new[OS_TIMER_TASK_STACK_DEPTH];

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype
void __Patch_EntryPoint(void) __attribute__((section("ENTRY_POINT")));
//static void __Patch_EntryPoint(void) __attribute__((used));
static void Main_FlashLayoutUpdate(void);
static void Main_AppInit_patch(void);


/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   __Patch_EntryPoint
*
* DESCRIPTION:
*   the entry point of SW patch
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void __Patch_EntryPoint(void)
{
    // don't remove this code
    SysInit_EntryPoint();
    
    // update the flash layout
    MwFim_FlashLayoutUpdate = Main_FlashLayoutUpdate;
    
    // modify the heap size, from 0x43C000 to 0x44F000
    g_ucaMemPartAddr = (uint8_t*) 0x43D000;
    g_ulMemPartTotalSize = 0x12000; //0x44F000 - 0x43E000
    //g_xaMemoryTable[0].ulBlockSize = 44;
    //g_xaMemoryTable[0].ulBlockNum = 8;
    //g_xaMemoryTable[1].ulBlockNum = 16;
    g_xaMemoryTable[2].ulBlockNum = 0;
    g_xaMemoryTable[3].ulBlockNum = 0;
    g_xaMemoryTable[4].ulBlockNum = 0;
    g_xaMemoryTable[5].ulBlockNum = 0;   

    Sys_SetUnsuedSramEndBound(0x43D000);

    /* Adjust timer task stack size */
    g_taTimerTaskStackBufferAddr = g_taTimerTaskStackBufferBody_new;
    g_ulTimerTaskStackSize = OS_TIMER_TASK_STACK_DEPTH;

    
    // application init
    Sys_AppInit = Main_AppInit_patch;
}

/*************************************************************************
* FUNCTION:
*   Main_FlashLayoutUpdate
*
* DESCRIPTION:
*   update the flash layout
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_FlashLayoutUpdate(void)
{
    // update here
}

/*************************************************************************
* FUNCTION:
*   Main_AppInit_patch
*
* DESCRIPTION:
*   the initial of application
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_AppInit_patch(void)
{
	AppInit();
}
