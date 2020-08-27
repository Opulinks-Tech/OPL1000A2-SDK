/* *****************************************************************************
 *  Copyright 2019, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2019
 *
 *******************************************************************************
 *
 *  @file at_cmd_ext_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _AT_CMD_EXT_PATCH_H_
#define _AT_CMD_EXT_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "at_cmd_wifi.h"
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
#if !defined(SDK_LITE)
extern at_command_t gAtCmdTbl_Ext[];
#endif /* !defined(SDK_LITE) */
/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

#ifdef __cplusplus
}
#endif
#endif  /* _AT_CMD_EXT_PATCH_H_ */
