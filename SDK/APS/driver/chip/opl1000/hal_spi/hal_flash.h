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
*  hal_flash.h
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of flash .
*
*  Author:
*  -------
*  Chung-chun Wang
******************************************************************************/

#ifndef __HAL_FLASH_H__
#define __HAL_FLASH_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include "hal_spi.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef enum 
{
    NOT_SUPPORTED,
    SUPPORTED_FLASH
} E_FlashCheckStatus_t;


typedef uint32_t (*T_Hal_Flash_Init)(E_SpiIdx_t u32SpiIdx);
typedef uint32_t (*T_Hal_Flash_4KSectorAddrErase)(E_SpiIdx_t u32SpiIdx, uint32_t u32SecAddr);
typedef uint32_t (*T_Hal_Flash_4KSectorIdxErase)(E_SpiIdx_t u32SpiIdx, uint32_t u32SecIdx);
typedef uint32_t (*T_Hal_Flash_PageAddrProgram)(E_SpiIdx_t u32SpiIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data);
typedef uint32_t (*T_Hal_Flash_PageIdxProgram)(E_SpiIdx_t u32SpiIdx, uint32_t u32PageIdx, uint8_t u8UseQuadMode, uint8_t *pu8Data);
typedef uint32_t (*T_Hal_Flash_PageAddrRead)(E_SpiIdx_t u32SpiIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data);
typedef uint32_t (*T_Hal_Flash_AddrProgram)(E_SpiIdx_t u32SpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
typedef uint32_t (*T_Hal_Flash_AddrRead)(E_SpiIdx_t u32SpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
typedef void (*T_Hal_Flash_Reset)(E_SpiIdx_t u32SpiIdx);
typedef E_FlashCheckStatus_t (*T_Hal_Flash_Check)(E_SpiIdx_t u32SpiIdx);

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
extern T_Hal_Flash_Init              Hal_Flash_Init;
extern T_Hal_Flash_4KSectorAddrErase Hal_Flash_4KSectorAddrErase;
extern T_Hal_Flash_4KSectorIdxErase  Hal_Flash_4KSectorIdxErase;
extern T_Hal_Flash_PageAddrProgram   Hal_Flash_PageAddrProgram;
extern T_Hal_Flash_PageIdxProgram    Hal_Flash_PageIdxProgram;
extern T_Hal_Flash_PageAddrRead      Hal_Flash_PageAddrRead;
extern T_Hal_Flash_AddrProgram       Hal_Flash_AddrProgram;
extern T_Hal_Flash_AddrRead          Hal_Flash_AddrRead;
extern T_Hal_Flash_Reset             Hal_Flash_Reset;
extern T_Hal_Flash_Check             Hal_Flash_Check;

// For cold boot init. include function poiters and parameters
void Hal_Flash_Pre_Init(void);

// For warm boot init. only parameters
void Hal_Flash_ParamInit(void);

#endif

