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
*  hal_spi.c
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This source file defines the functions of spi.
*  Ref. document are:
*    [1] << DesignWare DW_apb_ssi Databook >>
*    [2] SPI tag bits - User manual.docx
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
#include "hal_system.h"
#include "hal_vic.h"
#include "hal_spi.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define SPI_0     ((S_Spi_Reg_t *) SPI0_BASE)
#define SPI_1     ((S_Spi_Reg_t *) SPI1_BASE)
#define SPI_2     ((S_Spi_Reg_t *) SPI2_BASE)

#define SPI_TIMEOUT 0x5000

#define SPI_CTL_FMT_MOTO             (0<<4)
#define SPI_CTL_FMT_TI               (1<<4)
#define SPI_CTL_FMT_NSC              (2<<4)
#define SPI_CTL_FMT_MASK             (0x3<<4)
#define SPI_CTL_FMT_MASK             (0x3<<4)
#define SPI_CTL_CLK_PHASE_START      (0<<6)
#define SPI_CTL_CLK_PHASE_MIDDLE     (1<<6)
#define SPI_CTL_CLK_PHASE_MASK       (0x1<<6)
#define SPI_CTL_CLK_POLAR_H_ACT      (0<<7)
#define SPI_CTL_CLK_POLAR_L_ACT      (1<<7)
#define SPI_CTL_CLK_POLAR_MASK       (0x1<<7)
#define SPI_CTL_OPR_MODE_TX_RX       (0<<8)
#define SPI_CTL_OPR_MODE_TX_ONLY     (1<<8)
#define SPI_CTL_OPR_MODE_RX_ONLY     (2<<8)
#define SPI_CTL_OPR_MODE_EEPROM_READ (3<<8)
#define SPI_CTL_OPR_MODE_MASK        (0x3<<8)

#define SPI_SSIER_EN                 (1<<0)
#define SPI_SSIER_TAG                (1<<1)

#define SPI_SR_RX_FULL               (1<<4)
#define SPI_SR_RX_NOT_EMPTY          (1<<3)
#define SPI_SR_TX_EMPTY              (1<<2)
#define SPI_SR_TX_NOT_FULL           (1<<1)

#define SPI_BUADR_MIN 0x2
#define SPI_BUADR_MAX 0xFFFE

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
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
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
static uint32_t u32SpiSpeed[SPI_IDX_MAX] = {0,0,0};

RET_DATA T_Hal_Spi_BaudRateGet Hal_Spi_BaudRateGet;
RET_DATA T_Hal_Spi_BaudRateSet Hal_Spi_BaudRateSet;
RET_DATA T_Hal_Spi_Init Hal_Spi_Init;
RET_DATA T_Hal_Spi_Data_Send Hal_Spi_Data_Send;
RET_DATA T_Hal_Spi_Data_Recv Hal_Spi_Data_Recv;

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
*  Hal_Spi_BaudRateGet
*
* DESCRIPTION:
*   1. Get baud-rate of SPI
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refert to E_SpiIdx_t
*
* RETURNS
*   Baud-rate value. 1 for error
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_BaudRateGet_impl(E_SpiIdx_t eSpiIdx)
{
    if(eSpiIdx >= SPI_IDX_MAX)
        return 1;
    return u32SpiSpeed[eSpiIdx];
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_BaudRateSet
*
* DESCRIPTION:
*   1. Set baud-rate of SPI
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refert to E_SpiIdx_t
*   2. u32Baud  : Clock of SPI. 
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_BaudRateSet_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Baud)
{
    S_Spi_Reg_t *pSpi = 0;
    uint32_t u32Temp = 0;
    
    if(eSpiIdx == SPI_IDX_0)
	{
        pSpi = SPI_0;
	}
    else if(eSpiIdx == SPI_IDX_1)
    {
        pSpi = SPI_1;
    }
    else if(eSpiIdx == SPI_IDX_2)
    {
        pSpi = SPI_2;
    }
    else
    {
        return 1;
    }
    
    u32SpiSpeed[eSpiIdx] = u32Baud;
    
    // Round
    u32Temp = ( SystemCoreClockGet() + u32Baud/2 )/ u32Baud;

    // Round for even
    u32Temp = u32Temp + 1;
    
    // Page 106, range is 0x0002~0xFFFE, even only (ie. last bit always truncated)
    if( u32Temp < SPI_BUADR_MIN )
    {
        pSpi->BAUDR = SPI_BUADR_MIN;
        return 1;
    }else if( u32Temp >= SPI_BUADR_MAX )
    {
        pSpi->BAUDR = SPI_BUADR_MAX;
        return 1;
    }else{
        pSpi->BAUDR = u32Temp;
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Init
*
* DESCRIPTION:
*   1. Init SPI module
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refert to E_SpiIdx_t
*   2. u32Baud  : Clock of SPI.
*   3. ePolar   : Polarity of SPI clock. refert to E_SpiClkPolarity_t
*   4. ePhase   : Pase of SPI clock. refert to E_SpiClkPhase_t
*   5. eFormat  : Transmit protocal. refert to E_SpiFrameFormat_t
*   6. eDataSize: Transmit data size 
*   7. u8QModeEn: Qaud-mode select. 1 for enable/0 for disable
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Init_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Baud, E_SpiClkPolarity_t ePolar, E_SpiClkPhase_t ePhase, E_SpiFrameFormat_t eFormat, E_SpiDataFrameSize_t eDataSize, uint32_t u8QModeEn)
{
    S_Spi_Reg_t *pSpi = 0;

    if(eSpiIdx == SPI_IDX_0)
	{
        pSpi = SPI_0;
        Hal_Sys_ApsClkEn(1, APS_CLK_SPI_0);
        Hal_Sys_ApsModuleRst(APS_RST_SPI_0);
	}
    else if(eSpiIdx == SPI_IDX_1)
    {
        pSpi = SPI_1;
        Hal_Sys_ApsClkEn(1, APS_CLK_SPI_1);
        Hal_Sys_ApsModuleRst(APS_RST_SPI_1);
    }
    else if(eSpiIdx == SPI_IDX_2)
    {
        pSpi = SPI_2;
        Hal_Sys_ApsClkEn(1, APS_CLK_SPI_2);
        Hal_Sys_ApsModuleRst(APS_RST_SPI_2);
    }
    else
    {
        return 1;
    }
    
	// Disable SPI
	pSpi->SSIENR = 0;
    
    // Control setting
    pSpi->CTRLR0 = SPI_CTL_OPR_MODE_TX_RX|
                   (ePolar<< 7) |
                   (ePhase << 6) | 
                   (eFormat<<4) | 
                   (eDataSize);
    
    // Set speed
    Hal_Spi_BaudRateSet(eSpiIdx, u32Baud);    
    
    // Set slave number, only 1 slave
    pSpi->SER = 0x0001;

    // Rx buffer threshold
        // 0000_0000 ssi_rxf_intr is asserted when 1 or more data entry is present in receive FIFO
        // 0000_0001 ssi_rxf_intr is asserted when 2 or more data entries are present in receive FIFO
        // 0000_0010 ssi_rxf_intr is asserted when 3 or more data entries are present in receive FIFO
        // 0000_0011 ssi_rxf_intr is asserted when 4 or more data entries are present in receive FIFO
    // RX FIFO size is 4, so the max value could be set to 3.
    // try to set to 4, the behavior is abnormal.
    pSpi->RXFTLR = 2;
    
    // Tx buffer threshold
    pSpi->TXFTLR = 2;
    
    // Interrupt mask (Need any interrupt?)
    pSpi->IMR = 0x10;
    
    // Enable SPI
    pSpi->SSIENR = SPI_SSIER_EN;
    if(u8QModeEn)
        pSpi->SSIENR |= SPI_SSIER_TAG;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Data_Send
*
* DESCRIPTION:
*   1. SPI send data
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refert to E_SpiIdx_t
*   2. u32Data  : Data to send
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Data_Send_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Data)
{
    S_Spi_Reg_t *pSpi = 0;
    uint32_t u32TimeOut = 0;
    
    if(eSpiIdx == SPI_IDX_0)
	{
        pSpi = SPI_0;
	}
    else if(eSpiIdx == SPI_IDX_1)
    {
        pSpi = SPI_1;
    }
    else if(eSpiIdx == SPI_IDX_2)
    {
        pSpi = SPI_2;
    }
    else
    {
        return 1;
    }
    while( !(pSpi->SR & SPI_SR_TX_NOT_FULL) )
    {
        // Wait for TX not full...
        
        // Time-out check
        if( u32TimeOut > SPI_TIMEOUT )
            return 1;
        u32TimeOut++;
    }
    pSpi->DR[0] = u32Data;
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Data_Recv
*
* DESCRIPTION:
*   1. SPI receive data
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refert to E_SpiIdx_t
*   2. pu32Data : Pointer to receive data
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Data_Recv_impl(E_SpiIdx_t eSpiIdx, uint32_t *pu32Data)
{
    S_Spi_Reg_t *pSpi = 0;
    uint32_t u32TimeOut = 0;
    
    if(eSpiIdx == SPI_IDX_0)
	{
        pSpi = SPI_0;
	}
    else if(eSpiIdx == SPI_IDX_1)
    {
        pSpi = SPI_1;
    }
    else if(eSpiIdx == SPI_IDX_2)
    {
        pSpi = SPI_2;
    }
    else
    {
        return 1;
    }
    
    while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
    {
        // Wait for RX not Empty...
        
        //Time-out check
        if( u32TimeOut > SPI_TIMEOUT )
            return 1;
        u32TimeOut++;
    }
    *pu32Data = pSpi->DR[0];
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Pre_Init
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
void Hal_Spi_Pre_Init(void)
{
    Hal_Spi_BaudRateGet = Hal_Spi_BaudRateGet_impl;
    Hal_Spi_BaudRateSet = Hal_Spi_BaudRateSet_impl;
    Hal_Spi_Init = Hal_Spi_Init_impl;
    Hal_Spi_Data_Send = Hal_Spi_Data_Send_impl;
    Hal_Spi_Data_Recv = Hal_Spi_Data_Recv_impl;
}

