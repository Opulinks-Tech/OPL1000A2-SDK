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
#define RF_CORE_MODE_MAX                    8
#define RF_CLK_GATE_CTRL_AUXADC             (1UL << 6)
#define RF_POWER_CTRL_AUXADC                (1UL << 26)
extern uint32_t g_u32aMsqRfModeTable[ RF_CORE_MODE_NUM ][ RF_CTRL_IDX_NUM ];

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
extern uint32_t g_ulHalAux_AverageCount;

uint8_t g_ubHalAux_Cal;
uint8_t g_ubHalAux_Pu_WriteDirect;

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

// Sec 5: declaration of global function prototype



/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype
void Hal_Aux_AdcUpdateCtrlReg(uint32_t u32Enable);

/***********
C Functions
***********/
// Sec 8: C Functions

/* Update RF ctrl table before AUXADC starts */
void Hal_Aux_AdcUpdateCtrlReg(uint32_t u32Enable)
{
    uint8_t u8ModeIdx;

    if (u32Enable)
    {
        for (u8ModeIdx = 0; u8ModeIdx<RF_CORE_MODE_MAX; u8ModeIdx++)
        {
            g_u32aMsqRfModeTable[u8ModeIdx][RF_MODE_TABLE_IDX_CLK_GATE_CTRL] |= RF_CLK_GATE_CTRL_AUXADC;
            g_u32aMsqRfModeTable[u8ModeIdx][RF_MODE_TABLE_IDX_POWER_CTRL] |= RF_POWER_CTRL_AUXADC;
        }

        if( g_ubHalAux_Pu_WriteDirect )
        {
            // Turn on PU of AUXADC
            RF->PU_VAL |= (0x1 << 26);

            // Turn on clock to AUXADC
            RF->RG_CK_GATE_CTRL |= (0x1 << 6);
        }
    }
    else
    {
        for (u8ModeIdx = 0; u8ModeIdx<RF_CORE_MODE_MAX; u8ModeIdx++)
        {
            g_u32aMsqRfModeTable[u8ModeIdx][RF_MODE_TABLE_IDX_CLK_GATE_CTRL] &= ~RF_CLK_GATE_CTRL_AUXADC;
            g_u32aMsqRfModeTable[u8ModeIdx][RF_MODE_TABLE_IDX_POWER_CTRL] &= ~RF_POWER_CTRL_AUXADC;
        }
        
        if( g_ubHalAux_Pu_WriteDirect )
        {
            // Turn off PU of AUXADC
            RF->PU_VAL &= ~(0x1 << 26);

            // Turn off clock to AUXADC
            RF->RG_CK_GATE_CTRL &= ~(0x1 << 6);
        }
    }
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
    volatile uint32_t tmp;
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

    // Select output from PMU side
    tmp = AOS->PMS_SPARE;
    tmp &= ~(0x7 << 1);
    if(tSrc == HAL_AUX_SRC_GPIO)
        tmp |= (1 << 1);
    else if(tSrc == HAL_AUX_SRC_VBAT)
        tmp |= (0 << 1);
    else if(tSrc == HAL_AUX_SRC_VSS)
        tmp |= (5<< 1);
    else
        tmp |= (tSrc << 1);
    AOS->PMS_SPARE = tmp;
    
    g_tHalAux_CurrentType = tSrc;

    // Select input to AUXADC
    // GPIO
    if (tSrc == HAL_AUX_SRC_GPIO)
    {
        // External
        tmp = RF->RG_SPARE;
        tmp &= ~(0x1 << 19);
        tmp |= (0x0 << 19);
        RF->RG_SPARE = tmp;
        // Select the IO pin
        tmp = RF->RG_AUX_IN_SEL;
        tmp &= ~(0xF << 0);
        tmp |= (ubGpioIdx << 0);
        RF->RG_AUX_IN_SEL = tmp;
        
        g_ubHalAux_CurrentGpioIdx = ubGpioIdx;
    }
    // others
    else
    {
        // Select input to AUXADC
        // Internal
        tmp = RF->RG_SPARE;
        tmp &= ~(0x1 << 19);
        tmp |= (0x1 << 19);
        RF->RG_SPARE = tmp;
        // Fix to the channel 1
        tmp = RF->RG_AUX_IN_SEL;
        tmp &= ~(0xF << 0);
        tmp |= (0x1 << 0);
        RF->RG_AUX_IN_SEL = tmp;
    }

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
    volatile uint32_t tmp;
    volatile uint32_t i;
    uint8_t ubRet = HAL_AUX_FAIL;

    uint32_t ulAdcValue;
    uint32_t ulAdcCurrent;
    uint32_t ulRepeatCount;
    uint32_t j;
    uint32_t AUXEqualZeroCounter = 0;

    uint32_t ulCurrentTick;
    uint32_t ulDiffTick;
    uint32_t ulEscapeTime = HAL_AUX_ADC_READ_ERROR_TIME * Hal_Tick_PerMilliSec();

    Hal_Aux_AdcUpdateCtrlReg(1);

    // Enable AUXADC
    tmp = AOS->ADC_CTL;
    tmp &= ~(0x1 << 0);
    tmp |= (0x1 << 0);
    AOS->ADC_CTL = tmp;

    // Enable the internal temperature sensor
    if (g_tHalAux_CurrentType == HAL_AUX_SRC_TEMP_SEN)
    {
        tmp = AOS->HPBG_CTL;
        tmp &= ~(0x1 << 18);
        tmp |= (0x1 << 18);
        AOS->HPBG_CTL = tmp;
    }else if (g_tHalAux_CurrentType == HAL_AUX_SRC_LDO_VCO)
    {
        tmp = RF->RG_SPARE;
        tmp &= ~(0x1 << 22);
        tmp |= (0x1 << 22);
        RF->RG_SPARE = tmp;
    }
    else if (g_tHalAux_CurrentType == HAL_AUX_SRC_LDO_RF)
    {
        tmp = RF->RG_SPARE;
        tmp &= ~(0x1 << 23);
        tmp |= (0x1 << 23);
        RF->RG_SPARE = tmp;
    }else if (g_tHalAux_CurrentType == HAL_AUX_SRC_HPBG_REF)
    {
        tmp = AOS->PMS_SPARE;
        tmp &= ~(0x1 << 4);
        tmp |= (0x1 << 4);
        AOS->PMS_SPARE = tmp;
    }

    if (g_ulHalAux_AverageCount == 0)
        g_ulHalAux_AverageCount = 1;

    ulAdcValue = 0;
    ulRepeatCount = g_ulHalAux_AverageCount;
    for (j=0; j<ulRepeatCount; j++)
    {
        // Trigger
        tmp = RF->AUX_ADC_CK_GEN_CTL;
        tmp &= ~(0x1 << 0);
        tmp |= (0x1 << 0);
        RF->AUX_ADC_CK_GEN_CTL = tmp;

        ulCurrentTick = Hal_Tick_Diff(0);
        do
        {
            // get the ADC value
            i = 0;
            while (RF->RG_AUX_ADC_ECL_OUT & RF_RG_EOCB)
            {
                if (i >= 0xFF)
                    goto done;
                i++;
            }
            ulAdcCurrent = RF->RG_AUX_ADC_ECL_OUT & 0x03FF;
            ulAdcValue = ulAdcValue + ulAdcCurrent;

            // error handle if always zero
            ulDiffTick = Hal_Tick_Diff(ulCurrentTick);
            if (ulDiffTick >= ulEscapeTime)
            {
                osDelay(10);
                break;
            }

        } while (ulAdcCurrent == 0);

        if (ulAdcCurrent == 0)
            AUXEqualZeroCounter++;

#if 0
        // !!! ADC value should be not zero, the DC offset is not zero
        if (ulAdcCurrent == 0)
        {
            // error handle if always zero
            if (ulRepeatCount < (g_ulHalAux_AverageCount * HAL_AUX_ADC_ERROR_COUNT))
                ulRepeatCount++;
        }
#endif

        // Idle (non-trigger)
        tmp = RF->AUX_ADC_CK_GEN_CTL;
        tmp &= ~(0x1 << 0);
        tmp |= (0x0 << 0);
        RF->AUX_ADC_CK_GEN_CTL = tmp;
    }

    // Disable AUXADC
    tmp = AOS->ADC_CTL;
    tmp &= ~(0x1 << 0);
    tmp |= (0x0 << 0);
    AOS->ADC_CTL = tmp;

    // Disable the internal temperature sensor
    if (g_tHalAux_CurrentType == HAL_AUX_SRC_TEMP_SEN)
    {
        tmp = AOS->HPBG_CTL;
        tmp &= ~(0x1 << 18);
        tmp |= (0x0 << 18);
        AOS->HPBG_CTL = tmp;
    }else if (g_tHalAux_CurrentType == HAL_AUX_SRC_LDO_VCO)
    {
        tmp = RF->RG_SPARE;
        tmp &= ~(0x1 << 22);
        tmp |= (0x0 << 22);
        RF->RG_SPARE = tmp;
    }
    else if (g_tHalAux_CurrentType == HAL_AUX_SRC_LDO_RF)
    {
        tmp = RF->RG_SPARE;
        tmp &= ~(0x1 << 23);
        tmp |= (0x0 << 23);
        RF->RG_SPARE = tmp;
    }else if (g_tHalAux_CurrentType == HAL_AUX_SRC_HPBG_REF)
    {
        tmp = AOS->PMS_SPARE;
        tmp &= ~(0x1 << 4);
        tmp |= (0x0 << 4);
        AOS->PMS_SPARE = tmp;
    }

    Hal_Aux_AdcUpdateCtrlReg(0);

    *pulValue = (ulAdcValue + ((g_ulHalAux_AverageCount - AUXEqualZeroCounter) / 2)) / (g_ulHalAux_AverageCount - AUXEqualZeroCounter);
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
uint32_t Hal_Aux_AdcCal_LoadTable( void )
{
    uint32_t u32Res = 0;
    u32Res = Hal_Flash_AddrRead(SPI_IDX_0, AUXADC_FLASH_START_ADDR, 0, sizeof(sAuxadcCalTable), (uint8_t *)&sAuxadcCalTable);
    
    if( u32Res != 0)
        return HAL_AUX_FAIL;
    else
        return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_StoreTable
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
uint32_t Hal_Aux_AdcCal_StoreTable( void )
{
    uint32_t u32Res = 0;
    
    u32Res = Hal_Flash_4KSectorAddrErase( SPI_IDX_0, AUXADC_FLASH_START_ADDR);
    if( u32Res != 0)
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
*   Load default setting for all sources
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
    uint8_t u8Idx = 0;
    uint32_t u32Temp = 0;
    uint32_t u32Res = 0;

    // source VSS
    u32Res =Hal_Aux_SourceSelect( (E_HalAux_Src_t)HAL_AUX_SRC_VSS, 0 );
    if( u32Res == HAL_AUX_FAIL )
        return HAL_AUX_FAIL;
    u32Temp = 0;
    u32Res = Hal_Aux_AdcValueGet(&u32Temp);
    if( u32Res == HAL_AUX_FAIL )
        return HAL_AUX_FAIL;
    sAuxadcCalTable.stIntSrc[ 0 ].u16MiniVolt = 30;
    sAuxadcCalTable.stIntSrc[ 0 ].u16RawData  = u32Temp;

    // Source LDO_RF
    u32Res = Hal_Aux_SourceSelect( HAL_AUX_SRC_LDO_RF, 0 );
    if( u32Res == HAL_AUX_FAIL )
        return HAL_AUX_FAIL;
    u32Temp = 0;
    u32Res = Hal_Aux_AdcValueGet(&u32Temp);
    if( u32Res == HAL_AUX_FAIL )
        return HAL_AUX_FAIL;
    u8Idx = ( reg_read(0x4000108c) & 0x00000180 ) >> 7;
    sAuxadcCalTable.stIntSrc[ 1 ].u16MiniVolt = u16LdoRf_MiniVol[ u8Idx ];
    sAuxadcCalTable.stIntSrc[ 1 ].u16RawData  = u32Temp;

    // Setup GPIO default
    for(u8Idx = 0; u8Idx<HAL_AUX_GPIO_NUM_MAX; u8Idx++)
    {
        sAuxadcCalTable.u32Header = ADCCAL_VER;
        sAuxadcCalTable.stGpioSrc[ u8Idx ][ 0 ].u16MiniVolt = sAuxadcCalTable.stIntSrc[ 0 ].u16MiniVolt;
        sAuxadcCalTable.stGpioSrc[ u8Idx ][ 0 ].u16RawData  = sAuxadcCalTable.stIntSrc[ 0 ].u16RawData;
        sAuxadcCalTable.stGpioSrc[ u8Idx ][ 1 ].u16MiniVolt = sAuxadcCalTable.stIntSrc[ 1 ].u16MiniVolt;
        sAuxadcCalTable.stGpioSrc[ u8Idx ][ 1 ].u16RawData  = sAuxadcCalTable.stIntSrc[ 1 ].u16RawData;
    }
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
    uint32_t u32Res = 0;
    uint32_t u32Header = 0;

    if(g_ubHalAux_Cal)
        return;

    if(0 /* Load data form flash or OTP */)
    {
        // Reserved for future function: Read cal setting from flash or OTP
    }else{
        u32Res = Hal_Flash_AddrRead(SPI_IDX_0, AUXADC_FLASH_START_ADDR, 0, sizeof(sAuxadcCalTable.u32Header), (uint8_t *)&u32Header);
        
        if(u32Res != 0)
        {
            // Error on flash, calibrate by internal source
            Hal_Aux_AdcCal_LoadDef();
        }
        else
        {
            if( u32Header != 0xFFFFFFFF)
            {
                // Any data in flash
                Hal_Aux_AdcCal_LoadTable();
            }
            else
            {
                // Empty data
                Hal_Aux_AdcCal_LoadDef();
            }
        }
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
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcGpioInCal(uint8_t u8GpioIdx, uint16_t u16MiniVolt)
{
    uint32_t u32Temp = 0;
    uint32_t u32Res = 0;

    if (u8GpioIdx >= HAL_AUX_GPIO_NUM_MAX)
        return HAL_AUX_FAIL;

    u32Res = Hal_Aux_SourceSelect( HAL_AUX_SRC_GPIO, u8GpioIdx );
    if(u32Res == HAL_AUX_FAIL)
        return HAL_AUX_FAIL;
    u32Temp = 0;
    u32Res = Hal_Aux_AdcValueGet(&u32Temp);
    if(u32Res == HAL_AUX_FAIL)
        return HAL_AUX_FAIL;

    if( u16MiniVolt <= sAuxadcCalTable.stGpioSrc[ u8GpioIdx ][ 0 ].u16MiniVolt)
    {
        sAuxadcCalTable.stGpioSrc[ u8GpioIdx ][ 0 ].u16MiniVolt = u16MiniVolt;
        sAuxadcCalTable.stGpioSrc[ u8GpioIdx ][ 0 ].u16RawData = u32Temp;
    }else{
        sAuxadcCalTable.stGpioSrc[ u8GpioIdx ][ 1 ].u16MiniVolt = u16MiniVolt;
        sAuxadcCalTable.stGpioSrc[ u8GpioIdx ][ 1 ].u16RawData = u32Temp;
    } 
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
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcVbatInCal(uint16_t u16MiniVolt)
{
    uint32_t u32Temp = 0;
    uint32_t u32Res = 0;

    u32Res = Hal_Aux_SourceSelect( HAL_AUX_SRC_VBAT, 0 );
    if(u32Res == HAL_AUX_FAIL)
        return HAL_AUX_FAIL;
    u32Temp = 0;
    u32Res = Hal_Aux_AdcValueGet(&u32Temp);
    if(u32Res == HAL_AUX_FAIL)
        return HAL_AUX_FAIL;

    sAuxadcCalTable.stIntSrc[ 1 ].u16MiniVolt = u16MiniVolt;
    sAuxadcCalTable.stIntSrc[ 1 ].u16RawData = u32Temp;

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcMiniVolt_Convert
*
* DESCRIPTION:
*   Convert given AUXADC raw-data to mVolt
*
* PARAMETERS
*   1. tSrc      : [In] the source type of AUXADC
*   2. ubGpioIdx : [In] the index of GPIO
*   3. u32RawData: [In]
*
* RETURNS
*   1. uint16_t : mVolt
*
*************************************************************************/
uint16_t Hal_Aux_AdcMiniVolt_Convert( E_HalAux_Src_Patch_t tSrc, uint8_t ubGpioIdx, uint32_t u32RawData)
{
    uint16_t u16MiniVot[2];
    uint16_t u16RawData[2];

    // check the source type
    if (tSrc >= HAL_AUX_SRC_MAX_PATCH)
        return 0xFFFF;

    if( tSrc == HAL_AUX_SRC_GPIO )
    {
        u16MiniVot[ 0 ] = sAuxadcCalTable.stGpioSrc[ ubGpioIdx ][ 0 ].u16MiniVolt;
        u16RawData[ 0 ] = sAuxadcCalTable.stGpioSrc[ ubGpioIdx ][ 0 ].u16RawData;
        u16MiniVot[ 1 ] = sAuxadcCalTable.stGpioSrc[ ubGpioIdx ][ 1 ].u16MiniVolt;
        u16RawData[ 1 ] = sAuxadcCalTable.stGpioSrc[ ubGpioIdx ][ 1 ].u16RawData;
    }else{
        u16MiniVot[ 0 ] = sAuxadcCalTable.stIntSrc[ 0 ].u16MiniVolt;
        u16RawData[ 0 ] = sAuxadcCalTable.stIntSrc[ 0 ].u16RawData;
        u16MiniVot[ 1 ] = sAuxadcCalTable.stIntSrc[ 1 ].u16MiniVolt;
        u16RawData[ 1 ] = sAuxadcCalTable.stIntSrc[ 1 ].u16RawData;
    }

    // Under flow case
    if(u32RawData <= u16RawData[ 0 ])
        return u16MiniVot[ 0 ];

    // Compute the slope and offeset
    uint32_t u32Slope_x1000; // RawData/mv
    uint32_t u32Offset;

    u32Slope_x1000 = ROUND_DIV( (u16RawData[ 1 ] - u16RawData[ 0 ])*1000, (u16MiniVot[ 1 ] - u16MiniVot[ 0 ]) ); 
    u32Offset = u16RawData[ 0 ] - ROUND_DIV( u16MiniVot[ 0 ]*u32Slope_x1000, 1000);

    return ROUND_DIV( (u32RawData - u32Offset)*1000, u32Slope_x1000);
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
    uint32_t ulAdcValue;
    uint8_t ubRet = HAL_AUX_FAIL;

    // check init
    if (g_ubHalAux_Init != 1)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalAux_SemaphoreId, osWaitForever);

    if (HAL_AUX_OK != Hal_Aux_SourceSelect(HAL_AUX_SRC_VBAT, 0))
        goto done;

    if (HAL_AUX_OK != Hal_Aux_AdcValueGet(&ulAdcValue))
        goto done;

    *pfVbat = (float)Hal_Aux_AdcMiniVolt_Convert( (E_HalAux_Src_Patch_t)HAL_AUX_SRC_VBAT, 0, ulAdcValue)/(float)1000;

    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
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
    uint32_t ulAdcValue;
    uint8_t ubRet = HAL_AUX_FAIL;

    // check init
    if (g_ubHalAux_Init != 1)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalAux_SemaphoreId, osWaitForever);

    // check IO number
    if (ubGpioIdx >= HAL_AUX_GPIO_NUM_MAX)
        goto done;

    if (HAL_AUX_OK != Hal_Aux_SourceSelect(HAL_AUX_SRC_GPIO, ubGpioIdx))
        goto done;

    if (HAL_AUX_OK != Hal_Aux_AdcValueGet(&ulAdcValue))
        goto done;

    *pfVoltage = (float)Hal_Aux_AdcMiniVolt_Convert( (E_HalAux_Src_Patch_t)HAL_AUX_SRC_GPIO, ubGpioIdx, ulAdcValue)/(float)1000;

    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_VbatCalibration
*
* DESCRIPTION:
*   do the calibration of VBAT
*
* PARAMETERS
*   1. fVbat : [In] the voltage of VBAT
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_VbatCalibration_patch(float fVbat)
{
    uint8_t ubRet = HAL_AUX_FAIL;

    // check init
    if (g_ubHalAux_Init != 1)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalAux_SemaphoreId, osWaitForever);

    if (HAL_AUX_OK != Hal_Aux_AdcVbatInCal( (uint16_t)(fVbat*1000)) )
        goto done;

    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_IoVoltageCalibration
*
* DESCRIPTION:
*   do the calibration of the IO voltage
*
* PARAMETERS
*   1. ubGpioIdx : [In] the index of GPIO
*   2. fVoltage  : [In] the IO voltage value
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_IoVoltageCalibration_patch(uint8_t ubGpioIdx, float fVoltage)
{
    uint8_t ubRet = HAL_AUX_FAIL;

    // check init
    if (g_ubHalAux_Init != 1)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalAux_SemaphoreId, osWaitForever);

    // check IO number
    if (ubGpioIdx >= HAL_AUX_GPIO_NUM_MAX)
        goto done;

    if (HAL_AUX_OK != Hal_Aux_AdcGpioInCal( ubGpioIdx, (uint16_t)(fVoltage*1000)))
        goto done;

    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
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
    g_ubHalAux_Cal = 0;
    g_ubHalAux_Pu_WriteDirect = 0;

    // Here set via AOS document
    u16LdoRf_MiniVol[0] = 1000; // 1 V
    u16LdoRf_MiniVol[1] = 1100; // 1.1 V
    u16LdoRf_MiniVol[2] = 1200; // 1.2 V
    u16LdoRf_MiniVol[3] = 1300; // 1.3V

    //Hal_Aux_Init = Hal_Aux_Init_impl;
    Hal_Aux_SourceSelect = Hal_Aux_SourceSelect_patch;
    Hal_Aux_AdcValueGet = Hal_Aux_AdcValueGet_patch;
    Hal_Aux_VbatGet = Hal_Aux_VbatGet_patch;
    Hal_Aux_IoVoltageGet = Hal_Aux_IoVoltageGet_patch;
    Hal_Aux_VbatCalibration = Hal_Aux_VbatCalibration_patch;
    Hal_Aux_IoVoltageCalibration = Hal_Aux_IoVoltageCalibration_patch;
}

