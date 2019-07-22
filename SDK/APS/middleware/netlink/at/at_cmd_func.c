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
/**
 * @file at_cmd_func.c
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File contains the AT Command api interface initialization.
 *
 */

#include "at_cmd.h"
#include "at_cmd_common.h"
#include "at_cmd_task.h"
#include "at_cmd_msg_ext.h"
#include "at_cmd_data_process.h"
#include "at_cmd_app.h"

/*
 * @brief AT Module API Interface Initialization
 *
 */
void at_func_init(void)
{
    /** AT Cmd Architecture */
    at_cmd_func_init();

    /** AT Cmd Common */
    at_cmd_common_func_init();

    /** AT Cmd Task */
    at_task_func_init();

    /** AT Msg Extend */
    at_msg_ext_init();

    /** AT Data Process */
    data_process_func_init();

    at_cmd_app_func_preinit();
}

