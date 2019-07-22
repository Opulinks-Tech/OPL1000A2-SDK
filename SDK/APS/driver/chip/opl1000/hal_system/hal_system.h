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
*  hal_system.h
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of system functions
*  Include sys-reg and AOS domains.
*
*  Author:
*  -------
*  Chung-chun Wang
******************************************************************************/

#ifndef __HAL_SYSTEM_H__
#define __HAL_SYSTEM_H__

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
typedef enum 
{
    SPARE_0,
    SPARE_1, 
    SPARE_2, 
    SPARE_3
} E_SpareRegIdx_t;

typedef enum 
{
    SLEEP_TIMER,
    SLEEP_DEEP
} E_SleepModeIdx_t;

typedef enum 
{
    PMU_SF_VAL_0P65V = 0,
    PMU_SF_VAL_0P73V = 1, 
    PMU_SF_VAL_0P95V = 4, 
    PMU_SF_VAL_1P10V = 6,
    
    PMU_SF_VAL_MASK = 0x7
} E_PmuSfVal_t;

typedef enum 
{
    APS_RST_RFM, 
    APS_RST_I2C, 
    APS_RST_PWM, 
    APS_RST_TIMER_0,
    APS_RST_TIMER_1,
    APS_RST_WDT,
    APS_RST_VIC,
    APS_RST_SYS_BUS, 
    APS_RST_SPI_0, 
    APS_RST_SPI_1, 
    APS_RST_SPI_2,
    APS_RST_UART_0,
    APS_RST_UART_1,
    APS_RST_DBG_UART,
    APS_RST_OTP, 
    APS_RST_DMA, 
    APS_RST_ROM, 
    APS_RST_RAM,
    APS_RST_JTAG,
    APS_RST_AHB_TO_APB_BUS,
    APS_RST_BRIDGE_APS_2_MSQ,
    APS_RST_BRIDGE_MSQ_2_APS, 
    APS_RST_CORE_M3, 
    APS_RST_SCRT, 
    APS_RST_TAP
} E_ApsRstModule_t;

typedef enum 
{
    MSQ_RST_ROM, 
    MSQ_RST_SDM_XTAL, 
    MSQ_RST_BRIDGE_MSQ_TO_APS, 
    MSQ_RST_BRIDGE_APS_TO_MSQ,
    MSQ_RST_MAC,
    MSQ_RST_PHY,
    MSQ_RST_WDT,
    MSQ_RST_VIC, 
    MSQ_RST_PHY_REG, 
    MSQ_RST_AOS,
    MSQ_RST_CORE_M0,
    MSQ_RST_RFPHY,
    MSQ_RST_SPI,
    MSQ_RST_DBG_UART,
    MSQ_RST_PROG_PU
} E_MsqRstModule_t;


typedef enum 
{
    APS_CLKTREE_SRC_RC_BB, 
    APS_CLKTREE_SRC_XTAL, 
    APS_CLKTREE_SRC_XTAL_X2, 
    APS_CLKTREE_SRC_XTAL_X4,
    APS_CLKTREE_SRC_DECI,
    APS_CLKTREE_SRC_1P2G_DIV,
    APS_CLKTREE_SRC_EXTERNAL,
    APS_CLKTREE_SRC_1P2G_MIN,
    APS_CLKTREE_SRC_1P2G_078MHZ = APS_CLKTREE_SRC_1P2G_MIN,
    APS_CLKTREE_SRC_1P2G_081MHZ,
    APS_CLKTREE_SRC_1P2G_084MHZ,
    APS_CLKTREE_SRC_1P2G_087MHZ,
    APS_CLKTREE_SRC_1P2G_090MHZ,
    APS_CLKTREE_SRC_1P2G_093MHZ,
    APS_CLKTREE_SRC_1P2G_097MHZ,
    APS_CLKTREE_SRC_1P2G_101MHZ,
    APS_CLKTREE_SRC_1P2G_106MHZ,
    APS_CLKTREE_SRC_1P2G_110MHZ,
    APS_CLKTREE_SRC_1P2G_116MHZ,
    APS_CLKTREE_SRC_1P2G_122MHZ,
    APS_CLKTREE_SRC_1P2G_128MHZ,
    APS_CLKTREE_SRC_1P2G_135MHZ,
    APS_CLKTREE_SRC_1P2G_143MHZ,
    APS_CLKTREE_SRC_1P2G_152MHZ,
    APS_CLKTREE_SRC_1P2G_MAX = APS_CLKTREE_SRC_1P2G_152MHZ
} E_ApsClkTreeSrc_t;

typedef enum 
{
    MSQ_CLKTREE_SRC_RC,
    MSQ_CLKTREE_SRC_XTAL, 
    MSQ_CLKTREE_SRC_XTAL_X2, 
    MSQ_CLKTREE_SRC_EXTERNAL
} E_MsqClkTreeSrc_t;

typedef enum 
{
    PHY_TESTING_SRC_S_022M,
    PHY_TESTING_SRC_S_044M,
    PHY_TESTING_SRC_Q_044M,
    PHY_TESTING_SRC_RFMMD
} E_PhyRfTestingSrc_t;

typedef enum 
{
    PHY_CAP_SRC_S_044M,
    PHY_CAP_SRC_Q_176M
} E_PhyRfCapSrc_t;

typedef enum 
{
    PWM_CLK_SRC_32KHZ,
    PWM_CLK_SRC_22MHZ
} E_PwmClkSrc_t;

typedef enum 
{
    /* root */
    APS_CLK_GLOBAL,

    /* pclk domain */
    APS_CLK_I2C,
    APS_CLK_TIMER_0,
    APS_CLK_TIMER_1,
    APS_CLK_WDT,
    APS_CLK_VIC,
    APS_CLK_SPI_0,
    APS_CLK_SPI_1,
    APS_CLK_SPI_2,
    APS_CLK_UART_0,
    APS_CLK_UART_1,
    APS_CLK_DBG_UART,
    APS_CLK_OTP,
	
    /* hclk domain */
    APS_CLK_DMA,
    APS_CLK_ROM,
    APS_CLK_RAM,
    APS_CLK_AHB_TO_APB_BUS,
    APS_CLK_APS2MSQ_BRIDGE,
    APS_CLK_SCRT,
    APS_CLK_JTAG,
	
    /* fclk + hclk domain*/
    APS_CLK_CM3_CORE,

    /* stand-alone clock  */
    APS_CLK_PWM,

    /* all */
    APS_CLK_ALL
} E_ApsClkEnModule_t;

typedef enum 
{
    /* root */
    MSQ_CLK_GLOBAL,

    /* pclk domain */
    MSQ_CLK_SPI,
    MSQ_CLK_DBG_UART,
    
    /* hclk domain */
    MSQ_CLK_ROM,
    MSQ_CLK_RAM,
    MSQ_CLK_MSQ2APS_BRIDGE,
    MSQ_CLK_MAC,
    MSQ_CLK_PHY,
    MSQ_CLK_AOS,
    MSQ_CLK_WDT,
    MSQ_CLK_VIC,
    MSQ_CLK_PU,
    
    /* fclk + dclk + sclk + hclk domain*/
    MSQ_CLK_CM0_CORE,

    /* all */
    MSQ_CLK_ALL
} E_MsqClkEnModule_t;

typedef enum 
{
    /* roots */
    PHY_CLK_S,
    PHY_CLK_Q,
    PHY_CLK_RFMMD,

    /* modules */
    PHY_CLK_S_22M_TX,
    PHY_CLK_S_22M_RX,
    PHY_CLK_S_44M_TX,
    PHY_CLK_S_44M_RX,
    PHY_CLK_Q_176M_TX,
    PHY_CLK_Q_176M_RX,
    PHY_CLK_Q_088M_TX,
    PHY_CLK_Q_088M_RX,
    PHY_CLK_Q_044M_TX,
    PHY_CLK_Q_044M_RX,
    PHY_CLK_RF_CAP,

    /* all */
    PHY_CLK_ALL
} E_PhyClkEnModule_t;

typedef enum 
{
    RET_RAM_APS_00      = 0x1,
    RET_RAM_APS_01      = 0x2,
    RET_RAM_APS_02_03   = 0x4,
    RET_RAM_APS_04_05   = 0x8,
    RET_RAM_APS_06_07   = 0x10,
    RET_RAM_APS_08_09   = 0x20,
    RET_RAM_APS_10_11   = 0x40,
    RET_RAM_APS_12_13   = 0x80,
    RET_RAM_APS_14_15   = 0x100,
    RET_RAM_APS_16_17   = 0x200,
    RET_RAM_APS_18_19   = 0x400,
    RET_RAM_SHARE_00    = 0x800,
    RET_RAM_SHARE_01    = 0x1000,
    RET_RAM_SHARE_02_03 = 0x2000,
    RET_RAM_MSQ_00      = 0x4000,
    RET_RAM_MSQ_01      = 0x8000,
    RET_RAM_MSQ_02_03   = 0x10000,
    RET_RAM_SCRT_00     = 0x20000,
    RET_RAM_SCRT_01     = 0x40000,
    RET_RAM_SCRT_02_03  = 0x80000,
} E_RetRamIdx_t;

typedef enum 
{
    RESET_BY_M3_WDT = 0x1,
    RESET_BY_M0_WDT = 0x2,
    RESET_BY_SWRST  = 0x4,
    RESET_BY_CPOR   = 0x8,
    RESET_BY_SPOR   = 0x10,
} E_RstSrc_t;


typedef enum
{
    STRAP_MODE_NULL     = 0x0,      /* b'0000 No definition */
    STRAP_MODE_DDM      = 0x1,      /* b'0001 Digital debug mode */
    STRAP_MODE_ICE      = 0x2,      /* b'0010 ICE mode */
    STRAP_MODE_JTAG     = 0x3,      /* b'0011 JTAG debug mode */
    STRAP_MODE_MBIST    = 0x4,      /* b'0100 MBIST mode */
    STRAP_MODE_OTP      = 0x5,      /* b'0101 OTP Prog mode */
    STRAP_MODE_AUX      = 0x7,      /* b'0111 AUX ADC debug mode  */
    STRAP_MODE_RDM      = 0x8,      /* b'1000 RF Debug mode */
    STRAP_MODE_RFM      = 0x9,      /* b'1001 Radio only mode */
    STRAP_MODE_NORMAL   = 0xA,      /* b'1010 Normal mode */
    STRAP_MODE_FT       = 0xB       /* b'1011 FT mode */
} E_StrapMode_t;


/* Power relative */
typedef uint32_t (*T_Hal_Sys_PowerManualEn)(uint8_t u8Enable);
typedef uint32_t (*T_Hal_Sys_PowerDefaultSettings)(void);
typedef uint32_t (*T_Hal_Sys_PmuSfVolSet)(E_PmuSfVal_t eVol);
typedef uint32_t (*T_Hal_Sys_PtatEn)(uint8_t u8Enable);

/* Sleep Mode relative */
typedef void (*T_Hal_Sys_SleepInit)(void);
typedef uint64_t (*T_Hal_Sys_SleepTimerGet)(void);
typedef uint32_t (*T_Hal_Sys_SleepTimerSet)(E_SleepModeIdx_t eSleepMode, uint64_t u64WakeupTime);
typedef void (*T_Hal_Sys_SleepTrig)(void);

/* Pin-Mux relative*/
typedef void (*T_Hal_SysPinMuxBootROMInit)(void);
typedef void (*T_Hal_SysPinMuxAppInit)(void);
typedef void (*T_Hal_SysPinMuxDownloadInit)(void);
typedef void (*T_Hal_SysPinMuxSpiFlashInit)(void);
typedef void (*T_Hal_SysPinMuxM3UartSwitch)(void);

/* Ret RAM relative*/
typedef uint32_t (*T_Hal_Sys_RetRamStatusRead)(void);
typedef uint32_t (*T_Hal_Sys_RetRamTurnOff)(uint32_t u32RetRamIdxs );
typedef uint32_t (*T_Hal_Sys_RetRamTurnOn)(uint32_t u32RetRamIdxs);

/* Xtal fast starup relative */
typedef uint32_t (*T_Hal_Sys_FastStartUpEn)(uint8_t u8Enable);
typedef uint32_t (*T_Hal_Sys_RcCal)(void);

/* SW reset relative */
typedef uint32_t (*T_Hal_Sys_SwResetAll)(void);
typedef uint32_t (*T_Hal_Sys_ApsModuleRst)(E_ApsRstModule_t eModule);
typedef uint32_t (*T_Hal_Sys_MsqModuleRst)(E_MsqRstModule_t eModule);

/* Clock relative */
typedef uint32_t (*T_Hal_Sys_ApsClkTreeSetup)(E_ApsClkTreeSrc_t eClkTreeSrc, uint8_t u8ClkDivEn, uint8_t u8PclkDivEn );
typedef uint32_t (*T_Hal_Sys_MsqClkTreeSetup)(E_MsqClkTreeSrc_t eClkTreeSrc, uint8_t u8ClkDivEn );
typedef uint32_t (*T_Hal_Sys_PhyClkTreeSetup)(E_PhyRfTestingSrc_t eRfTestingSrc, E_PhyRfCapSrc_t eRfCapSrc, uint8_t u8RfClkSDivEn, uint8_t u8RfClkSInv, uint8_t u8RfClkQInv, uint8_t u8RfmmdClkInv);
typedef uint32_t (*T_Hal_Sys_PwmSrcSelect)(E_PwmClkSrc_t eSrc);
typedef uint32_t (*T_Hal_Sys_ApsClkEn)(uint8_t u8Enable, E_ApsClkEnModule_t eModule);
typedef uint32_t (*T_Hal_Sys_MsqClkEn)(uint8_t u8Enable, E_MsqClkEnModule_t eModule);
typedef uint32_t (*T_Hal_Sys_PhyClkEn)(uint8_t u8Enable, E_PhyClkEnModule_t eModule);
typedef uint32_t (*T_Hal_Sys_32kXtalGainSet)(uint8_t u8Gain);
typedef uint32_t (*T_Hal_Sys_32kXtalCapSet)(uint8_t u8Cap);
typedef void (*T_Hal_Sys_DisableClock)(void);
typedef void (*T_Hal_Sys_ApsClkStore)(void);
typedef void (*T_Hal_Sys_ApsClkResume)(void);

/* Remap relative */
typedef uint32_t (*T_Hal_Sys_MsqRomReampAll)(uint8_t u8Enable);

/* Miscellaneous */
typedef uint32_t (*T_Hal_Sys_SramDffBypass)(uint8_t u8Enable);
typedef uint32_t (*T_Hal_Sys_SwDebugEn)(uint8_t u8Enable);
typedef uint32_t (*T_Hal_Sys_StrapModeRead)(void);
typedef uint32_t (*T_Hal_Sys_StrapModeOverWrite)(uint8_t u8Enable, uint8_t u8StrapMode);
typedef uint32_t (*T_Hal_Sys_AosIdRead)(void);
typedef uint32_t (*T_Hal_Sys_ChipIdRead)(void);
typedef uint32_t (*T_Hal_Sys_SpareRegRead)(E_SpareRegIdx_t eSpareIdx, uint32_t *pu32Data);
typedef uint32_t (*T_Hal_Sys_SpareRegWrite)(E_SpareRegIdx_t eSpareIdx, uint32_t u32Data);
typedef uint32_t (*T_Hal_Sys_ApsClkMmFactorSet)(uint32_t u32Factor);
typedef void (*T_Hal_Sys_ClkChangeApply)(void);
typedef uint8_t *(*T_Hal_Sys_OtpRead)(uint16_t u16Offset, uint8_t *u8aBuf, uint16_t u16BufSize);

// For patch only (internal use)
typedef void (*T_Hal_Sys_SystemCoreClockUpdate)(uint32_t u32CoreClk);
typedef void (*T_Hal_Sys_ApsClkTreeUsedSrcEn)( E_ApsClkTreeSrc_t eClkTreeSrc );
typedef void (*T_Hal_Sys_MsqClkTreeUsedSrcEn)( E_MsqClkTreeSrc_t eClkTreeSrc );
typedef void (*T_Hal_Sys_ApsClkTreeUnusedSrcDis)( E_ApsClkTreeSrc_t eClkTreeSrc );
typedef void (*T_Hal_Sys_MsqClkTreeUnusedSrcDis)( E_MsqClkTreeSrc_t eClkTreeSrc );

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
/* Power relative */
extern T_Hal_Sys_PowerManualEn        Hal_Sys_PowerManualEn;
extern T_Hal_Sys_PowerDefaultSettings Hal_Sys_PowerDefaultSettings;
extern T_Hal_Sys_PmuSfVolSet          Hal_Sys_PmuSfVolSet;
extern T_Hal_Sys_PtatEn               Hal_Sys_PtatEn;

/* Sleep Mode relative */
extern T_Hal_Sys_SleepInit     Hal_Sys_SleepInit;
extern T_Hal_Sys_SleepTimerGet Hal_Sys_SleepTimerGet;
extern T_Hal_Sys_SleepTimerSet Hal_Sys_SleepTimerSet;
extern T_Hal_Sys_SleepTrig     Hal_Sys_SleepTrig;

/* Pin-Mux relative*/
extern T_Hal_SysPinMuxBootROMInit      Hal_SysPinMuxBootROMInit;
extern T_Hal_SysPinMuxAppInit          Hal_SysPinMuxAppInit;
extern T_Hal_SysPinMuxDownloadInit     Hal_SysPinMuxDownloadInit;
extern T_Hal_SysPinMuxSpiFlashInit     Hal_SysPinMuxSpiFlashInit;
extern T_Hal_SysPinMuxM3UartSwitch     Hal_SysPinMuxM3UartSwitch;

/* Ret RAM relative*/
extern T_Hal_Sys_RetRamStatusRead Hal_Sys_RetRamStatusRead;
extern T_Hal_Sys_RetRamTurnOff    Hal_Sys_RetRamTurnOff;
extern T_Hal_Sys_RetRamTurnOn     Hal_Sys_RetRamTurnOn;

/* Xtal fast starup relative */
extern T_Hal_Sys_RcCal            Hal_Sys_RcCal;
extern T_Hal_Sys_FastStartUpEn    Hal_Sys_FastStartUpEn;

/* SW reset relative */
extern T_Hal_Sys_SwResetAll   Hal_Sys_SwResetAll;
extern T_Hal_Sys_ApsModuleRst Hal_Sys_ApsModuleRst;
extern T_Hal_Sys_MsqModuleRst Hal_Sys_MsqModuleRst;

/* Clock relative */
extern T_Hal_Sys_ApsClkTreeSetup Hal_Sys_ApsClkTreeSetup;
extern T_Hal_Sys_MsqClkTreeSetup Hal_Sys_MsqClkTreeSetup;
extern T_Hal_Sys_PhyClkTreeSetup Hal_Sys_PhyClkTreeSetup;
extern T_Hal_Sys_PwmSrcSelect    Hal_Sys_PwmSrcSelect;
extern T_Hal_Sys_ApsClkEn        Hal_Sys_ApsClkEn;
extern T_Hal_Sys_MsqClkEn        Hal_Sys_MsqClkEn;
extern T_Hal_Sys_PhyClkEn        Hal_Sys_PhyClkEn;
extern T_Hal_Sys_32kXtalGainSet  Hal_Sys_32kXtalGainSet;
extern T_Hal_Sys_32kXtalGainSet  Hal_Sys_32kXtalCapSet;
extern T_Hal_Sys_DisableClock    Hal_Sys_DisableClock;
extern T_Hal_Sys_ApsClkStore     Hal_Sys_ApsClkStore;
extern T_Hal_Sys_ApsClkResume    Hal_Sys_ApsClkResume;

/* Remap relative */
extern T_Hal_Sys_MsqRomReampAll Hal_Sys_MsqRomReampAll;

/* Miscellaneous */
extern T_Hal_Sys_SramDffBypass      Hal_Sys_SramDffBypass;
extern T_Hal_Sys_SwDebugEn          Hal_Sys_SwDebugEn;
extern T_Hal_Sys_StrapModeRead      Hal_Sys_StrapModeRead;
extern T_Hal_Sys_StrapModeOverWrite Hal_Sys_StrapModeOverWrite;
extern T_Hal_Sys_AosIdRead          Hal_Sys_AosIdRead;
extern T_Hal_Sys_ChipIdRead         Hal_Sys_ChipIdRead;
extern T_Hal_Sys_SpareRegRead       Hal_Sys_SpareRegRead;
extern T_Hal_Sys_SpareRegWrite      Hal_Sys_SpareRegWrite;
extern T_Hal_Sys_ApsClkMmFactorSet  Hal_Sys_ApsClkMmFactorSet;
extern T_Hal_Sys_ClkChangeApply     Hal_Sys_ApsClkChangeApply;
extern T_Hal_Sys_ClkChangeApply     Hal_Sys_MsqClkChangeApply;
extern T_Hal_Sys_ClkChangeApply     Hal_Sys_PhyClkChangeApply;
extern T_Hal_Sys_OtpRead            Hal_Sys_OtpRead;

/* For patch only (internal use)*/
extern T_Hal_Sys_SystemCoreClockUpdate  _Hal_Sys_ApsSystemCoreClockUpdate;
extern T_Hal_Sys_SystemCoreClockUpdate  _Hal_Sys_MsqSystemCoreClockUpdate;
extern T_Hal_Sys_SystemCoreClockUpdate  _Hal_Sys_PhySystemCoreClockUpdate;
extern T_Hal_Sys_ApsClkTreeUsedSrcEn    Hal_Sys_ApsClkTreeUsedSrcEn;
extern T_Hal_Sys_MsqClkTreeUsedSrcEn    Hal_Sys_MsqClkTreeUsedSrcEn;
extern T_Hal_Sys_ApsClkTreeUnusedSrcDis Hal_Sys_ApsClkTreeUnusedSrcDis;
extern T_Hal_Sys_MsqClkTreeUnusedSrcDis Hal_Sys_MsqClkTreeUnusedSrcDis;

/* bootROM stage used, no patchable */ 
uint32_t Hal_Sys_ResetStatusRead(void);
uint32_t Hal_Sys_ResetStatusClear(uint32_t u32Sources);

/* For future used */
void Hal_Sys_ApsClkTreeUnusedSrcDis_default( E_ApsClkTreeSrc_t eClkTreeSrc );
void Hal_Sys_MsqClkTreeUnusedSrcDis_default( E_MsqClkTreeSrc_t eClkTreeSrc );

void Hal_Sys_Pre_Init(void);

#endif
