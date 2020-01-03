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
 *  @file controller_task_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _CONTROLLER_TASK_PATCH_H_
#define _CONTROLLER_TASK_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "controller_task.h"
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
typedef struct
{
    int8_t Counter_From_IPC_To_Ctrl_PUT;
    int8_t Counter_From_IPC_To_Ctrl_GET;
    int8_t Counter_From_Ctrl_To_Host_PUT;
    int8_t Counter_From_Ctrl_To_Host_GET;

} CTRL_BleQueueCounter_t;


/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern CTRL_BleQueueCounter_t *le_ctrl_queue_ctr;


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
void controller_task_func_init_patch(void);

#ifdef __cplusplus
}
#endif
#endif  /* _CONTROLLER_TASK_PATCH_H_ */
