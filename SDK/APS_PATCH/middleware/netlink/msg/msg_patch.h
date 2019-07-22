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
 *  @file msg_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _MSG_PATCH_H_
#define _MSG_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "msg.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

/* Wrapper for A1 patch */
#define T_TracerTaskInfoExt T_TracerTaskInfo
#define tracer_log_level_set_ext tracer_log_level_set

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


#ifdef __cplusplus
}
#endif
#endif  /* _MSG_PATCH_H_ */
