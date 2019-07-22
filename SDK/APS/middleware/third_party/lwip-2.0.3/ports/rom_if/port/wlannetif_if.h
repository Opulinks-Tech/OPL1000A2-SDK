#ifndef __WLANNETIF_IF_H__
#define __WLANNETIF_IF_H__


#include "lwip/err.h"
#include "lwip/netif.h"
#include <stdint.h>

#if defined(LWIP_ROMBUILD)

/* Private API fucntion used in wlannetif.c*/
void low_level_init(struct netif *netif);
err_t low_level_output(struct netif *netif, struct pbuf *p);
struct pbuf * low_level_input(struct netif *netif, void *buf, u16_t len);
int wifi_rx_callback(char *pbuf, u16_t length);
int wifi_tx_callback(void *userdata);
void __packet_rx_task(void* pvParameters);
void __packet_tx_task(void* pvParameters);


typedef void  (*low_level_init_fp_t)(struct netif *netif);
typedef err_t (*low_level_output_fp_t)(struct netif *netif, struct pbuf *p);
typedef struct pbuf *(*low_level_input_fp_t)(struct netif *netif, void *buf, u16_t len);
typedef int   (*wifi_rx_callback_fp_t)(char *pbuf, u16_t length);
typedef int   (*wifi_tx_callback_fp_t)(void *userdata);
typedef void  (*__packet_rx_task_fp_t)(void* pvParameters);
typedef void  (*__packet_tx_task_fp_t)(void* pvParameters);

extern low_level_init_fp_t      low_level_init_adpt;
extern low_level_output_fp_t    low_level_output_adpt;
extern low_level_input_fp_t     low_level_input_adpt;
extern wifi_rx_callback_fp_t    wifi_rx_callback_adpt;
extern wifi_tx_callback_fp_t    wifi_tx_callback_adpt;
#ifdef TRANSPORT_TASK
extern __packet_rx_task_fp_t    __packet_rx_task_adpt;
extern __packet_tx_task_fp_t    __packet_tx_task_adpt;
#endif

/* Public API fucntion used in wlannetif.c*/
typedef err_t (*ethernetif_init_fp_t)(struct netif *netif);
typedef void  (*ethernetif_input_fp_t)(struct netif *netif, void *buf, u16_t len);
typedef void  (*ethernetif_check_link_fp_t) (struct netif *netif);
typedef void  (*wlanif_input_fp_t)(struct netif *netif, void *buffer, u16_t len, void *arg);

extern ethernetif_init_fp_t     ethernetif_init_adpt;
extern ethernetif_input_fp_t    ethernetif_input_adpt;
extern wlanif_input_fp_t        wlanif_input_adpt;
#endif

#endif /* __WLANNETIF_IF_H__ */
