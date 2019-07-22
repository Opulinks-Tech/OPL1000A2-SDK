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

/******************************************************************************
*  Filename:
*  ---------
*  hal_wdt.c
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of Watchdog functions
*
*  Author:
*  -------
*  Luke Liao
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "opl1000.h"
#include "hal_system.h"
#include "hal_wdt.h"
#include "hal_vic.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define WATCHDOG                ((S_Wdt_Reg_t *) WDT_BASE)

/* KEY to unlock/lock the Watchdog registers */
#define WDT_UNLOCK_KEY          0x1ACCE551
#define WDT_LOCK_KEY            0x00000001

/* Register field definitions */
#define WDT_CTL_RESEN           ((uint32_t)1 << 1)    /*reset enable bit */
#define WDT_CTL_INTEN           ((uint32_t)1 << 0)    /*interrupt and counter enable bit */

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef struct
{
    volatile uint32_t  LOAD;              // 0x000
    volatile uint32_t  VALUE;             // 0x004
    volatile uint32_t  CTRL;              // 0x008
    volatile uint32_t  INTCLR;            // 0x00C
    volatile uint32_t  RAWINTSTAT;        // 0x010
    volatile uint32_t  MASKINTSTAT;       // 0x014
    volatile uint32_t  RESERVED0[762];
    volatile uint32_t  LOCK;              // 0xC00
} S_Wdt_Reg_t;

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
RET_DATA T_Hal_Wdt_Init Hal_Wdt_Init;
RET_DATA T_Hal_Wdt_InitForInt Hal_Wdt_InitForInt;
RET_DATA T_Hal_Wdt_Start Hal_Wdt_Start;
RET_DATA T_Hal_Wdt_Stop Hal_Wdt_Stop;
RET_DATA T_Hal_Wdt_Feed Hal_Wdt_Feed;
RET_DATA T_Hal_Wdt_Clear Hal_Wdt_Clear;

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable

// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*  Hal_Wdt_Init
*
* DESCRIPTION:
* This function is mainly used to initialize the watchdog hardware.
* It is used to set the WDT to either reset or interrupt modes.
* CALLS
*
* PARAMETERS
*   1. u32Ticks: number of clock tick to trigger WDT timeout
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Wdt_Init_impl(uint32_t u32Ticks)
{
    // Enable module
    Hal_Sys_ApsClkEn(1, APS_CLK_WDT);
    // Reset module
    Hal_Sys_ApsModuleRst(APS_RST_WDT);

    WATCHDOG->LOCK = WDT_UNLOCK_KEY;
    WATCHDOG->LOAD = u32Ticks;
    WATCHDOG->CTRL = (WDT_CTL_INTEN | WDT_CTL_RESEN); // Enable watchdog reset output
    WATCHDOG->LOCK = WDT_LOCK_KEY;

    // VIC 1) Clear interrupt
    Hal_Vic_IntClear(WDT_IRQn);
    // VIC 2) un-Mask VIC
    Hal_Vic_IntMask(WDT_IRQn, 0);
    // VIC 3) Enable VIC
    Hal_Vic_IntEn(WDT_IRQn, 1);

    // NVIC 1) Clean NVIC
    NVIC_ClearPendingIRQ(WDT_IRQn);
    // NVIC 2) Set prority
    NVIC_SetPriority(WDT_IRQn, IRQ_PRIORITY_WDT);
    // NVIC 3) Enable NVIC
    NVIC_EnableIRQ(WDT_IRQn);
}

/*************************************************************************
* FUNCTION:
*  Hal_Wdt_InitForInt
*
* DESCRIPTION:
* This function is mainly used to initialize the watchdog hardware.
* It is used to set the WDT to either reset or interrupt modes.
*
* CALLS
*
* PARAMETERS
*   1. u32Ticks: number of clock tick to trigger WDT timeout
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Wdt_InitForInt_impl(uint32_t u32Ticks)
{
    // Enable module
    Hal_Sys_ApsClkEn(1, APS_CLK_WDT);
    // Reset module
    Hal_Sys_ApsModuleRst(APS_RST_WDT);

    WATCHDOG->LOCK = WDT_UNLOCK_KEY;
    WATCHDOG->LOAD = u32Ticks;
    WATCHDOG->CTRL = WDT_CTL_INTEN; // Enable watchdog interrupt mode
    WATCHDOG->LOCK = WDT_LOCK_KEY;

    // VIC 1) Clear interrupt
    Hal_Vic_IntClear(WDT_IRQn);
    // VIC 2) un-Mask VIC
    Hal_Vic_IntMask(WDT_IRQn, 0);
    // VIC 3) Enable VIC
    Hal_Vic_IntEn(WDT_IRQn, 1);

    // NVIC 1) Clean NVIC
    NVIC_ClearPendingIRQ(WDT_IRQn);
    // NVIC 2) Set prority
    NVIC_SetPriority(WDT_IRQn, IRQ_PRIORITY_WDT);
    // NVIC 3) Enable NVIC
    NVIC_EnableIRQ(WDT_IRQn);
}

/*************************************************************************
* FUNCTION:
*  Hal_Wdt_Start
*
* DESCRIPTION:
* Enable the watchdog timer.
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Wdt_Start_impl(void)
{
    WATCHDOG->LOCK  = WDT_UNLOCK_KEY;
    WATCHDOG->CTRL |= WDT_CTL_INTEN;
    WATCHDOG->LOCK  = WDT_LOCK_KEY;
}

/*************************************************************************
* FUNCTION:
*  Hal_Wdt_Stop
*
* DESCRIPTION:
* Disable the watchdog timer.
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Wdt_Stop_impl(void)
{
    WATCHDOG->LOCK = WDT_UNLOCK_KEY;
    WATCHDOG->CTRL &= ~(WDT_CTL_RESEN | WDT_CTL_INTEN);
    WATCHDOG->LOCK = WDT_LOCK_KEY;
}

/*************************************************************************
* FUNCTION:
*  Hal_Wdt_Feed
*
* DESCRIPTION:
* This function is used to feed the watchdog (restart the watchdog timer)
* and the count is immediately restarted from the new value.
* To avoid the WDT from expiring, this function should be called regularly.
*
* CALLS
*
* PARAMETERS
*   1. u32Ticks: number of clock tick to trigger WDT timeout
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Wdt_Feed_impl(uint32_t u32Ticks)
{
    WATCHDOG->LOCK = WDT_UNLOCK_KEY;
    WATCHDOG->LOAD = u32Ticks;
    WATCHDOG->LOCK = WDT_LOCK_KEY;
}

/*************************************************************************
* FUNCTION:
*  Hal_Wdt_Clear
*
* DESCRIPTION:
* This function is used to clear watchdog interrupt & reload conter
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Wdt_Clear_impl(void)
{
    WATCHDOG->LOCK = WDT_UNLOCK_KEY;
    /* write any value to clear wdt interrupt & reload wdt counter */
    WATCHDOG->INTCLR = 0x94539453;
    WATCHDOG->LOCK = WDT_LOCK_KEY;
}

/*************************************************************************
* FUNCTION:
*  Hal_Wdt_Pre_Init
*
* DESCRIPTION:
*   1. Inital of functuin pointers
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Wdt_Pre_Init(void)
{
    Hal_Wdt_Init = Hal_Wdt_Init_impl;
    Hal_Wdt_InitForInt = Hal_Wdt_InitForInt_impl;
    Hal_Wdt_Start = Hal_Wdt_Start_impl;
    Hal_Wdt_Stop = Hal_Wdt_Stop_impl;
    Hal_Wdt_Feed = Hal_Wdt_Feed_impl;
    Hal_Wdt_Clear = Hal_Wdt_Clear_impl;
}
