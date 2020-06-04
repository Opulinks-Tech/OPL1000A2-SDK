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
 *  @file ipc_patch.h
 *
 *  @brief
 *
 ******************************************************************************/

#ifndef _IPC_PATCH_H_
#define _IPC_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "ipc.h"
#include "at_cmd_rf_patch.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define IPC_SPARE0_LOAD_PATCH_READY  (1 << 3)
#define IPC_SPARE0_MSQ_CLOCK_READY   (1 << 4)
#define IPC_SPARE0_DEEP_SLEEP_EN     (1 << 5)
#define IPC_SPARE0_MSQ_FLASH_READY   (1 << 6)
#define IPC_SPARE0_APS_CLOCK_READY   (1 << 7)
#define IPC_SPARE0_SEQ_32K_SRC_SEL   (1 << 24)
#define IPC_SPARE0_AUXADC_M0_ACT     (1 << 30)

#define IPC_DBG_TRX_PARAM_LEN_EXT    sizeof(S_TRX_DBG_EXT)

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
extern uint32_t g_u32IpcBleQueueCntAddr;
extern uint32_t g_u32IpcSleepIoCfgAddr;

extern uint32_t g_u32IpcMsqRxDataLen;
extern uint32_t g_u32IpcApsRxDataLen;

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
void Ipc_PreInit_patch(void);

#ifdef __cplusplus
}
#endif
#endif  /* _IPC_PATCH_H_ */
