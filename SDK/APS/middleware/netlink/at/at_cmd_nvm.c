/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#include "string.h"
#include "at_cmd_nvm.h"
#include "mw_fim_default_group02.h"

/******************************************************
 *                      Macros
 ******************************************************/
 
 
 /******************************************************
 *                    Constants
 ******************************************************/


/******************************************************
 *                   Enumerations
 ******************************************************/


/******************************************************
 *                 Type Definitions
 ******************************************************/
 
 
 /******************************************************
 *                    Structures
 ******************************************************/
 

/******************************************************
 *               Static Function Declarations
 ******************************************************/


/******************************************************
 *               Variable Definitions
 ******************************************************/
RET_DATA at_cmd_nvm_trans_config_get_fp_t   at_cmd_nvm_trans_config_get;
RET_DATA at_cmd_nvm_trans_config_set_fp_t   at_cmd_nvm_trans_config_set;
RET_DATA at_cmd_nvm_cw_ble_wifi_mode_get_fp_t at_cmd_nvm_cw_ble_wifi_mode_get;
RET_DATA at_cmd_nvm_cw_ble_wifi_mode_set_fp_t at_cmd_nvm_cw_ble_wifi_mode_set;

 /******************************************************
 *               Function Definitions
 ******************************************************/
int at_cmd_nvm_trans_config_get_impl(at_nvm_trans_config_t *cfg)
{
   if (cfg == NULL) {
       return -1;
   }

   if (MwFim_FileRead(MW_FIM_IDX_AT_TCPIP_TRANS_CFG, 0, MW_FIM_AT_TCPIP_TRANS_CFG_SIZE, (uint8_t *)cfg) != MW_FIM_OK) {
       memset(&cfg[0], 0, sizeof(at_nvm_trans_config_t));
       return -1;
   }

   return 0;
}

int at_cmd_nvm_trans_config_set_impl(at_nvm_trans_config_t *cfg)
{
   if (cfg == NULL) {
       return -1;
   }
    
   if (MwFim_FileWrite(MW_FIM_IDX_AT_TCPIP_TRANS_CFG, 0, MW_FIM_AT_TCPIP_TRANS_CFG_SIZE, (uint8_t *)cfg) != MW_FIM_OK) {
       return -1;
   }
   
   return 0;
}

int at_cmd_nvm_cw_ble_wifi_mode_get_impl(uint8_t *enable)
{
    if (enable == NULL) {
        return -1;
    }
    
   if (MwFim_FileRead(MW_FIM_IDX_AT_BLE_WIFI, 0, MW_FIM_AT_BLE_WIFI_SIZE, (uint8_t *)enable) != MW_FIM_OK) {
       *enable = 0;
       return -1;
   }
   
    return 0;
}

int at_cmd_nvm_cw_ble_wifi_mode_set_impl(uint8_t *enable)
{
    if (enable == NULL) {
        return -1;
    }
    
   if (MwFim_FileWrite(MW_FIM_IDX_AT_BLE_WIFI, 0, MW_FIM_AT_BLE_WIFI_SIZE, enable) != MW_FIM_OK) {
       return -1;
   }
   
    return 0;
}

void at_cmd_nvm_func_init(void)
{
    at_cmd_nvm_trans_config_get     = at_cmd_nvm_trans_config_get_impl;
    at_cmd_nvm_trans_config_set     = at_cmd_nvm_trans_config_set_impl;
    at_cmd_nvm_cw_ble_wifi_mode_get = at_cmd_nvm_cw_ble_wifi_mode_get_impl;
    at_cmd_nvm_cw_ble_wifi_mode_set = at_cmd_nvm_cw_ble_wifi_mode_set_impl;
}
