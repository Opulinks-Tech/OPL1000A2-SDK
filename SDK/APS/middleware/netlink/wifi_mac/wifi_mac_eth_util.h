/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#ifndef WLAN_MAC_ETH_UTIL_H_
#define WLAN_MAC_ETH_UTIL_H_

#include "wifi_mac_common.h"
#include "common/if_ether.h"


#define ETH_TYPE_ARP                                       0x0608
#define ETH_TYPE_IP                                        0x0008
#define ETH_TYPE_IPV6                                      0xDD86

#define LLC_SNAP                                           0xAA
#define LLC_CNTRL_UNNUMBERED                               0x03
#define LLC_TYPE_ARP                                       0x0608
#define LLC_TYPE_IP                                        0x0008
#define LLC_TYPE_IPV6                                      0xDD86
#define LLC_TYPE_WLAN_LTG                                  0x9090              // Non-standard type for LTG packets

#define ETH_ADDR_SIZE                                      6                   // Length of Ethernet MAC address (in bytes)
#define IP_ADDR_SIZE                                       4                   // Length of IP address (in bytes)


typedef struct {
    u8  dest_mac_addr[ETH_ADDR_SIZE];                      // Destination MAC address
    u8  src_mac_addr[ETH_ADDR_SIZE];                       // Source MAC address
    u16 ethertype;                                         // EtherType , big-endian
} __attribute__((packed)) ethernet_header_t;

/* 802.2 LLC/SNAP header used by 802.11 per 802.1H */
#define DOT11_LLC_SNAP_HDR_LEN      8   /* LLC/SNAP header length */
#define DOT11_OUI_LEN               3   /* OUI length */

// LLC Header(3) + SNAP(5)
typedef struct{
    u8  dsap;          /* always 0xAA */
    u8  ssap;          /* always 0xAA */
    u8  control_field; /* always 0x03 */
    u8  org_code[3];   /* RFC1042: 0x00 0x00 0x00,  Bridge-Tunnel: 0x00 0x00 0xF8 */
    u16 type;
} llc_header_t;


/*************************** Function Prototypes *****************************/
int wifi_mpdu_eth_send_impl(void* mpdu,  u8* eth_start_ptr);
int wifi_eth_encap_impl(u8* mpdu_start_ptr, u8* eth_start_ptr, u32 eth_rx_len);
int ieee80211_put_snap_impl(u8 *data, u16 h_proto);

/* Interface initial function */
void wifi_mac_ethutil_func_int(void);

/* Rom patch interface */
typedef int (*wifi_mpdu_eth_send_fp_t)(void* mpdu,  u8* eth_start_ptr);
typedef int (*wifi_eth_encap_fp_t)(u8* mpdu_start_ptr, u8* eth_start_ptr, u32 eth_rx_len);
typedef int (*ieee80211_put_snap_fp_t)(u8 *data, u16 h_proto);

/* Export interface funtion pointer */
extern wifi_mpdu_eth_send_fp_t  wifi_mpdu_eth_send;
extern wifi_eth_encap_fp_t wifi_eth_encap;
extern ieee80211_put_snap_fp_t ieee80211_put_snap;
#endif /* WLAN_MAC_ETH_UTIL_H_ */

