/* *****************************************************************************
 *  Copyright 2017 - 2018, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *
 *******************************************************************************
 * @file hal_spi_patch.c
 * 
 * @brief Patch for SPI API patch
 *  
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_spi_patch.h"
#include "hal_system_patch.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

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

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
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

/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/
uint32_t Hal_Spi_BaudRateSet_patch(E_SpiIdx_t eSpiIdx, uint32_t u32Baud);
uint32_t Hal_Spi_BaudRateGet_patch(E_SpiIdx_t eSpiIdx);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern uint32_t u32SpiSpeed[SPI_IDX_MAX];
S_Spi_Reg_t *g_Spi_psaSpiReg[SPI_IDX_MAX] = {SPI_0, SPI_1, SPI_2};
/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
 

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void Hal_Spi_PatchInit(void)
{
    Hal_Spi_BaudRateSet = Hal_Spi_BaudRateSet_patch;
    Hal_Spi_BaudRateGet = Hal_Spi_BaudRateGet_patch;
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */


/**
 * @brief Set baud-rate of SPI
 *        SPI clock source is from APS PCLK, and the divider range is any even value between 2 and 65534.
 *        This function will find the closet baud rate and set the divider to SPI.
 *        Using Hal_Spi_BaudRateGet can get current real SPI baud rate.
 *        SPI baud rate = (SPI source clock) / Divider
 *
 * @param eSpiIdx [in] Index of SPI.
 *                      SPI_IDX_0, SPI_IDX_1, SPI_IDX_2
 * @param u32Baud [in] Clock of SPI. 
 * @return Setting status
 * @retval 0 setting complete
 * @retval 1 error 
 */
uint32_t Hal_Spi_BaudRateSet_patch(E_SpiIdx_t eSpiIdx, uint32_t u32Baud)
{
    S_Spi_Reg_t *pSpi;
    uint32_t u32Div;
    uint32_t u32SsiEnr;
    uint32_t u32Ret = 1;
    
    if(eSpiIdx >= SPI_IDX_MAX)
        return 1;
    
    if (u32Baud == 0)
        return 1;
    
    pSpi = g_Spi_psaSpiReg[eSpiIdx];
    
    /* Disable SPI first */
    u32SsiEnr = pSpi->SSIENR;
    pSpi->SSIENR = 0;
    
    /* Find closet even value. Bit 0 is always truncated
     * Page 106, range is 0x0002~0xFFFE, even only (ie. last bit always truncated)
     * SPI source clock is from APS PCLK */
    u32Div = ( Hal_Sys_ApsPclkGet() + u32Baud - 1 )/ u32Baud;
    
    if( u32Div < SPI_BUADR_MIN )
        u32Div = SPI_BUADR_MIN;
    else if( u32Div > SPI_BUADR_MAX )
        u32Div = SPI_BUADR_MAX;
    else    /* Valid range */
        u32Ret = 0;
    
    /* Set baud rate and store it */
    pSpi->BAUDR = u32Div;
    u32SpiSpeed[eSpiIdx] = u32Baud;
    
    /* Recover SPI */
    pSpi->SSIENR = u32SsiEnr;
    
    return u32Ret;
}


/**
 * @brief Get SPI working baud rate
 *        SPI baud rate = (SPI source clock) / (SPI divider)
 *        This value might not equal to the setting baud rate in Hal_Spi_BaudRateSet
 * @param eSpiIdx [in] Index of SPI.
 *                      SPI_IDX_0, SPI_IDX_1, SPI_IDX_2
 * @return Baud rate
 * @retval 1 Invalid index
 */
uint32_t Hal_Spi_BaudRateGet_patch(E_SpiIdx_t eSpiIdx)
{  
    S_Spi_Reg_t *pSpi;

    if(eSpiIdx >= SPI_IDX_MAX)
        return 1;
    
    pSpi = g_Spi_psaSpiReg[eSpiIdx];    
    return Hal_Sys_ApsPclkGet() / pSpi->BAUDR;
}



/**
 * @brief Update SPI divider.
 *        When SPI clock source changed, call this function to update SPI divider.
 *        This function will use previous SPI setting baud rate to update divider.
 * @param eSpiIdx [in] Index of SPI.
 *                      SPI_IDX_0, SPI_IDX_1, SPI_IDX_2
 * @return Setting status
 * @retval 0 setting complete
 * @retval 1 error 
 */
uint32_t Hal_Spi_DividerUpdate(E_SpiIdx_t eSpiIdx)
{  
    if (eSpiIdx >= SPI_IDX_MAX)
        return 1;
    
    Hal_Spi_BaudRateSet_patch(eSpiIdx, u32SpiSpeed[eSpiIdx]);
    return 0;
}



