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

#if 0
#include "lwip/opt.h"

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ppp_oe.h"

#include "Driver_ETH_MAC.h"
#include "Driver_ETH_PHY.h"
#include <string.h>

/* Ethernet MAC & PHY Driver */
extern ARM_DRIVER_ETH_MAC Driver_ETH_MAC0;
extern ARM_DRIVER_ETH_PHY Driver_ETH_PHY0;

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 */
struct ethernetif {
  ARM_DRIVER_ETH_MAC      *mac;
  ARM_DRIVER_ETH_PHY      *phy;
  ARM_ETH_MAC_CAPABILITIES capabilities; // Driver capabilities
  ARM_ETH_LINK_STATE       link;         // Ethernet Link State
  bool                     phy_ok;       // Phy initialized successfully
};

__align(4) static u8_t rte_buffer[1536]; // Intermediate buffer
           static bool rx_event;

/* Forward declarations. */
static void  ethernetif_input(struct netif *netif);
static void  eth_notify (ARM_ETH_MAC_EVENT event);

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init(struct netif *netif)
{
  struct ethernetif *eth = netif->state;
  
  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set MAC hardware address */
  netif->hwaddr[0] =  MAC_ADDR0;
  netif->hwaddr[1] =  MAC_ADDR1;
  netif->hwaddr[2] =  MAC_ADDR2;
  netif->hwaddr[3] =  MAC_ADDR3;
  netif->hwaddr[4] =  MAC_ADDR4;
  netif->hwaddr[5] =  MAC_ADDR5;

  /* maximum transfer unit */
  netif->mtu = 1500;
  
  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
 
  /* Do whatever else is needed to initialize interface. */
  eth->phy = &Driver_ETH_PHY0;
  eth->mac = &Driver_ETH_MAC0;

  eth->link   = ARM_ETH_LINK_DOWN;
  eth->phy_ok = false;

  /* Only MII/RMII mode info is used */
  eth->capabilities = eth->mac->GetCapabilities ();
  eth->mac->Initialize (eth_notify, (ARM_ETH_MAC_ADDR *)netif->hwaddr);
  eth->mac->PowerControl (ARM_POWER_FULL);
  eth->mac->EnableTx (false);
  eth->mac->EnableRx (false);

  /* Initialize Physical Media Interface */
  if (eth->phy->Initialize (eth->mac->PHY_Read, eth->mac->PHY_Write) == ARM_ETH_OK) {
    eth->phy->PowerControl (ARM_POWER_FULL);
    eth->phy->SetInterface ((ARM_ETH_INTERFACE)eth->capabilities.media_interface);
    eth->phy->SetMode (ARM_ETH_MODE_AUTO_NEGOTIATE);
    eth->phy_ok = true;
  }

}

static void eth_notify (ARM_ETH_MAC_EVENT event) {
  /* Send notification on RX event */
  if (event == ARM_ETH_MAC_EVENT_RX_FRAME) {
    rx_event = true;
  }
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
low_level_output(struct netif *netif, struct pbuf *p)
{
  struct ethernetif *eth = netif->state;
  struct pbuf *q;
  int framelen = 0;
  
#if ETH_PAD_SIZE
  pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

  for(q = p; q != NULL; q = q->next) {
    /* Send the data from the pbuf to the interface, one pbuf at a
       time. The size of the data in each pbuf is kept in the ->len
       variable. */
    memcpy(&rte_buffer[framelen], q->payload, q->len);
    framelen += q->len;
  }
  /* The above memcpy() reduces the system performance, but 
     it has to be done, as the RTE ethernet driver expects only
     one and continuous packet data buffer. */
  eth->mac->SendFrame (rte_buffer, framelen);

#if ETH_PAD_SIZE
  pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
  
  LINK_STATS_INC(link.xmit);

  return ERR_OK;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input(struct netif *netif)
{
  struct ethernetif *eth = netif->state;
  struct pbuf *p, *q;
  int len, framelen;

  /* Obtain the size of the packet and put it into the "len"
     variable. */
  len = eth->mac->GetRxFrameSize ();
  if (len > 1514) {
    /* Drop oversized packet */
    eth->mac->ReadFrame (NULL, 0);
    return NULL;
  }

#if ETH_PAD_SIZE
  len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

  /* We allocate a pbuf chain of pbufs from the pool. */
  p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  
  if (p != NULL) {

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    /* Copy the data to intermediate buffer. This is required because
       the RTE ethernet driver copies all the data to one
       continuous packet data buffer. */
    eth->mac->ReadFrame (rte_buffer, len);
    framelen = 0;
    /* We iterate over the pbuf chain until we have read the entire
     * packet into the pbuf. */
    for(q = p; q != NULL; q = q->next) {
      /* Read enough bytes to fill this pbuf in the chain. The
       * available data in the pbuf is given by the q->len
       * variable.
       * This does not necessarily have to be a memcpy, you can also preallocate
       * pbufs for a DMA-enabled MAC and after receiving truncate it to the
       * actually received size. In this case, ensure the tot_len member of the
       * pbuf is the sum of the chained pbuf len members.
       */
       memcpy (q->payload, &rte_buffer[framelen], q->len);
       framelen += q->len;
    }

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.recv);
  } else {
    /* Drop packet */
    eth->mac->ReadFrame (NULL, 0);
    LINK_STATS_INC(link.memerr);
    LINK_STATS_INC(link.drop);
  }

  return p;  
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
static void
ethernetif_input(struct netif *netif)
{
  struct eth_hdr *ethhdr;
  struct pbuf *p;

  /* move received packet into a new pbuf */
  p = low_level_input(netif);
  /* no packet could be read, silently ignore this */
  if (p == NULL) return;
  /* points to packet payload, which starts with an Ethernet header */
  ethhdr = p->payload;

  switch (htons(ethhdr->type)) {
  /* IP or ARP packet? */
  case ETHTYPE_IP:
  case ETHTYPE_ARP:
#if PPPOE_SUPPORT
  /* PPPoE packet? */
  case ETHTYPE_PPPOEDISC:
  case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
    /* full packet send to tcpip_thread to process */
    if (netif->input(p, netif)!=ERR_OK)
     { LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
       pbuf_free(p);
       p = NULL;
     }
    break;

  default:
    pbuf_free(p);
    p = NULL;
    break;
  }
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ethernetif_init(struct netif *netif)
{
  struct ethernetif *ethernetif;

  LWIP_ASSERT("netif != NULL", (netif != NULL));
    
  ethernetif = mem_malloc(sizeof(struct ethernetif));
  if (ethernetif == NULL) {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
    return ERR_MEM;
  }

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

  netif->state = ethernetif;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}

void ethernetif_check_link (struct netif *netif) {
  struct ethernetif *eth = netif->state;
  ARM_ETH_LINK_STATE link;

  if (!eth->phy_ok) {
    return;
  }
  /* Check link status */
  link = eth->phy->GetLinkState ();
  if (link != eth->link) {
    eth->link = link;
    if (eth->link == ARM_ETH_LINK_UP) {
      /* Start EMAC DMA */
      ARM_ETH_LINK_INFO info = eth->phy->GetLinkInfo ();
      eth->mac->SetMode ((ARM_ETH_DUPLEX)info.duplex, (ARM_ETH_SPEED)info.speed);
      eth->mac->EnableTx (true);
      eth->mac->EnableRx (true);
      netif_set_link_up (netif);
    }
    else {
      /* Stop EMAC DMA */
      eth->mac->EnableRx (false);
      eth->mac->EnableTx (false);
      netif_set_link_down (netif);
    }
  }
}

void ethernetif_poll (struct netif *netif) {
  struct ethernetif *eth = netif->state;

  if (!eth->phy_ok || eth->link == ARM_ETH_LINK_DOWN) {
    return;
  }
  if (rx_event) {
    rx_event = false;
    /* process received ethernet packet */
    ethernetif_input (netif);
  }
}
#endif
