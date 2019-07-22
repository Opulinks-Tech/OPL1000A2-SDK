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
#include <stdlib.h>
#include <string.h>
#include "hal_auxadc.h"
#include "hal_auxadc_internal.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define AOS                     ((S_Aos_Reg_t *) AOS_BASE)


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


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
RET_DATA T_Hal_Aux_CmdParse_Fp Hal_Aux_CmdParse;
RET_DATA T_Hal_Aux_Pinmux_Fp Hal_Aux_Pinmux;

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

/*************************************************************************
* FUNCTION:
*   Hal_Aux_CmdParse
*
* DESCRIPTION:
*   Parse the AUXADC cmd
*
* PARAMETERS
*   1. sCmd    : [In] the input string
*
* RETURNS
*
*************************************************************************/
void Hal_Aux_CmdParse_impl(char *sCmd)
{
    float fValue;
    uint32_t ulValue;
    uint8_t ubType;
    uint8_t ubIdx;

    uint8_t ucaSeps[] = " \t\r\n";         // space, tab, new line
    uint8_t *pucaToken[8] = {0};
    uint8_t i;
    char *pSavedPtr = NULL;

    printf("\n");

    // get the first token
    pucaToken[0] = (uint8_t*)strtok_r(sCmd, (const char*)ucaSeps, &pSavedPtr);

    // get the other token
    if (NULL != pucaToken[0])
    {
        for (i=1; i<8; i++)
        {
            pucaToken[i] = (uint8_t*)strtok_r(NULL, (const char*)ucaSeps, &pSavedPtr);
            if (NULL == pucaToken[i])
                break;
        }
    }

    // select the source type
    if ((strcmp((const char*)pucaToken[1], "0") == 0) && ((i == 3) || (i == 4)))
    {
        // the type
        ubType = strtoul((const char*)pucaToken[2], NULL, 0);
        
        // the index
        if (i == 4)
            ubIdx = strtoul((const char*)pucaToken[3], NULL, 0);

        // select source
        if ((HAL_AUX_OK == Hal_Aux_SourceSelect((E_HalAux_Src_t)ubType, ubIdx)) && (i == 4))
        {
            Hal_Aux_Pinmux(ubIdx);
        }
    }
    // get the ADC value
    else if ((strcmp((const char*)pucaToken[1], "1") == 0) && (i == 2))
    {
        if (HAL_AUX_OK == Hal_Aux_AdcValueGet(&ulValue))
        {
            printf("AUXADC [%u]\n", ulValue);
        }
    }
    // get the VBAT voltage
    else if ((strcmp((const char*)pucaToken[1], "2") == 0) && (i == 2))
    {
        if (HAL_AUX_OK == Hal_Aux_VbatGet(&fValue))
        {
            ulValue = (uint32_t)(fValue * 1000);
            printf("VBAT [%u.%03u]\n", ulValue / 1000, ulValue % 1000);
        }
    }
    // get the IO voltage
    else if ((strcmp((const char*)pucaToken[1], "3") == 0) && (i == 3))
    {
        ubIdx = strtoul((const char*)pucaToken[2], NULL, 0);
    
        if (HAL_AUX_OK == Hal_Aux_IoVoltageGet(ubIdx, &fValue))
        {
            ulValue = (uint32_t)(fValue * 1000);
            printf("IO %u [%u.%03u]\n", ubIdx, ulValue / 1000, ulValue % 1000);
        }
    }
    else
    {
        printf("It is an invalid command.\n");
    }
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_Pinmux
*
* DESCRIPTION:
*   config the IO pad to AUXADC
*
* PARAMETERS
*   1. ubIdx    : [In] the number of IO pad
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Aux_Pinmux_impl(uint8_t ubIdx)
{
    volatile uint32_t tmp;

// AUXADC
    // IO2(AUXADC), IO3(AUXADC)
    // output source
    // input IO
    
    // input
    tmp = AOS->RG_PD_DIR;
    tmp |= (0x1 << ubIdx);
    AOS->RG_PD_DIR = tmp;

    // input disable
    tmp = AOS->RG_PD_IE;
    tmp &= ~(0x1 << ubIdx);
    AOS->RG_PD_IE = tmp;

    // pull-up / pull-down
    tmp = AOS->RG_PD_PE;
    tmp &= ~(0x1 << ubIdx);
    AOS->RG_PD_PE = tmp;

    // GPIO pin
    tmp = AOS->RG_PDOC_MODE;
    tmp &= ~(0x1 << ubIdx);
    AOS->RG_PDOC_MODE = tmp;

    tmp = AOS->RG_PDOV_MODE;
    tmp &= ~(0x1 << ubIdx);
    AOS->RG_PDOV_MODE = tmp;
}
