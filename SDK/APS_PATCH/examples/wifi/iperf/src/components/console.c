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

#include <stdlib.h>
#include "console.h"

typedef E_CLI_CMD_PROC (*T_Cli_UserCmdProcess_fp)(char *pbuf, int len);
typedef void (*Diag_UserCmdSet_fp_t)(T_Cli_UserCmdProcess_fp ptUserCmdFunc);
extern Diag_UserCmdSet_fp_t Diag_UserCmdSet;

int console_cmd_register(const void *cmd)
{
    Diag_UserCmdSet((T_Cli_UserCmdProcess_fp)cmd);
    return 0;
}
