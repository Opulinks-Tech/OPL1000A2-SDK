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
 * @file hal_flash_patch.c
 * 
 * @brief Flash control API
 *
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_flash_patch.h"
#include "hal_flash_internal.h"
#include "hal_tick.h"


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
uint32_t _Hal_Flash_WriteDoneCheck_patch(E_SpiIdx_t u32SpiIdx);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
uint32_t g_u32FlashPollingStatusTimeoutMs;

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


void Hal_Flash_PatchInit(void)
{   
    g_u32FlashPollingStatusTimeoutMs = FLASH_POLLING_BUSY_TIMEOUT_MS;
    _Hal_Flash_WriteDoneCheck = _Hal_Flash_WriteDoneCheck_patch;
}



/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */


uint32_t _Hal_Flash_WriteDoneCheck_patch(E_SpiIdx_t u32SpiIdx)
{
    uint32_t u32Status = 0;
    uint32_t u32Temp = 0;
    uint32_t u32TimeoutTicks;
    uint32_t u32TickStart, u32TickDiff;

    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32FlashPollingStatusTimeoutMs;
    
    if(u32SpiIdx >= SPI_IDX_MAX)
        return 1;

    u32TickStart = Hal_Tick_Diff(0);
    do
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if(u32TickDiff > u32TimeoutTicks)
            return 1;
        
        u32Temp = TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | FLASH_CMD_READ_STATUS_REG;
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
        u32Temp = TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_WRITE | FLASH_DR_DUMMY_VAL; // compelte
        Hal_Spi_Data_Send(u32SpiIdx, u32Temp);
        
        Hal_Spi_Data_Recv(u32SpiIdx, &u32Temp); // dummy
        Hal_Spi_Data_Recv(u32SpiIdx, &u32Status);
        
    } while(u32Status & FLASH_STATUS_BUSY);
    
    return 0;
}
