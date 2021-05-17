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

#define IPC_DBG_TRX_PARAM_LEN_EXT    sizeof(S_TRX_DBG_EXT)

#define IPC_RB_WRITE_GET(rb, buf, idx) \
{ \
    buf = rb->pdwaBuf[(*(rb->pdwWrite) + idx) & rb->dwMask]; \
}

#define IPC_RB_RESERVE_DONE_BY_NUM(rb, num) \
{ \
    *(rb->pdwWrite) += num; \
}

#define IPC_RB_PROCESS_DONE_BY_NUM(rb, num) \
{ \
    *(rb->pdwRead) += num; \
}

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[32];
} T_IpcWifiApsTxRb;


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

uint32_t ipc_aps_tx_rb_num_get(void);

#ifdef IPC_MSQ
void *ipc_aps_tx_read_buf_get_by_idx(uint32_t u32Idx);
int ipc_aps_tx_read_buf_get_done(uint32_t u32Num);
#else
void *ipc_aps_tx_write_buf_get_by_idx(uint32_t u32Idx);
int ipc_aps_tx_write_buf_get_done(uint32_t u32Num);

extern T_IpcCommonFp ipc_lock;
extern T_IpcCommonFp ipc_unlock;
#endif


#ifdef __cplusplus
}
#endif
#endif  /* _IPC_PATCH_H_ */
