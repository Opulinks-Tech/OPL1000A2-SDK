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
#include <string.h>


RET_DATA low_level_init_fp_t      low_level_init_adpt;
RET_DATA low_level_output_fp_t    low_level_output_adpt;
RET_DATA low_level_input_fp_t     low_level_input_adpt;
RET_DATA wifi_rx_callback_fp_t    wifi_rx_callback_adpt;
RET_DATA wifi_tx_callback_fp_t    wifi_tx_callback_adpt;
#ifdef TRANSPORT_TASK
RET_DATA __packet_rx_task_fp_t    __packet_rx_task_adpt;
RET_DATA __packet_tx_task_fp_t    __packet_tx_task_adpt;
#endif

RET_DATA ethernetif_init_fp_t     ethernetif_init_adpt;
RET_DATA ethernetif_input_fp_t    ethernetif_input_adpt;
RET_DATA wlanif_input_fp_t        wlanif_input_adpt;

void lwip_load_interface_wlannetif(void)
{
    TxReadySem = 0;
    RxReadySem = 0;
    TxCleanSem = 0;

    low_level_init_adpt         = LWIP_ROMFN(low_level_init);
    low_level_output_adpt       = LWIP_ROMFN(low_level_output);
    low_level_input_adpt        = LWIP_ROMFN(low_level_input);
    wifi_rx_callback_adpt       = LWIP_ROMFN(wifi_rx_callback);
    wifi_tx_callback_adpt       = LWIP_ROMFN(wifi_tx_callback);
#ifdef TRANSPORT_TASK
    __packet_rx_task_adpt       = LWIP_ROMFN(__packet_rx_task);
    __packet_tx_task_adpt       = LWIP_ROMFN(__packet_tx_task);
#endif

    ethernetif_init_adpt        = LWIP_ROMFN(ethernetif_init);
    ethernetif_input_adpt       = LWIP_ROMFN(ethernetif_input);
    wlanif_input_adpt           = LWIP_ROMFN(wlanif_input);
}



/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
void
low_level_init(struct netif *netif)
{
    low_level_init_adpt(netif);
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

err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
    return low_level_output_adpt(netif, p);
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
struct pbuf *
low_level_input(struct netif *netif, void *buf, u16_t len)
{
    return low_level_input_adpt(netif, buf, len);
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
ethernetif_input(struct netif *netif, void *buf, u16_t len)
{
    ethernetif_input_adpt(netif, buf, len);
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
    return ethernetif_init_adpt(netif);
}

void wlanif_input(struct netif *netif, void *buffer, u16_t len, void *arg)
{
    wlanif_input_adpt(netif, buffer, len, arg);
}

int wifi_rx_callback(char *pbuf, u16_t length)
{
    return wifi_rx_callback_adpt(pbuf, length);
}

int wifi_tx_callback(void *userdata)
{
    return wifi_tx_callback_adpt(userdata);
}


#ifdef TRANSPORT_TASK
#ifdef IF_LOOPBACK
/**
 * @brief	Packet reception task
 *          This task is called when a packet is received. It will
 *          pass the packet to the LWIP core.
 * @param   pvParameters Not used yet
 * @return  none
 */
void __packet_rx_task(void* pvParameters) {
    __packet_rx_task_adpt(pvParameters);
}

/**
 * @brief   Transmit cleanup task. This task is called when a transmit interrupt occurs
 *          and reclaims the pbuf and descriptor used for the packet once
 *          the packet has been transferred.
 * @param   pvParameters Not used yet
 * @return  none
 */
void __packet_tx_task(void* pvParameters) {
    __packet_tx_task_adpt(pvParameters);
}

#else /* IF_LOOPBACK */
/**
 * @brief	Packet reception task
 *          This task is called when a packet is received. It will
 *          pass the packet to the LWIP core.
 * @param   pvParameters Not used yet
 * @return  none
 */
void __packet_rx_task(void* pvParameters) {
    __packet_rx_task_adpt(pvParameters);
}

/**
 * @brief   Transmit cleanup task. This task is called when a transmit interrupt occurs
 *          and reclaims the pbuf and descriptor used for the packet once
 *          the packet has been transferred.
 * @param   pvParameters Not used yet
 * @return  none
 */
void __packet_tx_task(void* pvParameters) {
    __packet_tx_task_adpt(pvParameters);
}
#endif /* IF_LOOPBACK */
#endif /* TRANSPORT_TASK */


