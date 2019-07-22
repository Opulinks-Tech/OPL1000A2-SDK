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

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "at_cmd.h"
#include "at_cmd_common.h"
#include "at_cmd_data_process.h"
#include "sys_common_api.h"
#include "wifi_types.h"
#include "wifi_api.h"

int at_cmd_tcp_dhcp_arp_check(char *buf, int len, int mode)
{
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    u8 ret_st = 0;
    u8 arp_mode;
    
    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    switch(mode)
    {
        case AT_CMD_MODE_READ:
            tcp_get_config_dhcp_arp_check(&arp_mode);
            msg_print_uart1("\r\n+DHCPARPCHK:%d\r\n", arp_mode);
            
            ret_st = 1;
            break;
        case AT_CMD_MODE_SET:
            if (argc != 2) {
                goto done;
            }
            
            arp_mode = atoi(argv[1]);
            if (arp_mode > 1) {
                goto done;
            }
            
            if (tcp_set_config_dhcp_arp_check(arp_mode) != 0) {
                goto done;
            }
            
            ret_st = 1;
            
            break;
        default:
            break;
    }

done:
    if (ret_st)
        msg_print_uart1("\r\nOK\r\n");
    else 
        msg_print_uart1("\r\nError\r\n");
    
    return ret_st;
}

int at_cmd_mac_data_rate(char *buf, int len, int mode)
{
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    u8 ret_st = 0;
    u8 data_rate;
    
    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    switch(mode)
    {
        case AT_CMD_MODE_READ:
            wifi_config_get_mac_tx_data_rate((wifi_mac_data_rate_t *)&data_rate);
            msg_print_uart1("\r\n+MACDATARATE:%d\r\n", data_rate);
            ret_st = 1;
            break;
        case AT_CMD_MODE_SET:
            if (argc != 2) {
                goto done;
            }

            data_rate = atoi(argv[1]);
            if (data_rate > WIFI_MAC_DATA_RATE_11M) {
                goto done;
            }

            if (wifi_config_set_mac_tx_data_rate((wifi_mac_data_rate_t)data_rate) != 0) {
                goto done;
            }
            
            ret_st = 1;
            break;
        default:
            break;
    }

done:
    if (ret_st)
        msg_print_uart1("\r\nOK\r\n");
    else 
        msg_print_uart1("\r\nError\r\n");
    
    return ret_st;
}

/*
 * @brief Global variable g_AtCmdTbl_property_Ptr retention attribute segment
 *
 */
RET_DATA at_command_t *g_AtCmdTbl_property_Ptr;

/**
  * @brief extern AT Command Table for All Module
  *
  */
at_command_t gAtCmdTbl_property[] =
{
    { "at+dhcparpchk",          at_cmd_tcp_dhcp_arp_check,  "Enable/Disable DHCP ARP check mechanism"},
    { "at+macdatarate",         at_cmd_mac_data_rate,       "Control Wifi Mac Tx data rate"},
    { NULL,                     NULL,                     NULL},
};

/*
 * @brief AT Command Interface Initialization for Property modules
 *
 */
void at_cmd_property_func_init(void)
{
    g_AtCmdTbl_property_Ptr = gAtCmdTbl_property;
}
