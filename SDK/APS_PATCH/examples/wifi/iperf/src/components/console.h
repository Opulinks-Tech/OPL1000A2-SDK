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

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "opulinks_log.h"

typedef enum 
{
    CMD_FINISHED=0,
    CMD_CONTINUE=1, 
}E_CLI_CMD_PROC;

/**
 * @brief Register console command
 * @param cmd pointer to the command description; can point to a temporary value
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_NO_MEM if out of memory
 */
int console_cmd_register(const void *cmd);

/**
 * @brief Console command main function
 * @param argc number of arguments
 * @param argv array with argc entries, each pointing to a zero-terminated string argument
 * @return console command return code, 0 indicates "success"
 */
typedef int (*console_cmd_func_t)(int argc, char** argv);

#endif /* _CONSOLE_H_ */
