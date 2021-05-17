/* *****************************************************************************
 *  Copyright 2019, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *
 *******************************************************************************
 *
 *  @file wpa_cli_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _WPA_CLI_PATCH_H_
#define _WPA_CLI_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "wpa_cli.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */



/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
void wpa_cli_func_init_patch(void);
int wpa_cli_cmd_reset(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif
#endif  /* _WPA_CLI_PATCH_H_ */
