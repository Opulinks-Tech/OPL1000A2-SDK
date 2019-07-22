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
*  hal_flash_internal.h
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

#ifndef __HAL_FLASH_INTERNAL_H__
#define __HAL_FLASH_INTERNAL_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include "hal_spi.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define NO_FLASH       0
#define GIGADEVICE_ID  0xC8 /* GigaDevice, tested in GD25Q80C */
#define MACRONIX_ID    0xC2 /* Macronix (MX), tested in MX25V8035F */
#define WINBOND_NEX_ID 0xEF /* Winbond (ex Nexcom) serial flashes, tested in W25Q80DV */
#define NULL_0_ID      0x00
#define NULL_1_ID      0xFF

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef uint32_t (*T__Hal_Flash_ManufDeviceId)(E_SpiIdx_t u32SpiIdx, uint32_t *pu32Manufacturer, uint32_t *pu32MemoryType, uint32_t *pu32MemoryDensity );
typedef uint32_t (*T__Hal_Flash_StatusGet)(E_SpiIdx_t u32SpiIdx, uint32_t *pu32Status_0, uint32_t *pu32Status_1);
typedef uint32_t (*T__Hal_Flash_WriteDoneCheck)(E_SpiIdx_t u32SpiIdx);
typedef void (*T__Hal_Flash_WriteEn)(E_SpiIdx_t u32SpiIdx);
typedef uint32_t (*T__Hal_Flash_QuadModeEn)(E_SpiIdx_t u32SpiIdx, uint8_t u8QModeEn);

typedef uint32_t (*T_Hal_Flash_Init_Internal)(E_SpiIdx_t u32SpiIdx);
typedef uint32_t (*T_Hal_Flash_4KSectorAddrErase_Internal)(E_SpiIdx_t u32SpiIdx, uint32_t u32SecAddr);
typedef uint32_t (*T_Hal_Flash_4KSectorIdxErase_Internal)(E_SpiIdx_t u32SpiIdx, uint32_t u32SecIdx);
typedef uint32_t (*T_Hal_Flash_PageAddrProgram_Internal)(E_SpiIdx_t u32SpiIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data);
typedef uint32_t (*T_Hal_Flash_PageIdxProgram_Internal)(E_SpiIdx_t u32SpiIdx, uint32_t u32PageIdx, uint8_t u8UseQuadMode, uint8_t *pu8Data);
typedef uint32_t (*T_Hal_Flash_PageAddrRead_Internal)(E_SpiIdx_t u32SpiIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data);
typedef uint32_t (*T_Hal_Flash_AddrProgram_Internal)(E_SpiIdx_t u32SpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
typedef uint32_t (*T_Hal_Flash_AddrRead_Internal)(E_SpiIdx_t u32SpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
typedef void (*T_Hal_Flash_Reset_Internal)(E_SpiIdx_t u32SpiIdx);

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
extern T__Hal_Flash_ManufDeviceId  _Hal_Flash_ManufDeviceId;
extern T__Hal_Flash_StatusGet      _Hal_Flash_StatusGet;
extern T__Hal_Flash_WriteDoneCheck _Hal_Flash_WriteDoneCheck;
extern T__Hal_Flash_WriteEn        _Hal_Flash_WriteEn;
extern T__Hal_Flash_QuadModeEn     _Hal_Flash_QuadModeEn;

extern T_Hal_Flash_Init_Internal              Hal_Flash_Init_Internal;
extern T_Hal_Flash_4KSectorAddrErase_Internal Hal_Flash_4KSectorAddrErase_Internal;
extern T_Hal_Flash_4KSectorIdxErase_Internal  Hal_Flash_4KSectorIdxErase_Internal;
extern T_Hal_Flash_PageAddrProgram_Internal   Hal_Flash_PageAddrProgram_Internal;
extern T_Hal_Flash_PageIdxProgram_Internal    Hal_Flash_PageIdxProgram_Internal;
extern T_Hal_Flash_PageAddrRead_Internal      Hal_Flash_PageAddrRead_Internal;
extern T_Hal_Flash_AddrProgram_Internal       Hal_Flash_AddrProgram_Internal;
extern T_Hal_Flash_AddrRead_Internal          Hal_Flash_AddrRead_Internal;
extern T_Hal_Flash_Reset_Internal             Hal_Flash_Reset_Internal;

#endif

