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

#ifndef __WIFI_MAC_TASK_H__
#define __WIFI_MAC_TASK_H__

#include "cmsis_os.h"
#include "wifi_mac_common.h"


typedef struct
{
    uint32_t event;
    uint32_t length;
    uint8_t *pcMessage;
} xMACMessage_t;

#define WIFI_MAC_TX_QUEUE_SIZE                     (5)

#define WIFI_MAC_IPC_TX_EVENT                      0x0001
#define WIFI_MAC_IPC_RX_EVENT                      0x0002
#define WIFI_MAC_RX_REASM_TIMEOUT                  0x0003

#define WIFI_RX_DATA_USE_IPC


typedef int (*wifi_mac_rx_notify_tcp_callback_t)(u8 *rx_data, u16 tx_data_len);
typedef int (*wifi_mac_tx_notify_tcp_callback_t)(void *userdata);

void wifi_mac_task_create_impl(void);
void wifi_mac_task_init_impl(void);
void wifi_mac_task_impl( void *pvParameters );
int  wifi_mac_rx_done_handler_impl(void);
int  wifi_mac_tx_done_handler_impl(void);
void wifi_mac_rx_packet_ind_impl(int indicator, void *data, int *ipc_ret_opcode);
u32  wifi_mac_tx_start_impl(u8 *tx_data, u32 tx_data_len);

osStatus wifi_mac_rx_queue_first_entry_free(void);
osStatus wifi_mac_tx_queue_first_entry_free(void);

void wifi_mac_rx_notify_tcp_callback_registration(wifi_mac_rx_notify_tcp_callback_t callback);
void wifi_mac_tx_notify_tcp_callback_registration(wifi_mac_tx_notify_tcp_callback_t callback);

/**
  * @brief  The WiFi RX callback function
  *
  *         Each time the WiFi need to forward the packets to high layer, the callback function will be called
  */
typedef int (*wifi_mac_rxcb_t)(void *buffer, uint16_t len, void *param);

/**
  * @brief  Register WiFi RX callback
  *         Register function to be called by the low level transport driver
  *         when a new packet is available or when there is a state change in the
  *         data path. This function is typically used only by the TCP/IP stack driver.
  *
  * @param  ctx : Opaque context pointer that is passed unmodified to the
  *                 callback when it is invoked.
  * @param  fn  : WiFi RX callback fucntion
  *
  */
void wifi_mac_register_rxcb(void *ctx, wifi_mac_rxcb_t fn);


/* Interface initial function */
void wifi_mac_task_func_init(void);

/* Rom patch interface */
typedef void (*wifi_mac_task_create_fp_t)(void);
typedef void (*wifi_mac_task_init_fp_t)(void);
typedef void (*wifi_mac_rx_packet_ind_fp_t)(int indicator, void *data, int *ipc_ret_opcode);
typedef int  (*wifi_mac_rx_done_handler_fp_t)(void);
typedef int  (*wifi_mac_tx_done_handler_fp_t)(void);
typedef u32  (*wifi_mac_tx_start_fp_t)(u8 *tx_data, u32 tx_data_len);
typedef void (*wifi_mac_task_fp_t)(void *pvParameters);

/* Export interface funtion pointer */
extern wifi_mac_task_create_fp_t wifi_mac_task_create;
extern wifi_mac_task_init_fp_t wifi_mac_task_init;
extern wifi_mac_rx_packet_ind_fp_t wifi_mac_rx_packet_ind;
extern wifi_mac_rx_done_handler_fp_t wifi_mac_rx_done_handler;
extern wifi_mac_tx_done_handler_fp_t wifi_mac_tx_done_handler;
extern wifi_mac_tx_start_fp_t wifi_mac_tx_start;
extern wifi_mac_task_fp_t wifi_mac_task;
#endif
