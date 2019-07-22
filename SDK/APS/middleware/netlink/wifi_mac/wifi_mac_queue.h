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

#ifndef _WIFI_MAC_QUEUE_H_
#define _WIFI_MAC_QUEUE_H_

//#include "wifi_mac_common.h"
#include "ipc.h"

#ifdef ENHANCE_IPC

#define BSS_INFO_ADDR               IPC_WIFI_BSS_INFO_START  // bss_info
#define SHARE_MEMORY_ADDR           IPC_WIFI_SHARED_MEM_START // share_memory[SHARE_MEMERY_MAX]
#define DBG_TRX_CONFIG_PARAM_ADDR   IPC_DBG_TRX_PARAM_START
#define STA_INFO_ADDR               IPC_WIFI_STA_INFO_START

#ifdef RF_TCHIP_MODE

#ifdef CHIP_DEMO_WIFI
    #define WIFI_MAC_MSQ_TX_POLL_MAX    IPC_WIFI_MSQ_TX_BUF_NUM
    #define WIFI_MAC_MSQ_RX_POLL_MAX    IPC_WIFI_MSQ_RX_BUF_NUM
    #define WIFI_MAC_MSQ_MPDU_RX_MAX_SIZE   780
    #define WIFI_MAC_MSQ_TX_BUF_SIZE    (WIFI_MAC_MSQ_MPDU_RX_MAX_SIZE + sizeof(tx_frame_info_t))
    #define WIFI_MAC_MSQ_RX_BUF_SIZE    (WIFI_MAC_MSQ_MPDU_RX_MAX_SIZE + sizeof(rx_frame_info_t))

    #define WIFI_MAC_APS_TX_POLL_MAX    IPC_WIFI_APS_TX_BUF_NUM
    #define WIFI_MAC_APS_RX_POLL_MAX    IPC_WIFI_APS_RX_BUF_NUM
    #define WIFI_MAC_APS_MPDU_RX_MAX_SIZE   1600
    #define WIFI_MAC_APS_TX_BUF_SIZE    (WIFI_MAC_APS_MPDU_RX_MAX_SIZE + sizeof(tx_frame_info_t))
    #define WIFI_MAC_APS_RX_BUF_SIZE    (WIFI_MAC_APS_MPDU_RX_MAX_SIZE + sizeof(rx_frame_info_t))
#else
    #define WIFI_MAC_MSQ_TX_POLL_MAX    IPC_WIFI_MSQ_TX_BUF_NUM
    #define WIFI_MAC_MSQ_RX_POLL_MAX    IPC_WIFI_MSQ_RX_BUF_NUM
    #define WIFI_MAC_MSQ_MPDU_RX_MAX_SIZE   IEEE80211_MAX_FRAME_LEN
    #define WIFI_MAC_MSQ_TX_BUF_SIZE    (WIFI_MAC_MSQ_MPDU_RX_MAX_SIZE + sizeof(tx_frame_info_t))
    #define WIFI_MAC_MSQ_RX_BUF_SIZE    (WIFI_MAC_MSQ_MPDU_RX_MAX_SIZE + sizeof(rx_frame_info_t))

    #define WIFI_MAC_APS_TX_POLL_MAX    IPC_WIFI_APS_TX_BUF_NUM
    #define WIFI_MAC_APS_RX_POLL_MAX    IPC_WIFI_APS_RX_BUF_NUM
    #define WIFI_MAC_APS_MPDU_RX_MAX_SIZE   4
    #define WIFI_MAC_APS_TX_BUF_SIZE    (WIFI_MAC_APS_MPDU_RX_MAX_SIZE + sizeof(tx_frame_info_t))
    #define WIFI_MAC_APS_RX_BUF_SIZE    (WIFI_MAC_APS_MPDU_RX_MAX_SIZE + sizeof(rx_frame_info_t))
#endif // CHIP_DEMO_WIFI

#else
    #define WIFI_MAC_MSQ_TX_POLL_MAX    IPC_WIFI_MSQ_TX_BUF_NUM
    #define WIFI_MAC_MSQ_RX_POLL_MAX    IPC_WIFI_MSQ_RX_BUF_NUM
    #define WIFI_MAC_MSQ_MPDU_RX_MAX_SIZE   IEEE80211_MAX_FRAME_LEN
    #define WIFI_MAC_MSQ_TX_BUF_SIZE    (WIFI_MAC_MSQ_MPDU_RX_MAX_SIZE + sizeof(tx_frame_info_t))
    #define WIFI_MAC_MSQ_RX_BUF_SIZE    (WIFI_MAC_MSQ_MPDU_RX_MAX_SIZE + sizeof(rx_frame_info_t))

    #define WIFI_MAC_APS_TX_POLL_MAX    IPC_WIFI_APS_TX_BUF_NUM
    #define WIFI_MAC_APS_RX_POLL_MAX    IPC_WIFI_APS_RX_BUF_NUM
    #define WIFI_MAC_APS_MPDU_RX_MAX_SIZE   IEEE80211_MAX_FRAME_LEN
    #define WIFI_MAC_APS_TX_BUF_SIZE    (WIFI_MAC_APS_MPDU_RX_MAX_SIZE + sizeof(tx_frame_info_t))
    #define WIFI_MAC_APS_RX_BUF_SIZE    (WIFI_MAC_APS_MPDU_RX_MAX_SIZE + sizeof(rx_frame_info_t))
#endif // RF_TCHIP_MODE

#else //#ifdef ENHANCE_IPC

#define MEM_ALIGNMENT 16
#define QUEUE_MEM_ALIGN(addr) ((((u32)(addr) + MEM_ALIGNMENT - 1) & ~(u32)(MEM_ALIGNMENT-1)))

#ifdef CHIP_DEMO_WIFI
#ifdef __NL1000_An__
#define APS_MEM_WIFI_BASE 0x80001C00
#else   // __NL1000_TC__
#define APS_MEM_WIFI_BASE 0x459C00
#endif

#define BSS_INFO_ADDR      (APS_MEM_WIFI_BASE)  // bss_info
#define SHARE_MEMORY_ADDR  (BSS_INFO_ADDR + sizeof(bss_info_t)) // share_memory[SHARE_MEMERY_MAX]
#define WIFI_MAC_MSQ_TX_POOL_LIST_ADDR (SHARE_MEMORY_ADDR + SHARE_MEMERY_MAX) // memory_pool_t wifi_mac_msq_tx_pool_list __attribute__((at(0x004080a0)));
#define WIFI_MAC_MSQ_RX_POOL_LIST_ADDR (WIFI_MAC_MSQ_TX_POOL_LIST_ADDR +  sizeof(memory_pool_t)) //memory_pool_t wifi_mac_msq_rx_pool_list __attribute__((at(0x004080a4)));
#define WIFI_MAC_APS_TX_POOL_LIST_ADDR (WIFI_MAC_MSQ_RX_POOL_LIST_ADDR +  sizeof(memory_pool_t)) //memory_pool_t wifi_mac_aps_tx_pool_list __attribute__((at(0x004080a8)));
#define WIFI_MAC_APS_RX_POOL_LIST_ADDR (WIFI_MAC_APS_TX_POOL_LIST_ADDR +  sizeof(memory_pool_t)) //memory_pool_t wifi_mac_aps_rx_pool_list __attribute__((at(0x004080ac)));
#define WIFI_MAC_MSQ_RX_QUEUE_ADDR (WIFI_MAC_APS_RX_POOL_LIST_ADDR + sizeof(memory_pool_t)) //sl_list_t g_wifi_mac_msq_rx_queue __attribute__((at(0x0040e788)));
#define WIFI_MAC_MSQ_TX_QUEUE_ADDR (WIFI_MAC_MSQ_RX_QUEUE_ADDR + sizeof(sl_list_t)) //sl_list_t g_wifi_mac_msq_tx_queue __attribute__((at(0x0040e794)));
#define WIFI_MAC_APS_RX_QUEUE_ADDR (WIFI_MAC_MSQ_TX_QUEUE_ADDR + sizeof(sl_list_t)) //sl_list_t g_wifi_mac_aps_rx_queue __attribute__((at(0x0040e7a0)));
#define WIFI_MAC_APS_TX_QUEUE_ADDR (WIFI_MAC_APS_RX_QUEUE_ADDR + sizeof(sl_list_t)) //sl_list_t g_wifi_mac_aps_tx_queue __attribute__((at(0x0040e7ac)));
#define WIFI_MAC_MSQ_TX_POOL_STORAGE_ADDR (QUEUE_MEM_ALIGN(WIFI_MAC_APS_TX_QUEUE_ADDR + sizeof(sl_list_t))) //static wifi_mac_msq_tx_entry_t wifi_mac_msq_tx_pool_storage[WIFI_MAC_MSQ_TX_POLL_MAX];
#define WIFI_MAC_MSQ_RX_POOL_STORAGE_ADDR (QUEUE_MEM_ALIGN(WIFI_MAC_MSQ_TX_POOL_STORAGE_ADDR + (sizeof(wifi_mac_msq_tx_entry_t) * WIFI_MAC_MSQ_TX_POLL_MAX))) //static wifi_mac_msq_rx_entry_t wifi_mac_msq_rx_pool_storage[WIFI_MAC_MSQ_RX_POLL_MAX];
#define WIFI_MAC_APS_TX_POOL_STORAGE_ADDR (QUEUE_MEM_ALIGN(WIFI_MAC_MSQ_RX_POOL_STORAGE_ADDR + (sizeof(wifi_mac_msq_rx_entry_t) * WIFI_MAC_MSQ_RX_POLL_MAX))) //static wifi_mac_aps_tx_entry_t wifi_mac_aps_tx_pool_storage[WIFI_MAC_APS_TX_POLL_MAX];
#define WIFI_MAC_APS_RX_POOL_STORAGE_ADDR (QUEUE_MEM_ALIGN(WIFI_MAC_APS_TX_POOL_STORAGE_ADDR + (sizeof(wifi_mac_aps_tx_entry_t) * WIFI_MAC_APS_TX_POLL_MAX))) //static wifi_mac_aps_rx_entry_t wifi_mac_aps_rx_pool_storage[WIFI_MAC_APS_RX_POLL_MAX];
#define DBG_TRX_CONFIG_PARAM_ADDR (QUEUE_MEM_ALIGN(WIFI_MAC_APS_RX_POOL_STORAGE_ADDR + (sizeof(wifi_mac_aps_rx_entry_t) * WIFI_MAC_APS_RX_POLL_MAX)))

#define WIFI_MAC_MSQ_TX_POLL_MAX 5
#define WIFI_MAC_MSQ_RX_POLL_MAX 5
#define WIFI_MAC_MSQ_TX_BUF_SIZE 780   //IEEE80211_MAX_FRAME_LEN
#define WIFI_MAC_MSQ_RX_BUF_SIZE 780   //IEEE80211_MAX_FRAME_LEN
#define WIFI_MAC_MSQ_MPDU_RX_MAX_SIZE (WIFI_MAC_MSQ_RX_BUF_SIZE - sizeof(rx_frame_info_t))

#define WIFI_MAC_APS_TX_POLL_MAX 5
#define WIFI_MAC_APS_RX_POLL_MAX 5
#define WIFI_MAC_APS_TX_BUF_SIZE 1600  //IEEE80211_MAX_FRAME_LEN
#define WIFI_MAC_APS_RX_BUF_SIZE 1600  //IEEE80211_MAX_FRAME_LEN
#define WIFI_MAC_APS_MPDU_RX_MAX_SIZE (WIFI_MAC_APS_RX_BUF_SIZE - sizeof(rx_frame_info_t))
#else //CHIP_DEMO_WIFI

#define WIFI_MAC_MSQ_TX_POLL_MAX 5
#define WIFI_MAC_MSQ_RX_POLL_MAX 5
#define WIFI_MAC_MSQ_TX_BUF_SIZE 64   //IEEE80211_MAX_FRAME_LEN

#define WIFI_MAC_MSQ_RX_BUF_SIZE 780   //IEEE80211_MAX_FRAME_LEN
#define WIFI_MAC_MSQ_MPDU_RX_MAX_SIZE (WIFI_MAC_MSQ_RX_BUF_SIZE - sizeof(rx_frame_info_t))

#define WIFI_MAC_APS_TX_POLL_MAX 2
#define WIFI_MAC_APS_RX_POLL_MAX 2
#define WIFI_MAC_APS_TX_BUF_SIZE 64  //IEEE80211_MAX_FRAME_LEN
#define WIFI_MAC_APS_RX_BUF_SIZE 64  //IEEE80211_MAX_FRAME_LEN

#define WIFI_MAC_APS_MPDU_RX_MAX_SIZE (WIFI_MAC_APS_RX_BUF_SIZE - sizeof(rx_frame_info_t))
#endif //CHIP_DEMO_WIFI

#endif //#ifdef ENHANCE_IPC

typedef enum {
    LOCKED,
    WAIT_PROCESS,
    FAIL,
    SUCCESS
} entry_state_t;

typedef enum {
    CLI_COMMAND,
    APS_TX_DATA,
    APS_RX_DATA,
    MSQ_TX_DATA,
    MSQ_RX_DATA,
    MSQ_TX_PSPOLL
} entry_type_t;

typedef struct {
    linked_item_t item;
    u32 entry_state;                // locked, wait_process,
                                    // fail, success
    u32 entry_type;                 // cli_command,
                                    // aps_tx_data, aps_rx_data,
                                    // msq_tx_data, msq_rx_data
	char data[WIFI_MAC_MSQ_TX_BUF_SIZE];  // data buffer
} wifi_mac_msq_tx_entry_t;

typedef struct {
    linked_item_t item;
    u32 entry_state;                // locked, wait_process,
                                    // fail, success
    u32 entry_type;                 // cli_command,
                                    // aps_tx_data, aps_rx_data,
                                    // msq_tx_data, msq_rx_data
	char data[WIFI_MAC_APS_TX_BUF_SIZE];  // data buffer
} wifi_mac_aps_tx_entry_t;

typedef struct {
	linked_item_t item;
	u32 entry_state;                 // locked, wait_process,
                                     // fail, success
	u32  entry_type;                 // cli_command,
                                     // aps_tx_data, aps_rx_data,
                                     // msq_tx_data, msq_rx_data
	char data[WIFI_MAC_MSQ_RX_BUF_SIZE]; // data buffer
} wifi_mac_msq_rx_entry_t;

typedef struct {
	linked_item_t item;
	u32 entry_state;                 // locked, wait_process,
                                     // fail, success
	u32  entry_type;                 // cli_command,
                                     // aps_tx_data, aps_rx_data,
                                     // msq_tx_data, msq_rx_data
	char data[WIFI_MAC_APS_RX_BUF_SIZE]; // data buffer
} wifi_mac_aps_rx_entry_t;

wifi_mac_msq_tx_entry_t *wifi_mac_msq_tx_entry_get(void);
wifi_mac_msq_rx_entry_t *wifi_mac_msq_rx_entry_get(void);
wifi_mac_aps_tx_entry_t *wifi_mac_aps_tx_entry_get(void);
wifi_mac_aps_rx_entry_t *wifi_mac_aps_rx_entry_get(void);

void wifi_mac_msq_tx_entry_free(wifi_mac_msq_tx_entry_t *wifi_mac_tx_entry);
void wifi_mac_msq_rx_entry_free(wifi_mac_msq_rx_entry_t *wifi_mac_rx_entry);
void wifi_mac_aps_tx_entry_free(wifi_mac_aps_tx_entry_t *wifi_mac_tx_entry);
void wifi_mac_aps_rx_entry_free(wifi_mac_aps_rx_entry_t *wifi_mac_rx_entry);

void wifi_mac_entry_init(void);

#endif // _WIFI_QUEUE_H_
