/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

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
#include "wifi_nvm.h"
#include "sys_common_ctrl.h"

/* Define those to better describe your network interface. */
#define IFNAME0 's'
#define IFNAME1 't'

#define TRANSPORT_TASK
//#define IF_LOOPBACK
//#define TX_PKT_DUMP
//#define RX_PKT_DUMP

#if defined(LWIP_ROMBUILD)
#include "port/wlannetif_if.h"
#endif

#include "wlannetif.h"

#define TX_TASK_STACKSIZE           (512)
#ifdef LWIP_DEBUG
#define RX_TASK_STACKSIZE           (512*2)
#else
#define RX_TASK_STACKSIZE           (512)
#endif

#define RX_PRIORITY                 (tskIDLE_PRIORITY + 2)//(osPriorityBelowNormal)
#define TX_PRIORITY                 (tskIDLE_PRIORITY + 2)//(osPriorityBelowNormal)

LWIP_RETDATA sys_sem_t TxReadySem;
LWIP_RETDATA sys_sem_t RxReadySem; /**< RX packet ready semaphore */
LWIP_RETDATA sys_sem_t TxCleanSem;
LWIP_RETDATA sys_thread_t rx_thread_handle;

extern struct netif netif;

char g_sLwipHostName[WLAN_HOST_NAME_LEN + 1] = {WLAN_DEF_HOST_NAME};

static char *rx_packet_buf;
static u16_t rx_packet_len;

static void LWIP_ROMFN(__packet_rx_task)(void *data);
static void LWIP_ROMFN(__packet_tx_task)(void *data);
static int LWIP_ROMFN(wifi_rx_callback)(char *pbuf, u16_t length);
static int LWIP_ROMFN(wifi_tx_callback)(void *userdata);
void dump_buffer(char *pbuf, int len, int mode);

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
  struct eth_addr *ethaddr;
  /* Add whatever per-interface state that is needed here. */
};

void LWIP_ROMFN(wlanif_input)(struct netif *netif, void *buffer, u16_t len, void *arg)
{
    ethernetif_input(netif, buffer, len);
}

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
LWIP_ROMFN(low_level_init)(struct netif *netif)
{
  u8 type = BASE_NVM_MAC_SRC_TYPE_ID_OTP;
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
  //memcpy(netif->hwaddr, s_StaInfo.au8Dot11MACAddress, MAC_ADDR_LEN);

  //ToDo: we need to get mac address through wifi drvier api, before we call lwip_init()
  //      should be set mac address after wifi ready
  base_mac_addr_src_get_cfg(BASE_NVM_MAC_SRC_IFACE_ID_STA, &type);
  if (type == BASE_NVM_MAC_SRC_TYPE_ID_OTP) {
    //TODO
    //Wait OTP function.
    memcpy(netif->hwaddr, ps_StaInfo->au8Dot11MACAddress, MAC_ADDR_LEN);
  }
  else {
    wifi_nvm_sta_info_read(WIFI_NVM_STA_INFO_ID_MAC_ADDR, MAC_ADDR_LEN, netif->hwaddr);
  }
  
  /* maximum transfer unit */
  netif->mtu = 1500;

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
LWIP_ROMFN(low_level_output)(struct netif *netif, struct pbuf *p)
{
    struct ethernetif *ethernetif = netif->state;
    struct pbuf *q;
    static int full_count = 0;
    static int write_count = 0;
    //u16_t len = 0; //I0060 0000897

    LWIP_UNUSED_ARG(ethernetif);

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    //I0060 0000897_add_beg
    q = p;
    
    if (q->next == NULL) {
        #ifdef TX_PKT_DUMP
            dump_buffer(q->payload, q->len, 1);
        #endif
    
        if ( TX_QUEUE_FULL == wifi_mac_tx_start(q->payload, q->len) ) {
            full_count++;
            printf("__packet_tx_task: Tx WriteCount: %d  FullCount:%d\r\n", write_count, full_count);
            sys_arch_sem_wait(&TxReadySem, 1);
            printf("__packet_tx_task: recevie Tx ready event to wakeup\r\n");
        }
    }
    else {
        q = pbuf_alloc(PBUF_RAW_TX, p->tot_len, PBUF_RAM);
        
        if (q != NULL) {
            pbuf_copy(q, p);
        }
        else {
            printf("__packet_tx_task: pbuf malloc failed\r\n");
            return ERR_OK;
        }

        #ifdef TX_PKT_DUMP
            dump_buffer(q->payload, q->len, 1);
        #endif
        
        if ( TX_QUEUE_FULL == wifi_mac_tx_start(q->payload, q->len) ) {
            full_count++;
            printf("__packet_tx_task: Tx WriteCount: %d  FullCount:%d\r\n", write_count, full_count);
            sys_arch_sem_wait(&TxReadySem, 1);
            printf("__packet_tx_task: recevie Tx ready event to wakeup\r\n");
        }
        
        pbuf_free(q);
    }
    //I0060 0000897_add_end
    
#if 0 //I0060 0000897_del_beg
    for(q = p; q != NULL; q = q->next) {
        /* Send the data from the pbuf to the interface, one pbuf at a
           time. The size of the data in each pbuf is kept in the ->len
           variable. */
        #ifdef TX_PKT_DUMP
        dump_buffer(q->payload, q->len, 1);
        #endif

        /* Wait for transmit cleanup task to wakeup */
        if ( TX_QUEUE_FULL == wifi_mac_tx_start(q->payload, q->len) )
        {
            full_count++;
            printf("__packet_tx_task: Tx WriteCount: %d  FullCount:%d\r\n", write_count, full_count);
            //sys_arch_sem_wait(&TxReadySem, 2000); //I0060 0000779
            sys_arch_sem_wait(&TxReadySem, 1); //I0060 0000779
            printf("__packet_tx_task: recevie Tx ready event to wakeup\r\n");
            //return ERR_MEM;
        }
        else
        {
            full_count = 0;
            write_count++;
            //printf("__packet_tx_task: Tx WriteCount: %d  FullCount:%d\r\n", write_count, full_count); //I0060 0000779
        }
        len = len + q->len;
    }
#endif //I0060 0000897_del_end

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
LWIP_ROMFN(low_level_input)(struct netif *netif, void *buf, u16_t len)
{
    struct ethernetif *ethernetif = netif->state;
    struct pbuf *p = NULL, *q = NULL;
    //u16_t l = 0;

    LWIP_UNUSED_ARG(ethernetif);

    /* Drop oversized packet */
    if (len > 1514) {
        goto done;
        //return NULL;
    }

    if (!netif || !buf || len <= 0)
        goto done;
        //return NULL;

#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

    /* We allocate a pbuf chain of pbufs from the pool. */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

    /* We allocate a continous pbuf */
    //p = pbuf_alloc(PBUF_RAW, len, PBUF_RAM);

    if (p != NULL) {

#if ETH_PAD_SIZE
        pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

        /* pbufs allocated from the RAM pool should be non-chained. */
        LWIP_ASSERT("lpc_rx_queue: pbuf is not contiguous (chained)",
        pbuf_clen(p) <= 1);

        /* Copy the data to intermediate buffer. This is required because
           the driver copies all the data to one continuous packet data buffer. */

        memcpy(p->payload, buf, len);

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

            //memcpy((u8_t*)q->payload, (u8_t*)&buf[l], q->len);
            //l = l + q->len;
        }

        // Acknowledge that packet has been read;
        //wifi_mac_rx_queue_first_entry_free();

#if ETH_PAD_SIZE
        pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

        LINK_STATS_INC(link.recv);
    } else {
        /* drop packet(); */
        //wifi_mac_rx_queue_first_entry_free();
        LINK_STATS_INC(link.memerr);
        LINK_STATS_INC(link.drop);
    }

done:
    wifi_mac_rx_queue_first_entry_free();
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
void
LWIP_ROMFN(ethernetif_input)(struct netif *netif, void *buf, u16_t len)
{
  struct eth_hdr *ethhdr;
  struct pbuf *p;

  /* move received packet into a new pbuf */
  p = low_level_input(netif, buf, len);
  /* no packet could be read, silently ignore this */
  if (p == NULL) return;
  /* points to packet payload, which starts with an Ethernet header */
  ethhdr = p->payload;

  switch (htons(ethhdr->type)) {
  /* IP or ARP packet? */
  case ETHTYPE_IP:
#ifdef LWIP_IPV6
  case ETHTYPE_IPV6:
#endif
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
LWIP_ROMFN(ethernetif_init)(struct netif *netif)
{
    err_t err;
    struct ethernetif *ethernetif;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    ethernetif = mem_malloc(sizeof(struct ethernetif));
    if (ethernetif == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
        return ERR_MEM;
    }

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    //netif->hostname = "netlink";
    /*
    snprintf(g_sLwipHostName, sizeof(g_sLwipHostName), "%s%02X%02X%02X",
             WLAN_DEF_HOST_NAME_PREFIX,
             s_StaInfo.au8Dot11MACAddress[3], s_StaInfo.au8Dot11MACAddress[4], s_StaInfo.au8Dot11MACAddress[5]);
    */
    netif->hostname = g_sLwipHostName;
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
#if LWIP_ARP
    netif->output = etharp_output;
#else /* LWIP_ARP */
    netif->output = NULL; /* not used for PPPoE */
#endif /* LWIP_ARP */
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif
    netif->linkoutput = low_level_output;

    ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

    /* initialize the hardware */
    low_level_init(netif);

#ifdef TRANSPORT_TASK
    /* Packet receive task */
    #if 0
    err = sys_sem_new(&RxReadySem, 0);
    LWIP_ASSERT("RxReadySem creation error", (err == ERR_OK));
    if((rx_thread_handle = sys_thread_new("receive_thread", __packet_rx_task, netif, RX_TASK_STACKSIZE, RX_PRIORITY)) == NULL)
    {
        printf("__packet_rx_task create failed\r\n");
    }
    #endif
	/* Transmit cleanup task */
	err = sys_sem_new(&TxReadySem, 0);
	LWIP_ASSERT("TxReadySem  creation error", (err == ERR_OK));

#ifdef IF_LOOPBACK
    err = sys_sem_new(&TxCleanSem, 0);
	LWIP_ASSERT("TxCleanSem creation error", (err == ERR_OK));
    if( NULL == sys_thread_new("txclean_thread", __packet_tx_task, netif, TX_TASK_STACKSIZE, TX_PRIORITY))
    {
        printf("__packet_tx_task create failed\r\n");
    }
#endif
    wifi_mac_rx_notify_tcp_callback_registration((wifi_mac_rx_notify_tcp_callback_t)wifi_rx_callback);
    wifi_mac_tx_notify_tcp_callback_registration((wifi_mac_tx_notify_tcp_callback_t)wifi_tx_callback);
#endif /* TRANSPORT_TASK */

    return ERR_OK;
}

void dump_buffer(char *pbuf, int len, int mode)
{
    int i;

    printf("%s packet buf addr:%#08x, packet size:%d", mode? "tx": "rx", (uint32_t)pbuf, len);
    for (i=0; i<len; i++)
    {
        if (i%16 == 0)
            printf("\r\n");
        printf("0x%02x ", pbuf[i]);

    }
    printf("\r\n");
}

static int LWIP_ROMFN(wifi_rx_callback)(char *pbuf, u16_t length)
{
    rx_packet_buf = pbuf;
    rx_packet_len = length;
    sys_sem_signal(&RxReadySem);
    return 0;
}

static int LWIP_ROMFN(wifi_tx_callback)(void *userdata)
{
    sys_sem_signal(&TxReadySem);
    return 0;
}

#ifdef TRANSPORT_TASK
#ifdef IF_LOOPBACK
/*
Ether Frame, Type IP */
unsigned char data_frame[] = {
  0x11,0x22,0x33,0xaa, 0xbb,0xcc,0x22,0x33, 0x44,0x55,0x66,0x76, 0x08,0x00,0xff,0xff,
  0x00,0x41,0x20,0x00, 0x00,0x11,0x22,0x33, 0x44,0x55,0xc0,0x3f, 0x0e,0x93,0x88,0x00,
  0x84,0xc9,0xb2,0x05, 0xbd,0xdc,0x00,0x00, 0xa4,0x54,0xa5,0xc1, 0xa5,0x44,0xb8,0x71,
  0xa7,0x5e,0xc4,0xd2, 0xa9,0xd2,0xa9,0x76, 0x0d,0x0a,0xa5,0x48, 0xab,0xd8,0xa5,0xc1,
  0xb0,0xea,0xa5,0x48, 0xb6,0x69,0xa4,0x6a, 0xa6,0x50,0x0d,0x0a, 0xab,0x74,0xba,0xb8,
  0xa6,0x68,0xa4,0x68, 0xac,0xb0,0xa5,0xc1, 0xab,0x65,0xbe,0x57, 0x0d,0x0a,0xa6,0x67,
};


/**
 * @brief	Packet reception task
 *          This task is called when a packet is received. It will
 *          pass the packet to the LWIP core.
 * @param   pvParameters Not used yet
 * @return  none
 */
static void LWIP_ROMFN(__packet_rx_task)(void* pvParameters) {
    struct netif *netif = (struct netif*)pvParameters;
    LWIP_UNUSED_ARG(netif);

    while (1) {
        /* Wait for receive task to wakeup */
        sys_arch_sem_wait(&RxReadySem, 0);
        printf("__packet_rx_task: recevie Rx ready event\r\n");

        /* Process packets until all empty */
        #ifdef RX_PKT_DUMP
        dump_buffer(rx_packet_buf, rx_packet_len, 0);
        #endif
        // Acknowledge that packet has been read;
        wifi_mac_rx_queue_first_entry_free();
    }
}

/**
 * @brief   Transmit cleanup task. This task is called when a transmit interrupt occurs
 *          and reclaims the pbuf and descriptor used for the packet once
 *          the packet has been transferred.
 * @param   pvParameters Not used yet
 * @return  none
 */
static void LWIP_ROMFN(__packet_tx_task)(void* pvParameters) {
    struct netif *netif = (struct netif*)pvParameters;
    u32 status;
    static int full_count = 0;
    static int write_count = 0;

    LWIP_UNUSED_ARG(netif);

    while (1) {
        /* Wait for transmit cleanup task to wakeup */
        if (TX_QUEUE_FULL == wifi_mac_tx_start(&data_frame[0], sizeof(data_frame)))
        {
            full_count++;
            printf("__packet_tx_task: Tx WriteCount: %d  FullCount:%d\r\n", write_count, full_count);
            sys_arch_sem_wait(&TxReadySem, 0);
            printf("__packet_tx_task: recevie Tx ready event to wakeup\r\n");
        }
        else
        {
            full_count = 0;
            write_count++;
            data_frame[0] = write_count;
            printf("__packet_tx_task: Tx WriteCount: %d  FullCount:%d\r\n", write_count, full_count);
        }
    }
}

#else /* IF_LOOPBACK */
/**
 * @brief	Packet reception task
 *          This task is called when a packet is received. It will
 *          pass the packet to the LWIP core.
 * @param   pvParameters Not used yet
 * @return  none
 */
static void LWIP_ROMFN(__packet_rx_task)(void* pvParameters) {
    struct netif *netif = (struct netif*)pvParameters;

    while (1) {
        /* Wait for receive task to wakeup */
        sys_arch_sem_wait(&RxReadySem, 0);
        //printf("__packet_rx_task: recevie Rx ready event\r\n");

        /* Process packets until all empty */
        #ifdef RX_PKT_DUMP
        dump_buffer(rx_packet_buf, rx_packet_len, 0);
        #endif
        ethernetif_input(netif, rx_packet_buf, rx_packet_len);
    }
}

/**
 * @brief   Transmit cleanup task. This task is called when a transmit interrupt occurs
 *          and reclaims the pbuf and descriptor used for the packet once
 *          the packet has been transferred.
 * @param   pvParameters Not used yet
 * @return  none
 */
static void LWIP_ROMFN(__packet_tx_task)(void* pvParameters) {
    struct netif *netif = (struct netif*)pvParameters;

    LWIP_UNUSED_ARG(netif);

    while (1) {
         sys_arch_sem_wait(&TxCleanSem, 0);
        /* Wait for transmit cleanup task to wakeup */
    }
}
#endif /* IF_LOOPBACK */
#endif /* TRANSPORT_TASK */

#if defined(LWIP_ROMBUILD)
#include "port/wlannetif_if.c"
#endif

