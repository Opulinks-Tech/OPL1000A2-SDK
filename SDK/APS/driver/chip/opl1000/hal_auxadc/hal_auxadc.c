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
*  hal_auxadc.c
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
#include "hal_auxadc.h"
#include "hal_auxadc_internal.h"
#include "cmsis_os.h"
#include "boot_sequence.h"
#include "mw_fim_default_group03.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define AOS                     ((S_Aos_Reg_t *) AOS_BASE)
#define RF                      ((S_Rf_Reg_t *) RF_BASE)

#define RF_RG_EOCB              (1 << 16)   // the bit[16] of RG_AUX_ADC_ECL_OUT: End of conversion sign

#define HAL_AUX_ADC_ERROR_COUNT (3)


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
uint8_t g_ubHalAux_Init;
RET_DATA E_HalAux_Src_t g_tHalAux_CurrentType;
RET_DATA uint8_t g_ubHalAux_CurrentGpioIdx;
RET_DATA T_HalAuxCalData g_tHalAux_CalData;
RET_DATA osSemaphoreId g_taHalAux_SemaphoreId;
RET_DATA uint32_t g_ulHalAux_AverageCount;

RET_DATA T_Hal_Aux_Init_Fp Hal_Aux_Init;
RET_DATA T_Hal_Aux_SourceSelect_Fp Hal_Aux_SourceSelect;
RET_DATA T_Hal_Aux_AdcValueGet_Fp Hal_Aux_AdcValueGet;
RET_DATA T_Hal_Aux_VbatGet_Fp Hal_Aux_VbatGet;
RET_DATA T_Hal_Aux_IoVoltageGet_Fp Hal_Aux_IoVoltageGet;
RET_DATA T_Hal_Aux_VbatCalibration_Fp Hal_Aux_VbatCalibration;
RET_DATA T_Hal_Aux_IoVoltageCalibration_Fp Hal_Aux_IoVoltageCalibration;

// Sec 5: declaration of global function prototype
extern void Hal_Aux_CmdParse_impl(char *sCmd);
extern void Hal_Aux_Pinmux_impl(uint8_t ubIdx);


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions

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
void Hal_Aux_Init_impl(void)
{
    osSemaphoreDef_t tSemaphoreDef;
    volatile uint32_t tmp;

    // before init
    g_ubHalAux_Init = 0;

    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP03_CAL_AUXADC, 0, MW_FIM_CAL_AUXADC_SIZE, (uint8_t*)&g_tHalAux_CalData))
        {
            // if fail, get the default value
            memcpy(&g_tHalAux_CalData, &g_tMwFimDefaultCalAuxadc, MW_FIM_CAL_AUXADC_SIZE);
        }
    }

    // AUXADC and HPBG bias
    tmp = AOS->ADC_CTL;
    tmp &= ~((0x1F << 7) | (0x1F << 2));
    tmp |= ((0x10 << 7) | (0x10 << 2));
    AOS->ADC_CTL = tmp;

    // Disable AUXADC
    tmp = AOS->ADC_CTL;
    tmp &= ~(0x1 << 0);
    tmp |= (0x0 << 0);
    AOS->ADC_CTL = tmp;
    
    // Disable the internal temperature sensor
    tmp = AOS->HPBG_CTL;
    tmp &= ~(0x1 << 18);
    tmp |= (0x0 << 18);
    AOS->HPBG_CTL = tmp;

    // Select output from PMU side
    tmp = AOS->PMS_SPARE;
    tmp &= ~((0x1 << 7) | (0x1 << 5) | (0x7 << 1) | (0x1 << 0));
    tmp |= ((0x1 << 7) | (0x1 << 5) | (0x0 << 1) | (0x0 << 0));
    AOS->PMS_SPARE = tmp;

    // Turn off PU of AUXADC
    tmp = RF->PU_VAL;
    tmp &= ~(0x1 << 26);
    tmp |= (0x0 << 26);
    RF->PU_VAL = tmp;

    // Turn off clock to AUXADC
    tmp = RF->RG_CK_GATE_CTRL;
    tmp &= ~(0x1 << 6);
    tmp |= (0x0 << 6);
    RF->RG_CK_GATE_CTRL = tmp;

    // Select input to AUXADC
    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        g_tHalAux_CurrentType = HAL_AUX_SRC_GPIO;
        g_ubHalAux_CurrentGpioIdx = 0;
    }
    Hal_Aux_SourceSelect(g_tHalAux_CurrentType, g_ubHalAux_CurrentGpioIdx);

    // Adjust AUXADC performance
    tmp = RF->AUXADC_CTRL0;
    tmp &= ~((0x3 << 4) | (0xF << 0));
    tmp |= ((0x1 << 4) | (0x9 << 0));
    RF->AUXADC_CTRL0 = tmp;

    // Idle (non-trigger)
    tmp = RF->AUX_ADC_CK_GEN_CTL;
    tmp &= ~(0x1 << 0);
    tmp |= (0x0 << 0);
    RF->AUX_ADC_CK_GEN_CTL = tmp;

    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        // create the semaphore
        tSemaphoreDef.dummy = 0;                            // reserved, it is no used
        g_taHalAux_SemaphoreId = osSemaphoreCreate(&tSemaphoreDef, 1);
        if (g_taHalAux_SemaphoreId == NULL)
        {
            printf("To create the semaphore for Hal_Aux is fail.\n");
            return;
        }
    }

    // after init
    g_ubHalAux_Init = 1;
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
uint8_t Hal_Aux_SourceSelect_impl(E_HalAux_Src_t tSrc, uint8_t ubGpioIdx)
{
    volatile uint32_t tmp;
    uint8_t ubRet = HAL_AUX_FAIL;

    // check the source type
    if (tSrc >= HAL_AUX_SRC_MAX)
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
uint8_t Hal_Aux_AdcValueGet_impl(uint32_t *pulValue)
{
    volatile uint32_t tmp;
    volatile uint32_t i;
    uint8_t ubRet = HAL_AUX_FAIL;
    
    uint32_t ulAdcValue;
    uint32_t ulAdcCurrent;
    uint32_t ulRepeatCount;
    uint32_t j;
    
    if (g_ulHalAux_AverageCount == 0)
        g_ulHalAux_AverageCount = 1;
    
    ulAdcValue = 0;
    ulRepeatCount = g_ulHalAux_AverageCount;
    for (j=0; j<ulRepeatCount; j++)
    {
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
        }
        
        // Turn on PU of AUXADC
        tmp = RF->PU_VAL;
        tmp &= ~(0x1 << 26);
        tmp |= (0x1 << 26);
        RF->PU_VAL = tmp;

        // Turn on clock to AUXADC
        tmp = RF->RG_CK_GATE_CTRL;
        tmp &= ~(0x1 << 6);
        tmp |= (0x1 << 6);
        RF->RG_CK_GATE_CTRL = tmp;

        // Trigger
        tmp = RF->AUX_ADC_CK_GEN_CTL;
        tmp &= ~(0x1 << 0);
        tmp |= (0x1 << 0);
        RF->AUX_ADC_CK_GEN_CTL = tmp;

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

        // !!! ADC value should be not zero, the DC offset is not zero
        if (ulAdcCurrent == 0)
        {
            // error handle if always zero
            if (ulRepeatCount < (g_ulHalAux_AverageCount * HAL_AUX_ADC_ERROR_COUNT))
                ulRepeatCount++;
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
        }

        // Turn off PU of AUXADC
        tmp = RF->PU_VAL;
        tmp &= ~(0x1 << 26);
        tmp |= (0x0 << 26);
        RF->PU_VAL = tmp;

        // Turn off clock to AUXADC
        tmp = RF->RG_CK_GATE_CTRL;
        tmp &= ~(0x1 << 6);
        tmp |= (0x0 << 6);
        RF->RG_CK_GATE_CTRL = tmp;
        
        // Idle (non-trigger)
        tmp = RF->AUX_ADC_CK_GEN_CTL;
        tmp &= ~(0x1 << 0);
        tmp |= (0x0 << 0);
        RF->AUX_ADC_CK_GEN_CTL = tmp;
    }

    *pulValue = (ulAdcValue + (g_ulHalAux_AverageCount / 2)) / g_ulHalAux_AverageCount;
    ubRet = HAL_AUX_OK;

done:
    return ubRet;
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
uint8_t Hal_Aux_VbatGet_impl(float *pfVbat)
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

    // check the base(DC offset)
    if (ulAdcValue <= g_tHalAux_CalData.wDcOffsetVbat)
    {
        *pfVbat = HAL_AUX_BASE_VBAT;
    }
    // others
    else
    {
        *pfVbat = HAL_AUX_BASE_VBAT + (ulAdcValue - g_tHalAux_CalData.wDcOffsetVbat) * g_tHalAux_CalData.fSlopeVbat;
    }

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
uint8_t Hal_Aux_IoVoltageGet_impl(uint8_t ubGpioIdx, float *pfVoltage)
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

    // check the base(DC offset)
    if (ulAdcValue <= g_tHalAux_CalData.wDcOffsetIo)
    {
        *pfVoltage = HAL_AUX_BASE_IO_VOL;
    }
    // others
    else
    {
        *pfVoltage = HAL_AUX_BASE_IO_VOL + (ulAdcValue - g_tHalAux_CalData.wDcOffsetIo) * g_tHalAux_CalData.fSlopeIo;
    }

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
uint8_t Hal_Aux_VbatCalibration_impl(float fVbat)
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

    // Vbat = base voltage + (ADC value - base ADC value) * slope
    // base ADC value = ADC value - (Vbat - base voltage) / slope
    g_tHalAux_CalData.wDcOffsetVbat = ulAdcValue - (fVbat - HAL_AUX_BASE_VBAT) / g_tHalAux_CalData.fSlopeVbat;

    if (MW_FIM_OK != MwFim_FileWrite(MW_FIM_IDX_GP03_CAL_AUXADC, 0, MW_FIM_CAL_AUXADC_SIZE, (uint8_t*)&g_tHalAux_CalData))
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
uint8_t Hal_Aux_IoVoltageCalibration_impl(uint8_t ubGpioIdx, float fVoltage)
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

    // IoVoltage = base voltage + (ADC value - base ADC value) * slope
    // base ADC value = ADC value - (IoVoltage - base voltage) / slope
    g_tHalAux_CalData.wDcOffsetIo = ulAdcValue - (fVoltage - HAL_AUX_BASE_IO_VOL) / g_tHalAux_CalData.fSlopeIo;

    if (MW_FIM_OK != MwFim_FileWrite(MW_FIM_IDX_GP03_CAL_AUXADC, 0, MW_FIM_CAL_AUXADC_SIZE, (uint8_t*)&g_tHalAux_CalData))
        goto done;
    
    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_PreInitCold
*
* DESCRIPTION:
*   the previous initial for cold boot
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Aux_PreInitCold(void)
{
    g_tHalAux_CurrentType = HAL_AUX_SRC_GPIO;
    g_ubHalAux_CurrentGpioIdx = 0;
    memcpy(&g_tHalAux_CalData, &g_tMwFimDefaultCalAuxadc, MW_FIM_CAL_AUXADC_SIZE);
    g_taHalAux_SemaphoreId = NULL;
    g_ulHalAux_AverageCount = HAL_AUX_AVERAGE_COUNT;

    Hal_Aux_Init = Hal_Aux_Init_impl;
    Hal_Aux_SourceSelect = Hal_Aux_SourceSelect_impl;
    Hal_Aux_AdcValueGet = Hal_Aux_AdcValueGet_impl;
    Hal_Aux_VbatGet = Hal_Aux_VbatGet_impl;
    Hal_Aux_IoVoltageGet = Hal_Aux_IoVoltageGet_impl;
    Hal_Aux_VbatCalibration = Hal_Aux_VbatCalibration_impl;
    Hal_Aux_IoVoltageCalibration = Hal_Aux_IoVoltageCalibration_impl;

    Hal_Aux_CmdParse = Hal_Aux_CmdParse_impl;
    Hal_Aux_Pinmux = Hal_Aux_Pinmux_impl;
}
