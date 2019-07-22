/******************************************************************************
*  Copyright 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#include <string.h>

#include "console.h"
#include "common_cli.h"
#include "cmd_parser.h"
#include "iperf_cli.h"
#include "opulinks_log.h"
#include "wifi_cli.h"

static const char *TAG="cmd_pser";

static int commands_help(int argc, char** argv)
{
    LOGI_DRCT(TAG, "Wifi Part:");
    LOGI_DRCT(TAG, "  scan                            Scan AP");
    LOGI_DRCT(TAG, "  connect [SSID] [Passphrase]     Connect to specific AP");
    LOGI_DRCT(TAG, "  disconnect                      Disconnect the link with AP");
    LOGI_DRCT(TAG, "  query                           Query Wifi States");
    LOGI_DRCT(TAG, "  wifi_data_rate [TYPE]           Set Tx Data rate.");
    LOGI_DRCT(TAG, "        [TYPE]                    - 0:DTO, 1:1M, 2:2M, 3:5.5M, 4:11M");
    LOGI_DRCT(TAG, "  sta_mac                         Get current setting of STA mac address.");
    LOGI_DRCT(TAG, "  sta_mac set [MAC_ADDR]          Set STA mac address. Format likes XX:XX:XX:XX:XX:XX.");
    LOGI_DRCT(TAG, "                                  Please reboot after modified to effective.");
    LOGI_DRCT(TAG, "Iperf Part:");
    LOGI_DRCT(TAG, "  iperf -h                        Show iperf command help");
    LOGI_DRCT(TAG, "Common Part:");
    LOGI_DRCT(TAG, "  version                         Show current SDK version");
    LOGI_DRCT(TAG, "  reset                           Reboot device");
    LOGI_DRCT(TAG, "  ping s                          Stop ping command.");
    LOGI_DRCT(TAG, "  ping [IP] [Pkt counts] [Pkt len] [Recv timeout] [Ping period]");
    LOGI_DRCT(TAG, "                                  Start ping command. Default send 3 ICMP packets to target HOST.");
    LOGI_DRCT(TAG, "       [IP]                       - The target HOTS IP address");
    LOGI_DRCT(TAG, "       [Pkt counts]               - which total of packet numbers to be send.");
    LOGI_DRCT(TAG, "       [Pkt len]                  - which packet size per packet.");
    LOGI_DRCT(TAG, "       [Recv timeout]             - which timeout per packet.");
    LOGI_DRCT(TAG, "       [Ping period]              - which ping interval time.");
    
    return 0;
}

static int cmd_buf_to_arg(int *argc, char **argv, char *pbuf)
{
    int count = 0;
    char *p = strtok(pbuf, " ");
    
    argv[count] = p;
    count++;
    
    while ((p = strtok(NULL, " ")) != NULL) {
        argv[count] = p;
        count++;
    }
    
    *argc = count;
    return 0;
}

int cmd_parser_func(char *pbuf, int len)
{
    char *argv[10] = {0};
    int argc = 0;
    
    cmd_buf_to_arg(&argc, &argv[0], pbuf);
    
    tracer_drct_printf("\n");
    if (!strcmp(argv[0], "connect")) {
        wifi_cmd_sta(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "scan")) {
        wifi_cmd_scan(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "iperf")) {
        cmd_iperf(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "help")) {
        commands_help(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "query")) {
        wifi_cmd_query(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "disconnect")) {
        wifi_cmd_disconnect(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "wifi_data_rate")) {
        wifi_cmd_data_rate(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "ping")) {
        common_cmd_ping(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "sta_mac")) {
        wifi_cmd_sta_mac_addr(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "reset")) {
        common_cmd_reset(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "version")) {
        common_cmd_version(argc, argv);
        return CMD_FINISHED;
    }
    
    return CMD_CONTINUE;
}
