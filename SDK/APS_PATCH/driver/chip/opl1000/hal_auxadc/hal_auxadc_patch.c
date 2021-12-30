/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
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
*  hal_auxadc_patch.c
*
*  Project:
*  --------
*  OPL1000 Project - the AUXADC implement file
*
*  Description:
*  ------------
*  This implement file is include the AUXADC function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include <string.h>
#include "hal_auxadc_patch.h"
#include "hal_auxadc_internal.h"
#include "cmsis_os.h"
#include "boot_sequence.h"
#include "mw_fim_default_group03.h"
#include "hal_tick.h"
#include "hal_flash.h"
#include "hal_system_patch.h"
#include "sys_cfg_patch.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define AOS                     ((S_Aos_Reg_t *) AOS_BASE)
#define RF                      ((S_Rf_Reg_t *) RF_BASE)

#define RF_RG_EOCB              (1 << 16)   // the bit[16] of RG_AUX_ADC_ECL_OUT: End of conversion sign

#define HAL_AUX_ADC_ERROR_COUNT (3)

/* Bit band access */
#define BITBAND_ADDR(addr, bitnum)      ( ((addr) & 0xF0000000) + 0x2000000 + (((addr) & 0xFFFFF) << 5) + ((bitnum) << 2) )
#define REG_BB_READ(addr, bitnum)       ( reg_read(BITBAND_ADDR((uint32_t)(addr), (uint32_t)(bitnum))) )
#define REG_BB_WRITE(addr, bitnum, val) ( reg_write(BITBAND_ADDR((uint32_t)(addr), (uint32_t)(bitnum)), ((uint32_t)(val) & 1)) )



/* RF mode table definition */
#define RF_CORE_MODE_NUM 10
#define RF_CTRL_IDX_NUM  30

#define RF_MODE_TABLE_IDX_CLK_GATE_CTRL     15          /* 48 */
#define RF_MODE_TABLE_IDX_POWER_CTRL        20          /* 90 */
#define RF_CORE_MODE_IDLE                   0
#define RF_CORE_MODE_MAX                    8
#define RF_CLK_GATE_CTRL_AUXADC             (1UL << 6)
#define RF_POWER_CTRL_AUXADC                (1UL << 26)

#define HAL_AUX_ADC_READ_ERROR_TIME         300

#define ROUND_DIV( NUM, DIV ) ( ( (NUM) + (DIV/2) ) / (DIV) )

#define AUXADC_FLASH_START_ADDR 0x8F000
#define ADCCAL_VER 0x1

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list
typedef struct
{
    volatile uint32_t RET_MUX;            // 0x000
    volatile uint32_t MODE_CTL;           // 0x004
    volatile uint32_t OSC_SEL;            // 0x008
    volatile uint32_t SLP_TIMER_CURR_L;   // 0x00C
    volatile uint32_t SLP_TIMER_CURR_H;   // 0x010
    volatile uint32_t SLP_TIMER_PRESET_L; // 0x014
    volatile uint32_t SLP_TIMER_PRESET_H; // 0x018
    volatile uint32_t PS_TIMER_PRESET;    // 0x01C
    volatile uint32_t RET_SF_VAL_CTL;     // 0x020
    volatile uint32_t PMU_SF_VAL_CTL;     // 0x024
    volatile uint32_t HPBG_CTL;           // 0x028
    volatile uint32_t LPBG_CTL;           // 0x02C
    volatile uint32_t BUCK_CTL;           // 0x030
    volatile uint32_t ON1_TIME;           // 0x034
    volatile uint32_t ON2_TIME;           // 0x038
    volatile uint32_t ON3_TIME;           // 0x03C
    volatile uint32_t ON4_TIME;           // 0x040
    volatile uint32_t ON5_TIME;           // 0x044
    volatile uint32_t ON6_TIME;           // 0x048
    volatile uint32_t ON7_TIME;           // 0x04C
    volatile uint32_t CPOR_N_ON_TIME;     // 0x050
    volatile uint32_t reserve_054;        // 0x054, reserved
    volatile uint32_t SPS_TIMER_PRESET;   // 0x058
    volatile uint32_t SON1_TIME;          // 0x05C
    volatile uint32_t SON2_TIME;          // 0x060
    volatile uint32_t SON3_TIME;          // 0x064
    volatile uint32_t SON4_TIME;          // 0x068
    volatile uint32_t SON5_TIME;          // 0x06C
    volatile uint32_t SON6_TIME;          // 0x070
    volatile uint32_t SON7_TIME;          // 0x074
    volatile uint32_t SCPOR_N_ON_TIME;    // 0x078
    volatile uint32_t PU_CTL;             // 0x07C
    volatile uint32_t OSC_CTL;            // 0x080
    volatile uint32_t PMS_SPARE;          // 0x084, HW reservd for debug
    volatile uint32_t ADC_CTL;            // 0x088
    volatile uint32_t LDO_CTL;            // 0x08C
    volatile uint32_t RG_PD_IE;           // 0x090
    volatile uint32_t RG_PD_PE;           // 0x094
    volatile uint32_t RG_PD_O_INV;        // 0x098
    volatile uint32_t RG_PD_DS;           // 0x09C
    volatile uint32_t RG_GPO;             // 0x0A0
    volatile uint32_t RG_PD_I_INV;        // 0x0A4
    volatile uint32_t RG_PDOV_MODE;       // 0x0A8
    volatile uint32_t RG_PD_DIR;          // 0x0AC
    volatile uint32_t RG_PD_OENP_INV;     // 0x0B0
    volatile uint32_t RG_PDOC_MODE;       // 0x0B4
    volatile uint32_t RG_GPI;             // 0x0B8
    volatile uint32_t reserve_0bc;        // 0x0BC, reserved
    volatile uint32_t RG_PDI_SRC_IO_A;    // 0x0C0
    volatile uint32_t RG_PDI_SRC_IO_B;    // 0x0C4
    volatile uint32_t RG_PDI_SRC_IO_C;    // 0x0C8
    volatile uint32_t RG_PDI_SRC_IO_D;    // 0x0CC
    volatile uint32_t RG_PTS_INMUX_A;     // 0x0D0
    volatile uint32_t RG_PTS_INMUX_B;     // 0x0D4
    volatile uint32_t RG_PTS_INMUX_C;     // 0x0D8
    volatile uint32_t RG_PTS_INMUX_D;     // 0x0DC
    volatile uint32_t RG_SRAM_IOS_EN;     // 0x0E0
    volatile uint32_t RG_SRAM_RET_OFF;    // 0x0E4
    volatile uint32_t RG_PHY_WR_SRAM;     // 0x0E8
    volatile uint32_t RG_PHY_RD_SRAM;     // 0x0EC
    volatile uint32_t CAL_CEN;            // 0x0F0
    volatile uint32_t CAL_STR;            // 0x0F4
    volatile uint32_t SDM_PT_SEL;         // 0x0F8
    volatile uint32_t SDM_CTL;            // 0x0FC
    volatile uint32_t R_STRAP_MODE_CTL;   // 0x100
    volatile uint32_t R_APS_SWRST;        // 0x104
    volatile uint32_t R_MSQ_SWRST;        // 0x108
    volatile uint32_t RG_SPARE;           // 0x10C
    volatile uint32_t RG_PTS_INMUX_E;     // 0x110
    volatile uint32_t RG_PTS_INMUX_F;     // 0x114
    volatile uint32_t RG_SRAM_RET_ACK;    // 0x118
    volatile uint32_t RG_MSQ_ROM_MAP;     // 0x11C
    volatile uint32_t RG_AOS_ID;          // 0x120
    volatile uint32_t RG_SPARE_1;         // 0x124
    volatile uint32_t RG_RSTS;            // 0x128
    volatile uint32_t RG_SPARE_2;         // 0x12C
    volatile uint32_t RG_SPARE_3;         // 0x130
    volatile uint32_t R_M3CLK_SEL;        // 0x134
    volatile uint32_t R_M0CLK_SEL;        // 0x138
    volatile uint32_t R_RFPHY_SEL;        // 0x13C
    volatile uint32_t R_SCRT_EN;          // 0x140
    volatile uint32_t reserve_144[21];    // 0x144 ~ 0x194, move to sys_reg
    volatile uint32_t R_CLK_MMFACTOR_CM3; // 0x198
} S_Aos_Reg_t;

typedef struct
{
    volatile uint32_t reserve_000[2];     // 0x000 ~ 0x004, reserved
    volatile uint32_t RG_SPARE;           // 0x008
    volatile uint32_t reserve_00C[15];    // 0x00C ~ 0x044, reserved
    volatile uint32_t RG_CK_GATE_CTRL;    // 0x048
    volatile uint32_t reserve_04C[4];     // 0x04C ~ 0x058, reserved
    volatile uint32_t AUXADC_CTRL0;       // 0x05C
    volatile uint32_t reserve_060[1];     // 0x060 ~ 0x060, reserved
    volatile uint32_t RG_AUX_IN_SEL;      // 0x064
    volatile uint32_t reserve_068[10];    // 0x068 ~ 0x08C, reserved
    volatile uint32_t PU_VAL;             // 0x090
    volatile uint32_t reserve_094[14];    // 0x094 ~ 0x0C8, reserved
    volatile uint32_t AUX_ADC_CK_GEN_CTL; // 0x0CC
    volatile uint32_t RG_AUX_ADC_ECL_OUT; // 0x0D0
} S_Rf_Reg_t;


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern uint8_t g_ubHalAux_Init;
extern E_HalAux_Src_t g_tHalAux_CurrentType;
extern uint8_t g_ubHalAux_CurrentGpioIdx;
extern osSemaphoreId g_taHalAux_SemaphoreId;

uint8_t g_ubHalAux_Cal;
uint8_t g_ubHalAux_AdcAlwaysOn;
uint32_t g_ulHalAux_PuEnDelay_Us;

char *pAuxadcSrcName[ HAL_AUX_SRC_MAX_PATCH ] =
{
    "GPIO",    // HAL_AUX_SRC_GPIO,
    "VBAT",    // HAL_AUX_SRC_VBAT,
    "LDO_VCO", // HAL_AUX_SRC_LDO_VCO,
    "LDO_RF",  // HAL_AUX_SRC_LDO_RF,
    "TEMP",    // HAL_AUX_SRC_TEMP_SEN,
    "HPBG",    // HAL_AUX_SRC_HPBG_REF,
    "LPBG",    // HAL_AUX_SRC_LPBG_REF,
    "PMU_SF",  // HAL_AUX_SRC_PMU_SF,
    "VSS"      // HAL_AUX_SRC_VSS
};

uint16_t u16LdoRf_MiniVol[4];
S_AuxadcCalTable_t sAuxadcCalTable;
float g_fSlope; // RawData/mv
float g_fOffset;

E_HalAux_Src_t g_tAuxAdcSrc = HAL_AUX_SRC_MAX;
uint8_t g_ubAuxAdcGpioIdx = 0;

// Sec 5: declaration of global function prototype



/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
/* Config Average times*/ 
uint32_t Hal_Aux_Adc_AvgCnt_Get( void )
{
    return g_ulHalAux_AverageCount;
}

/* Config Average times*/ 
uint32_t Hal_Aux_Adc_AvgCnt_Set(uint32_t u32AvgCnt)
{
    if( u32AvgCnt == 0 )
        return HAL_AUX_FAIL;

    g_ulHalAux_AverageCount = u32AvgCnt;

    return HAL_AUX_OK;
}

uint32_t Hal_Aux_Adc_PuEnDelay_Get( void )
{
    return g_ulHalAux_PuEnDelay_Us;
}

/* Config PuEn delay. This value caculate from Application Note*/
uint32_t Hal_Aux_Adc_PuEnDelay_Set(uint32_t u32DelayUs)
{
    #if 0
    if( u32DelayUs > DEFAULT_AUXADC_PU_EN_DELAY_US_MAX )
        return HAL_AUX_FAIL;
    #endif

    g_ulHalAux_PuEnDelay_Us = u32DelayUs;

    return HAL_AUX_OK;
}

/* Regression function for AUXADC */
void Hal_Aux_LseRegressUpdate(uint8_t u8Num, S_AuxadcCal_t *puStAdcCalPts)
{
    uint8_t u8Idx = 0;
    float fAvgMiniVol = 0;
    float fAvgAdcCode = 0;
    float fTemp_diff_x = 0, fTemp_diff_y = 0;
    float fTemp_den = 0, fTemp_num = 0;
    
    // LSE regression
    // Step 1: compute AVG
    for(u8Idx = 0; u8Idx<u8Num ; u8Idx++)
    {
        fAvgMiniVol += (float)puStAdcCalPts[ u8Idx ].u16MiniVolt;
        fAvgAdcCode += (float)puStAdcCalPts[ u8Idx ].u16RawData;
    }
    fAvgMiniVol /= u8Num;
    fAvgAdcCode /= u8Num;

    // Step 2: compute slove and offset
    for(u8Idx = 0; u8Idx<u8Num ; u8Idx++)
    {
        fTemp_diff_x = (float)puStAdcCalPts[ u8Idx ].u16MiniVolt - fAvgMiniVol;
        fTemp_diff_y = (float)puStAdcCalPts[ u8Idx ].u16RawData  - fAvgAdcCode;
        
        fTemp_den += fTemp_diff_x * fTemp_diff_y;
        fTemp_num += fTemp_diff_x * fTemp_diff_x;
    }
    
    g_fSlope  = fTemp_den/fTemp_num; // RawData/mv
    g_fOffset = fAvgAdcCode - g_fSlope*fAvgMiniVol;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_SourceSelect
*
* DESCRIPTION:
*   select the source of AUXADC
*
* PARAMETERS
*   1. tSrc      : [In] the source type of AUXADC
*   2. ubGpioIdx : [In] the index of GPIO
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_SourceSelect_patch(E_HalAux_Src_t tSrc, uint8_t ubGpioIdx)
{
    uint8_t ubRet = HAL_AUX_FAIL;

    // check the source type
    if (tSrc >= HAL_AUX_SRC_MAX_PATCH)
        goto done;

    // check the index of GPIO
    if (tSrc == HAL_AUX_SRC_GPIO)
    {
        if (ubGpioIdx >= HAL_AUX_GPIO_NUM_MAX)
            goto done;
    }

    g_tAuxAdcSrc = tSrc;
    g_ubAuxAdcGpioIdx = ubGpioIdx;

    ubRet = HAL_AUX_OK;

done:
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcValueGet
*
* DESCRIPTION:
*   get the ADC value from AUXADC
*
* PARAMETERS
*   1. pulValue : [Out] the ADC value
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcValueGet_patch(uint32_t *pulValue)
{
    uint8_t ubRet = HAL_AUX_FAIL;
    T_AuxAdcReq tReq = {0};
    T_AuxAdcRsp tRsp = {0};

    tReq.u8Type          = SYS_CFG_AUXADC_VAL_GET;
    tReq.u8Src           = g_tAuxAdcSrc;
    tReq.u8GpioIdx       = g_ubAuxAdcGpioIdx;
    tReq.u8AdcAlwaysOn   = g_ubHalAux_AdcAlwaysOn;

    tReq.u32RepeatCnt    = g_ulHalAux_AverageCount;
    tReq.u32PuEnDelay_Us = g_ulHalAux_PuEnDelay_Us;

    if(sys_cfg_auxadc_req(&tReq, &tRsp))
    {
        goto done;
    }

    *pulValue = tRsp.u32Value;

    ubRet = HAL_AUX_OK;

done:
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_LoadTable
*
* DESCRIPTION:
*   Load calibration data from flash
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcCal_LoadFlash( void )
{
    uint32_t u32Res = 0;
    uint32_t u32Header = 0;
    
    u32Res = Hal_Flash_AddrRead(SPI_IDX_0, AUXADC_FLASH_START_ADDR, 0, sizeof(sAuxadcCalTable.u32Header), (uint8_t *)&u32Header);
    if( u32Header == 0xFFFFFFFF)
        return HAL_AUX_FAIL;

    u32Res = Hal_Flash_AddrRead(SPI_IDX_0, AUXADC_FLASH_START_ADDR, 0, sizeof(sAuxadcCalTable), (uint8_t *)&sAuxadcCalTable);

    if( u32Res != 0 )
    {
        // Error happen
        return HAL_AUX_FAIL;
    }else{
        // No error, updated slope and offset
        Hal_Aux_LseRegressUpdate(2, sAuxadcCalTable.stIntSrc);
        return HAL_AUX_OK;
    }
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_EraseFlash
*
* DESCRIPTION:
*   Erase calibration data to flash
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcCal_EraseFlash( void )
{
    uint32_t u32Res = 0;
    
    u32Res = Hal_Flash_4KSectorAddrErase( SPI_IDX_0, AUXADC_FLASH_START_ADDR);
    if( u32Res != 0)
        return HAL_AUX_FAIL;
    else
        return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_StoreFlash
*
* DESCRIPTION:
*   Store calibration data to flash
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcCal_StoreFlash( void )
{
    uint32_t u32Res = 0;
    
    u32Res = Hal_Aux_AdcCal_EraseFlash();
    if( u32Res != HAL_AUX_OK)
        return HAL_AUX_FAIL;

    u32Res = Hal_Flash_AddrProgram(SPI_IDX_0, AUXADC_FLASH_START_ADDR, 0, sizeof(sAuxadcCalTable), (uint8_t *)&sAuxadcCalTable);
    if( u32Res != 0)
        return HAL_AUX_FAIL;
    else
        return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_LoadDef
*
* DESCRIPTION:
*   Init default setting from internal sources
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcCal_LoadDef( void )
{
    sAuxadcCalTable.stIntSrc[ 0 ].u16MiniVolt = 0;
    sAuxadcCalTable.stIntSrc[ 0 ].u16RawData  = 0;

    sAuxadcCalTable.stIntSrc[ 1 ].u16MiniVolt = 0;
    sAuxadcCalTable.stIntSrc[ 1 ].u16RawData  = 0;

    g_fSlope = 0.3; // RawData/mv
    g_fOffset = 64;

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_LoadOtp
*
* DESCRIPTION:
*   Load default setting form OTP
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcCal_LoadOtp( void )
{
    uint8_t u8Idx = 0;
    uint32_t u32Otp_temp;
    S_AuxadcCal_t stOtpRef[5];

    for(u8Idx = 0; u8Idx<5; u8Idx++)
    {
        Hal_Sys_OtpRead((0x140 + 4*u8Idx), (uint8_t *)&u32Otp_temp, 4);
        
        stOtpRef[ u8Idx ].u16MiniVolt = (u8Idx + 1)*500;
        stOtpRef[ u8Idx ].u16RawData = u32Otp_temp;
    }

    if(  stOtpRef[ 0 ].u16RawData ==  stOtpRef[ 4 ].u16RawData )
        return HAL_AUX_FAIL;

    sAuxadcCalTable.stIntSrc[ 0 ].u16MiniVolt = stOtpRef[ 0 ].u16MiniVolt;
    sAuxadcCalTable.stIntSrc[ 0 ].u16RawData  = stOtpRef[ 0 ].u16RawData;
    
    sAuxadcCalTable.stIntSrc[ 1 ].u16MiniVolt = stOtpRef[ 4 ].u16MiniVolt;
    sAuxadcCalTable.stIntSrc[ 1 ].u16RawData  = stOtpRef[ 4 ].u16RawData;
    
    // Updated slope and offset
    Hal_Aux_LseRegressUpdate(5, stOtpRef);

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_Init
*
* DESCRIPTION:
*   init data for convert raw-data to mini-volt
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Aux_AdcCal_Init( void )
{
    if(g_ubHalAux_Cal)
        return;

    if( Hal_Aux_AdcCal_LoadFlash() == HAL_AUX_OK )
    {
        // Load from flash
        }
    else if( Hal_Aux_AdcCal_LoadOtp() == HAL_AUX_OK )
        {
        // Load from OTP
            }
    else
    {
        // Load from internal sources
        Hal_Aux_AdcCal_LoadDef();
    }
    g_ubHalAux_Cal = 1;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcGpioInCal
*
* DESCRIPTION:
*   Cal auxadc value of given GPIO
*
* PARAMETERS
*   1. u8GpioIdx   : [In] the given GPIO
*   2. u16MiniVolt : [In] the input mVolt
*   3. u8PtsIdx    : [In] the index of calibration pts
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcGpioInCal(uint8_t u8GpioIdx, uint16_t u16MiniVolt, uint8_t u8PtsIdx)
{
    uint32_t u32Temp = 0;
    uint32_t u32Res = 0;

    // Check input valid
    if (u8GpioIdx >= HAL_AUX_GPIO_NUM_MAX)
        return HAL_AUX_FAIL;
    if(u8PtsIdx >= HAL_AUX_CAL_PTS_NUM)
        return HAL_AUX_FAIL;

    // Get ADC raw-data
    u32Res = Hal_Aux_AdcRawData_Get( (E_HalAux_Src_Patch_t)HAL_AUX_SRC_GPIO, u8GpioIdx, &u32Temp);
    if(u32Res == HAL_AUX_FAIL)
        return HAL_AUX_FAIL;

    // Update table
    sAuxadcCalTable.stIntSrc[ u8PtsIdx ].u16MiniVolt = u16MiniVolt;
    sAuxadcCalTable.stIntSrc[ u8PtsIdx ].u16RawData = u32Temp;
    Hal_Aux_LseRegressUpdate(2, sAuxadcCalTable.stIntSrc);

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcVbatInCal
*
* DESCRIPTION:
*   Cal auxadc value of given Vbat
*
* PARAMETERS
*   1. u16MiniVolt : [In] the input mVolt
*   2. u8PtsIdx    : [In] the index of calibration pts
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcVbatInCal(uint16_t u16MiniVolt, uint8_t u8PtsIdx)
{
    uint32_t u32Temp = 0;
    uint32_t u32Res = 0;

    // Check input valid
    if(u8PtsIdx >= HAL_AUX_CAL_PTS_NUM)
        return HAL_AUX_FAIL;

    // Get ADC raw-data
    u32Res = Hal_Aux_AdcRawData_Get( (E_HalAux_Src_Patch_t)HAL_AUX_SRC_VBAT, 0, &u32Temp);
    if(u32Res == HAL_AUX_FAIL)
        return HAL_AUX_FAIL;

    // Update table
    sAuxadcCalTable.stIntSrc[ u8PtsIdx ].u16MiniVolt = u16MiniVolt;
    sAuxadcCalTable.stIntSrc[ u8PtsIdx ].u16RawData = u32Temp;
    Hal_Aux_LseRegressUpdate(2, sAuxadcCalTable.stIntSrc);

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcRawData_Get
*
* DESCRIPTION:
*   Got AUXADC raw-data
*
* PARAMETERS
*   1. tSrc      : [In] the source type of AUXADC
*   2. ubGpioIdx : [In] the index of GPIO
*   3. pulRaw  : [Out] the ADC rawData 
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcRawData_Get( E_HalAux_Src_Patch_t tSrc, uint8_t ubGpioIdx, uint32_t *pulRaw)
{
    uint8_t ubRet = HAL_AUX_FAIL;

    // check init
    if(g_ubHalAux_Init != 1)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalAux_SemaphoreId, osWaitForever);

    if( HAL_AUX_OK != Hal_Aux_SourceSelect( (E_HalAux_Src_t)tSrc, ubGpioIdx) )
        goto done;

    if( HAL_AUX_OK != Hal_Aux_AdcValueGet(pulRaw) )
        goto done;

    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcMiniVolt_Convert
*
* DESCRIPTION:
*   Convert given AUXADC raw-data to mVolt
*
* PARAMETERS
*   1. u32RawData: [In] the given ADC rawData 
*
* RETURNS
*   1. float : mVolt
*
*************************************************************************/
float Hal_Aux_AdcMiniVolt_Convert(uint32_t u32RawData)
{
    return ((float)u32RawData - g_fOffset)/g_fSlope;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcMiniVolt_Get
*
* DESCRIPTION:
*   Got AUXADC voltage in mVolt
*
* PARAMETERS
*   1. tSrc      : [In] the source type of AUXADC
*   2. ubGpioIdx : [In] the index of GPIO
*   3. pfVbat    : [Out] the mVolt of the source
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcMiniVolt_Get( E_HalAux_Src_Patch_t tSrc, uint8_t ubGpioIdx, float *pfVbat)
{
    uint32_t ulAdcValue = 0;

    if( HAL_AUX_OK != Hal_Aux_AdcRawData_Get(tSrc, ubGpioIdx, &ulAdcValue) )
        return HAL_AUX_FAIL;

    // Convert RawData to mVlot
    *pfVbat = (float)Hal_Aux_AdcMiniVolt_Convert(ulAdcValue);
    if(*pfVbat < 0)
    {
        *pfVbat = 0;
    }

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcConvValue_Get
*
* DESCRIPTION:
*   Got the converted ADC value (map 0x000~0x3FF to 0 ~ 3000 mV)
*
* PARAMETERS
*   1. tSrc      : [In] the source type of AUXADC
*   2. ubGpioIdx : [In] the index of GPIO
*   3. pulValue  : [Out] the converted ADC value
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcConvValue_Get( E_HalAux_Src_Patch_t tSrc, uint8_t ubGpioIdx, uint32_t *pulValue)
{
    float fAdcVol = 0;

    if( HAL_AUX_OK != Hal_Aux_AdcMiniVolt_Get(tSrc, ubGpioIdx, &fAdcVol) )
        return HAL_AUX_FAIL;

    // mVlot convert to ideal ADC code
    *pulValue = (uint32_t)( (fAdcVol * 1023) + 3000/2 ) / 3000;
    if( *pulValue >0x3FF)
    {
        *pulValue = 0x3FF;
    }

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcConvValue_Get_v2
*
* DESCRIPTION:
*   New API for gotting the converted ADC value (map 0x000~0x3FF to 0 ~ 3000 mV)
*
* PARAMETERS
*   1. tSrc      : [In] the source type of AUXADC
*   2. ubGpioIdx : [In] the index of GPIO
*   3. ulAvgCnt  : [In] the average counts of the source
*   4. ulDelayUs : [In] the delay times for R-R-C type source
*   5. pulValue  : [Out] the converted ADC value
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcConvValue_Get_v2( E_HalAux_Src_Patch_t tSrc, uint8_t ubGpioIdx, uint32_t ulAvgCnt, uint32_t ulDelayUs, uint32_t *pulValue)
{
    g_ulHalAux_AverageCount = ulAvgCnt;
    g_ulHalAux_PuEnDelay_Us = ulDelayUs;

    return Hal_Aux_AdcConvValue_Get( tSrc, ubGpioIdx, pulValue);
}
/*************************************************************************
* FUNCTION:
*   Hal_Aux_VbatGet
*
* DESCRIPTION:
*   get the VBAT value from AUXADC
*
* PARAMETERS
*   1. pfVbat : [Out] the VBAT value
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_VbatGet_patch(float *pfVbat)
{
    if( HAL_AUX_OK != Hal_Aux_AdcMiniVolt_Get( (E_HalAux_Src_Patch_t)HAL_AUX_SRC_VBAT, 0, pfVbat) )
    {
        return HAL_AUX_FAIL;
    }else{
        *pfVbat /= 1000;
        return HAL_AUX_OK;
    }
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_IoVoltageGet
*
* DESCRIPTION:
*   get the IO voltage value from AUXADC
*
* PARAMETERS
*   1. ubGpioIdx : [In] the index of GPIO
*   2. pfVoltage : [Out] the IO voltage value
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_IoVoltageGet_patch(uint8_t ubGpioIdx, float *pfVoltage)
{
    if( HAL_AUX_OK != Hal_Aux_AdcMiniVolt_Get((E_HalAux_Src_Patch_t)HAL_AUX_SRC_GPIO, ubGpioIdx, pfVoltage) )
    {
        return HAL_AUX_FAIL;
    }else{
        *pfVoltage /= 1000;
        return HAL_AUX_OK;
    }
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_Init
*
* DESCRIPTION:
*   AUXADC init
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Aux_Init_Patch(void)
{
    osSemaphoreDef_t tSemaphoreDef;

    if( g_ubHalAux_Init )
        return;

    // Set default source
    Hal_Aux_SourceSelect( HAL_AUX_SRC_GPIO, 0);

    // create the semaphore
    tSemaphoreDef.dummy = 0; // reserved, it is no used
    g_taHalAux_SemaphoreId = osSemaphoreCreate(&tSemaphoreDef, 1);
    if (g_taHalAux_SemaphoreId == NULL)
    {
        printf("To create the semaphore for Hal_Aux is fail.\n");
        return;
    }

    // after init
    g_ubHalAux_Init = 1;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_PatchInit
*
* DESCRIPTION:
*   Patch function initialization
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Aux_PatchInit(void)
{
    g_ubHalAux_Init = 0;
    g_tHalAux_CurrentType = HAL_AUX_SRC_GPIO;
    g_ubHalAux_CurrentGpioIdx = 0;
    g_ubHalAux_Cal = 0;
    g_ulHalAux_AverageCount = DEFAULT_AUXADC_AVG_CNT;
    g_ulHalAux_PuEnDelay_Us = DEFAULT_AUXADC_PU_EN_DELAY_US;

    Hal_Aux_AdcCal_LoadDef();

    // Here set via AOS document
    u16LdoRf_MiniVol[0] = 1000; // 1 V
    u16LdoRf_MiniVol[1] = 1100; // 1.1 V
    u16LdoRf_MiniVol[2] = 1200; // 1.2 V
    u16LdoRf_MiniVol[3] = 1300; // 1.3V

    Hal_Aux_Init = Hal_Aux_Init_Patch;
    Hal_Aux_SourceSelect = Hal_Aux_SourceSelect_patch;
    Hal_Aux_AdcValueGet = Hal_Aux_AdcValueGet_patch;
    Hal_Aux_VbatGet = Hal_Aux_VbatGet_patch;
    Hal_Aux_IoVoltageGet = Hal_Aux_IoVoltageGet_patch;

    // Patch for warning. Do NOT use this function
    //Hal_Aux_VbatCalibration = Hal_Aux_VbatCalibration_impl;
    //Hal_Aux_IoVoltageCalibration = Hal_Aux_IoVoltageCalibration_impl;
}

