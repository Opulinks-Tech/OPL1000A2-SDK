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
 *  @file data_flow_patch.h
 *
 *  @brief
 *
 ******************************************************************************/

#ifndef _DATA_FLOW_PATCH_H_
#define _DATA_FLOW_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "data_flow.h"
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
int ipc_rf_at_process_M0ToM3_patch(void* pdata);
int ipc_m0_msg_handle_patch(uint32_t dwType, uint32_t dwIdx, void *pBuf, uint32_t dwBufSize);

#ifdef __cplusplus
}
#endif
#endif  /* _DATA_FLOW_PATCH_H_ */
