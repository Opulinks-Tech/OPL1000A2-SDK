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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "cli.h"
#include "lwip_cli.h"
extern int ping_cli_handler(int len, char *param[]);
extern cli_command_t lwip_cli[];

static int sys_cmd_echo(int argc, char *argv[])
{
    int loop_i;

    printf("\r\n");
    for(loop_i=0; loop_i<argc; loop_i++) {
        printf("argv[%2d]: %s\n", loop_i, argv[loop_i]);
    }
    return true;
}

const cli_command_t _cli_cmd_tbl[] =
{
    { "echo",       "display echo ",        sys_cmd_echo,       NULL},
    //{ "iperf",      "iperf",                NULL        , iperf_cli },
    //{ "ping",       "ping cmd",             ping_cli_handler,   NULL},
    //{ "lwip",       "lwip network api",     lwip_cli_handler,   NULL},
    //{ "lwip_debug", "lwip debug info",      NULL,               lwip_cli},
    { NULL,          NULL,                  NULL,               NULL}
};
