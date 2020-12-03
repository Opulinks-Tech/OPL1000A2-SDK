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
#include "hal_flash_patch.h"
#include "hal_uart_patch.h"
#include "hal_dbg_uart_patch.h"
#include "hal_auxadc_patch.h"
#include "hal_flash.h"
#include "hal_wdt.h"
#include "boot_sequence.h"
#include "at_cmd_common.h"
#include "at_cmd_func_patch.h"
#include "controller_task_patch.h"
#include "controller_wifi_patch.h"
#include "diag_task_patch.h"
#include "wpa_cli_patch.h"
#include "driver_netlink_patch.h"
#include "ipc_patch.h"
#include "agent_patch.h"
#include "wifi_service_func_init_patch.h"
#include "wifi_mac_tx_data_patch.h"
#include "lwip_jmptbl_patch.h"
#include "le_ctrl_patch.h"
#include "ble_host_patch_init.h"
#include "ps_patch.h"
#include "mw_fim_patch.h"
#include "mw_fim_default_group01_patch.h"
#include "sys_cfg_patch.h"
#include "opl1000_it_patch.h"
#include "events_netlink_patch.h"
#include "cmsis_os_patch.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define BOOT_MODE_ICE       0x2
#define BOOT_MODE_JTAG      0x3
#define BOOT_MODE_NORMAL    0xA

#define WDT_TIMEOUT_SECS    10


#define DEV_32K_SRC_SEL_32K_XTAL    0   /* Default */
#define DEV_32K_SRC_SEL_32K_RC      IPC_SPARE0_SEQ_32K_SRC_SEL

#define IRQ_PRIORITY_IPC3_PATCH     0x0A
/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

/*
 *************************************************************************
 *                           Declarions of Private Functions
 *************************************************************************
 */
extern T_Sys_PowerSetup_fp Sys_PowerSetup;
extern T_Sys_ClockSetup_fp Sys_ClockSetup;
extern T_Sys_UartInit_fp Sys_UartInit;
extern T_Sys_MiscModulesInit_fp Sys_MiscModulesInit;
extern T_Sys_MiscDriverConfigSetup_fp Sys_MiscDriverConfigSetup;
extern T_Sys_DriverInit_fp Sys_DriverInit;
extern T_Sys_WaitforMsqReady_fp Sys_WaitforMsqReady;
extern T_Sys_WaitForMsqFlashAccessDone_fp Sys_WaitForMsqFlashAccessDone;
extern T_Sys_RomVersion_fp Sys_RomVersion;
extern T_Sys_ServiceInit_fp Sys_ServiceInit;
extern T_Sys_AppInit_fp Sys_AppInit;
extern T_Sys_PostInit_fp Sys_PostInit;
extern T_Sys_StackOverflowHook_fp Sys_StackOverflowHook;
extern T_Sys_IdleHook_fp Sys_IdleHook;

__forceinline static void Sys_NotifyReadyToMsq(uint32_t indicator);

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
    extern uint32_t __bss_start__;
    extern uint32_t __bss_end__;
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
extern void Sys_ServiceInit_impl(void);

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

    //uint32_t offMsk = (1 << 11);           // block 11 off (shared memory 0)
    uint32_t offMsk = 0;                   // block 11 on (shared memory 0)
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
*   Sys_SwitchTo32kRC
*
* DESCRIPTION:
*   Must called at __Patch_EntryPoint
*   Select 32k RC as RTC timer/SEQ clock source
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_SwitchTo32kRC(void)
{
    uint32_t u32SpareReg;

    if (Boot_CheckWarmBoot())
        return;

    while (1) {
        Hal_Sys_SpareRegRead(SPARE_0, &u32SpareReg);

        /* Wait MSQ get load patch done finished, avoid data corruption */
        if ((u32SpareReg & IPC_SPARE0_LOAD_PATCH_READY) == 0)
            break;
    }

    u32SpareReg |= DEV_32K_SRC_SEL_32K_RC;
    Hal_Sys_SpareRegWrite(SPARE_0, u32SpareReg);
}

/*************************************************************************
* FUNCTION:
*   Sys_DriverInit
*
* DESCRIPTION:
*   the initial for driver
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_DriverInit_patch(void)
{
    // Wait for M0 initialization to be completed
    Sys_WaitforMsqReady();

    // First restore APS clock after MSQ ready
    if (Boot_CheckWarmBoot())
    {
		Hal_Sys_ApsClkResume();
        Sys_NotifyReadyToMsq(IPC_SPARE0_APS_CLOCK_READY);
    }

    // Set power
    Sys_PowerSetup();

    // Set system clock
    Sys_ClockSetup();

    // Set pin-mux
    // cold boot
    if (0 == Boot_CheckWarmBoot())
        Hal_SysPinMuxAppInit();

    if (!Boot_CheckWarmBoot())
    {
        // Init VIC
        Hal_Vic_Init();

	    // Init GPIO
	    Hal_Vic_GpioInit();
    }
    // Fetch GPIO interrupt status at warm-boot
    else
    {
        ps_update_boot_gpio_int_status(Hal_Vic_GpioIntStatRead());
    }

	// Init IPC
    Hal_Vic_IpcIntEn(IPC_IDX_0, 1);
    Hal_Vic_IpcIntEn(IPC_IDX_1, 1);
    Hal_Vic_IpcIntEn(IPC_IDX_2, 1);
    Hal_Vic_IpcIntEn(IPC_IDX_3, 1);
    NVIC_SetPriority(IPC3_IRQn, IRQ_PRIORITY_IPC3_PATCH);   // NOTE: for sleep IO detect

    // Init DBG_UART
    Hal_DbgUart_Init(115200);
    printf("\n");

    // Init SPI 0
    Hal_Spi_Init(SPI_IDX_0, SystemCoreClockGet()/2,
        SPI_CLK_PLOAR_HIGH_ACT, SPI_CLK_PHASE_START, SPI_FMT_MOTOROLA, SPI_DFS_08_bit, 1);

    // Init flash on SPI 0
    Hal_Flash_Init(SPI_IDX_0);

    // FIM
    MwFim_Init();

    // Init UART1
    Sys_UartInit();

    // for APP patch only. Do NOT patch it!!!
    // Other modules' init
    Sys_MiscModulesInit();

    //-------------------------------------------------------------------------------------
    // Other driver config need by Task-level (sleep strategy)

    // Diag task
    //Hal_DbgUart_RxCallBackFuncSet(uartdbg_rx_int_handler);  //Assigned in diag_task
    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        // the default is on
        Hal_DbgUart_RxIntEn(1);
    }
    // warm boot
    else
    {
        Hal_DbgUart_RxIntEn(Hal_DbgUart_RxIntEnStatusGet());
    }

    // HCI and AT command
    uart1_mode_set_default();

    // Turn-off SRAM
    if (!Boot_CheckWarmBoot())
    {
      #if defined(GCC)
        Sys_SwitchOffUnusedSram((uint32_t) &__bss_end__);
      #else
        Sys_SwitchOffUnusedSram((uint32_t) Image$$RW_IRAM1$$ZI$$Base + (uint32_t) &Image$$RW_IRAM1$$ZI$$Length);
      #endif
    }

	// power-saving module init
	ps_init();

	if (Boot_CheckWarmBoot())
	{
		// TODO: Revision will be provided by Ophelia after peripheral restore mechanism completed
		uart1_mode_set_default();
		uart1_mode_set_at();

        // Init GPIO
        Hal_Vic_GpioInit();
	}

    //Watch Dog
    if (Hal_Sys_StrapModeRead() == BOOT_MODE_NORMAL)
    {
        Hal_Vic_IntTypeSel(WDT_IRQn, INT_TYPE_FALLING_EDGE);
        Hal_Vic_IntInv(WDT_IRQn, 1);
        Hal_Wdt_Init(WDT_TIMEOUT_SECS * SystemCoreClockGet());
    }

    // for APP patch only. Do NOT patch it!!!
    // Other tasks' driver config.
    Sys_MiscDriverConfigSetup();
}

/*************************************************************************
* FUNCTION:
*   Sys_ServiceInit
*
* DESCRIPTION:
*   the initial for service
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_ServiceInit_patch(void)
{
    Sys_ServiceInit_impl();

    // SYS config
    sys_cfg_init();
}

/*************************************************************************
* FUNCTION:
*   Sys_PostInit
*
* DESCRIPTION:
*   the post initial for sys init
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_PostInit_patch(void)
{
    extern void Sys_PostInit_impl(void);

    sys_cfg_rf_init_patch(NULL);

    Sys_PostInit_impl();
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
    memset(&__bss_start__, 0, ((uint32_t) &__bss_end__ - (uint32_t) &__bss_start__));
  #else
    memset(Image$$RW_IRAM1$$ZI$$Base, 0, (unsigned int)&Image$$RW_IRAM1$$ZI$$Length);
  #endif

    // 0. Tracer

    // 1. hal patch
    Test_ForSwPatch = Test_ForSwPatch_patch;

    // 2. boot sequence

    // 3. sys init
    Sys_DriverInit = Sys_DriverInit_patch;
    Sys_RomVersion = Sys_RomVersion_patch;
    Sys_ServiceInit = Sys_ServiceInit_patch;
    Sys_PostInit = Sys_PostInit_patch;

    // 4. IPC
    Ipc_PreInit_patch();

    // 5. Control task
    controller_task_func_init_patch();

    // 6. Wifi
    wifi_ctrl_init_patch();
    wifi_service_func_init_patch();
    wifi_mac_txdata_func_init_patch();

    // 7. le_ctrl
    le_ctrl_pre_patch_init();

    // 8. le_host
    LeHostPatchAssign();

    // 9. LwIP
    lwip_module_interface_init_patch();

    // 10. WPAS
    wpa_cli_func_init_patch();
    wpa_driver_func_init_patch();
    wpa_events_func_init_patch();
    
    // 11. AT
    at_func_init_patch();

    // 12. SCRT

    // 13. HAL driver API
    Hal_Gpio_Init();
    Hal_Sys_PatchInit();
    Hal_Pin_PatchInit();
    Hal_Vic_PatchInit();
    Hal_Spi_PatchInit();
    Hal_Flash_PatchInit();
    Hal_Uart_PatchInit();
    Hal_DbgUart_PatchInit();
    Hal_Aux_PatchInit();
    
    // 14. os
    osPatchInit();

    // 15. util api

    // 16. Power Saving
    ps_patch_init();

    // 17. ISR
    ISR_Pre_PatchInit();

    // 18. DIAG
    Diag_PatchInit();

    // 19. FIM
    MwFim_PreInit_patch();
    MwFim_Group01_patch();

    // 20. AUXADC

    // 21. Temperature

    // 22. Main

    // 23. Agent
    agent_pre_init_patch();

    // 24. OTA

    // 25. System Common

    // 26. SYS config
    sys_cfg_pre_init_patch();

    // 27. User App Task
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

__forceinline static void Sys_NotifyReadyToMsq(uint32_t indicator)
{
	uint32_t reg_spare_0_val;

	Hal_Sys_SpareRegRead(SPARE_0, &reg_spare_0_val);
	Hal_Sys_SpareRegWrite(SPARE_0, reg_spare_0_val | indicator);
}
