/******************************************************************************
*  Copyright 2017, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2017
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  hal_pin.c
*
*  Project:
*  --------
*  NL1000_A1 series
*
*  Description:
*  ------------
*  This include file defines the patch proto-types of system functions
*
*  Author:
*  -------
*  Jeff
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "hal_pin_def.h"
#include "hal_pin.h"
#include "hal_vic.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define AOS             ((S_Aos_Reg_t *) AOS_BASE)

typedef uint8_t (*T_Hal_Pin_IoConfigSet_Fp)(uint16_t uwConfig);

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
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
// Sec 4: declaration of global  variable
// External
RET_DATA T_Hal_Pin_ConfigSet_Fp Hal_Pin_ConfigSet;
RET_DATA T_Hal_Pin_IoConfigSet_Fp g_taHal_Pin_IoConfigTbl[HAL_PIN_IO_COUNT];

// Internal

// Sec 5: declaration of global function prototype
uint8_t Hal_Pin_ConfigSet_IO_0(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_1(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_2(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_3(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_4(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_5(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_6(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_7(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_8(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_9(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_10(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_11(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_12(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_13(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_14(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_15(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_16(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_17(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_18(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_19(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_20(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_21(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_22(uint16_t uwConfig);
uint8_t Hal_Pin_ConfigSet_IO_23(uint16_t uwConfig);

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
*  Hal_Pin_ConfigSet
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. ubIoIdx   : [In] the index of IO
*   2. uwConfig  : [In] the type config of IO
*   3. ubDriving : [In] the driving of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_impl(uint8_t ubIoIdx, uint16_t uwConfig, uint8_t ubDriving)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;
    uint8_t bRet = HAL_PIN_FAIL;

    // error handle
    if (ubIoIdx >= HAL_PIN_IO_COUNT)
        goto done;

    // check the function pointer
    if (NULL == g_taHal_Pin_IoConfigTbl[ubIoIdx])
        goto done;

    // set the IO config
    if (HAL_PIN_OK != g_taHal_Pin_IoConfigTbl[ubIoIdx](uwConfig))
        goto done;

    // pull-up / pull-down
    tmp = AOS->RG_PD_PE;
    tmp &= ~(0x1 << ubIoIdx);
    if (PIN_DRIVING_FLOAT != ubDriving)
        tmp |= (0x1 << ubIoIdx);
    AOS->RG_PD_PE = tmp;

    // output level + GPIO direction
    switch (uwConfig)
    {
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            Hal_Vic_GpioOutput((E_GpioIdx_t)ubIoIdx, GPIO_LEVEL_HIGH);
            Hal_Vic_GpioDirection((E_GpioIdx_t)ubIoIdx, GPIO_OUTPUT);
            break;

        case PIN_TYPE_GPIO_OUTPUT_LOW:
            Hal_Vic_GpioOutput((E_GpioIdx_t)ubIoIdx, GPIO_LEVEL_LOW);
            Hal_Vic_GpioDirection((E_GpioIdx_t)ubIoIdx, GPIO_OUTPUT);
            break;

        case PIN_TYPE_NONE:
        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_AUX_0:
        case PIN_TYPE_AUX_1:
        case PIN_TYPE_AUX_2:
        case PIN_TYPE_AUX_3:
        case PIN_TYPE_AUX_4:
        case PIN_TYPE_AUX_5:
        case PIN_TYPE_AUX_6:
        case PIN_TYPE_AUX_7:
        case PIN_TYPE_AUX_8:
        case PIN_TYPE_AUX_9:
        case PIN_TYPE_AUX_10:
        case PIN_TYPE_AUX_11:
        case PIN_TYPE_AUX_12:
        case PIN_TYPE_AUX_13:
        case PIN_TYPE_AUX_14:
        case PIN_TYPE_AUX_15:
            Hal_Vic_GpioDirection((E_GpioIdx_t)ubIoIdx, GPIO_INPUT);
            break;

        default:
            break;
    }

    // function pin
    tmp = AOS->RG_PDOC_MODE;
    tmp &= ~(0x1 << ubIoIdx);
    tmp2 = AOS->RG_PDOV_MODE;
    tmp2 &= ~(0x1 << ubIoIdx);
    switch (uwConfig)
    {
        case PIN_TYPE_NONE:
        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
        case PIN_TYPE_AUX_0:
        case PIN_TYPE_AUX_1:
        case PIN_TYPE_AUX_2:
        case PIN_TYPE_AUX_3:
        case PIN_TYPE_AUX_4:
        case PIN_TYPE_AUX_5:
        case PIN_TYPE_AUX_6:
        case PIN_TYPE_AUX_7:
        case PIN_TYPE_AUX_8:
        case PIN_TYPE_AUX_9:
        case PIN_TYPE_AUX_10:
        case PIN_TYPE_AUX_11:
        case PIN_TYPE_AUX_12:
        case PIN_TYPE_AUX_13:
        case PIN_TYPE_AUX_14:
        case PIN_TYPE_AUX_15:
            break;

        default:
            tmp |= (0x1 << ubIoIdx);
            tmp2 |= (0x1 << ubIoIdx);
            break;
    }
    AOS->RG_PDOC_MODE = tmp;
    AOS->RG_PDOV_MODE = tmp2;

    bRet = HAL_PIN_OK;

done:
    return bRet;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_0
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_0(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_A;
    tmp &= ~(0xF << 0);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_TX:
            tmp |= (0x4 << 0);
            break;

        case PIN_TYPE_I2C_SCL:
            tmp |= (0x2 << 0);
            break;

        case PIN_TYPE_SPI2_IO_3:
            tmp |= (0x0 << 0);
            break;

        case PIN_TYPE_UART_APS_TX:
            tmp |= (0xA << 0);
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp |= (0x8 << 0);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 0);
            break;
    }
    AOS->RG_PDI_SRC_IO_A = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 0);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_CTS:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 0);
            tmp |= (0x0 << 0);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SCL:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 24);
            tmp |= (0x0 << 24);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI2_IO_3:
            tmp = AOS->RG_PTS_INMUX_D;
            tmp &= ~(0xF << 12);
            tmp |= (0x0 << 12);
            AOS->RG_PTS_INMUX_D = tmp;
            break;

        case PIN_TYPE_UART_MSQ_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 20);
            tmp |= (0x0 << 20);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 12);
            tmp |= (0x0 << 12);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 24);
            tmp |= (0x0 << 24);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 0);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_1
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_1(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_A;
    tmp &= ~(0xF << 4);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_RTS:
            tmp |= (0x3 << 4);
            break;

        case PIN_TYPE_I2C_SDA:
            tmp |= (0x2 << 4);
            break;

        case PIN_TYPE_SPI2_IO_2:
            tmp |= (0x0 << 4);
            break;

        case PIN_TYPE_UART_MSQ_TX:
            tmp |= (0xB << 4);
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp |= (0x9 << 4);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 4);
            break;
    }
    AOS->RG_PDI_SRC_IO_A = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 1);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 12);
            tmp |= (0x0 << 12);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SDA:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~((uint32_t)0xF << 28);
            tmp |= ((uint32_t)0x0 << 28);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI2_IO_2:
            tmp = AOS->RG_PTS_INMUX_D;
            tmp &= ~(0xF << 8);
            tmp |= (0x0 << 8);
            AOS->RG_PTS_INMUX_D = tmp;
            break;

        case PIN_TYPE_UART_APS_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 16);
            tmp |= (0x0 << 16);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 16);
            tmp |= (0x0 << 16);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 20);
            tmp |= (0x0 << 20);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 1);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_2
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_2(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_A;
    tmp &= ~(0xF << 8);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_TX:
            tmp |= (0x3 << 8);
            break;

        case PIN_TYPE_I2C_SDA:
            tmp |= (0x2 << 8);
            break;

        case PIN_TYPE_SPI2_IO_1:
        case PIN_TYPE_SPI2_IO_MISO:
            tmp |= (0x0 << 8);
            break;

        case PIN_TYPE_UART_APS_TX:
            tmp |= (0xA << 8);
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp |= (0x8 << 8);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 8);
            break;
    }
    AOS->RG_PDI_SRC_IO_A = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 2);
    switch (uwConfig)
    {
        case PIN_TYPE_I2C_SDA:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~((uint32_t)0xF << 28);
            tmp |= ((uint32_t)0x1 << 28);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI2_IO_1:
        case PIN_TYPE_SPI2_IO_MISO:
            tmp = AOS->RG_PTS_INMUX_D;
            tmp &= ~(0xF << 4);
            tmp |= (0x0 << 4);
            AOS->RG_PTS_INMUX_D = tmp;
            break;

        case PIN_TYPE_UART_MSQ_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 20);
            tmp |= (0x1 << 20);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 12);
            tmp |= (0x1 << 12);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 24);
            tmp |= (0x1 << 24);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 2);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_3
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_3(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_A;
    tmp &= ~(0xF << 12);
    switch (uwConfig)
    {
        case PIN_TYPE_I2C_SCL:
            tmp |= (0x2 << 12);
            break;

        case PIN_TYPE_SPI2_IO_0:
        case PIN_TYPE_SPI2_IO_MOSI:
            tmp |= (0x0 << 12);
            break;

        case PIN_TYPE_UART_MSQ_TX:
            tmp |= (0xB << 12);
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp |= (0x9 << 12);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 12);
            break;
    }
    AOS->RG_PDI_SRC_IO_A = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 3);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 4);
            tmp |= (0x0 << 4);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SCL:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 24);
            tmp |= (0x1 << 24);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI2_IO_0:
        case PIN_TYPE_SPI2_IO_MOSI:
            tmp = AOS->RG_PTS_INMUX_D;
            tmp &= ~(0xF << 0);
            tmp |= (0x1 << 0);
            AOS->RG_PTS_INMUX_D = tmp;
            break;

        case PIN_TYPE_UART_APS_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 16);
            tmp |= (0x1 << 16);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 16);
            tmp |= (0x1 << 16);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 20);
            tmp |= (0x1 << 20);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 3);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_4
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_4(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_A;
    tmp &= ~(0xF << 16);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_TX:
            tmp |= (0x3 << 16);
            break;

        case PIN_TYPE_I2C_SCL:
            tmp |= (0x2 << 16);
            break;

        case PIN_TYPE_SPI2_CLK:
            tmp |= (0x0 << 16);
            break;

        case PIN_TYPE_UART_APS_TX:
            tmp |= (0xA << 16);
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp |= (0x8 << 16);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 16);
            break;
    }
    AOS->RG_PDI_SRC_IO_A = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 4);
    switch (uwConfig)
    {
        case PIN_TYPE_I2C_SCL:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 24);
            tmp |= (0x2 << 24);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_UART_MSQ_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 20);
            tmp |= (0x2 << 20);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 12);
            tmp |= (0x2 << 12);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 24);
            tmp |= (0x2 << 24);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 4);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_5
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_5(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_A;
    tmp &= ~(0xF << 20);
    switch (uwConfig)
    {
        case PIN_TYPE_I2C_SDA:
            tmp |= (0x2 << 20);
            break;

        case PIN_TYPE_SPI2_CS:
            tmp |= (0x0 << 20);
            break;

        case PIN_TYPE_UART_MSQ_TX:
            tmp |= (0xB << 20);
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp |= (0x9 << 20);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 20);
            break;
    }
    AOS->RG_PDI_SRC_IO_A = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 5);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 12);
            tmp |= (0x1 << 12);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SDA:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~((uint32_t)0xF << 28);
            tmp |= ((uint32_t)0x2 << 28);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_UART_APS_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 16);
            tmp |= (0x2 << 16);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 16);
            tmp |= (0x2 << 16);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 20);
            tmp |= (0x2 << 20);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 5);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_6
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_6(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_A;
    tmp &= ~(0xF << 24);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_TX:
            tmp |= (0x4 << 24);
            break;

        case PIN_TYPE_UART1_RTS:
            tmp |= (0x3 << 24);
            break;

        case PIN_TYPE_I2C_SCL:
            tmp |= (0x2 << 24);
            break;

        case PIN_TYPE_SPI1_CS:
            tmp |= (0x0 << 24);
            break;

        case PIN_TYPE_UART_APS_TX:
            tmp |= (0xA << 24);
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp |= (0x8 << 24);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 24);
            break;
    }
    AOS->RG_PDI_SRC_IO_A = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 6);
    switch (uwConfig)
    {
        case PIN_TYPE_I2C_SCL:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 24);
            tmp |= (0x3 << 24);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_UART_MSQ_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 20);
            tmp |= (0x3 << 20);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 12);
            tmp |= (0x3 << 12);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 24);
            tmp |= (0x3 << 24);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 6);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_7
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_7(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_A;
    tmp &= ~((uint32_t)0xF << 28);
    switch (uwConfig)
    {
        case PIN_TYPE_I2C_SDA:
            tmp |= ((uint32_t)0x2 << 28);
            break;

        case PIN_TYPE_SPI1_CLK:
            tmp |= ((uint32_t)0x0 << 28);
            break;

        case PIN_TYPE_UART_MSQ_TX:
            tmp |= ((uint32_t)0xB << 28);
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp |= ((uint32_t)0x9 << 28);
            break;

        default:
            // the others are input only
            tmp |= ((uint32_t)0xD << 28);
            break;
    }
    AOS->RG_PDI_SRC_IO_A = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 7);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 4);
            tmp |= (0x1 << 4);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_UART1_CTS:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 8);
            tmp |= (0x0 << 8);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SDA:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~((uint32_t)0xF << 28);
            tmp |= ((uint32_t)0x3 << 28);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_UART_APS_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 16);
            tmp |= (0x3 << 16);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 16);
            tmp |= (0x3 << 16);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 20);
            tmp |= (0x3 << 20);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 7);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_8
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_8(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_B;
    tmp &= ~(0xF << 0);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_TX:
            tmp |= (0x4 << 0);
            break;

        case PIN_TYPE_I2C_SDA:
            tmp |= (0x2 << 0);
            break;

        case PIN_TYPE_SPI1_IO_0:
        case PIN_TYPE_SPI1_IO_MOSI:
            tmp |= (0x0 << 0);
            break;

        case PIN_TYPE_UART_APS_TX:
            tmp |= (0xA << 0);
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp |= (0x8 << 0);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 0);
            break;
    }
    AOS->RG_PDI_SRC_IO_B = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 8);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_CTS:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 0);
            tmp |= (0x1 << 0);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SDA:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~((uint32_t)0xF << 28);
            tmp |= ((uint32_t)0x4 << 28);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI1_IO_0:
        case PIN_TYPE_SPI1_IO_MOSI:
            tmp = AOS->RG_PTS_INMUX_C;
            tmp &= ~(0xF << 16);
            tmp |= (0x0 << 16);
            AOS->RG_PTS_INMUX_C = tmp;
            break;

        case PIN_TYPE_UART_MSQ_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 20);
            tmp |= (0x4 << 20);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 12);
            tmp |= (0x4 << 12);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 24);
            tmp |= (0x4 << 24);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 8);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_9
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_9(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_B;
    tmp &= ~(0xF << 4);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_RTS:
            tmp |= (0x3 << 4);
            break;

        case PIN_TYPE_I2C_SCL:
            tmp |= (0x2 << 4);
            break;

        case PIN_TYPE_SPI1_IO_1:
        case PIN_TYPE_SPI1_IO_MISO:
            tmp |= (0x0 << 4);
            break;

        case PIN_TYPE_UART_MSQ_TX:
            tmp |= (0xB << 4);
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp |= (0x9 << 4);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 4);
            break;
    }
    AOS->RG_PDI_SRC_IO_B = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 9);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 12);
            tmp |= (0x2 << 12);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SCL:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 24);
            tmp |= (0x4 << 24);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI1_IO_1:
        case PIN_TYPE_SPI1_IO_MISO:
            tmp = AOS->RG_PTS_INMUX_C;
            tmp &= ~(0xF << 20);
            tmp |= (0x0 << 20);
            AOS->RG_PTS_INMUX_C = tmp;
            break;

        case PIN_TYPE_UART_APS_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 16);
            tmp |= (0x4 << 16);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 16);
            tmp |= (0x4 << 16);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 20);
            tmp |= (0x4 << 20);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 9);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_10
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_10(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_B;
    tmp &= ~(0xF << 8);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_TX:
            tmp |= (0x3 << 8);
            break;

        case PIN_TYPE_I2C_SCL:
            tmp |= (0x2 << 8);
            break;

        case PIN_TYPE_SPI1_IO_2:
            tmp |= (0x0 << 8);
            break;

        case PIN_TYPE_UART_APS_TX:
            tmp |= (0xA << 8);
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp |= (0x8 << 8);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 8);
            break;
    }
    AOS->RG_PDI_SRC_IO_B = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 10);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 12);
            tmp |= (0x3 << 12);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SCL:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 24);
            tmp |= (0x5 << 24);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI1_IO_2:
            tmp = AOS->RG_PTS_INMUX_C;
            tmp &= ~(0xF << 24);
            tmp |= (0x0 << 24);
            AOS->RG_PTS_INMUX_C = tmp;
            break;

        case PIN_TYPE_UART_MSQ_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 20);
            tmp |= (0x5 << 20);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 12);
            tmp |= (0x5 << 12);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 24);
            tmp |= (0x5 << 24);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 10);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_11
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_11(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_B;
    tmp &= ~(0xF << 12);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_TX:
            tmp |= (0x4 << 12);
            break;

        case PIN_TYPE_I2C_SDA:
            tmp |= (0x2 << 12);
            break;

        case PIN_TYPE_SPI1_IO_3:
            tmp |= (0x0 << 12);
            break;

        case PIN_TYPE_UART_MSQ_TX:
            tmp |= (0xB << 12);
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp |= (0x9 << 12);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 12);
            break;
    }
    AOS->RG_PDI_SRC_IO_B = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 11);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 4);
            tmp |= (0x2 << 4);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SDA:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~((uint32_t)0xF << 28);
            tmp |= ((uint32_t)0x5 << 28);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI1_IO_3:
            tmp = AOS->RG_PTS_INMUX_C;
            tmp &= ~((uint32_t)0xF << 28);
            tmp |= ((uint32_t)0x0 << 28);
            AOS->RG_PTS_INMUX_C = tmp;
            break;

        case PIN_TYPE_UART_APS_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 16);
            tmp |= (0x5 << 16);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 16);
            tmp |= (0x5 << 16);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 20);
            tmp |= (0x5 << 20);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 11);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_12
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_12(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_B;
    tmp &= ~(0xF << 16);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_TX:
            tmp |= (0x4 << 16);
            break;

        case PIN_TYPE_I2C_SCL:
            tmp |= (0x2 << 16);
            break;

        case PIN_TYPE_SPI0_CS:
            tmp |= (0x0 << 16);
            break;

        case PIN_TYPE_UART_APS_TX:
            tmp |= (0xA << 16);
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp |= (0x8 << 16);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 16);
            break;
    }
    AOS->RG_PDI_SRC_IO_B = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 12);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_CTS:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 8);
            tmp |= (0x1 << 8);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SCL:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 24);
            tmp |= (0x6 << 24);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_UART_MSQ_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 20);
            tmp |= (0x6 << 20);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 12);
            tmp |= (0x6 << 12);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 24);
            tmp |= (0x6 << 24);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 12);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_13
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_13(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_B;
    tmp &= ~(0xF << 20);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_RTS:
            tmp |= (0x3 << 20);
            break;

        case PIN_TYPE_I2C_SDA:
            tmp |= (0x2 << 20);
            break;

        case PIN_TYPE_SPI0_CLK:
            tmp |= (0x0 << 20);
            break;

        case PIN_TYPE_UART_MSQ_TX:
            tmp |= (0xB << 20);
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp |= (0x9 << 20);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 20);
            break;
    }
    AOS->RG_PDI_SRC_IO_B = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 13);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 4);
            tmp |= (0x3 << 4);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SDA:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~((uint32_t)0xF << 28);
            tmp |= ((uint32_t)0x6 << 28);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_UART_APS_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 16);
            tmp |= (0x6 << 16);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 16);
            tmp |= (0x6 << 16);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 20);
            tmp |= (0x6 << 20);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 13);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_14
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_14(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_B;
    tmp &= ~(0xF << 24);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_TX:
            tmp |= (0x3 << 24);
            break;

        case PIN_TYPE_I2C_SDA:
            tmp |= (0x2 << 24);
            break;

        case PIN_TYPE_SPI0_IO_0:
        case PIN_TYPE_SPI0_IO_MOSI:
            tmp |= (0x0 << 24);
            break;

        case PIN_TYPE_UART_APS_TX:
            tmp |= (0xA << 24);
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp |= (0x8 << 24);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 24);
            break;
    }
    AOS->RG_PDI_SRC_IO_B = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 14);
    switch (uwConfig)
    {
        case PIN_TYPE_I2C_SDA:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~((uint32_t)0xF << 28);
            tmp |= ((uint32_t)0x7 << 28);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI0_IO_0:
        case PIN_TYPE_SPI0_IO_MOSI:
            tmp = AOS->RG_PTS_INMUX_C;
            tmp &= ~(0xF << 0);
            tmp |= (0x0 << 0);
            AOS->RG_PTS_INMUX_C = tmp;
            break;

        case PIN_TYPE_UART_MSQ_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 20);
            tmp |= (0x7 << 20);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 12);
            tmp |= (0x7 << 12);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 24);
            tmp |= (0x7 << 24);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 14);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_15
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_15(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_B;
    tmp &= ~((uint32_t)0xF << 28);
    switch (uwConfig)
    {
        case PIN_TYPE_I2C_SCL:
            tmp |= ((uint32_t)0x2 << 28);
            break;

        case PIN_TYPE_SPI0_IO_1:
        case PIN_TYPE_SPI0_IO_MISO:
            tmp |= ((uint32_t)0x0 << 28);
            break;

        case PIN_TYPE_UART_MSQ_TX:
            tmp |= ((uint32_t)0xB << 28);
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp |= ((uint32_t)0x9 << 28);
            break;

        default:
            // the others are input only
            tmp |= ((uint32_t)0xD << 28);
            break;
    }
    AOS->RG_PDI_SRC_IO_B = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 15);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 12);
            tmp |= (0x4 << 12);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SCL:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 24);
            tmp |= (0x7 << 24);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI0_IO_1:
        case PIN_TYPE_SPI0_IO_MISO:
            tmp = AOS->RG_PTS_INMUX_C;
            tmp &= ~(0xF << 4);
            tmp |= (0x0 << 4);
            AOS->RG_PTS_INMUX_C = tmp;
            break;

        case PIN_TYPE_UART_APS_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 16);
            tmp |= (0x7 << 16);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 16);
            tmp |= (0x7 << 16);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 20);
            tmp |= (0x7 << 20);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 15);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_16
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_16(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_C;
    tmp &= ~(0xF << 0);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_TX:
            tmp |= (0x3 << 0);
            break;

        case PIN_TYPE_I2C_SCL:
            tmp |= (0x2 << 0);
            break;

        case PIN_TYPE_SPI0_IO_2:
            tmp |= (0x0 << 0);
            break;

        case PIN_TYPE_UART_APS_TX:
            tmp |= (0xA << 0);
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp |= (0x8 << 0);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 0);
            break;
    }
    AOS->RG_PDI_SRC_IO_C = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 16);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 12);
            tmp |= (0x5 << 12);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SCL:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 24);
            tmp |= (0x8 << 24);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI0_IO_2:
            tmp = AOS->RG_PTS_INMUX_C;
            tmp &= ~(0xF << 8);
            tmp |= (0x0 << 8);
            AOS->RG_PTS_INMUX_C = tmp;
            break;

        case PIN_TYPE_UART_MSQ_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 20);
            tmp |= (0x8 << 20);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 12);
            tmp |= (0x8 << 12);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 24);
            tmp |= (0x8 << 24);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 16);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_17
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_17(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_C;
    tmp &= ~(0xF << 4);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_TX:
            tmp |= (0x4 << 4);
            break;

        case PIN_TYPE_I2C_SDA:
            tmp |= (0x2 << 4);
            break;

        case PIN_TYPE_SPI0_IO_3:
            tmp |= (0x0 << 4);
            break;

        case PIN_TYPE_UART_MSQ_TX:
            tmp |= (0xB << 4);
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp |= (0x9 << 4);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 4);
            break;
    }
    AOS->RG_PDI_SRC_IO_C = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 17);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 4);
            tmp |= (0x4 << 4);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SDA:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~((uint32_t)0xF << 28);
            tmp |= ((uint32_t)0x8 << 28);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI0_IO_3:
            tmp = AOS->RG_PTS_INMUX_C;
            tmp &= ~(0xF << 12);
            tmp |= (0x0 << 12);
            AOS->RG_PTS_INMUX_C = tmp;
            break;

        case PIN_TYPE_UART_APS_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 16);
            tmp |= (0x8 << 16);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 16);
            tmp |= (0x8 << 16);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 20);
            tmp |= (0x8 << 20);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 17);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_18
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_18(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_C;
    tmp &= ~(0xF << 8);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_TX:
            tmp |= (0x0 << 8);
            break;

        case PIN_TYPE_I2C_SDA:
            tmp |= (0x2 << 8);
            break;

        case PIN_TYPE_SPI2_CS:
            tmp |= (0x5 << 8);
            break;

        case PIN_TYPE_PWM_5:
            tmp |= (0x1 << 8);
            break;

        case PIN_TYPE_UART_APS_TX:
            tmp |= (0xA << 8);
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp |= (0x8 << 8);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 8);
            break;
    }
    AOS->RG_PDI_SRC_IO_C = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 18);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_CTS:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 0);
            tmp |= (0x2 << 0);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SDA:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~((uint32_t)0xF << 28);
            tmp |= ((uint32_t)0x9 << 28);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_UART_MSQ_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 20);
            tmp |= (0x9 << 20);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 12);
            tmp |= (0x9 << 12);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 24);
            tmp |= (0x9 << 24);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 18);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_19
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_19(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_C;
    tmp &= ~(0xF << 12);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_RTS:
            tmp |= (0x3 << 12);
            break;

        case PIN_TYPE_I2C_SCL:
            tmp |= (0x2 << 12);
            break;

        case PIN_TYPE_SPI2_CLK:
            tmp |= (0x5 << 12);
            break;

        case PIN_TYPE_PWM_4:
            tmp |= (0x1 << 12);
            break;

        case PIN_TYPE_UART_MSQ_TX:
            tmp |= (0xB << 12);
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp |= (0x9 << 12);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 12);
            break;
    }
    AOS->RG_PDI_SRC_IO_C = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 19);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 12);
            tmp |= (0x6 << 12);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SCL:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 24);
            tmp |= (0x9 << 24);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_UART_APS_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 16);
            tmp |= (0x9 << 16);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 16);
            tmp |= (0x9 << 16);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 20);
            tmp |= (0x9 << 20);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 19);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_20
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_20(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_C;
    tmp &= ~(0xF << 16);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_TX:
            tmp |= (0x0 << 16);
            break;

        case PIN_TYPE_I2C_SCL:
            tmp |= (0x2 << 16);
            break;

        case PIN_TYPE_SPI2_IO_0:
        case PIN_TYPE_SPI2_IO_MOSI:
            tmp |= (0x5 << 16);
            break;

        case PIN_TYPE_PWM_3:
            tmp |= (0x1 << 16);
            break;

        case PIN_TYPE_UART_APS_TX:
            tmp |= (0xA << 16);
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp |= (0x8 << 16);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 16);
            break;
    }
    AOS->RG_PDI_SRC_IO_C = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 20);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_CTS:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 8);
            tmp |= (0x2 << 8);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_UART1_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 12);
            tmp |= (0x7 << 12);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SCL:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 24);
            tmp |= (0xA << 24);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI2_IO_0:
        case PIN_TYPE_SPI2_IO_MOSI:
            tmp = AOS->RG_PTS_INMUX_D;
            tmp &= ~(0xF << 0);
            tmp |= (0x4 << 0);
            AOS->RG_PTS_INMUX_D = tmp;
            break;

        case PIN_TYPE_UART_MSQ_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 20);
            tmp |= (0xA << 20);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 12);
            tmp |= (0xA << 12);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 24);
            tmp |= (0xA << 24);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 20);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_21
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_21(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_C;
    tmp &= ~(0xF << 20);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_RTS:
            tmp |= (0x3 << 20);
            break;

        case PIN_TYPE_UART1_TX:
            tmp |= (0x4 << 20);
            break;

        case PIN_TYPE_I2C_SDA:
            tmp |= (0x2 << 20);
            break;

        case PIN_TYPE_SPI2_IO_1:
        case PIN_TYPE_SPI2_IO_MISO:
            tmp |= (0x5 << 20);
            break;

        case PIN_TYPE_PWM_2:
            tmp |= (0x1 << 20);
            break;

        case PIN_TYPE_UART_MSQ_TX:
            tmp |= (0xB << 20);
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp |= (0x9 << 20);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 20);
            break;
    }
    AOS->RG_PDI_SRC_IO_C = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 21);
    switch (uwConfig)
    {
        case PIN_TYPE_UART0_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 4);
            tmp |= (0x5 << 4);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SDA:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~((uint32_t)0xF << 28);
            tmp |= ((uint32_t)0xA << 28);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI2_IO_1:
        case PIN_TYPE_SPI2_IO_MISO:
            tmp = AOS->RG_PTS_INMUX_D;
            tmp &= ~(0xF << 4);
            tmp |= (0x2 << 4);
            AOS->RG_PTS_INMUX_D = tmp;
            break;

        case PIN_TYPE_UART_APS_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 16);
            tmp |= (0xA << 16);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 16);
            tmp |= (0xA << 16);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 20);
            tmp |= (0xA << 20);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 21);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_22
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_22(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_C;
    tmp &= ~(0xF << 24);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_TX:
            tmp |= (0x3 << 24);
            break;

        case PIN_TYPE_I2C_SDA:
            tmp |= (0x2 << 24);
            break;

        case PIN_TYPE_SPI2_IO_2:
            tmp |= (0x0 << 24);
            break;

        case PIN_TYPE_PWM_1:
            tmp |= (0x1 << 24);
            break;

        case PIN_TYPE_UART_APS_TX:
            tmp |= (0xA << 24);
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp |= (0x8 << 24);
            break;

        default:
            // the others are input only
            tmp |= (0xD << 24);
            break;
    }
    AOS->RG_PDI_SRC_IO_C = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 22);
    switch (uwConfig)
    {
        case PIN_TYPE_I2C_SDA:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~((uint32_t)0xF << 28);
            tmp |= ((uint32_t)0xB << 28);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI2_IO_2:
            tmp = AOS->RG_PTS_INMUX_D;
            tmp &= ~(0xF << 8);
            tmp |= (0x2 << 8);
            AOS->RG_PTS_INMUX_D = tmp;
            break;

        case PIN_TYPE_UART_MSQ_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 20);
            tmp |= (0xB << 20);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 12);
            tmp |= (0xB << 12);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 24);
            tmp |= (0xB << 24);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 22);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet_IO_23
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. uwConfig  : [In] the type config of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_IO_23(uint16_t uwConfig)
{
    volatile uint32_t tmp;
    volatile uint32_t tmp2;

    // output source
    tmp = AOS->RG_PDI_SRC_IO_C;
    tmp &= ~((uint32_t)0xF << 28);
    switch (uwConfig)
    {
        case PIN_TYPE_I2C_SCL:
            tmp |= ((uint32_t)0x2 << 28);
            break;

        case PIN_TYPE_SPI2_IO_3:
            tmp |= ((uint32_t)0x0 << 28);
            break;

        case PIN_TYPE_PWM_0:
            tmp |= ((uint32_t)0x1 << 28);
            break;

        case PIN_TYPE_UART_MSQ_TX:
            tmp |= ((uint32_t)0xB << 28);
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp |= ((uint32_t)0x9 << 28);
            break;

        default:
            // the others are input only
            tmp |= ((uint32_t)0xD << 28);
            break;
    }
    AOS->RG_PDI_SRC_IO_C = tmp;

    // input IO + input Enable
    tmp2 = AOS->RG_PD_IE;
    tmp2 |= (0x1 << 23);
    switch (uwConfig)
    {
        case PIN_TYPE_UART1_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 12);
            tmp |= (0x8 << 12);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_I2C_SCL:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 24);
            tmp |= (0xB << 24);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_SPI2_IO_3:
            tmp = AOS->RG_PTS_INMUX_D;
            tmp &= ~(0xF << 12);
            tmp |= (0x2 << 12);
            AOS->RG_PTS_INMUX_D = tmp;
            break;

        case PIN_TYPE_UART_APS_RX:
            tmp = AOS->RG_PTS_INMUX_A;
            tmp &= ~(0xF << 16);
            tmp |= (0xB << 16);
            AOS->RG_PTS_INMUX_A = tmp;
            break;

        case PIN_TYPE_ICE_M3_CLK:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 16);
            tmp |= (0xB << 16);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_ICE_M0_DAT:
            tmp = AOS->RG_PTS_INMUX_B;
            tmp &= ~(0xF << 20);
            tmp |= (0xB << 20);
            AOS->RG_PTS_INMUX_B = tmp;
            break;

        case PIN_TYPE_GPIO_INPUT:
        case PIN_TYPE_GPIO_OUTPUT_LOW:
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            break;

        default:
            // the others are output only.
            tmp2 &= ~(0x1 << 23);
            break;
    }
    AOS->RG_PD_IE = tmp2;

    return HAL_PIN_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Pin_PreInitCold
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
void Hal_Pin_PreInitCold(void)
{
    // External
    Hal_Pin_ConfigSet = Hal_Pin_ConfigSet_impl;

    // Internal
    g_taHal_Pin_IoConfigTbl[0] = Hal_Pin_ConfigSet_IO_0;
    g_taHal_Pin_IoConfigTbl[1] = Hal_Pin_ConfigSet_IO_1;
    g_taHal_Pin_IoConfigTbl[2] = Hal_Pin_ConfigSet_IO_2;
    g_taHal_Pin_IoConfigTbl[3] = Hal_Pin_ConfigSet_IO_3;
    g_taHal_Pin_IoConfigTbl[4] = Hal_Pin_ConfigSet_IO_4;
    g_taHal_Pin_IoConfigTbl[5] = Hal_Pin_ConfigSet_IO_5;
    g_taHal_Pin_IoConfigTbl[6] = Hal_Pin_ConfigSet_IO_6;
    g_taHal_Pin_IoConfigTbl[7] = Hal_Pin_ConfigSet_IO_7;
    g_taHal_Pin_IoConfigTbl[8] = Hal_Pin_ConfigSet_IO_8;
    g_taHal_Pin_IoConfigTbl[9] = Hal_Pin_ConfigSet_IO_9;
    g_taHal_Pin_IoConfigTbl[10] = Hal_Pin_ConfigSet_IO_10;
    g_taHal_Pin_IoConfigTbl[11] = Hal_Pin_ConfigSet_IO_11;
    g_taHal_Pin_IoConfigTbl[12] = Hal_Pin_ConfigSet_IO_12;
    g_taHal_Pin_IoConfigTbl[13] = Hal_Pin_ConfigSet_IO_13;
    g_taHal_Pin_IoConfigTbl[14] = Hal_Pin_ConfigSet_IO_14;
    g_taHal_Pin_IoConfigTbl[15] = Hal_Pin_ConfigSet_IO_15;
    g_taHal_Pin_IoConfigTbl[16] = Hal_Pin_ConfigSet_IO_16;
    g_taHal_Pin_IoConfigTbl[17] = Hal_Pin_ConfigSet_IO_17;
    g_taHal_Pin_IoConfigTbl[18] = Hal_Pin_ConfigSet_IO_18;
    g_taHal_Pin_IoConfigTbl[19] = Hal_Pin_ConfigSet_IO_19;
    g_taHal_Pin_IoConfigTbl[20] = Hal_Pin_ConfigSet_IO_20;
    g_taHal_Pin_IoConfigTbl[21] = Hal_Pin_ConfigSet_IO_21;
    g_taHal_Pin_IoConfigTbl[22] = Hal_Pin_ConfigSet_IO_22;
    g_taHal_Pin_IoConfigTbl[23] = Hal_Pin_ConfigSet_IO_23;
    g_taHal_Pin_IoConfigTbl[24] = NULL;
    g_taHal_Pin_IoConfigTbl[25] = NULL;
    g_taHal_Pin_IoConfigTbl[26] = NULL;
    g_taHal_Pin_IoConfigTbl[27] = NULL;
    g_taHal_Pin_IoConfigTbl[28] = NULL;
    g_taHal_Pin_IoConfigTbl[29] = NULL;
    g_taHal_Pin_IoConfigTbl[30] = NULL;
}
