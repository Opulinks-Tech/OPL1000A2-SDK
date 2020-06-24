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
 *  @file at_cmd_rf_patch.h
 *
 *  @brief
 *
 ******************************************************************************/

#ifndef _AT_CMD_RF_PATCH_H_
#define _AT_CMD_RF_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "at_cmd_rf.h"
#include "controller_wifi.h"
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
typedef enum
{
//    RF_EVT_MODE = 0,
//    RF_EVT_GO,
//    RF_EVT_CHANNEL,
//    RF_EVT_RX_RESET_CNTS,
//    RF_EVT_RX_CNTS,
//    RF_EVT_WIFI_RX,
//    RF_EVT_WIFI_TX,
//
//    RF_EVT_BLE_DTM,
//    RF_EVT_BLE_ADV,
//
//    RF_EVT_SHOW_SCA,
//    RF_EVT_SET_SCA,
//    RF_EVT_CAL_VCO,
//    RF_EVT_TEST_MODE,
//    RF_EVT_PHY_DUMP,
//
//    RF_EVT_RELOAD_OW,
//    RF_EVT_STORE_OW,
//    RF_EVT_SHOW_OW,
//    RF_EVT_ADD_OW,
//    RF_EVT_DEL_OW,
//
//    RF_EVT_IPC_ENABLE,
//
//    RF_EVT_MAX
    RF_EVT_COME = RF_EVT_MAX + 1,
    RF_EVT_DCOC,
    RF_EVT_DCTH
} T_RfCmdEvtType_Ext;


typedef struct
{
    int16_t     s16aFreqOfs[4]; // [0] Max, [1] Min, [2] Avg, [3] Current value
    uint8_t     u8aTa[MAC_ADDR_LEN];    // Target address
    uint16_t    u16Rssi256;     // RSSI * 256
    uint32_t    u32RxTtl;       // Received total count
    uint8_t     bRxTaMatchEn; // Whether enable TA match filter
} S_TRX_DBG_EXT;

typedef struct
{
    uint8_t u8DcBeforeDcoc_I;
    uint8_t u8DcBeforeDcoc_Q;

    uint8_t u8DcAfterDcoc_I;
    uint8_t u8DcAfterDcoc_Q;

    uint32_t u32DrssiBeforeDcoc;
    uint32_t u32DrssiAfterDcoc[4];

    uint8_t u8DcocDone;
    uint8_t u8Rsv[3];
}S_Rf_DcocDbg_t;

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern volatile uint8_t g_u8RfCmdRun;
extern T_RfCmd g_tRfCmd;


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
int at_cmd_rf_come(char *buf, int len, int mode);
int at_cmd_rf_dcoc(char *buf, int len, int mode);
int at_cmd_rf_dcth(char *buf, int len, int mode);

#ifdef __cplusplus
}
#endif
#endif  /* _AT_CMD_RF_PATCH_H_ */
