#ifndef __WLANNETIF_H__
#define __WLANNETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"
#include <stdint.h>

#define WLAN_IF_NAME                "st1"
//#define WLAN_DEF_HOST_NAME_PREFIX   "OPL_"
#define WLAN_DEF_HOST_NAME          "opulink"
#define WLAN_HOST_NAME_LEN          32


extern char g_sLwipHostName[WLAN_HOST_NAME_LEN + 1];

err_t ethernetif_init(struct netif *netif);
void  ethernetif_input(struct netif *netif, void *buf, u16_t len);
void  ethernetif_check_link (struct netif *netif);

void wlanif_input(struct netif *netif, void *buffer, u16_t len, void *arg);

#endif
