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
 *  @file wifi_mac_tx_data_patch.h
 *
 *  @brief
 *
 ******************************************************************************/

#ifndef _WIFI_MAC_TX_DATA_PATCH_H_
#define _WIFI_MAC_TX_DATA_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "wifi_mac_data.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define WIFI_MAC_LAST_FRG_ENCRPT_DATA_SIZE_MIN  28
#define WIFI_MAC_LAST_FRG_OPEN_DATA_SIZE_MIN    44

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
void wifi_mac_txdata_func_init_patch(void);
uint32_t wifi_mac_xmit_fragment(uint8_t *pu8Payload, uint32_t u32PayloadLen,
                                const uint8_t *pu8DestAddr, uint16_t u16ProtocalType, int32_t s32Encrypt);
int wifi_mac_xmit_eapol_frame_patch(u8 *buf, size_t len, const u8 *eth_dest, u16 proto);
u32 wifi_mac_xmit_frame_patch(u8 *tx_data, u32 tx_data_len);

#ifdef __cplusplus
}
#endif
#endif  /* _WIFI_MAC_TX_DATA_PATCH_H_ */
