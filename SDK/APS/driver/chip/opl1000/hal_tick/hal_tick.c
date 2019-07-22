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
*  hal_tick.c
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This source file defines the functions of tick .
*
*  Author:
*  -------
*  Chung-Chun Wang
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "opl1000.h"
#include "hal_tick.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define DWT_LOAD_MAX 0xFFFFFFFF

#define CORE_DWT     1 /* M3 used DWT */
#define SYS_TICK     2 /* M0 used SYS_TICK */
#define TICK_SRC     CORE_DWT

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable


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
*  Hal_Tick_Init
* 
* DESCRIPTION:
*   1. Init tick
* 
* CALLS
*   None
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
void Hal_Tick_Init(void)
{
#if( TICK_SRC == CORE_DWT)
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk))
	{
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CYCCNT = 0;
		DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	}
#else
    if (!(SysTick->CTRL & SysTick_CTRL_ENABLE_Msk))
	{
	    SysTick->LOAD = SysTick_LOAD_RELOAD_Msk;
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	}
#endif
}

/*************************************************************************
* FUNCTION:
*  Hal_Tick_Diff
* 
* DESCRIPTION:
*   1. Get diff between current and given base
* 
* CALLS
*   None
* 
* PARAMETERS
*   1. u32Base : Base time to diff with current
*
* RETURNS
*   uint32_t : Diff time
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Tick_Diff(uint32_t u32Base)
{
    uint32_t u32Curr = 0;

#if( TICK_SRC == CORE_DWT)
    // DWT is upper counter
    u32Curr = DWT->CYCCNT;

    if(u32Curr >= u32Base) {
    	return u32Curr - u32Base;
    } else {
        return (DWT_LOAD_MAX - u32Base + u32Curr + 1);
    }
#else
    // SYS_TICK is down counter
    u32Curr = SysTick_LOAD_RELOAD_Msk - SysTick->VAL;

    if(u32Curr >= u32Base) {
    	return u32Curr - u32Base;
    } else {
        return (SysTick_LOAD_RELOAD_Msk - u32Base + u32Curr + 1);
    }
#endif
}

/*************************************************************************
* FUNCTION:
*  Hal_Tick_DiffEx
* 
* DESCRIPTION:
*   1. Get diff between current and given base
*   2. Get the current tick, too
* 
* CALLS
*   None
* 
* PARAMETERS
*   1. u32Base : Base time to diff with current
*   2. pu32Current : [Out] feedback the current tick
*
* RETURNS
*   uint32_t : Diff time
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Tick_DiffEx(uint32_t u32Base, uint32_t *pu32Current)
{
    uint32_t u32Curr = 0;

#if( TICK_SRC == CORE_DWT)
    // DWT is upper counter
    u32Curr = DWT->CYCCNT;
    *pu32Current = u32Curr;

    if(u32Curr >= u32Base) {
    	return u32Curr - u32Base;
    } else {
        return (DWT_LOAD_MAX - u32Base + u32Curr + 1);
    }
#else
    // SYS_TICK is down counter
    u32Curr = SysTick_LOAD_RELOAD_Msk - SysTick->VAL;
    *pu32Current = u32Curr;

    if(u32Curr >= u32Base) {
    	return u32Curr - u32Base;
    } else {
        return (SysTick_LOAD_RELOAD_Msk - u32Base + u32Curr + 1);
    }
#endif
}

/*************************************************************************
* FUNCTION:
*  Hal_Tick_PerMilliSec
* 
* DESCRIPTION:
*   1. Get number of ticks in 1 ms
* 
* CALLS
*   None
* 
* PARAMETERS
*   None
*
* RETURNS
*   uint32_t : Number of ticks
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Tick_PerMilliSec(void)
{
    return SystemCoreClockGet()/1000;
}

/*************************************************************************
* FUNCTION:
*  Hal_Tick_MilliSecMax
* 
* DESCRIPTION:
*   1. Get max resolution
* 
* CALLS
*   None
* 
* PARAMETERS
*   None
*
* RETURNS
*   uint32_t : Number of ms
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Tick_MilliSecMax(void)
{
#if( TICK_SRC == CORE_DWT)
    return DWT_LOAD_MAX/Hal_Tick_PerMilliSec();
#else
    return SysTick_LOAD_RELOAD_Msk /Hal_Tick_PerMilliSec();
#endif
}

