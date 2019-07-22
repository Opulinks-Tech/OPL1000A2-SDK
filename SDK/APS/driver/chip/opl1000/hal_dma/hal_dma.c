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
*  hal_dma.c
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of DMA functions
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
#include "hal_dma.h"
#include "hal_vic.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define DMA                ((S_Dma_Reg_t *) DMA_BASE)

#define DMA_CMD_SET_INT    0x01
#define DMA_CMD_LAST       0x02
#define DMA_CMD_CLEAR_INT  0x04
#define DMA_EN             (1<<30)

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef struct
{
	volatile uint32_t  RDADDR;              // 0x000
    volatile uint32_t  WRADDR;              // 0x004
    volatile uint32_t  CMDCTRL;             // 0x008
    volatile uint32_t  DATALEN;             // 0x00C
} S_Dma_Reg_t;

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
RET_DATA T_Hal_Dma_Init Hal_Dma_Init;
RET_DATA T_Hal_Dma_Transfer Hal_Dma_Transfer;
RET_DATA T_Hal_Dma_IntClear Hal_Dma_IntClear;

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
*  Hal_Dma_Init
*
* DESCRIPTION:
* This function is mainly used to initialize the DMA hardware.
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
void Hal_Dma_Init_impl(void)
{
    Hal_Sys_ApsModuleRst(APS_RST_DMA);
    Hal_Sys_ApsClkEn(1, APS_CLK_DMA);

    // VIC 1) Clear interrupt
    Hal_Vic_IntClear(DMA_IRQn);
    // VIC 2) un-Mask VIC
    Hal_Vic_IntMask(DMA_IRQn, 0);
    // VIC 3) Enable VIC
    Hal_Vic_IntEn(DMA_IRQn, 1);

    // NVIC 1) Clean NVIC
    NVIC_ClearPendingIRQ(DMA_IRQn);
    // NVIC 2) Set prority
    NVIC_SetPriority(DMA_IRQn, IRQ_PRIORITY_DMA);
    // NVIC 3) Enable NVIC
    NVIC_EnableIRQ(DMA_IRQn);
}

/*************************************************************************
* FUNCTION:
*  Hal_Dma_Transfer
*
* DESCRIPTION:
* This function is mainly use DMA to transfer data.
*
* CALLS
*
* PARAMETERS
*   1. u32RdAddr: Read data address
*   2. u32WrAddr: Write data address
*   3. u32DataLen: Data lenth to transfer
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Dma_Transfer_impl(uint32_t u32RdAddr, uint32_t u32WrAddr, uint32_t u32DataLen)
{
    DMA->RDADDR = u32RdAddr;
    DMA->WRADDR = u32WrAddr;
    DMA->CMDCTRL = (DMA_CMD_SET_INT | DMA_CMD_LAST);
    DMA->DATALEN = (DMA_EN | u32DataLen);
}

/*************************************************************************
* FUNCTION:
*  Hal_Dma_IntClear
*
* DESCRIPTION:
*   clear the DMA interrupt
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
void Hal_Dma_IntClear_impl(void)
{
    DMA->CMDCTRL = DMA->CMDCTRL | DMA_CMD_CLEAR_INT;
}

/*************************************************************************
* FUNCTION:
*  Hal_Dma_Pre_Init
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
void Hal_Dma_Pre_Init(void)
{
    Hal_Dma_Init = Hal_Dma_Init_impl;
    Hal_Dma_Transfer = Hal_Dma_Transfer_impl;
    Hal_Dma_IntClear = Hal_Dma_IntClear_impl;
}
