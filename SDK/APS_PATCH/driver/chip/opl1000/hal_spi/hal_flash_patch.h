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
 *  @file hal_flash_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _HAL_FLASH_PATCH_H_
#define _HAL_FLASH_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_flash.h"
#include "hal_spi_patch.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define FLASH_CMD_READ_STATUS_REG       0x05

#define FLASH_DR_DUMMY_VAL              0x00
#define FLASH_POLLING_BUSY_TIMEOUT_MS   400 /* 400 ms for erasing 4K sector */
#define FLASH_STATUS_BUSY               0x01
/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */



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
void Hal_Flash_PatchInit(void);

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_FLASH_PATCH_H_ */
