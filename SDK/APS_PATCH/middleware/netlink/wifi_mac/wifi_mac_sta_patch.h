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
 *  @file wifi_mac_sta_patch.h
 *
 *  @brief
 *
 ******************************************************************************/

#ifndef _WIFI_MAC_STA_PATCH_H_
#define _WIFI_MAC_STA_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "wifi_mac_sta.h"
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
typedef struct {
    u8      au8Dot11MACAddress[MAC_ADDR_LEN];
    u8      u8Dot11ShortRetryLimit;
    u8      u8Dot11LongRetryLimit;
    u8      u8CwExpMin;
    u8      u8CwExpMax;
    // 11th
    u8      u8SilenceThr;
    u8      u8KeepConnectThr;
    u16     u16TimeUnit;
    u8      u8Sifs;
    u8      u8Difs;
    u16     u16Eifs;
    u8      u8Slot;
    u8      u8MaEnLen;
    // 21st
    u8      u8TrxEnLen;
    u8      u8PhyMode;
    u8      u8ConsSuccThr;      // The threshold to increase data rate when u8ConsSuccCnt of bss_info reaches
    u8      u8ConsFailThr;      // The threshold to decrease data rate when u8ConsFailCnt of bss_info reaches
    u16     u16RtsThr;          // The threshold to exchange RTS/CTS when packet size is larger
    u8      u8RetryThr1;        // The threshold to exchange RTS/CTS when u8RetryCnt of g_DcfInfo reaches
    u8      u8RetryThr2;        // The threshold to decrease current rate when u8RetryCnt of g_DcfInfo is the power of
    uint16_t    u16ListenInv;
    // 31st
    uint16_t    u16PrbRsqTimeout;
    uint16_t    u16AuthRsqTimeout;
    uint16_t    u16AssocRsqTimeout;
    uint16_t    u16ScrtConnTimeout;
    uint8_t     u8PrbRsqReTtlCnt;
    uint8_t     u8AuthRsqReTtlCnt;
    // 41st
    uint8_t     u8AssocRsqReTtlCnt;
    uint8_t     u8ScrtConnReTtlCnt;
    uint8_t     u8SkipDtimPeriods;
    uint8_t     u8Reserved;
    uint16_t    u16FragmentThr;
    uint16_t    u16FragmentLifeTime;    // Unit: ms
    // N bytes

    u8      reserved[208];   // 256 - N bytes
} S_WIFI_STAINFO_EXT;



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
#endif  /* _WIFI_MAC_STA_PATCH_H_ */
