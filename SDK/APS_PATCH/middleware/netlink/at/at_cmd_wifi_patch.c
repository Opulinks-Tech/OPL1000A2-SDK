/******************************************************************************
*  Copyright 2019, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2019
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "at_cmd.h"
#include "at_cmd_common.h"
#include "at_cmd_data_process.h"
#include "at_cmd_nvm.h"
#include "controller_wifi_com.h"
#include "wpa_at_if.h"
#include "wpa_cli.h"
#include "driver_netlink.h"

#include "at_cmd_wifi_patch.h"
#include "at_cmd_app_patch.h"
#include "controller_wifi_com_patch.h"
#include "at_cmd_task_patch.h"

#define AT_SCAN_PASSIVE_INT_DEF 150 //ms
#define AT_BLE_WIFI_MODE 4

#ifdef AT_CMD_WIFI_DBG
    #define AT_LOG                  printf
#else
    #define AT_LOG(...)
#endif

extern at_command_t *g_AtCmdTbl_Wifi_Ptr;
extern int g_wifi_argc;
extern char *g_wifi_argv[AT_MAX_CMD_ARGS];
extern uint8_t g_wifi_init_mode;

S_WIFI_MLME_SCAN_CFG gATScanCfg = {0};
at_lap_opt_t gATLapOpt = {0};

extern int at_cmd_wifi_cwmode(char *buf, int len, int mode);

/*
 * @brief Command at+cwmode
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_wifi_cwmode_patch(char *buf, int len, int mode)
{
    int cwmode;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    int argc = 0;
    uint8_t ret = false;
    static uint8_t pre_wifi_mode = 0;
    
    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    switch(mode)
    {
        case AT_CMD_MODE_READ:
            msg_print_uart1("\r\n+CWMODE:%d\r\n", g_wifi_init_mode);
            ret = true;
            break;

        case AT_CMD_MODE_EXECUTION:
            //Do nothing
            break;

        case AT_CMD_MODE_SET:
            if(argc > 1) {
                if (at_cmd_get_para_as_digital(argv[1], &cwmode) != 0) {
                    goto exit;
                }
                
                switch (cwmode)
                {
                    case 0:
                    {
                        if (g_wifi_init_mode == 0) {
                            ret = true;
                            goto exit;
                        }
                        
                        if (at_cmd_nvm_cw_ble_wifi_mode_set((uint8_t *)&cwmode))
                            goto exit;
                        
                        //Keep previous mode 
                        pre_wifi_mode = g_wifi_init_mode;
                        
                        g_wifi_init_mode = 0;
                        
                        ret = true;
                    }
                        break;

                    case 1:
                        if ((g_wifi_init_mode == AT_BLE_WIFI_MODE))
                            goto exit;
                        
                        //wpa_set_wpa_mode(WPA_MODE_STA);
                        g_wifi_init_mode = WPA_MODE_STA;
                        
                        //Initialize AT task (TCPIP data task, event loop task)
                        at_wifi_net_task_init();
                        
                        at_cmd_crlf_term_set(1); // Enable CR-LF termination for WiFi AT commands
                        
                        ret = true;
                        break;
                        
                    case 4:
                    {
                        if (g_wifi_init_mode == AT_BLE_WIFI_MODE) {
                            ret = true;
                            goto exit;
                        }
                            
                        if (g_wifi_init_mode == WPA_MODE_STA)
                            goto exit;
                        
                        uint8_t enable = 1;
                        if (at_cmd_nvm_cw_ble_wifi_mode_set(&enable))
                            goto exit;
                        
                        g_wifi_init_mode = AT_BLE_WIFI_MODE;
                        
                        //Initialize BleWifi task
                        if (pre_wifi_mode != AT_BLE_WIFI_MODE) {
                            at_blewifi_init();
                            at_cmd_crlf_term_set(1); // Enable CR-LF termination for WiFi AT commands
                        }

                        ret = true;
                    }
                        break;
                    default:
                        break;
                }
            }
            break;

        case AT_CMD_MODE_TESTING:
            msg_print_uart1("\r\n+CWMODE:%d\r\n", g_wifi_init_mode);
            ret = true;
            break;

        default:
            break;
    }

exit:
    if (ret == true) {
        msg_print_uart1("\r\nOK\r\n");
    }
    else 
        msg_print_uart1("\r\nERROR\r\n");
    
    return ret;
}

/*
 * @brief Command at+cwlapopt
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
static int at_cmd_wifi_cwlapopt_patch(char *buf, int len, int mode)
{
    char *argv[AT_MAX_CMD_ARGS] = {0};
    int argc = 0;
    int sort_en, mask, times, probe_counters;
    int iRet = 0;

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    switch(mode)
    {
        case AT_CMD_MODE_READ:
            at_output("\r\n+CWLAPOPT:%d,%d,%d,%d\r\n", 
                      gATLapOpt.sort_en, gATLapOpt.mask, gATLapOpt.times,
                      gATLapOpt.probe_counters);
            iRet = 1;
            break;

        case AT_CMD_MODE_EXECUTION:
        case AT_CMD_MODE_TESTING:
            break;

        case AT_CMD_MODE_SET:
            if (argc >= 2) {
                /* sorting */
                if (at_cmd_get_para_as_digital(argv[1], &sort_en) != 0) {
                    goto done;
                }
                
                if (sort_en > 1 || sort_en < 0) {
                    AT_LOG("\r\n invalid para: <sort_enable> should be 0 or 1 \r\n");
                    goto done;
                }
                
                gATLapOpt.sort_en = sort_en;
                
                /* display mask bit */
                if (argc >= 3) {
                    if (at_cmd_get_para_as_digital(argv[2], &mask) != 0) {
                        goto done;
                    }
                    
                    if (mask > 31 || mask < 0) {
                        AT_LOG("\r\n invalid para: <mask> should be 0~31 \r\n");
                        goto done;
                    }
                    
                    gATLapOpt.mask = mask;
                }
                
                set_sorting(gATLapOpt.sort_en, gATLapOpt.mask);
            }

            /* scan duration */
            if (argc >= 4) {
                if (at_cmd_get_para_as_digital(argv[3], &times) != 0) {
                    goto done;
                }
                
                if (times > 10 || times < 1) {
                    AT_LOG("\r\n invalid para: <times> should be 1~10 \r\n");
                    goto done;
                }
                
                gATLapOpt.times = times;
                gATScanCfg.u32PassiveScanDur = AT_SCAN_PASSIVE_INT_DEF * times;
            }
            
            /* probe request counter */
            if (argc >= 5) {
                if (at_cmd_get_para_as_digital(argv[4], &probe_counters) != 0) {
                    goto done;
                }
                
                if (probe_counters > 5 || probe_counters < 1) {
                    AT_LOG("\r\n invalid para: <probe_counters> should 1 ~ 5\r\n");
                    goto done;
                }
                else if (probe_counters > 1) {
                    gATScanCfg.u32PassiveScanDur = gATScanCfg.u32PassiveScanDur / probe_counters;
                    if (gATScanCfg.u32PassiveScanDur < SCAN_MIN_DURATION_TIME) {
                        gATScanCfg.u32PassiveScanDur = SCAN_MIN_DURATION_TIME;
                    }
                }
                
                gATLapOpt.probe_counters = probe_counters;
                gATScanCfg.u8ResendCnt = gATLapOpt.probe_counters;
            }
            
            iRet = 1;
        default:
            break;
    }

done:
    if(iRet)
    {
        at_output("\r\nOK\r\n");
    }
    else
    {
        at_output("\r\nERROR\r\n");
    }
    
    return iRet;
}

/*
 * @brief Command at+cwlap
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
static int at_cmd_wifi_cwlap_patch(char *buf, int len, int mode)
{
    char *argv[AT_MAX_CMD_ARGS] = {0};
    int argc = 0, iRet = 0;
    uint8_t ssid_len = 0;
    uint8_t mac[MAC_ADDR_LEN] = {0};
    int ch;
    char *pstr;
    S_WIFI_MLME_SCAN_CFG scan_cfg;

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    g_wifi_argc = 1;
    
    memcpy(&scan_cfg, &gATScanCfg, sizeof(S_WIFI_MLME_SCAN_CFG));

    switch(mode)
    {
        case AT_CMD_MODE_EXECUTION:
        case AT_CMD_MODE_READ:
        case AT_CMD_MODE_TESTING:
            break;
        
        case AT_CMD_MODE_SET:
            if (argc >= 2) { //ssid
                if (strlen(argv[1]) != 0) {
                    if (strlen(argv[1]) > MAX_LEN_OF_SSID) {
                        goto done;
                    }
                    
                    ssid_len = strlen(argv[1]);
                    if (ssid_len > MAX_LEN_OF_SSID)
                        ssid_len = MAX_LEN_OF_SSID;
                    memcpy(&scan_cfg.u8aSsid[0], argv[1], ssid_len);
                }
            }
            
            if (argc >= 3) { //mac address
                if (strlen(argv[2]) != 0) {
                    pstr = at_cmd_param_trim(argv[2]);
                    
                    if (!pstr)
                    {
                        goto done;
                    }
                    
                    if (hwaddr_aton2(pstr, mac) == -1) {
                        goto done;
                    }
                
                    if (is_broadcast_ether_addr(mac)) {
                        goto done;
                    }
                    
                    if (is_multicast_ether_addr(mac)) {
                        goto done;
                    }

                    if (is_zero_ether_addr(mac)) {
                        goto done;
                    }
                    
                    memcpy(&scan_cfg.u8aBssid, &mac[0], MAC_ADDR_LEN);
                }
            }
            
            if (argc >= 4) { //channel
                if (at_cmd_get_para_as_digital(argv[3], &ch) != 0) {
                    goto done;
                }
                
                if (ch < 1 || ch > 14) {
                    goto done;
                }
                
                scan_cfg.u8Channel = ch;
            }
            break;
            
        default:
            break;
    }

    if (!wpa_cli_scan_by_cfg(&scan_cfg)) {
        //Error
        goto done;
    }
    else {
        iRet = 1;
    }

done:
    if(!iRet)
    {
        g_wifi_argc = 0;
        at_output("\r\nERROR\r\n");
    }

    return iRet;
}

void at_cmd_wifi_func_init_patch(void)
{
    /* Initialize the configuration. */
    gATScanCfg.tScanType = WIFI_MLME_SCAN_TYPE_MIX;
    gATScanCfg.u32ActiveScanDur = 100;
    gATScanCfg.u32PassiveScanDur = 150;
    gATScanCfg.u8Channel = 0;
    memset(gATScanCfg.u8aSsid, 0, MAX_LEN_OF_SSID + 1);
    memset(gATScanCfg.u8aBssid, 0, MAC_ADDR_LEN);
    
    gATLapOpt.mask = 31;
    gATLapOpt.times = 1;
    gATLapOpt.probe_counters = 1;
    
    g_AtCmdTbl_Wifi_Ptr[0].cmd_handle = at_cmd_wifi_cwmode_patch;
    g_AtCmdTbl_Wifi_Ptr[2].cmd_handle = at_cmd_wifi_cwlapopt_patch;
    g_AtCmdTbl_Wifi_Ptr[3].cmd_handle = at_cmd_wifi_cwlap_patch;
}
