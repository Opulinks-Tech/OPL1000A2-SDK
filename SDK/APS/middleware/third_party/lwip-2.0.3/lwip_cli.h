/**
 * @file lwip_cli.h
 *
 *  lwip_cli command
 *
 */

#ifndef __LWIP_CLI_H__
#define __LWIP_CLI_H__

/**
 * @ingroup LwIP
 * @addtogroup CLI
 * @{
 */
#include <stdint.h>
#include "cli.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * LwIP CLI declaration.
 *
 * Provides commands to debug
 */
extern cli_command_t lwip_cli[];


typedef int (*lwip_cli_handler_fp_t)(int token_cnt, char *param[]);

extern lwip_cli_handler_fp_t lwip_cli_handler;

#ifdef __cplusplus
}
#endif


/** }@ */


#endif /* __LWIP_CLI_H__ */


