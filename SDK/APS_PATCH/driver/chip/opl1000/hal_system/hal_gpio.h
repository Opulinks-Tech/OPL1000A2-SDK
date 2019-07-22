/* *****************************************************************************
 *  Copyright 2019, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *
 *******************************************************************************
 *
 *  @file hal_gpio.h 
 * 
 *  @brief GPIO API
 *
 ***************************************************************************************************
 

## Sleep IO
### GPIO output
- As is
- User control can force GPIO output pin state

### GPIO input
- Disabled except wakeup pin

### Peripheral output
- Most are enabled by auto control. Refer to following table
- Using User control API can force the output disable or change level.
- e.g.
    - IO8 is APS_UART_TX in run mode
    - Auto control: Set Output high in sleep mode
    - If needed, using user control API to force IO8 disable output in sleep mode.
        ```c
        Hal_Gpio_SleepIoUsrCtrlSet(GPIO_IDX_08, SLEEP_OUTPUT_DISABLE);
        ```
### Peripheral input
- All disabled
  
  
##Sleep mode IO output auto control
  
|  Block |  Pin   | I/O | Output enable  |   Level  |                   Note                  |
| :----: | :----: | :-: | :------------: | :------: | :-------------------------------------- |
|  GPIO  | Input  |  I  |  Disable       |    x     |                                         |
|  GPIO  | Output |  O  |  Keep setting  |   Keep   | Output setting is kept                  |
|  UART  |  TX    |  O  |  Enable        |   High   |                                         |
|  UART  |  RX    |  I  |  Disable       |    x     |                                         |
|  UART  |  RTS   |  O  |  Enable        |   High   |                                         |
|  UART  |  CTS   |  I  |  Disable       |    x     |                                         |
|  I2C   |  SDA   | I/O |  Enable        | **High** | User control the IO when in slave mode  |
|  I2C   |  SCK   | I/O |  Enable        | **High** | User control the IO when in slave mode  |
|  PWM   |  PWM   |  O  |  **Disable**   |    x     | By application                          |
|  SPI   |  CS    |  O  |  Enable        |   High   |                                         |
|  SPI   |  CLK   |  O  |  Enable        | **Low**  | User control when polarity not matched  |
|  SPI   |  MOSI  |  O  |  Enable        |   High   |                                         |
|  SPI   |  MISO  |  I  |  Disable       |    x     |                                         |
|  SPI   |  IO0   |  O  |  Enable        |   High   |                                         |
|  SPI   |  IO1   |  O  |  Enable        |   High   |                                         |
|  SPI   |  IO2   |  O  |  Enable        |   High   | (WP for flash)                          |
|  SPI   |  IO3   |  O  |  Enable        |   High   | (HOLD for flash)                        |
|     Others     ||  x  |  Disable       |    x     |                                         |
  
 ******************************************************************************/

#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stddef.h>
#include "hal_vic.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

#define SLEEP_CFG_OUT_EN_BIT_SHIFT       (offsetof(S_SLEEP_CFG_DEF, u8OutEn)*8)
#define SLEEP_CFG_OUT_LVL_BIT_SHIFT      (offsetof(S_SLEEP_CFG_DEF, u8OutLvl)*8)
#define SLEEP_CFG_IO_CTRL_BIT_SHIFT      (offsetof(S_SLEEP_CFG_DEF, u8IoCtl)*8)
    
/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef enum
{
    SLEEP_IO_FAIL = 0,
    SLEEP_IO_SUCCESS,
} E_SLEEP_IO_STATUS;

#pragma push
#pragma anon_unions
typedef struct
{
    union
    {
        uint32_t u32Value;
        struct
        {
            uint8_t u8OutEn;
            uint8_t u8OutLvl;
            uint8_t u8IoCtl;
            uint8_t u8Reserved;
        };
    };
} S_SLEEP_CFG_DEF;
#pragma pop

typedef struct
{
    int32_t  s32WkupIoIdx;      /* Wakeup IO index */
    uint32_t u32UsrCtrl;        /* Output control by User. If the bit is set to one, the IO setting will use oen_user and level_user.
                                   Otherwise, user oen_auto and level_auto. */
    uint32_t u32UsrOutEn;       /* Output enable. Control by user, priority is higher than oen_auto. (1: Enable, 0:Disable) */
    uint32_t u32UsrOutLvl;         /* Otuput level. Control by user, priority is higher than level_auto. (1: High, 0: Low) */
    uint32_t u32AutoOutEn;      /* Output enable. Auto check by PIN and GPIO API. (1: Enable, 0:Disable) */
    uint32_t u32AutoOutLvl;     /* Output level. Auto check by PIN and GPIO API. (1: High, 0: Low) */
} S_SLEEP_IO_CFG;




typedef enum 
{
    SLEEP_USER_CTRL_OFF         = 0,
    SLEEP_OUTPUT_DISABLE        = (1 << SLEEP_CFG_IO_CTRL_BIT_SHIFT) | (0 << SLEEP_CFG_OUT_EN_BIT_SHIFT),
    SLEEP_OUTPUT_ENABLE_HIGH    = (1 << SLEEP_CFG_IO_CTRL_BIT_SHIFT) | (1 << SLEEP_CFG_OUT_EN_BIT_SHIFT) | (1 << SLEEP_CFG_OUT_LVL_BIT_SHIFT),
    SLEEP_OUTPUT_ENABLE_LOW     = (1 << SLEEP_CFG_IO_CTRL_BIT_SHIFT) | (1 << SLEEP_CFG_OUT_EN_BIT_SHIFT) | (0 << SLEEP_CFG_OUT_LVL_BIT_SHIFT),
} E_SLEEP_IO_CFG;


/*
 *************************************************************************
 *                          Public Variables
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
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoUsrCtrlSet(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG eSleepIoCfg);

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
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoUsrCtrlGet(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG *peSleepIoCfg);

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
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoAutoCtrlSet(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG eSleepIoCfg);

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
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoAutoCtrlGet(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG *peSleepIoCfg);
/**
 * @brief Set wake up IO. Call by PS
 * @param eIdx [in] Assign IO as wake up pin.
 *                  GPIO_IDX_00, GPIO_IDX_01, and etc.
 *                  Set GPIO_IDX_MAX or other value can disable wake up IO
 */
void Hal_Gpio_SetWakeupIo(E_GpioIdx_t eIdx);

void Hal_Gpio_Init(void);

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_GPIO_H_ */
