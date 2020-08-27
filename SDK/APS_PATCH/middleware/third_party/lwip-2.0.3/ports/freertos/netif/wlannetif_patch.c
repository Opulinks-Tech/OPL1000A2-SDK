/******************************************************************************
*  Copyright 2019, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2019
******************************************************************************/

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
 
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/ethip6.h"
#include "netif/etharp.h"
#include "arch/sys_arch.h"
#include "wlannetif.h"
//#include "netif/ppp_oe.h"
#include <string.h>

#include "wifi_mac_task.h"
#include "msg.h"

#if defined(LWIP_ROMBUILD)
#include "port/wlannetif_if.h"
#endif

#include "wlannetif_patch.h"

extern u8 gsta_cfg_mac[MAC_ADDR_LEN];

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init_patch(struct netif *netif)
{
  struct ethernetif *ethernetif = netif->state;

  LWIP_UNUSED_ARG(ethernetif);

  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set MAC hardware address */
  /*netif->hwaddr[0] = 0x00;
  netif->hwaddr[1] = 0x11;
  netif->hwaddr[2] = 0x22;
  netif->hwaddr[3] = 0x33;
  netif->hwaddr[4] = 0x44;
  netif->hwaddr[5] = 0x55;

  netif->hwaddr[0] = 0x22;
  netif->hwaddr[1] = 0x33;
  netif->hwaddr[2] = 0x44;
  netif->hwaddr[3] = 0x55;
  netif->hwaddr[4] = 0x66;
  netif->hwaddr[5] = 0x76;
  */
  
  memcpy(netif->hwaddr, gsta_cfg_mac, MAC_ADDR_LEN);
  
  /* maximum transfer unit */
  netif->mtu = 576; //1500;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

#if LWIP_IGMP
  netif->flags |= NETIF_FLAG_IGMP;
#endif

  /* Do whatever else is needed to initialize interface. */
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t
low_level_output_patch(struct netif *netif, struct pbuf *p)
{
    struct ethernetif *ethernetif = netif->state;
    struct pbuf *q;
    int ret = ERR_OK;
    
    LWIP_UNUSED_ARG(ethernetif);

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
    
    q = p;
        
    if (q->next == NULL) {
        #ifdef TX_PKT_DUMP
            dump_buffer(q->payload, q->len, 1);
        #endif
        
        if (TX_QUEUE_FULL == wifi_mac_tx_start(q->payload, q->len)) {
            ret = ERR_MEM;
        }
    }
    else {
        q = pbuf_alloc(PBUF_RAW_TX, p->tot_len, PBUF_RAM);
        
        if (q != NULL) {
            pbuf_copy(q, p);
        #ifdef TX_PKT_DUMP
            dump_buffer(q->payload, q->len, 1);
        #endif

            if (TX_QUEUE_FULL == wifi_mac_tx_start(q->payload, q->len)) {
                ret = ERR_MEM;
            }
            pbuf_free(q);
        }
        else {
            printf("__packet_tx_task: pbuf malloc failed\r\n");
            ret = ERR_MEM;
        }
    }

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.xmit);

    return ret;
}

void lwip_load_interface_wlannetif_patch(void)
{
    low_level_init_adpt   = low_level_init_patch;
    low_level_output_adpt = low_level_output_patch;
}
