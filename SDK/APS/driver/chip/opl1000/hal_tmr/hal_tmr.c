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
*  hal_tmr.c
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of Timer functions
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
#include "hal_tmr.h"
#include "hal_vic.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define TMR0                ((S_Tmr_Reg_t *) TIM0_BASE)
#define TMR1                ((S_Tmr_Reg_t *) TIM1_BASE)

#define TIMER_MAX_VALUE     0xFFFFFFFF
#define TIMER_TICKS_US      XTAL_PER_US
#define TIMER_RATIO         8   // !!!
                                // the clock is from external, it is fixed to Xtal, not SystemCoreClockGet()
                                // the Xtal is 22MHz, and the clock is divided by 4
                                // if TIMER_CTRL_EXT_CLOCK is set to 1, the clock will be divided by 2
                                // the total divided ratio is 8

#define TIMER_CTRL_EN             (0x01 << 0)
#define TIMER_CTRL_EXT_ENABLE     (0x01 << 1)
#define TIMER_CTRL_EXT_CLOCK      (0x01 << 2)
#define TIMER_CTRL_IRQEN          (0x01 << 3)


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef struct
{
	volatile uint32_t  CTRL;              // 0x000
	volatile uint32_t  VALUE;             // 0x004
	volatile uint32_t  LOAD;              // 0x008
	volatile uint32_t  INTSTATUS;         // 0x00C
} S_Tmr_Reg_t;

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
RET_DATA uint32_t Tmr_TickOfUs;
RET_DATA uint32_t Tmr_Ratio;
RET_DATA T_Tmr_CallBack Tmr_CallBack[2];

RET_DATA T_Hal_Tmr_Init Hal_Tmr_Init;
RET_DATA T_Hal_Tmr_Start Hal_Tmr_Start;
RET_DATA T_Hal_Tmr_Stop Hal_Tmr_Stop;
RET_DATA T_Hal_Tmr_Reload Hal_Tmr_Reload;
RET_DATA T_Hal_Tmr_ReadCurrValue Hal_Tmr_ReadCurrValue;
RET_DATA T_Hal_Tmr_IntClear Hal_Tmr_IntClear;
RET_DATA T_Hal_Tmr_CallBackFuncSet Hal_Tmr_CallBackFuncSet;

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
*  Hal_Tmr_Init
*
* DESCRIPTION:
* This function is mainly used to initialize the Timer0 or Timer1 hardware.
*
* CALLS
*
* PARAMETERS
*   1. u32Timer: timer number 0 or 1
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Tmr_Init_impl(uint32_t u32Timer)
{
    S_Tmr_Reg_t *pTmer = 0;
    IRQn_Type eIrq = IRQn_MAX;
    uint32_t u32IrqPriorty = 0xFF;

    if(u32Timer == 0)
    {
        Hal_Sys_ApsModuleRst(APS_RST_TIMER_0);
        Hal_Sys_ApsClkEn(1, APS_CLK_TIMER_0);

        pTmer = TMR0;
        eIrq = TIM0_IRQn;
        u32IrqPriorty = IRQ_PRIORITY_TIM0;
    }
    else if(u32Timer == 1)
    {
        Hal_Sys_ApsModuleRst(APS_RST_TIMER_1);
        Hal_Sys_ApsClkEn(1, APS_CLK_TIMER_1);

        pTmer = TMR1;
        eIrq = TIM1_IRQn;
        u32IrqPriorty = IRQ_PRIORITY_TIM1;
    }
    else
    {
        return;
    }
    
    // Clean module parts
    pTmer->INTSTATUS = 1;

    // VIC 1) Clear interrupt
    Hal_Vic_IntClear(eIrq);
    // VIC 2) Enable VIC
    Hal_Vic_IntEn(eIrq, 1);
    // VIC 3) un-Mask VIC
    Hal_Vic_IntMask(eIrq, 0);

    // NVIC 1) Clean NVIC
    NVIC_ClearPendingIRQ(eIrq);
    // NVIC 2) Enable NVIC
    NVIC_EnableIRQ(eIrq);
    // NVIC 3) Set priority
    NVIC_SetPriority(eIrq, u32IrqPriorty);
}

/*************************************************************************
* FUNCTION:
*  Hal_Tmr_Start
*
* DESCRIPTION:
* This function is mainly used to start the Timer0 or Timer1 hardware.
*
* CALLS
*
* PARAMETERS
*   1. u32Timer: timer number 0 or 1
*	2. u32Us: timer reload value in us
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Tmr_Start_impl(uint32_t u32Timer, uint32_t u32Us)
{
    S_Tmr_Reg_t *pTmer = 0;
    uint32_t reload = 0;

    if(u32Timer == 0)
    {
        pTmer = TMR0;

    }else if(u32Timer == 1)
    {
        pTmer = TMR1;
    }else{
        return;
    }
    
    // error handle
    if (u32Us == 0)
        return;

    if (u32Us > (TIMER_MAX_VALUE/Tmr_TickOfUs*Tmr_Ratio))
        reload = TIMER_MAX_VALUE;
    else if (u32Us == 0)
        reload = TIMER_MAX_VALUE;
    else
        reload = (u32Us*Tmr_TickOfUs + Tmr_Ratio/2) / Tmr_Ratio - 1;
    
    pTmer->CTRL = 0;
    pTmer->VALUE = 0;
    pTmer->LOAD = reload;
    pTmer->CTRL |= (TIMER_CTRL_EN | TIMER_CTRL_IRQEN | TIMER_CTRL_EXT_CLOCK);
}

/*************************************************************************
* FUNCTION:
*  Hal_Tmr_Stop
*
* DESCRIPTION:
* This function is mainly used to stop the Timer0 or Timer1 hardware.
*
* CALLS
*
* PARAMETERS
*   1. u32Timer: timer number 0 or 1
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Tmr_Stop_impl(uint32_t u32Timer)
{
    switch(u32Timer) {
        case 0:
        	TMR0->CTRL = 0;
            break;
        case 1:
        	TMR1->CTRL = 0;
            break;
        default: 
        	break;
    }
}

/*************************************************************************
* FUNCTION:
*  Hal_Tmr_Reload
*
* DESCRIPTION:
* This function is mainly used to reload the Timer0 or Timer1 hardware.
*
* CALLS
*
* PARAMETERS
*   1. u32Timer: timer number 0 or 1
*   2. u32Us: timer reload value in us
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Tmr_Reload_impl(uint32_t u32Timer, uint32_t u32Us)
{
    S_Tmr_Reg_t *pTmer = 0;
    uint32_t reload = 0;

    if(u32Timer == 0)
    {
        pTmer = TMR0;

    }else if(u32Timer == 1)
    {
        pTmer = TMR1;
    }else{
        return;
    }
    
    // error handle
    if (u32Us == 0)
        return;

    if (u32Us > (TIMER_MAX_VALUE/Tmr_TickOfUs*Tmr_Ratio))
        reload = TIMER_MAX_VALUE;
    else if (u32Us == 0)
        reload = TIMER_MAX_VALUE;
    else
        reload = (u32Us*Tmr_TickOfUs + Tmr_Ratio/2) / Tmr_Ratio - 1;
    
    pTmer->LOAD = reload;
    pTmer->VALUE = 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Tmr_ReadCurrValue
*
* DESCRIPTION:
* This function is mainly used to read the Timer0 or Timer1 value.
*
* CALLS
*
* PARAMETERS
*   1. u32Timer: timer number 0 or 1
*
* RETURNS
*   timer value us
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Tmr_ReadCurrValue_impl(uint32_t u32Timer)
{
    switch(u32Timer) {
        case 0:
            return (TMR0->VALUE / Tmr_TickOfUs * Tmr_Ratio);
        case 1:
            return (TMR1->VALUE / Tmr_TickOfUs * Tmr_Ratio);
        default: 
        	break;
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Tmr_IntClear
*
* DESCRIPTION:
* This function is mainly used to clear the Timer0 or Timer1 interrupt.
*
* CALLS
*
* PARAMETERS
*   1. u32Timer: timer number 0 or 1
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Tmr_IntClear_impl(uint32_t u32Timer)
{
    S_Tmr_Reg_t *pTmer = 0;

    if(u32Timer == 0)
    {
        pTmer = TMR0;
    }else if(u32Timer == 1)
    {
        pTmer = TMR1;
    }else{
        return;
    }

    // Clean module parts
    pTmer->INTSTATUS = 1;
}

/*************************************************************************
* FUNCTION:
*  Hal_Tmr_CallBackFuncSet
*
* DESCRIPTION:
*   1. Setup timer callback function
*
* CALLS
*
* PARAMETERS
*   1. u32Timer: timer number 0 or 1
*   2. tFunc: Callback function
*
* RETURNS
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Tmr_CallBackFuncSet_impl(uint32_t u32Timer, T_Tmr_CallBack tFunc)
{
    if(u32Timer == 0)
    {
        Tmr_CallBack[0] = tFunc;
    }else if(u32Timer == 1)
    {
        Tmr_CallBack[1] = tFunc;
    }else{
        return;
    }
}

/*************************************************************************
* FUNCTION:
*  Hal_Tmr_Pre_Init
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
void Hal_Tmr_Pre_Init(void)
{
    Tmr_TickOfUs = TIMER_TICKS_US;
    Tmr_Ratio = TIMER_RATIO;
    Tmr_CallBack[0] = NULL;
    Tmr_CallBack[1] = NULL;
    
    Hal_Tmr_Init = Hal_Tmr_Init_impl;
    Hal_Tmr_Start = Hal_Tmr_Start_impl;
    Hal_Tmr_Stop = Hal_Tmr_Stop_impl;
    Hal_Tmr_Reload = Hal_Tmr_Reload_impl;
    Hal_Tmr_ReadCurrValue = Hal_Tmr_ReadCurrValue_impl;
    Hal_Tmr_IntClear = Hal_Tmr_IntClear_impl;
    Hal_Tmr_CallBackFuncSet = Hal_Tmr_CallBackFuncSet_impl;
}
