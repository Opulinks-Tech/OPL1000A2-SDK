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
 *  @file at_cmd_data_process_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _AT_CMD_DATA_PROCESS_PATCH_H_
#define _AT_CMD_DATA_PROCESS_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "at_cmd_data_process.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define LOCK_APP        (LOCK_OTHERS << 1)
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
void data_process_lock_patch(int module, int data_len);
int data_process_handler_patch(char *pbuf, int len);

#ifdef __cplusplus
}
#endif
#endif  /* _AT_CMD_DATA_PROCESS_PATCH_H_ */
