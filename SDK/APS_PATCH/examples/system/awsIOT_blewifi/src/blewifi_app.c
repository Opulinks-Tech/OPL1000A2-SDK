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
#include "blewifi_configuration.h"
#include "blewifi_common.h"
#include "blewifi_app.h"
#include "blewifi_wifi_api.h"
#include "blewifi_ble_api.h"
#include "blewifi_ctrl.h"
#include "blewifi_ctrl_http_ota.h"
#include "iot_data.h"
#include "sys_common_api.h"
#include "ps_public.h"
#include "mw_fim_default_group03.h"
#include "mw_fim_default_group03_patch.h"
#include "mw_fim_default_group08.h"
#include "mw_fim_default_group08_project.h"
#include "mw_fim_default_group11_project.h"
#include "app_at_cmd.h"
#include "hal_dbg_uart.h"
#include "FreeRTOS.h"

#include "aws_system_init.h"
#include "aws_dev_mode_key_provisioning.h"


blewifi_ota_t *gTheOta = 0;
T_MwFim_GP08_AWS_PKCS11_KEYS tGP08_AWSPKCSKeys;
T_MwFim_GP11_AWS_PKCS11_KEYS tGP11_AWSPKCSKeys;
T_AWS_PKCS11_KEYS tAWSPKCSKeys;

extern void vStartSubpubDemoTasks( void ); 

void BleWifiAppInit(void)
{
    T_MwFim_SysMode tSysMode;
    T_MwFim_GP11_PowerSaving tPowerSaving;
    
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
	if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP11_PROJECT_POWER_SAVING, 0, MW_FIM_GP11_POWER_SAVING_SIZE, (uint8_t*)&tPowerSaving))
    {
        // if fail, get the default value
        memcpy(&tPowerSaving, &g_tMwFimDefaultGp11PowerSaving, MW_FIM_GP11_POWER_SAVING_SIZE);
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

        // move the settings to blewifi_ctrl, when the sys status is changed from Init to Noraml
        /* Power saving settings */
        //if (tSysMode.ubSysMode == MW_FIM_SYS_MODE_USER)
        //    ps_smart_sleep(tPowerSaving.ubPowerSaving);
        
        /* RF Power settings */
        BleWifi_RFPowerSetting(tPowerSaving.ubRFPower);
    }

    // update the system mode
    BleWifi_Ctrl_SysModeSet(tSysMode.ubSysMode);

    // add app cmd
		app_at_cmd_add();
			
	
		// get the AWS PKCS11 Keys from FIM group8 and group11 
	  if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP08_PROJECT_AWS_PKCS11_KEY, 0, MW_FIM_GP08_AWS_PKCS11_KEY_SIZE, (uint8_t*)&tGP08_AWSPKCSKeys))
    {
        // if fail, get the default value, copy tGP08_AWSPKCSKeys content to tAWSPKCSKeys element {P11_Key}
        memcpy(&tAWSPKCSKeys.P11_Key, &g_tMwFimDefaultGp08AWSPKCS11Keys.P11_Key, P11_KEY_SIZE);
    }
	  if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP11_PROJECT_AWS_PKCS11_KEYS, 0, MW_FIM_GP11_AWS_PKCS11_KEYS_SIZE, (uint8_t*)&tGP11_AWSPKCSKeys))
    {
        // if fail,  get the default value, copy tGP11_AWSPKCSKeys content to tAWSPKCSKeys elements {P11_Certificate,P11_CodeSignKey}
        memcpy(&tAWSPKCSKeys.P11_Certificate, &g_tMwFimDefaultGp11AwsPkcs11Keys.P11_Certificate, P11_CERTIFICATE_SIZE);
			  memcpy(&tAWSPKCSKeys.P11_CodeSignKey, &g_tMwFimDefaultGp11AwsPkcs11Keys.P11_CodeSignKey, P11_CODESIGNKEY);			
    }
    
    // Initial AWS task
    if( AWS_SYSTEM_Init() == pdPASS )
    {
        printf("===== AWS_SYSTEM_Init is OK ===========\n");
        vDevModeKeyProvisioning();
        vStartSubpubDemoTasks();
        //vStartMQTTEchoDemo();
       // vStartShadowDemoTasks();

    }
	
}
