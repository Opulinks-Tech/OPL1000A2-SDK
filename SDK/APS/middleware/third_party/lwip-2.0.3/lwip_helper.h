#ifndef __LWIP_HELPER_H__
#define __LWIP_HELPER_H__

#include <stdio.h>
#include <stdint.h>

#include "lwip/ip4_addr.h"
#include "lwip/inet.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    STA_IP_MODE_STATIC   = 0,
    STA_IP_MODE_DHCP     = 1
} sta_ip_mode_t;

typedef enum {
    NETIF_TYPE_LOOPBACK = 0,
    NETIF_TYPE_AP       = 1,
    NETIF_TYPE_STA      = 2
} netif_type_t;

typedef struct {
    ip4_addr_t sta_ip;
    ip4_addr_t sta_mask;
    ip4_addr_t sta_gw;
} lwip_tcpip_config_t;


#define WIFI_MODE_STA_ONLY      (1)
#define WIFI_MODE_AP_ONLY       (2)


/* Rom patch interface */

/**
  * @brief  network init function. initial wifi and lwip config
  * @param None
  * @retval None
  */
typedef void (*lwip_network_init_fp_t)(uint8_t opmode);

/**
  * @brief  network start function. network link-up and start dchp client
  * @param None
  * @retval None
  */
typedef void (*lwip_net_start_fp_t)(uint8_t opmode);


/**
  * @brief  network stop function. network link-down and stop dchp client
  * @param None
  * @retval None
  */
typedef void (*lwip_net_stop_fp_t)(uint8_t opmode);


/**
  * @brief  when wifi and ip ready will return.
  * @param None
  * @retval None
  */
typedef void (*lwip_net_ready_fp_t)(void);


typedef int  (*lwip_wlan_sta_input_fp_t)(void *buf, uint16_t len, void *arg);



/* Private static api function */
typedef void (*lwip_tcpip_init_fp_t)(void);
typedef void (*tcpip_init_done_cb_fp_t)(void *arg);
typedef int  (*lwip_get_ip_info_fp_t)(char *ifname);
typedef void (*ip_ready_callback_fp_t)(struct netif *netif);

typedef int32_t (*wifi_station_connected_event_handler_fp_t)(void *arg);
typedef int32_t (*wifi_station_disconnected_event_handler_fp_t)(void *arg);
typedef int32_t (*lwip_get_netif_fp_t)(struct netif *iface);

/* Export interface funtion pointer */
extern lwip_network_init_fp_t   lwip_network_init;
extern lwip_net_start_fp_t      lwip_net_start;
extern lwip_net_stop_fp_t       lwip_net_stop;
extern lwip_net_ready_fp_t      lwip_net_ready;
extern lwip_wlan_sta_input_fp_t lwip_wlan_sta_input;
extern lwip_tcpip_init_fp_t     lwip_tcpip_init;
extern tcpip_init_done_cb_fp_t  tcpip_init_done_cb;
extern lwip_get_ip_info_fp_t    lwip_get_ip_info;
extern ip_ready_callback_fp_t   ip_ready_callback;

extern wifi_station_connected_event_handler_fp_t    wifi_station_connected_event_handler;
extern wifi_station_disconnected_event_handler_fp_t wifi_station_disconnected_event_handler;
extern lwip_get_netif_fp_t      lwip_get_netif;

#ifdef __cplusplus
}
#endif

#endif /* __LWIP_HELPER_H__ */
