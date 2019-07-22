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

#ifndef __CLI_H__
#define __CLI_H__
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CLI_MAX_CMD_ARGS            20   // The max number of arguments

/**
 * @brief function pointer type of CLI command handlers.
 *
 * All CLI handlers must follow this prototype.
 *
 * @note    Currently, the return value of CLI handlers are ignored. However,
 *          to keep backward compatiblity, CLI handlers must return 0.
 */
typedef int  (*cli_cmd_handler_t)(int argc, char *argv[]);


/**
* @brief struct cli_command_t - The data structure to describe the cli user commands.
*
*/
typedef struct cli_command {
	const char *cmd;
    const char *cmd_usage;
	cli_cmd_handler_t cmd_handle;
    struct cli_command *sub;       /**< Sub-commands. */
}cli_command_t;


/**
 * @brief The CLI control block definition.
 *
 */
typedef struct cli_cb {
    uint8_t         state;                  /**< Defines the init status. */
    cli_command_t   *cmd;                   /**< A pointer to the CLI commands. */
    char            *tok[CLI_MAX_CMD_ARGS]; /**< The tokenized input command. */
}cli_cb_t;



/* *****************************************************
 *               Function Declarations
 ******************************************************/

void nl_cli_init(cli_cb_t *cli);
void nl_cli_task(void);

#ifdef __cplusplus
}
#endif

#endif /* _NL_CLI_H_ */
