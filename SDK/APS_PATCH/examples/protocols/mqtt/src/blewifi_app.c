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

/**
 * @file blewifi_app.c
 * @author Vincent Chen
 * @date 12 Feb 2018
 * @brief File creates the wifible app task architecture.
 *
 */
#include <stdio.h>
#include <string.h>

#include "sys_init.h"
#include "sys_init_patch.h"
#include "hal_system.h"
#include "cmsis_os.h"
#include "sys_os_config.h"
#include "event_groups.h" 
#include "blewifi_configuration.h"
#include "blewifi_common.h"
#include "blewifi_app.h"
#include "blewifi_wifi_api.h"
#include "blewifi_ble_api.h"
#include "blewifi_ctrl.h"
#include "blewifi_ctrl_http_ota.h"
#include "sys_common_api.h"
#include "ps_public.h"
#include "mw_fim_default_group03.h"
#include "mw_fim_default_group03_patch.h"
#include "mw_fim_default_group08.h"
#include "mw_fim_default_group08_project.h"
#include "app_at_cmd.h"
#include "mqtt_client.h"

blewifi_ota_t *gTheOta = 0;

osThreadId mqttClientThreadId;

/*EventGroupHandle_t event_group;

const int GOTIP_BIT0 = 0x01;

static void init_event_group(void)
{
    event_group = xEventGroupCreate();
}
*/
void App_Entry(void *args)
{
    if(true == BleWifi_Ctrl_EventStatusWait(BLEWIFI_CTRL_EVENT_BIT_GOT_IP, 0xFFFFFFFF)) // start mqtt client after got IP.
    {
        printf("... Got IP\n");
			  BleWifi_Wifi_SetDTIM(BleWifi_Ctrl_DtimTimeGet());
        mqtt_client();
    }	
}

void BleWifiAppInit(void)
{
	  osThreadDef_t tThreadDef;
    T_MwFim_SysMode tSysMode;
    T_MwFim_GP08_PowerSaving tPowerSaving;
    
	gTheOta = 0;

#if (SNTP_FUNCTION_EN == 1)
    g_ulSntpSecondInit = SNTP_SEC_2019;     // Initialize the Sntp Value
    g_ulSystemSecondInit = 0;               // Initialize System Clock Time
#endif

    // get the settings of system mode
	if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP03_PATCH_SYS_MODE, 0, MW_FIM_SYS_MODE_SIZE, (uint8_t*)&tSysMode))
    {
        // if fail, get the default value
        memcpy(&tSysMode, &g_tMwFimDefaultSysMode, MW_FIM_SYS_MODE_SIZE);
    }

    // get the settings of power saving
	if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP08_PROJECT_POWER_SAVING, 0, MW_FIM_GP08_POWER_SAVING_SIZE, (uint8_t*)&tPowerSaving))
    {
        // if fail, get the default value
        memcpy(&tPowerSaving, &g_tMwFimDefaultGp08PowerSaving, MW_FIM_GP08_POWER_SAVING_SIZE);
    }

    // only for the user mode
    if ((tSysMode.ubSysMode == MW_FIM_SYS_MODE_INIT) || (tSysMode.ubSysMode == MW_FIM_SYS_MODE_USER))
    {
        /* Wi-Fi Initialization */
        BleWifi_Wifi_Init();

        /* BLE Stack Initialization */
        BleWifi_Ble_Init();

        /* blewifi "control" task Initialization */
        BleWifi_Ctrl_Init();

        /* blewifi HTTP OTA */
        #if (WIFI_OTA_FUNCTION_EN == 1)
        blewifi_ctrl_http_ota_task_create();
        #endif

        /* IoT device Initialization */
        #if (IOT_DEVICE_DATA_TX_EN == 1) || (IOT_DEVICE_DATA_RX_EN == 1)
        Iot_Data_Init();
        #endif

        /* Power saving settings */
        if (tSysMode.ubSysMode == MW_FIM_SYS_MODE_USER)
            ps_smart_sleep(tPowerSaving.ubPowerSaving);
        
        /* RF Power settings */
        BleWifi_RFPowerSetting(tPowerSaving.ubRFPower);
    }

    // update the system mode
    BleWifi_Ctrl_SysModeSet(tSysMode.ubSysMode);
		
		// create app_entry thread.
    tThreadDef.name = "APP_Entry";
    tThreadDef.pthread = App_Entry;
    tThreadDef.tpriority = OS_TASK_PRIORITY_APP;        // osPriorityNormal
    tThreadDef.instances = 0;                           // reserved, it is no used
    tThreadDef.stacksize = 2*OS_TASK_STACK_SIZE_APP;      // (512), unit: 4-byte, the size is 512*4 bytes
    mqttClientThreadId = osThreadCreate(&tThreadDef, NULL);
    if (mqttClientThreadId == NULL)
    {
        printf("To create the thread for Thread mqtt client is fail.\n");
    }

    // add app cmd
    app_at_cmd_add();
}
