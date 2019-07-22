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
 * @file sys_init_patch.c
 * 
 * @brief Patch for Sys init patch
 *  
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <string.h>
#include "sys_init_patch.h"
#include "hal_patch.h"
#include "hal_system_patch.h"
#include "hal_pin_patch.h"
#include "hal_vic_patch.h"
#include "hal_gpio.h"
#include "hal_spi_patch.h"
#include "hal_uart_patch.h"
#include "hal_dbg_uart_patch.h"
#include "boot_sequence.h"
#include "at_cmd_func_patch.h"
#include "controller_task_patch.h"
#include "controller_wifi_patch.h"
#include "diag_task_patch.h"
#include "wpa_cli_patch.h"
#include "driver_netlink_patch.h"
#include "ipc_patch.h"
#include "agent_patch.h"
#include "wifi_service_func_init_patch.h"
#include "lwip_jmptbl_patch.h"
#include "le_ctrl_patch.h"
#include "ble_host_patch_init.h"
#include "ps_patch.h"
#include "mw_fim_patch.h"


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


/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
#if defined(GCC)
    extern uint32_t __zi_start__;
    extern uint32_t __zi_end__;
#else
    extern unsigned int Image$$RW_IRAM1$$ZI$$Length;
    extern char Image$$RW_IRAM1$$ZI$$Base[];
#endif

uint32_t sramEndBound;

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */


void Test_ForSwPatch_patch(void)
{
    /* Do nothing. Removed in Tritium */
}

/*************************************************************************
* FUNCTION:
*   Sys_SetUnsuedSramEndBound
*
* DESCRIPTION:
*   set unused SRAM end bound
*
* PARAMETERS
*   endAddr: APS memory footprint from high location address, this changes according by chip version, projects
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_SetUnsuedSramEndBound(uint32_t endAddr)
{
    sramEndBound = endAddr;
}

/*************************************************************************
* FUNCTION:
*   Sys_SwitchOffUnusedSram
*
* DESCRIPTION:
*   to reduce current consumption, switch off unused SRAM blocks
*
* PARAMETERS
*   memFootPrint: APS memory footprint, this changes according by chip version, projects
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_SwitchOffUnusedSram(uint32_t memFootPrint)
{
    typedef struct {
        uint32_t idxMsk;
        uint32_t startAddr;
        uint32_t endAddr;
    } S_SramSwitchMap;

    S_SramSwitchMap mapping[] = {
        (1 << 4), 0x00418000, 0x00420000,  // block 4 off
        (1 << 5), 0x00420000, 0x00428000,  // block 5 off
        (1 << 6), 0x00428000, 0x00430000,  // block 6 off
        (1 << 7), 0x00430000, 0x00438000,  // block 7 off
        (1 << 8), 0x00438000, 0x00440000,  // block 8 off
    };

    uint32_t offMsk = (1 << 11);           // block 11 off (shared memory 0)
    int i;

    if (!sramEndBound)
        sramEndBound = 0x00440000;

    for (i = 0; i < sizeof(mapping) / sizeof(S_SramSwitchMap); i++)
    {
        S_SramSwitchMap entry = mapping[i];

        if (memFootPrint > entry.startAddr)
            continue;
        else if (sramEndBound < entry.endAddr)
            break;
        else
            offMsk |= entry.idxMsk;
    }

    Hal_Sys_RetRamTurnOff(offMsk);
}

/*************************************************************************
* FUNCTION:
*   Sys_MiscDriverConfigSetup
*
* DESCRIPTION:
*   set the configuration for misc driver
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_MiscDriverConfigSetup_patch(void)
{
    if (!Boot_CheckWarmBoot())
    {
      #if defined(GCC)
        Sys_SwitchOffUnusedSram((uint32_t) &__zi_end__);
      #else
        Sys_SwitchOffUnusedSram((uint32_t) Image$$RW_IRAM1$$ZI$$Base + (uint32_t) &Image$$RW_IRAM1$$ZI$$Length);
      #endif
    }
}

void Sys_RomVersion_patch(void)
{
    printf("[OPL1000 A2 PATCH]\n");
}


/**
 * @bried Init patch function when cold boot
 */
void SysInit_EntryPoint(void)
{
    if (Boot_CheckWarmBoot())
        return;

    // init bss section
  #if defined(GCC)
    memset(&__zi_start__, 0, ((uint32_t) &__zi_end__ - (uint32_t) &__zi_start__));
  #else
    memset(Image$$RW_IRAM1$$ZI$$Base, 0, (unsigned int)&Image$$RW_IRAM1$$ZI$$Length);
  #endif
    
    // 0. Tracer

    // 1. hal patch
    Test_ForSwPatch = Test_ForSwPatch_patch;

    // 2. boot sequence

    // 3. sys init
    Sys_MiscDriverConfigSetup = Sys_MiscDriverConfigSetup_patch;
    Sys_RomVersion = Sys_RomVersion_patch;
    
    // 4. IPC
    Ipc_PreInit_patch();

    // 5. Control task
    controller_task_func_init_patch();
    
    // 6. Wifi
    wifi_ctrl_init_patch();
    wifi_service_func_init_patch();
    
    // 7. le_ctrl
    le_ctrl_pre_patch_init();
    
    // 8. le_host
    LeHostPatchAssign();

    // 9. LwIP
    lwip_module_interface_init_patch();

    // 10. WPAS
    wpa_cli_func_init_patch();
    wpa_driver_func_init_patch();
    
    // 11. AT
    at_func_init_patch();
    
    // 12. SCRT

    // 13. HAL driver API
    Hal_Gpio_Init();
    Hal_Sys_PatchInit();
    Hal_Pin_PatchInit();
    Hal_Vic_PatchInit();
    Hal_Spi_PatchInit();
    Hal_Uart_PatchInit();
    Hal_DbgUart_PatchInit();
    
    // 14. os

    // 15. util api

    // 16. Power Saving
    ps_patch_init();

    // 17. ISR

    // 18. DIAG
    Diag_PatchInit();
    
    // 19. FIM
    MwFim_PreInit_patch();

    // 20. AUXADC

    // 21. Temperature

    // 22. Main

    // 23. Agent
    agent_pre_init_patch();
    
    // 24. OTA
    
    // 25. System Common
    
    // 26. RF config

    // 27. User App Task
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */



