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
*  hal_flash.c
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This source file defines the functions of flash.
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
#include "hal_flash.h"
#include "hal_flash_internal.h"
#include "cmsis_os.h"
#include "boot_sequence.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define DUMMY          0x00
#define FLASH_TIMEOUT  0x5000

#define SPI_0     ((S_Spi_Reg_t *) SPI0_BASE)
#define SPI_1     ((S_Spi_Reg_t *) SPI1_BASE)
#define SPI_2     ((S_Spi_Reg_t *) SPI2_BASE)

#define SPI_SR_RX_NOT_EMPTY          (1<<3)
#define SPI_SR_TX_NOT_FULL           (1<<1)
#define SPI_TIMEOUT 0x5000

typedef struct
{
    volatile uint32_t CTRLR0;  // 0x00
	volatile uint32_t CTRLR1;  // 0x04
	volatile uint32_t SSIENR;  // 0x08
	volatile uint32_t MWCR;    // 0x0C
	volatile uint32_t SER;     // 0x10
	volatile uint32_t BAUDR;   // 0x14
	volatile uint32_t TXFTLR;  // 0x18
	volatile uint32_t RXFTLR;  // 0x1C
	volatile uint32_t TXFLR;   // 0x20
	volatile uint32_t RXFLR;   // 0x24
	volatile uint32_t SR;      // 0x28
	volatile uint32_t IMR;     // 0x2C
	volatile uint32_t ISR;     // 0x30
	volatile uint32_t PISR;    // 0x34
	volatile uint32_t TXOICR;  // 0x38
	volatile uint32_t RXOICR;  // 0x3C
	volatile uint32_t RXUICR;  // 0x40
	volatile uint32_t MSTICR;  // 0x44
	volatile uint32_t ICR;     // 0x48
	volatile uint32_t DMACR;   // 0x4C
	volatile uint32_t DMATDLR; // 0x50
	volatile uint32_t DMARDLR; // 0x54
	volatile uint32_t IDR;     // 0x58
	volatile uint32_t SSI_VER; // 0x5C
	volatile uint32_t DR[36];  // 0x60 ~ 0xEC
} S_Spi_Reg_t;

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
RET_DATA uint8_t g_u8aHalFlashID[SPI_IDX_MAX];
RET_DATA osSemaphoreId g_taHalFlashSemaphoreId[SPI_IDX_MAX];

RET_DATA T__Hal_Flash_ManufDeviceId  _Hal_Flash_ManufDeviceId;
RET_DATA T__Hal_Flash_StatusGet      _Hal_Flash_StatusGet;
RET_DATA T__Hal_Flash_WriteDoneCheck _Hal_Flash_WriteDoneCheck;
RET_DATA T__Hal_Flash_WriteEn        _Hal_Flash_WriteEn;
RET_DATA T__Hal_Flash_QuadModeEn     _Hal_Flash_QuadModeEn;

RET_DATA T_Hal_Flash_Init_Internal              Hal_Flash_Init_Internal;
RET_DATA T_Hal_Flash_4KSectorAddrErase_Internal Hal_Flash_4KSectorAddrErase_Internal;
RET_DATA T_Hal_Flash_4KSectorIdxErase_Internal  Hal_Flash_4KSectorIdxErase_Internal;
RET_DATA T_Hal_Flash_PageAddrProgram_Internal   Hal_Flash_PageAddrProgram_Internal;
RET_DATA T_Hal_Flash_PageIdxProgram_Internal    Hal_Flash_PageIdxProgram_Internal;
RET_DATA T_Hal_Flash_PageAddrRead_Internal      Hal_Flash_PageAddrRead_Internal;
RET_DATA T_Hal_Flash_AddrProgram_Internal       Hal_Flash_AddrProgram_Internal;
RET_DATA T_Hal_Flash_AddrRead_Internal          Hal_Flash_AddrRead_Internal;
RET_DATA T_Hal_Flash_Reset_Internal             Hal_Flash_Reset_Internal;

RET_DATA T_Hal_Flash_Init              Hal_Flash_Init;
RET_DATA T_Hal_Flash_4KSectorAddrErase Hal_Flash_4KSectorAddrErase;
RET_DATA T_Hal_Flash_4KSectorIdxErase  Hal_Flash_4KSectorIdxErase;
RET_DATA T_Hal_Flash_PageAddrProgram   Hal_Flash_PageAddrProgram;
RET_DATA T_Hal_Flash_PageIdxProgram    Hal_Flash_PageIdxProgram;
RET_DATA T_Hal_Flash_PageAddrRead      Hal_Flash_PageAddrRead;
RET_DATA T_Hal_Flash_AddrProgram       Hal_Flash_AddrProgram;
RET_DATA T_Hal_Flash_AddrRead          Hal_Flash_AddrRead;
RET_DATA T_Hal_Flash_Reset             Hal_Flash_Reset;
RET_DATA T_Hal_Flash_Check             Hal_Flash_Check;

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
*  _Hal_Flash_ManufDeviceId
*
* DESCRIPTION:
*   1. Get flash Manufacturer ID, Memory Type and Memory Density
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx          : Index of SPI. refert to E_SpiIdx_t
*   2. pu32Manufacturer : Pointer to receive Manufacturer ID
*   3. pu32MemoryType   : Pointer to receive Memory Type
*   4. pu32MemoryDensity: Pointer to receive Memory Density
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Flash_ManufDeviceId_impl(E_SpiIdx_t u32SpiIdx, uint32_t *pu32Manufacturer, uint32_t *pu32MemoryType, uint32_t *pu32MemoryDensity )
{
    uint32_t u32Temp = 0;

    if(u32SpiIdx >= SPI_IDX_MAX)
        return 1;

    u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x9F;
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | DUMMY;
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | DUMMY;
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_WRITE | DUMMY;  // compelete
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    Hal_Spi_Data_Recv(u32SpiIdx, pu32Manufacturer);
    Hal_Spi_Data_Recv(u32SpiIdx, pu32MemoryType);
    Hal_Spi_Data_Recv(u32SpiIdx, pu32MemoryDensity);

    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_StatusGet
*
* DESCRIPTION:
*   1. Get the status 0 and status 1 of flash
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx          : Index of SPI. refert to E_SpiIdx_t
*   2. pu32Status_0     : Pointer to receive status 0
*   3. pu32Status_1     : Pointer to receive status 1
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Flash_StatusGet_impl(E_SpiIdx_t u32SpiIdx, uint32_t *pu32Status_0, uint32_t *pu32Status_1)
{
    uint32_t u32Temp = 0;

    if(u32SpiIdx >= SPI_IDX_MAX)
        return 1;

    // status 0: bit 0~7
    u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x05;
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_WRITE | DUMMY; // compelte
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    Hal_Spi_Data_Recv(u32SpiIdx, pu32Status_0);

    // status 1: bit 8~15
    if(g_u8aHalFlashID[u32SpiIdx] == MACRONIX_ID)
    {
        u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x15;
    }
    else
    {
        u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x35;
    }
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_WRITE | DUMMY; // compelte
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    Hal_Spi_Data_Recv(u32SpiIdx, pu32Status_1);

    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_WriteDoneCheck
*
* DESCRIPTION:
*   1. Check flash write finished
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refert to E_SpiIdx_t
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Flash_WriteDoneCheck_impl(E_SpiIdx_t u32SpiIdx)
{
    uint32_t u32Status = 0;
    uint32_t u32TimeOut = 0;
    uint32_t u32Temp = 0;

    if(u32SpiIdx >= SPI_IDX_MAX)
        return 1;

    do
    {
        if(u32TimeOut > FLASH_TIMEOUT)
            return 1;
        
        u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x05;
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
        u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_WRITE | DUMMY; // compelte
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
        
        Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
        Hal_Spi_Data_Recv(u32SpiIdx, &u32Status);
        
        u32TimeOut++;
    }while( u32Status & 0x01 );
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_WriteEn
*
* DESCRIPTION:
*   1. Enable flash to write
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refert to E_SpiIdx_t
*
* RETURNS
*   NONE
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void _Hal_Flash_WriteEn_impl(E_SpiIdx_t u32SpiIdx)
{
    uint32_t u32Temp = 0;

    if(u32SpiIdx >= SPI_IDX_MAX)
            return;

    u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_WRITE | 0x06;  // complete
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
}

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_QuadModeEn
*
* DESCRIPTION:
*   1. Enable flash into Quad mode
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx   : Index of SPI. refert to E_SpiIdx_t
*   2. u8QModeEn : Qaud-mode select. 1 for enable/0 for disable
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Flash_QuadModeEn_impl(E_SpiIdx_t u32SpiIdx, uint8_t u8QModeEn)
{
    uint32_t u32Temp = 0;
    _Hal_Flash_WriteEn(u32SpiIdx);
    
    if(g_u8aHalFlashID[u32SpiIdx] == MACRONIX_ID)
    {
        u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x01;
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
        if (u8QModeEn == 1)
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x40;
        else
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x00;
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
        u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_WRITE | 0x00; // comeplte
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    }
    else
    {
        u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x01;
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
        u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x00;
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
        if (u8QModeEn)
            u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_WRITE | 0x02; // complete
        else
            u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_WRITE | 0x00; // complete
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    }
    
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    
    return _Hal_Flash_WriteDoneCheck(u32SpiIdx);
}


/*************************************************************************
* FUNCTION:
*  Hal_Flash_Check
*
* DESCRIPTION:
*   1. Check flash is OK or not
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refert to E_SpiIdx_t
*
* RETURNS
*   refert to E_FlashCheckStatus_t
* 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
E_FlashCheckStatus_t Hal_Flash_Check_impl(E_SpiIdx_t u32SpiIdx)
{
    if(g_u8aHalFlashID[u32SpiIdx] == NO_FLASH) {
        return NOT_SUPPORTED;
    } else {
        return SUPPORTED_FLASH;
    }
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_Init
*
* DESCRIPTION:
*   1. Init flash status
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refert to E_SpiIdx_t
*
* RETURNS
*   0: setting complete (Identified SPI flash)
*   1: error  (No SPI flash)
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_Init_Internal_impl(E_SpiIdx_t u32SpiIdx)
{
    uint32_t u32Manufacturer =0, u32MemoryType=0, u32MemoryDensity=0;

    if(u32SpiIdx >= SPI_IDX_MAX)
        return 1;

    // Read flash ID
    _Hal_Flash_ManufDeviceId(u32SpiIdx, &u32Manufacturer, &u32MemoryType, &u32MemoryDensity);

    // ID Cheack
    if ((u32Manufacturer == NULL_0_ID) || (u32Manufacturer == NULL_1_ID))
    {
        // Not supported
        g_u8aHalFlashID[u32SpiIdx] = NO_FLASH;
        return 1;
    }
    else
    {
        // Suppoerted 
        g_u8aHalFlashID[u32SpiIdx] = u32Manufacturer;
    }

    // Enable Quad mode
    _Hal_Flash_QuadModeEn(u32SpiIdx, 1);

    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_4KSectorAddrErase
*
* DESCRIPTION:
*   1. Erase a sector (4 KB)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refert to E_SpiIdx_t
*   2. u32SecAddr : Address of sector (must sector aligned, LSBs truncated)
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_4KSectorAddrErase_Internal_impl(E_SpiIdx_t u32SpiIdx, uint32_t u32SecAddr)
{
    uint32_t u32Temp = 0;
    uint32_t u32Addr = 0;

    if(u32SpiIdx >= SPI_IDX_MAX)
        return 1;

    if(g_u8aHalFlashID[u32SpiIdx] == NO_FLASH)
        return 1;

    _Hal_Flash_WriteEn(u32SpiIdx);

    // Cmd
    u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x20;

    // Addr
    u32Addr = u32SecAddr & ~0xFFF; // aligned
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | ( (u32Addr>>16) & 0xFF );
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | ( (u32Addr>>8) & 0xFF );
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_WRITE | (u32Addr & 0xFF); // Complete
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    
    return _Hal_Flash_WriteDoneCheck(u32SpiIdx);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_4KSectorIdxErase
*
* DESCRIPTION:
*   1. Erase a sector (4 KB)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refert to E_SpiIdx_t
*   2. u32SecAddr : Index of sector.
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_4KSectorIdxErase_Internal_impl(E_SpiIdx_t u32SpiIdx, uint32_t u32SecIdx)
{
    return Hal_Flash_4KSectorAddrErase_Internal(u32SpiIdx, u32SecIdx*0x1000);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_PageAddrProgram
*
* DESCRIPTION:
*   1. Program(write) a page (256 B)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx      : Index of SPI. refert to E_SpiIdx_t
*   2. u32PageAddr  : Address of page. (must page aligned, LSBs truncated)
*   3. u8UseQuadMode: Qaud-mode select. 1 for enable/0 for disable
*   4. pu8Data      : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_PageAddrProgram_Internal_impl(E_SpiIdx_t u32SpiIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data)
{
    uint32_t u32Idx = 0;    
    uint32_t u32Temp = 0;
    uint32_t u32Addr = 0;

    if(u32SpiIdx >= SPI_IDX_MAX)
        return 1;

    if(g_u8aHalFlashID[u32SpiIdx] == NO_FLASH)
        return 1;
    
    _Hal_Flash_WriteEn(u32SpiIdx);

    u32Addr = u32PageAddr & ~0xFF; // aligned
    if(g_u8aHalFlashID[u32SpiIdx] == MACRONIX_ID)
    {
        if (u8UseQuadMode)
        {
            // Cmd
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x38;
            Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
            // Addr
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_4_BIT | TAG_WRITE | ( (u32Addr>>16) & 0xFF );
            Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_4_BIT | TAG_WRITE | ( (u32Addr>>8) & 0xFF );
            Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_4_BIT | TAG_WRITE | (u32Addr & 0xFF);
            Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
        }
        else
        {
            // Cmd
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x02;
            Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
            // Addr
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | ( (u32Addr>>16) & 0xFF );
            Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | ( (u32Addr>>8) & 0xFF );
            Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | (u32Addr & 0xFF);
            Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
            
        }
    }
    else
    {
        // Cmd
        if (u8UseQuadMode)
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x32;
        else
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x02;
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
        // Addr
        u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | ( (u32Addr>>16) & 0xFF );
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
        u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | ( (u32Addr>>8) & 0xFF );
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
        u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | (u32Addr & 0xFF);
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    }

    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    
    // programe data
    if (u8UseQuadMode)
    {
        for (u32Idx=0; u32Idx<256; u32Idx++)
        {
            if (u32Idx != 255)
                u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_4_BIT | TAG_WRITE | pu8Data[u32Idx];
            else
                u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_4_BIT | TAG_WRITE | pu8Data[u32Idx]; // complete
            Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
            
            Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
        }
    }
    else
    {
        for (u32Idx=0; u32Idx<256; u32Idx++)
        {
            if (u32Idx != 255)
                u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | pu8Data[u32Idx];
            else
                u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_WRITE | pu8Data[u32Idx]; // complete
            Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
            
            Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
        }
    }
    return _Hal_Flash_WriteDoneCheck(u32SpiIdx);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_PageIdxProgram
*
* DESCRIPTION:
*   1. Program(write) a page (256 B)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx      : Index of SPI. refert to E_SpiIdx_t
*   2. u32PageAddr  : Index of page.
*   3. u8UseQuadMode: Qaud-mode select. 1 for enable/0 for disable
*   4. pu8Data      : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_PageIdxProgram_Internal_impl(E_SpiIdx_t u32SpiIdx, uint32_t u32PageIdx, uint8_t u8UseQuadMode, uint8_t *pu8Data)
{
    return Hal_Flash_PageAddrProgram_Internal(u32SpiIdx, u32PageIdx*0x100, u8UseQuadMode, pu8Data);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_PageAddrRead
*
* DESCRIPTION:
*   1. Read a page (256 B) 
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx      : Index of SPI. refert to E_SpiIdx_t
*   2. u32PageAddr  : Address of page.
*   3. u8UseQuadMode: Qaud-mode select. 1 for enable/0 for disable
*   4. pu8Data      : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_PageAddrRead_Internal_impl(E_SpiIdx_t u32SpiIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data)
{
    uint32_t u32Idx = 0;
    uint32_t u32Temp = 0;

    if(u32SpiIdx >= SPI_IDX_MAX)
        return 1;

    if(g_u8aHalFlashID[u32SpiIdx] == NO_FLASH)
        return 1;

    // read cmd
    if (u8UseQuadMode)
    {
        u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x6B;
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    }
    else
    {
        u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x0B;
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    }
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    
    // Addr
    u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | ( (u32PageAddr>>16) & 0xFF );
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | ( (u32PageAddr>>8) & 0xFF );
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | (u32PageAddr & 0xFF);
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | DUMMY;
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    
    // read data
    if (u8UseQuadMode)
    {
        for (u32Idx=0; u32Idx<256; u32Idx++)
        {
            if (u32Idx != 255)
                u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_4_BIT | TAG_READ | DUMMY;
            else
                u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_4_BIT | TAG_READ | DUMMY; // complete
            Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
            
            Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp);
            pu8Data[u32Idx] = (uint8_t)( u32Temp&0xFF );
        }
    }
    else
    {
        for (u32Idx=0; u32Idx<256; u32Idx++)
        {
            if (u32Idx != 255)
                u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_READ | DUMMY;
            else
                u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_READ | DUMMY; // complete
            Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
            
            Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp);
            pu8Data[u32Idx] = (uint8_t)( u32Temp&0xFF );
        }
    }
		return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_AddrProgram
*
* DESCRIPTION:
*   1. Program(write) n bytes from the start address
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx      : Index of SPI. refert to E_SpiIdx_t
*   2. u32StartAddr : Start address
*   3. u8UseQuadMode: Qaud-mode select. 1 for enable/0 for disable
*   4. u32Size      : Data size
*   5. pu8Data      : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_AddrProgram_Internal_impl(E_SpiIdx_t u32SpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    S_Spi_Reg_t *pSpi = 0;
    uint32_t u32Idx = 0;    
    uint32_t u32Temp = 0;
    uint32_t u32QuadBit = 0;
    uint32_t u32DataAddr = 0;
    uint32_t u32DataSize = 0;
    uint32_t u32DataIdx = 0;
    
    uint32_t u32TimeOut;
    uint32_t i;

    if (u32SpiIdx == SPI_IDX_0)
	{
        pSpi = SPI_0;
	}
    else if (u32SpiIdx == SPI_IDX_1)
    {
        pSpi = SPI_1;
    }
    else if (u32SpiIdx == SPI_IDX_2)
    {
        pSpi = SPI_2;
    }
    else
    {
        return 1;
    }

    if (g_u8aHalFlashID[u32SpiIdx] == NO_FLASH)
        return 1;
    
    if (u32Size == 0)
        return 1;
    
    // 1. handle the data size of the first page if the start address is not aligned
    u32DataAddr = u32StartAddr;
    u32DataSize = 256 - (u32DataAddr & 0xFF);
    if (u32DataSize > u32Size)
        u32DataSize = u32Size;
    u32DataIdx = 0;
    
    // 2. program the data
    while (u32Size > 0)
    {
        _Hal_Flash_WriteEn(u32SpiIdx);

        if(g_u8aHalFlashID[u32SpiIdx] == MACRONIX_ID)
        {
            if (u8UseQuadMode)
            {
                // Cmd
                u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x38;
                pSpi->DR[0] = u32Temp;
                // Addr
                u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_4_BIT | TAG_WRITE | ( (u32DataAddr>>16) & 0xFF );
                pSpi->DR[0] = u32Temp;
                u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_4_BIT | TAG_WRITE | ( (u32DataAddr>>8) & 0xFF );
                pSpi->DR[0] = u32Temp;
                u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_4_BIT | TAG_WRITE | (u32DataAddr & 0xFF);
                pSpi->DR[0] = u32Temp;
            }
            else
            {
                // Cmd
                u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x02;
                pSpi->DR[0] = u32Temp;
                // Addr
                u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | ( (u32DataAddr>>16) & 0xFF );
                pSpi->DR[0] = u32Temp;
                u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | ( (u32DataAddr>>8) & 0xFF );
                pSpi->DR[0] = u32Temp;
                u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | (u32DataAddr & 0xFF);
                pSpi->DR[0] = u32Temp;
                
            }
        }
        else
        {
            // Cmd
            if (u8UseQuadMode)
                u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x32;
            else
                u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x02;
            pSpi->DR[0] = u32Temp;
            // Addr
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | ( (u32DataAddr>>16) & 0xFF );
            pSpi->DR[0] = u32Temp;
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | ( (u32DataAddr>>8) & 0xFF );
            pSpi->DR[0] = u32Temp;
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | (u32DataAddr & 0xFF);
            pSpi->DR[0] = u32Temp;
        }

        u32TimeOut = 0;
        while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
        {
            // Wait for RX not Empty...
            
            //Time-out check
            if( u32TimeOut > SPI_TIMEOUT )
                return 1;
            u32TimeOut++;
        }
        u32Temp = pSpi->DR[0];  // dummy
        
        u32TimeOut = 0;
        while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
        {
            // Wait for RX not Empty...
            
            //Time-out check
            if( u32TimeOut > SPI_TIMEOUT )
                return 1;
            u32TimeOut++;
        }
        u32Temp = pSpi->DR[0];  // dummy
        
        u32TimeOut = 0;
        while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
        {
            // Wait for RX not Empty...
            
            //Time-out check
            if( u32TimeOut > SPI_TIMEOUT )
                return 1;
            u32TimeOut++;
        }
        u32Temp = pSpi->DR[0];  // dummy
        
        u32TimeOut = 0;
        while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
        {
            // Wait for RX not Empty...
            
            //Time-out check
            if( u32TimeOut > SPI_TIMEOUT )
                return 1;
            u32TimeOut++;
        }
        u32Temp = pSpi->DR[0];  // dummy
        
        
        // programe data
        // decide the quad mode or standard mode
        if (u8UseQuadMode)
            u32QuadBit = TAG_4_BIT;
        else
            u32QuadBit = TAG_1_BIT;
            
        // 4 bytes per round
        for (u32Idx=0; (u32Idx+4)<u32DataSize; u32Idx+=4)
        {
            pSpi->DR[0] = TAG_DFS_08 | TAG_CS_CONT | u32QuadBit | TAG_WRITE | pu8Data[u32DataIdx+u32Idx];
            pSpi->DR[0] = TAG_DFS_08 | TAG_CS_CONT | u32QuadBit | TAG_WRITE | pu8Data[u32DataIdx+u32Idx+1];
            pSpi->DR[0] = TAG_DFS_08 | TAG_CS_CONT | u32QuadBit | TAG_WRITE | pu8Data[u32DataIdx+u32Idx+2];
            pSpi->DR[0] = TAG_DFS_08 | TAG_CS_CONT | u32QuadBit | TAG_WRITE | pu8Data[u32DataIdx+u32Idx+3];
            
            u32TimeOut = 0;
            while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
            {
                // Wait for RX not Empty...
                
                //Time-out check
                if( u32TimeOut > SPI_TIMEOUT )
                    return 1;
                u32TimeOut++;
            }
            u32Temp = pSpi->DR[0];  // dummy
            
            u32TimeOut = 0;
            while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
            {
                // Wait for RX not Empty...
                
                //Time-out check
                if( u32TimeOut > SPI_TIMEOUT )
                    return 1;
                u32TimeOut++;
            }
            u32Temp = pSpi->DR[0];  // dummy
            
            u32TimeOut = 0;
            while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
            {
                // Wait for RX not Empty...
                
                //Time-out check
                if( u32TimeOut > SPI_TIMEOUT )
                    return 1;
                u32TimeOut++;
            }
            u32Temp = pSpi->DR[0];  // dummy
            
            u32TimeOut = 0;
            while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
            {
                // Wait for RX not Empty...
                
                //Time-out check
                if( u32TimeOut > SPI_TIMEOUT )
                    return 1;
                u32TimeOut++;
            }
            u32Temp = pSpi->DR[0];  // dummy
        }
        
        // the rest data
        for (i=u32Idx; i<u32DataSize; i++)
        {
            if (i != (u32DataSize-1))
                u32Temp = TAG_DFS_08 | TAG_CS_CONT | u32QuadBit | TAG_WRITE | pu8Data[u32DataIdx+i];
            else
                u32Temp = TAG_DFS_08 | TAG_CS_COMP | u32QuadBit | TAG_WRITE | pu8Data[u32DataIdx+i]; // complete
            pSpi->DR[0] = u32Temp;
        }
        for (i=u32Idx; i<u32DataSize; i++)
        {
            u32TimeOut = 0;
            while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
            {
                // Wait for RX not Empty...
                
                //Time-out check
                if( u32TimeOut > SPI_TIMEOUT )
                    return 1;
                u32TimeOut++;
            }
            u32Temp = pSpi->DR[0];  // dummy
        }

        if (0 != _Hal_Flash_WriteDoneCheck(u32SpiIdx))
            return 1;
        
        // update the next size and address
        u32DataIdx += u32DataSize;
        u32DataAddr += u32DataSize;
        u32Size -= u32DataSize;
        if (u32Size > 256)
            u32DataSize = 256;
        else
            u32DataSize = u32Size;
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_AddrRead
*
* DESCRIPTION:
*   1. Read n bytes from the start address
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx      : Index of SPI. refert to E_SpiIdx_t
*   2. u32StartAddr : Start address
*   3. u8UseQuadMode: Qaud-mode select. 1 for enable/0 for disable
*   4. u32Size      : Data size
*   5. pu8Data      : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_AddrRead_Internal_impl(E_SpiIdx_t u32SpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    S_Spi_Reg_t *pSpi = 0;
    uint32_t u32Idx = 0;
    uint32_t u32Temp = 0;
    uint32_t u32QuadBit = 0;
    
    uint32_t u32TimeOut;
    uint32_t i;

    if (u32SpiIdx == SPI_IDX_0)
	{
        pSpi = SPI_0;
	}
    else if (u32SpiIdx == SPI_IDX_1)
    {
        pSpi = SPI_1;
    }
    else if (u32SpiIdx == SPI_IDX_2)
    {
        pSpi = SPI_2;
    }
    else
    {
        return 1;
    }

    if (g_u8aHalFlashID[u32SpiIdx] == NO_FLASH)
        return 1;
    
    if ((u32Size == 0) || (pu8Data == NULL))
        return 1;

    // read cmd
    if (u8UseQuadMode)
    {
        u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x6B;
        pSpi->DR[0] = u32Temp;
    }
    else
    {
        u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | 0x0B;
        pSpi->DR[0] = u32Temp;
    }
    u32TimeOut = 0;
    while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
    {
        // Wait for RX not Empty...
        
        //Time-out check
        if( u32TimeOut > SPI_TIMEOUT )
            return 1;
        u32TimeOut++;
    }
    u32Temp = pSpi->DR[0];  // dummy
    
    // Addr
    u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | ( (u32StartAddr>>16) & 0xFF );
    pSpi->DR[0] = u32Temp;
    u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | ( (u32StartAddr>>8) & 0xFF );
    pSpi->DR[0] = u32Temp;
    u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | (u32StartAddr & 0xFF);
    pSpi->DR[0] = u32Temp;
    u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | DUMMY;
    pSpi->DR[0] = u32Temp;
    
    u32TimeOut = 0;
    while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
    {
        // Wait for RX not Empty...
        
        //Time-out check
        if( u32TimeOut > SPI_TIMEOUT )
            return 1;
        u32TimeOut++;
    }
    u32Temp = pSpi->DR[0];  // dummy
    
    u32TimeOut = 0;
    while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
    {
        // Wait for RX not Empty...
        
        //Time-out check
        if( u32TimeOut > SPI_TIMEOUT )
            return 1;
        u32TimeOut++;
    }
    u32Temp = pSpi->DR[0];  // dummy
    
    u32TimeOut = 0;
    while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
    {
        // Wait for RX not Empty...
        
        //Time-out check
        if( u32TimeOut > SPI_TIMEOUT )
            return 1;
        u32TimeOut++;
    }
    u32Temp = pSpi->DR[0];  // dummy
    
    u32TimeOut = 0;
    while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
    {
        // Wait for RX not Empty...
        
        //Time-out check
        if( u32TimeOut > SPI_TIMEOUT )
            return 1;
        u32TimeOut++;
    }
    u32Temp = pSpi->DR[0];  // dummy
    
    // read data
    // decide the quad mode or standard mode
    if (u8UseQuadMode)
        u32QuadBit = TAG_4_BIT;
    else
        u32QuadBit = TAG_1_BIT;

    // 4 bytes per round
    u32Temp = TAG_DFS_08 | TAG_CS_CONT | u32QuadBit | TAG_READ | DUMMY;
    for (u32Idx=0; (u32Idx+4)<u32Size; u32Idx+=4)
    {
        pSpi->DR[0] = u32Temp;
        pSpi->DR[0] = u32Temp;
        pSpi->DR[0] = u32Temp;
        pSpi->DR[0] = u32Temp;
        
        u32TimeOut = 0;
        while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
        {
            // Wait for RX not Empty...
            
            //Time-out check
            if( u32TimeOut > SPI_TIMEOUT )
                return 1;
            u32TimeOut++;
        }
        pu8Data[u32Idx] = (uint8_t)( pSpi->DR[0] & 0xFF );
        
        u32TimeOut = 0;
        while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
        {
            // Wait for RX not Empty...
            
            //Time-out check
            if( u32TimeOut > SPI_TIMEOUT )
                return 1;
            u32TimeOut++;
        }
        pu8Data[u32Idx+1] = (uint8_t)( pSpi->DR[0] & 0xFF );
        
        u32TimeOut = 0;
        while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
        {
            // Wait for RX not Empty...
            
            //Time-out check
            if( u32TimeOut > SPI_TIMEOUT )
                return 1;
            u32TimeOut++;
        }
        pu8Data[u32Idx+2] = (uint8_t)( pSpi->DR[0] & 0xFF );
        
        u32TimeOut = 0;
        while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
        {
            // Wait for RX not Empty...
            
            //Time-out check
            if( u32TimeOut > SPI_TIMEOUT )
                return 1;
            u32TimeOut++;
        }
        pu8Data[u32Idx+3] = (uint8_t)( pSpi->DR[0] & 0xFF );
    }

    // the rest data
    for (i=u32Idx; i<u32Size; i++)
    {
        if (i != (u32Size-1))
            u32Temp = TAG_DFS_08 | TAG_CS_CONT | u32QuadBit | TAG_READ | DUMMY;
        else
            u32Temp = TAG_DFS_08 | TAG_CS_COMP | u32QuadBit | TAG_READ | DUMMY; // complete
        pSpi->DR[0] = u32Temp;
    }
    for (i=u32Idx; i<u32Size; i++)
    {
        u32TimeOut = 0;
        while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
        {
            // Wait for RX not Empty...
            
            //Time-out check
            if( u32TimeOut > SPI_TIMEOUT )
                return 1;
            u32TimeOut++;
        }
        pu8Data[i] = (uint8_t)( pSpi->DR[0] & 0xFF );
    }

    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_Reset
*
* DESCRIPTION:
*   1. Flash reset
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx      : Index of SPI. refert to E_SpiIdx_t
*
* RETURNS
*   None
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Flash_Reset_Internal_impl(E_SpiIdx_t u32SpiIdx)
{
    uint32_t u32Temp = 0;

    if(u32SpiIdx >= SPI_IDX_MAX)
        return;

    if(g_u8aHalFlashID[u32SpiIdx] == NO_FLASH)
        return;

    // Reset enable
    u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_WRITE | 0x66;
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
    
    // Reset
    u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_WRITE | 0x99;
    Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
    
    Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_Init
*
* DESCRIPTION:
*   1. Init flash status
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refert to E_SpiIdx_t
*
* RETURNS
*   0: setting complete (Identified SPI flash)
*   1: error  (No SPI flash)
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_Init_impl(E_SpiIdx_t u32SpiIdx)
{
    osSemaphoreDef_t tSemaphoreDef;

    // 0. check for cold boot only
    if (0 != Boot_CheckWarmBoot())
        return 0;   // the init is done in the cold boot

    if (0 != Hal_Flash_Init_Internal(u32SpiIdx))
    {
        return 1;
    }

    // create the semaphore
    tSemaphoreDef.dummy = 0;                            // reserved, it is no used
    g_taHalFlashSemaphoreId[u32SpiIdx] = osSemaphoreCreate(&tSemaphoreDef, 1);
    if (g_taHalFlashSemaphoreId[u32SpiIdx] == NULL)
    {
        printf("To create the semaphore for Hal_Flash is fail.\n");
        return 1;
    }

    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_4KSectorAddrErase
*
* DESCRIPTION:
*   1. Erase a sector (4 KB)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refert to E_SpiIdx_t
*   2. u32SecAddr : Address of sector (must sector aligned, LSBs truncated)
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_4KSectorAddrErase_impl(E_SpiIdx_t u32SpiIdx, uint32_t u32SecAddr)
{
    uint8_t ubRet = 1;

    if(u32SpiIdx >= SPI_IDX_MAX)
        return ubRet;

    if(g_u8aHalFlashID[u32SpiIdx] == NO_FLASH)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[u32SpiIdx], osWaitForever);

    if (0 != Hal_Flash_4KSectorAddrErase_Internal(u32SpiIdx, u32SecAddr))
    {
        goto done;
    }

    ubRet = 0;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalFlashSemaphoreId[u32SpiIdx]);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_4KSectorIdxErase
*
* DESCRIPTION:
*   1. Erase a sector (4 KB)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refert to E_SpiIdx_t
*   2. u32SecAddr : Index of sector.
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_4KSectorIdxErase_impl(E_SpiIdx_t u32SpiIdx, uint32_t u32SecIdx)
{
    return Hal_Flash_4KSectorAddrErase(u32SpiIdx, u32SecIdx*0x1000);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_PageAddrProgram
*
* DESCRIPTION:
*   1. Program(write) a page (256 B)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx      : Index of SPI. refert to E_SpiIdx_t
*   2. u32PageAddr  : Address of page. (must page aligned, LSBs truncated)
*   3. u8UseQuadMode: Qaud-mode select. 1 for enable/0 for disable
*   4. pu8Data      : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_PageAddrProgram_impl(E_SpiIdx_t u32SpiIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data)
{
    uint8_t ubRet = 1;
        
    if(u32SpiIdx >= SPI_IDX_MAX)
        return ubRet;

    if(g_u8aHalFlashID[u32SpiIdx] == NO_FLASH)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[u32SpiIdx], osWaitForever);

    if (0 != Hal_Flash_PageAddrProgram_Internal(u32SpiIdx, u32PageAddr, u8UseQuadMode, pu8Data))
    {
        goto done;
    }

    ubRet = 0;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalFlashSemaphoreId[u32SpiIdx]);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_PageIdxProgram
*
* DESCRIPTION:
*   1. Program(write) a page (256 B)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx      : Index of SPI. refert to E_SpiIdx_t
*   2. u32PageAddr  : Index of page.
*   3. u8UseQuadMode: Qaud-mode select. 1 for enable/0 for disable
*   4. pu8Data      : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_PageIdxProgram_impl(E_SpiIdx_t u32SpiIdx, uint32_t u32PageIdx, uint8_t u8UseQuadMode, uint8_t *pu8Data)
{
    return Hal_Flash_PageAddrProgram(u32SpiIdx, u32PageIdx*0x100, u8UseQuadMode, pu8Data);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_PageAddrRead
*
* DESCRIPTION:
*   1. Read a page (256 B) 
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx      : Index of SPI. refert to E_SpiIdx_t
*   2. u32PageAddr  : Address of page.
*   3. u8UseQuadMode: Qaud-mode select. 1 for enable/0 for disable
*   4. pu8Data      : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_PageAddrRead_impl(E_SpiIdx_t u32SpiIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data)
{
    uint8_t ubRet = 1;
    
    if(u32SpiIdx >= SPI_IDX_MAX)
        return ubRet;

    if(g_u8aHalFlashID[u32SpiIdx] == NO_FLASH)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[u32SpiIdx], osWaitForever);

    if (0 != Hal_Flash_PageAddrRead_Internal(u32SpiIdx, u32PageAddr, u8UseQuadMode, pu8Data))
    {
        goto done;
    }

    ubRet = 0;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalFlashSemaphoreId[u32SpiIdx]);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_AddrProgram
*
* DESCRIPTION:
*   1. Program(write) n bytes from the start address
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx      : Index of SPI. refert to E_SpiIdx_t
*   2. u32StartAddr : Start address
*   3. u8UseQuadMode: Qaud-mode select. 1 for enable/0 for disable
*   4. u32Size      : Data size
*   5. pu8Data      : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_AddrProgram_impl(E_SpiIdx_t u32SpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    uint8_t ubRet = 1;
    
    if(u32SpiIdx >= SPI_IDX_MAX)
        return ubRet;

    if(g_u8aHalFlashID[u32SpiIdx] == NO_FLASH)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[u32SpiIdx], osWaitForever);

    if (0 != Hal_Flash_AddrProgram_Internal(u32SpiIdx, u32StartAddr, u8UseQuadMode, u32Size, pu8Data))
    {
        goto done;
    }

    ubRet = 0;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalFlashSemaphoreId[u32SpiIdx]);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_AddrRead
*
* DESCRIPTION:
*   1. Read n bytes from the start address
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx      : Index of SPI. refert to E_SpiIdx_t
*   2. u32StartAddr : Start address
*   3. u8UseQuadMode: Qaud-mode select. 1 for enable/0 for disable
*   4. u32Size      : Data size
*   5. pu8Data      : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_AddrRead_impl(E_SpiIdx_t u32SpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    uint8_t ubRet = 1;
        
    if(u32SpiIdx >= SPI_IDX_MAX)
        return ubRet;

    if(g_u8aHalFlashID[u32SpiIdx] == NO_FLASH)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[u32SpiIdx], osWaitForever);

    if (0 != Hal_Flash_AddrRead_Internal(u32SpiIdx, u32StartAddr, u8UseQuadMode, u32Size, pu8Data))
    {
        goto done;
    }

    ubRet = 0;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalFlashSemaphoreId[u32SpiIdx]);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_Reset
*
* DESCRIPTION:
*   1. Flash reset
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx      : Index of SPI. refert to E_SpiIdx_t
*
* RETURNS
*   None
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Flash_Reset_impl(E_SpiIdx_t u32SpiIdx)
{
    if(u32SpiIdx >= SPI_IDX_MAX)
        return;

    if(g_u8aHalFlashID[u32SpiIdx] == NO_FLASH)
        return;

    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[u32SpiIdx], osWaitForever);

    Hal_Flash_Reset_Internal(u32SpiIdx);

    // release the semaphore
    osSemaphoreRelease(g_taHalFlashSemaphoreId[u32SpiIdx]);
}

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_FunPoiterInit
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
void _Hal_Flash_FunPoiterInit(void)
{
    _Hal_Flash_ManufDeviceId  = _Hal_Flash_ManufDeviceId_impl;
    _Hal_Flash_StatusGet      = _Hal_Flash_StatusGet_impl;
    _Hal_Flash_WriteDoneCheck = _Hal_Flash_WriteDoneCheck_impl;
    _Hal_Flash_WriteEn        = _Hal_Flash_WriteEn_impl;
    _Hal_Flash_QuadModeEn     = _Hal_Flash_QuadModeEn_impl;

    Hal_Flash_Init_Internal              = Hal_Flash_Init_Internal_impl;
    Hal_Flash_4KSectorAddrErase_Internal = Hal_Flash_4KSectorAddrErase_Internal_impl;
    Hal_Flash_4KSectorIdxErase_Internal  = Hal_Flash_4KSectorIdxErase_Internal_impl;
    Hal_Flash_PageAddrProgram_Internal   = Hal_Flash_PageAddrProgram_Internal_impl;
    Hal_Flash_PageIdxProgram_Internal    = Hal_Flash_PageIdxProgram_Internal_impl;
    Hal_Flash_PageAddrRead_Internal      = Hal_Flash_PageAddrRead_Internal_impl;
    Hal_Flash_AddrProgram_Internal       = Hal_Flash_AddrProgram_Internal_impl;
    Hal_Flash_AddrRead_Internal          = Hal_Flash_AddrRead_Internal_impl;
    Hal_Flash_Reset_Internal             = Hal_Flash_Reset_Internal_impl;

    Hal_Flash_Init              = Hal_Flash_Init_impl;
    Hal_Flash_4KSectorAddrErase = Hal_Flash_4KSectorAddrErase_impl;
    Hal_Flash_4KSectorIdxErase  = Hal_Flash_4KSectorIdxErase_impl;
    Hal_Flash_PageAddrProgram   = Hal_Flash_PageAddrProgram_impl;
    Hal_Flash_PageIdxProgram    = Hal_Flash_PageIdxProgram_impl;
    Hal_Flash_PageAddrRead      = Hal_Flash_PageAddrRead_impl;
    Hal_Flash_AddrProgram       = Hal_Flash_AddrProgram_impl;
    Hal_Flash_AddrRead          = Hal_Flash_AddrRead_impl;
    Hal_Flash_Reset             = Hal_Flash_Reset_impl;
    Hal_Flash_Check             = Hal_Flash_Check_impl;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_ParamInit
*
* DESCRIPTION:
*   1. Inital of glabal parameters
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
void Hal_Flash_ParamInit(void)
{
    g_u8aHalFlashID[SPI_IDX_0] = NO_FLASH;
    g_u8aHalFlashID[SPI_IDX_1] = NO_FLASH;
    g_u8aHalFlashID[SPI_IDX_2] = NO_FLASH;

    g_taHalFlashSemaphoreId[SPI_IDX_0] = NULL;
    g_taHalFlashSemaphoreId[SPI_IDX_1] = NULL;
    g_taHalFlashSemaphoreId[SPI_IDX_2] = NULL;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_Pre_Init
*
* DESCRIPTION:
*   1. Inital for functuin pointers and glabal parameters
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
void Hal_Flash_Pre_Init(void)
{
    // Param
    Hal_Flash_ParamInit();

    // Function pointers
    _Hal_Flash_FunPoiterInit();
}
