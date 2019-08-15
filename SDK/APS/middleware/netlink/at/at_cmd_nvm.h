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

#ifndef _AT_CMD_NVM_H_
#define _AT_CMD_NVM_H_

#include <stdint.h>

typedef struct  {
    uint8_t  enable;             /* whether enter transmit transparently mode */
    uint8_t  link_id;
    uint8_t  link_type;
    uint8_t  change_mode;
    uint8_t  remote_ip[64];
    int32_t  remote_port;
    int32_t  local_port;
    uint16_t keep_alive;
    uint8_t  ssl_enable;
    uint8_t  reserved;
} at_nvm_trans_config_t;

void at_cmd_nvm_func_init(void);

typedef int (*at_cmd_nvm_trans_config_get_fp_t)(at_nvm_trans_config_t *cfg);
typedef int (*at_cmd_nvm_trans_config_set_fp_t)(at_nvm_trans_config_t *cfg);
typedef int (*at_cmd_nvm_cw_ble_wifi_mode_get_fp_t)(uint8_t *enable);
typedef int (*at_cmd_nvm_cw_ble_wifi_mode_set_fp_t)(uint8_t *enable);

extern at_cmd_nvm_trans_config_get_fp_t at_cmd_nvm_trans_config_get;
extern at_cmd_nvm_trans_config_set_fp_t at_cmd_nvm_trans_config_set;
extern at_cmd_nvm_cw_ble_wifi_mode_get_fp_t at_cmd_nvm_cw_ble_wifi_mode_get;
extern at_cmd_nvm_cw_ble_wifi_mode_set_fp_t at_cmd_nvm_cw_ble_wifi_mode_set;

#endif /* _AT_CMD_NVM_H_ */
