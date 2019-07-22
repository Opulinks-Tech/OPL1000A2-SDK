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
 *  @file at_cmd_task_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _AT_CMD_TASK_PATCH_H_
#define _AT_CMD_TASK_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "at_cmd_task.h"
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
typedef void (*at_cmd_crlf_term_set_fp_t)(uint8_t u8Enable);
typedef uint8_t (*at_cmd_crlf_term_get_fp_t)(void);
typedef void (*at_term_char_remove_fp_t)(char *sBuf);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern at_cmd_crlf_term_set_fp_t at_cmd_crlf_term_set;
extern at_cmd_crlf_term_get_fp_t at_cmd_crlf_term_get;
extern at_term_char_remove_fp_t at_term_char_remove;
/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
void at_task_func_init_patch(void);

#ifdef __cplusplus
}
#endif
#endif  /* _AT_CMD_TASK_PATCH_H_ */
