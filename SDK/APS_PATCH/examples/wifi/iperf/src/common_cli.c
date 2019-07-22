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

#include "cmsis_os.h"
#include "common_cli.h"
#include "hal_system.h"
#include "ping_cli.h"
#include "opulinks_log.h"
#include "svn_rev.h"

static const char *TAG="cmd_common";

int common_cmd_version(int argc, char **argv)
{
    LOGI_DRCT(TAG, "SDK version info: %d", SVN_REVISION);
    LOGI_DRCT(TAG, "Compile time:     %s", SVN_TIME_NOW);
    return 0;
}

int common_cmd_reset(int argc, char **argv)
{
    osDelay(1000);

    // reset all
    Hal_Sys_SwResetAll();
    
    return 0;
}

int common_cmd_ping(int argc, char **argv)
{
    if (argc > 1) {
        ping_cli_handler((argc - 1), &argv[1]);
    }
    
    return 0;
}
