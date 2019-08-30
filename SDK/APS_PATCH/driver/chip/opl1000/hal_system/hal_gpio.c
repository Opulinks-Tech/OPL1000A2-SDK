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
 * @file hal_gpio.c
 * 
 * @brief GPIO API
 *  
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_gpio.h"
#include "ipc_patch.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */

S_SLEEP_IO_CFG *g_psHal_Gpio_SleepIoCfg = NULL;

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


/**
 * @brief Set user control pin setting in sleep mode
 *
 *        In OPL1000, if the IO is input by GPIO or peripheral, it will be disable in sleep mode.
 *        If the IO is GPIO output, it will keep output setting in sleep mode.
 *        If the IO is peripheral output, it might be enable high/low or disable according to the peripheral.
 *        Besides the auto configuration, uesr can use this function to assign IO pin as output high/low or disable in sleep mode.
 *
 * @param eIdx [in] The IO index. GPIO_IDX_00, GPIO_IDX_01, and etc
 * @param eSleepIoCfg [in] To configure the pin setting in sleep mode
 *          SLEEP_USER_CTRL_OFF: Using auto control.
 *          SLEEP_OUTPUT_DISABLE: Using user contorl, force the pin disable output.
 *          SLEEP_OUTPUT_ENABLE_HIGH: Using user control, force the pin enable output and pull high.
 *          SLEEP_OUTPUT_ENABLE_LOW: Using user control, force the pin enable output and pull low.
 * @return Configuration status
 * @retval SLEEP_IO_SUCCESS     Set success
 * @retval SLEEP_IO_FAIL        Set fail
 */
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoUsrCtrlSet(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG eSleepIoCfg)
{
    uint32_t u32IoBit = (1 << eIdx);
    S_SLEEP_CFG_DEF sSleepDef;
    
    if (eIdx >=GPIO_IDX_MAX)
        return SLEEP_IO_FAIL;
    
    sSleepDef.u32Value = (uint32_t)eSleepIoCfg;
    
    if (sSleepDef.u8IoCtl)
        g_psHal_Gpio_SleepIoCfg->u32UsrCtrl |= u32IoBit;
    else
        g_psHal_Gpio_SleepIoCfg->u32UsrCtrl &= ~u32IoBit;
    
    if (sSleepDef.u8OutEn)
        g_psHal_Gpio_SleepIoCfg->u32UsrOutEn |= u32IoBit;
    else
        g_psHal_Gpio_SleepIoCfg->u32UsrOutEn &= ~u32IoBit;
    
    if (sSleepDef.u8OutLvl)
        g_psHal_Gpio_SleepIoCfg->u32UsrOutLvl |= u32IoBit;
    else
        g_psHal_Gpio_SleepIoCfg->u32UsrOutLvl &= ~u32IoBit;
    
    return SLEEP_IO_SUCCESS;
}


/**
 * @brief Get user control pin setting in sleep mode
 *
 *        In OPL1000, if the IO is input by GPIO or peripheral, it will be disable in sleep mode.
 *        If the IO is GPIO output, it will keep output setting in sleep mode.
 *        If the IO is peripheral output, it might be enable high/low or disable according to the peripheral.
 *        Besides the auto configuration, uesr can use this function to assign IO pin as output high/low or disable in sleep mode.
 *
 * @param eIdx [in] The IO index. GPIO_IDX_00, GPIO_IDX_01, and etc
 * @param peSleepIoCfg [out] Read pin user control setting in sleep mode
 *          SLEEP_USER_CTRL_OFF: Using auto control.
 *          SLEEP_OUTPUT_DISABLE: Using user contorl, force the pin disable output.
 *          SLEEP_OUTPUT_ENABLE_HIGH: Using user control, force the pin enable output and pull high.
 *          SLEEP_OUTPUT_ENABLE_LOW: Using user control, force the pin enable output and pull low.
 * @return Read control status
 * @retval SLEEP_IO_SUCCESS     Set success
 * @retval SLEEP_IO_FAIL        Set fail
 */
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoUsrCtrlGet(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG *peSleepIoCfg)
{
    uint32_t u32IoBit = (1 << eIdx);
    
    if (eIdx >=GPIO_IDX_MAX)
        return SLEEP_IO_FAIL;
    
    if (!(g_psHal_Gpio_SleepIoCfg->u32UsrCtrl & u32IoBit))
    {   /* user control is off */
        *peSleepIoCfg = SLEEP_USER_CTRL_OFF;
    }
    else if (!(g_psHal_Gpio_SleepIoCfg->u32UsrOutEn & u32IoBit))
    {
        *peSleepIoCfg = SLEEP_OUTPUT_DISABLE;
    }
    else if (g_psHal_Gpio_SleepIoCfg->u32UsrOutLvl & u32IoBit)
    {
        *peSleepIoCfg = SLEEP_OUTPUT_ENABLE_HIGH;
    }
    else
    {
        *peSleepIoCfg = SLEEP_OUTPUT_ENABLE_LOW;
    }
    return SLEEP_IO_SUCCESS;
}

/**
 * @brief Set auto control pin setting in sleep mode
 * 
 * @warning Only called by Hal driver. Don't call this function in user layer.
 *
 * @param eIdx [in] The IO index. GPIO_IDX_00, GPIO_IDX_01, and etc
 * @param peSleepIoCfg [out] Read pin user control setting in sleep mode
 *          SLEEP_OUTPUT_DISABLE: Using user contorl, force the pin disable output.
 *          SLEEP_OUTPUT_ENABLE_HIGH: Using user control, force the pin enable output and pull high.
 *          SLEEP_OUTPUT_ENABLE_LOW: Using user control, force the pin enable output and pull low.
 * @return Read control status
 * @retval SLEEP_IO_SUCCESS     Set success
 * @retval SLEEP_IO_FAIL        Set fail
 */
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoAutoCtrlSet(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG eSleepIoCfg)
{
    uint32_t u32IoBit = (1 << eIdx);
    S_SLEEP_CFG_DEF sSleepDef;
    
    if (eIdx >=GPIO_IDX_MAX)
        return SLEEP_IO_FAIL;
    
    sSleepDef.u32Value = (uint32_t)eSleepIoCfg;
    
    if (sSleepDef.u8OutEn)
        g_psHal_Gpio_SleepIoCfg->u32AutoOutEn |= u32IoBit;
    else
        g_psHal_Gpio_SleepIoCfg->u32AutoOutEn &= ~u32IoBit;
    
    if (sSleepDef.u8OutLvl)
        g_psHal_Gpio_SleepIoCfg->u32AutoOutLvl |= u32IoBit;
    else
        g_psHal_Gpio_SleepIoCfg->u32AutoOutLvl &= ~u32IoBit;
    
    return SLEEP_IO_SUCCESS;
}

/**
 * @brief Read auto control pin setting in sleep mode
 *
 *        In OPL1000, if the IO is input by GPIO or peripheral, it will be disable in sleep mode.
 *        If the IO is GPIO output, it will keep output setting in sleep mode.
 *        If the IO is peripheral output, it might be enable high/low or disable according to the peripheral.
 *        Besides the auto configuration, uesr can use this function to assign IO pin as output high/low or disable in sleep mode.
 *
 * @param eIdx [in] The IO index. GPIO_IDX_00, GPIO_IDX_01, and etc
 * @param peSleepIoCfg [out] Read pin user control setting in sleep mode
 *          SLEEP_OUTPUT_DISABLE: Using user contorl, force the pin disable output.
 *          SLEEP_OUTPUT_ENABLE_HIGH: Using user control, force the pin enable output and pull high.
 *          SLEEP_OUTPUT_ENABLE_LOW: Using user control, force the pin enable output and pull low.
 * @return Read control status
 * @retval SLEEP_IO_SUCCESS     Set success
 * @retval SLEEP_IO_FAIL        Set fail
 */
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoAutoCtrlGet(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG *peSleepIoCfg)
{
    uint32_t u32IoBit = (1 << eIdx);
    
    if (eIdx >=GPIO_IDX_MAX)
        return SLEEP_IO_FAIL;
    
    if (!(g_psHal_Gpio_SleepIoCfg->u32AutoOutEn & u32IoBit))
    {
        *peSleepIoCfg = SLEEP_OUTPUT_DISABLE;
    }
    else if (g_psHal_Gpio_SleepIoCfg->u32AutoOutLvl & u32IoBit)
    {
        *peSleepIoCfg = SLEEP_OUTPUT_ENABLE_HIGH;
    }
    else
    {
        *peSleepIoCfg = SLEEP_OUTPUT_ENABLE_LOW;
    }
    return SLEEP_IO_SUCCESS;
}


void Hal_Gpio_Init(void)
{
    g_psHal_Gpio_SleepIoCfg = (S_SLEEP_IO_CFG *)g_u32IpcSleepIoCfgAddr;
    g_psHal_Gpio_SleepIoCfg->u32UsrCtrl       = 0;
    g_psHal_Gpio_SleepIoCfg->u32UsrOutEn      = 0;
    g_psHal_Gpio_SleepIoCfg->u32UsrOutLvl     = 0;
    g_psHal_Gpio_SleepIoCfg->u32AutoOutEn     = 0;
    g_psHal_Gpio_SleepIoCfg->u32AutoOutLvl    = 0;
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */





