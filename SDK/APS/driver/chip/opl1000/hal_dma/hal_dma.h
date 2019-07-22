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
*  hal_dma.h
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

#ifndef __HAL_DMA_H__
#define __HAL_DMA_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include <stdint.h>

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef void (*T_Hal_Dma_Init)(void);
typedef void (*T_Hal_Dma_Transfer)(uint32_t u32RdAddr, uint32_t u32WrAddr, uint32_t u32DataLen);
typedef void (*T_Hal_Dma_IntClear)(void);


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
extern T_Hal_Dma_Init Hal_Dma_Init;
extern T_Hal_Dma_Transfer Hal_Dma_Transfer;
extern T_Hal_Dma_IntClear Hal_Dma_IntClear;

void Hal_Dma_Pre_Init(void);

#endif
