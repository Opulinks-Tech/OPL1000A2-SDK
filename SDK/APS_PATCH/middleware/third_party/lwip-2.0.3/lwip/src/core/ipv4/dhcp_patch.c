/**
 * @file
 * Dynamic Host Configuration Protocol client
 *
 * @defgroup dhcp4 DHCPv4
 * @ingroup ip4
 * DHCP (IPv4) related functions
 * This is a DHCP client for the lwIP TCP/IP stack. It aims to conform
 * with RFC 2131 and RFC 2132.
 *
 * @todo:
 * - Support for interfaces other than Ethernet (SLIP, PPP, ...)
 *
 * Options:
 * @ref DHCP_COARSE_TIMER_SECS (recommended 60 which is a minute)
 * @ref DHCP_FINE_TIMER_MSECS (recommended 500 which equals TCP coarse timer)
 *
 * dhcp_start() starts a DHCP client instance which
 * configures the interface by obtaining an IP address lease and maintaining it.
 *
 * Use dhcp_release() to end the lease and use dhcp_stop()
 * to remove the DHCP client.
 *
 * @see netifapi_dhcp4
 */

/*
 * Copyright (c) 2001-2004 Leon Woestenberg <leon.woestenberg@gmx.net>
 * Copyright (c) 2001-2004 Axon Digital Design B.V., The Netherlands.
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
 * The Swedish Institute of Computer Science and Adam Dunkels
 * are specifically granted permission to redistribute this
 * source code.
 *
 * Author: Leon Woestenberg <leon.woestenberg@gmx.net>
 *
 */

#include "lwip/opt.h"

#if LWIP_IPV4 && LWIP_DHCP /* don't build if not configured for use in lwipopts.h */

#include "lwip/stats.h"
#include "lwip/mem.h"
#include "lwip/udp.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/def.h"
#include "lwip/dhcp.h"
#include "lwip/autoip.h"
#include "lwip/dns.h"
#include "lwip/etharp.h"
#include "lwip/prot/dhcp.h"

#include <string.h>

#if defined(LWIP_ROMBUILD)
#include "ipv4/dhcp_if.h"
#endif

extern void LWIP_ROMFN(dhcp_stop)(struct netif *netif);

extern uint8_t g_dhcp_retry_mode;
extern uint32_t g_lwip_dhcp_autoip_coop_tries;
extern uint32_t g_dhcp_retry_interval;

/** DHCP_CREATE_RAND_XID: if this is set to 1, the xid is created using
 * LWIP_RAND() (this overrides DHCP_GLOBAL_XID)
 */
#ifndef DHCP_CREATE_RAND_XID
#define DHCP_CREATE_RAND_XID        1
#endif

/** Default for DHCP_GLOBAL_XID is 0xABCD0000
 * This can be changed by defining DHCP_GLOBAL_XID and DHCP_GLOBAL_XID_HEADER, e.g.
 *  \#define DHCP_GLOBAL_XID_HEADER "stdlib.h"
 *  \#define DHCP_GLOBAL_XID rand()
 */
#ifdef DHCP_GLOBAL_XID_HEADER
#include DHCP_GLOBAL_XID_HEADER /* include optional starting XID generation prototypes */
#endif


/** Option handling: options are parsed in dhcp_parse_reply
 * and saved in an array where other functions can load them from.
 * This might be moved into the struct dhcp (not necessarily since
 * lwIP is single-threaded and the array is only used while in recv
 * callback). */
enum dhcp_option_idx {
  DHCP_OPTION_IDX_OVERLOAD = 0,
  DHCP_OPTION_IDX_MSG_TYPE,
  DHCP_OPTION_IDX_SERVER_ID,
  DHCP_OPTION_IDX_LEASE_TIME,
  DHCP_OPTION_IDX_T1,
  DHCP_OPTION_IDX_T2,
  DHCP_OPTION_IDX_SUBNET_MASK,
  DHCP_OPTION_IDX_ROUTER,
#if LWIP_DHCP_PROVIDE_DNS_SERVERS
  DHCP_OPTION_IDX_DNS_SERVER,
  DHCP_OPTION_IDX_DNS_SERVER_LAST = DHCP_OPTION_IDX_DNS_SERVER + LWIP_DHCP_PROVIDE_DNS_SERVERS - 1,
#endif /* LWIP_DHCP_PROVIDE_DNS_SERVERS */
#if LWIP_DHCP_GET_NTP_SRV
  DHCP_OPTION_IDX_NTP_SERVER,
  DHCP_OPTION_IDX_NTP_SERVER_LAST = DHCP_OPTION_IDX_NTP_SERVER + LWIP_DHCP_MAX_NTP_SERVERS - 1,
#endif /* LWIP_DHCP_GET_NTP_SRV */
  DHCP_OPTION_IDX_MAX
};

/** Holds the decoded option values, only valid while in dhcp_recv.
    @todo: move this into struct dhcp? */
extern u32_t dhcp_rx_options_val[DHCP_OPTION_IDX_MAX];
/** Holds a flag which option was received and is contained in dhcp_rx_options_val,
    only valid while in dhcp_recv.
    @todo: move this into struct dhcp? */
extern u8_t  dhcp_rx_options_given[DHCP_OPTION_IDX_MAX];

/** DHCP_OPTION_MAX_MSG_SIZE is set to the MTU
 * MTU is checked to be big enough in dhcp_start */
#define DHCP_MAX_MSG_LEN(netif)        (netif->mtu)
#define DHCP_MAX_MSG_LEN_MIN_REQUIRED  576
/** Minimum length for reply before packet is parsed */
#define DHCP_MIN_REPLY_LEN             44

#define REBOOT_TRIES                2

#if LWIP_DNS && LWIP_DHCP_MAX_DNS_SERVERS
#if DNS_MAX_SERVERS > LWIP_DHCP_MAX_DNS_SERVERS
#define LWIP_DHCP_PROVIDE_DNS_SERVERS LWIP_DHCP_MAX_DNS_SERVERS
#else
#define LWIP_DHCP_PROVIDE_DNS_SERVERS DNS_MAX_SERVERS
#endif
#else
#define LWIP_DHCP_PROVIDE_DNS_SERVERS 0
#endif

static u8_t dhcp_discover_request_options[] = {
  DHCP_OPTION_SUBNET_MASK,
  DHCP_OPTION_ROUTER,
  DHCP_OPTION_BROADCAST
#if LWIP_DHCP_PROVIDE_DNS_SERVERS
  , DHCP_OPTION_DNS_SERVER
#endif /* LWIP_DHCP_PROVIDE_DNS_SERVERS */
/**add options for support more router**/
   ,DHCP_OPTION_DOMAIN_NAME,
    DHCP_OPTION_NB_TINS,
    DHCP_OPTION_NB_TINT,
    DHCP_OPTION_NB_TIS,
    DHCP_OPTION_PRD,
    DHCP_OPTION_STATIC_ROUTER,
    DHCP_OPTION_CLASSLESS_STATIC_ROUTER,
    DHCP_OPTION_VSN

#if LWIP_DHCP_GET_NTP_SRV
  , DHCP_OPTION_NTP
#endif /* LWIP_DHCP_GET_NTP_SRV */
  };


#define dhcp_option_given(dhcp, idx)          (dhcp_rx_options_given[idx] != 0)
#define dhcp_got_option(dhcp, idx)            (dhcp_rx_options_given[idx] = 1)
#define dhcp_clear_option(dhcp, idx)          (dhcp_rx_options_given[idx] = 0)
#define dhcp_clear_all_options(dhcp)          (memset(dhcp_rx_options_given, 0, sizeof(dhcp_rx_options_given)))
#define dhcp_get_option_value(dhcp, idx)      (dhcp_rx_options_val[idx])
#define dhcp_set_option_value(dhcp, idx, val) (dhcp_rx_options_val[idx] = (val))


extern struct udp_pcb *dhcp_pcb;
extern LWIP_RETDATA int dhcp_does_arp_check_flag;

#if OPL_DISABLE_PHY_EQU
#include "controller_wifi_patch.h"
#endif

/*
 * Set the DHCP state of a DHCP client.
 *
 * If the state changed, reset the number of tries.
 */
void
dhcp_set_state(struct dhcp *dhcp, u8_t new_state)
{
  if (new_state != dhcp->state) {
    dhcp->state = new_state;
    dhcp->tries = 0;
    dhcp->request_timeout = 0;
  }
}

/**
 * Start the DHCP process, discover a DHCP server.
 *
 * @param netif the netif under DHCP control
 */
static err_t
dhcp_discover_patch(struct netif *netif)
{
  struct dhcp *dhcp = netif_dhcp_data(netif);
  err_t result = ERR_OK;
  u16_t msecs;
  u8_t i;
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_discover()\n"));
  ip4_addr_set_any(&dhcp->offered_ip_addr);
  dhcp_set_state(dhcp, DHCP_STATE_SELECTING);
  /* create and initialize the DHCP message header */
  result = dhcp_create_msg(netif, dhcp, DHCP_DISCOVER);
  if (result == ERR_OK) {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_discover: making request\n"));

    dhcp_option(dhcp, DHCP_OPTION_MAX_MSG_SIZE, DHCP_OPTION_MAX_MSG_SIZE_LEN);
    dhcp_option_short(dhcp, DHCP_MAX_MSG_LEN(netif));

#if OPL_DHCP
#if LWIP_NETIF_HOSTNAME
    dhcp_option_hostname(dhcp, netif);
#endif /* LWIP NETIF HOSTNAME */
#endif

    dhcp_option(dhcp, DHCP_OPTION_PARAMETER_REQUEST_LIST, LWIP_ARRAYSIZE(dhcp_discover_request_options));
    for (i = 0; i < LWIP_ARRAYSIZE(dhcp_discover_request_options); i++) {
      dhcp_option_byte(dhcp, dhcp_discover_request_options[i]);
    }
    dhcp_option_trailer(dhcp);

    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_discover: realloc()ing\n"));
    pbuf_realloc(dhcp->p_out, sizeof(struct dhcp_msg) - DHCP_OPTIONS_LEN + dhcp->options_out_len);

    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_discover: sendto(DISCOVER, IP_ADDR_BROADCAST, DHCP_SERVER_PORT)\n"));
    udp_sendto_if_src(dhcp_pcb, dhcp->p_out, IP_ADDR_BROADCAST, DHCP_SERVER_PORT, netif, IP4_ADDR_ANY);
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_discover: deleting()ing\n"));
    dhcp_delete_msg(dhcp);
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp_discover: SELECTING\n"));
  } else {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS, ("dhcp_discover: could not allocate DHCP request\n"));
  }
  if (dhcp->tries < 255) {
    dhcp->tries++;
  }
#if LWIP_DHCP_AUTOIP_COOP
  if (dhcp->tries >= g_lwip_dhcp_autoip_coop_tries && dhcp->autoip_coop_state == DHCP_AUTOIP_COOP_STATE_OFF) {
    dhcp->autoip_coop_state = DHCP_AUTOIP_COOP_STATE_ON;
    autoip_start(netif);
  }
#endif /* LWIP_DHCP_AUTOIP_COOP */
  if(g_dhcp_retry_mode==0){
     msecs = g_dhcp_retry_interval;
  }
  else{
     msecs = (dhcp->tries < 6 ? 1 << dhcp->tries : 60) * g_dhcp_retry_interval;
  }
  dhcp->request_timeout = (msecs + DHCP_FINE_TIMER_MSECS - 1) / DHCP_FINE_TIMER_MSECS;
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp_discover(): set request timeout %"U16_F" msecs\n", msecs));
  return result;
}

extern struct eth_addr src_hwaddr;
extern struct eth_addr dhcp_svr_hwaddr;

static void dhcp_recv_patch(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
  struct netif *netif = ip_current_input_netif();
  struct dhcp *dhcp = netif_dhcp_data(netif);
  struct dhcp_msg *reply_msg = (struct dhcp_msg *)p->payload;
  u8_t msg_type;
  u8_t i;

  LWIP_UNUSED_ARG(arg);

  /* Caught DHCP message from netif that does not have DHCP enabled? -> not interested */
  if ((dhcp == NULL) || (dhcp->pcb_allocated == 0)) {
    goto free_pbuf_and_return;
  }

  LWIP_ASSERT("invalid server address type", IP_IS_V4(addr));

  LWIP_DEBUGF(DHCP_DEBUG, ("etharp_input: - src hwaddr: %02"X16_F":%02"X16_F":%02"X16_F":%02"X16_F":%02"X16_F":%02"X16_F"\n",
  (u16_t)(src_hwaddr.addr[0]), (u16_t)src_hwaddr.addr[1], (u16_t)src_hwaddr.addr[2],
  (u16_t)src_hwaddr.addr[3], (u16_t)src_hwaddr.addr[4], (u16_t)src_hwaddr.addr[5]));


  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_recv(pbuf = %p) from DHCP server %"U16_F".%"U16_F".%"U16_F".%"U16_F" port %"U16_F"\n", (void*)p,
    ip4_addr1_16(ip_2_ip4(addr)), ip4_addr2_16(ip_2_ip4(addr)), ip4_addr3_16(ip_2_ip4(addr)), ip4_addr4_16(ip_2_ip4(addr)), port));
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("pbuf->len = %"U16_F"\n", p->len));
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("pbuf->tot_len = %"U16_F"\n", p->tot_len));
  /* prevent warnings about unused arguments */
  LWIP_UNUSED_ARG(pcb);
  LWIP_UNUSED_ARG(addr);
  LWIP_UNUSED_ARG(port);

  LWIP_ASSERT("reply wasn't freed", dhcp->msg_in == NULL);

  if (p->len < DHCP_MIN_REPLY_LEN) {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING, ("DHCP reply message or pbuf too short\n"));
    goto free_pbuf_and_return;
  }

  if (reply_msg->op != DHCP_BOOTREPLY) {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING, ("not a DHCP reply message, but type %"U16_F"\n", (u16_t)reply_msg->op));
    goto free_pbuf_and_return;
  }
  /* iterate through hardware address and match against DHCP message */
  for (i = 0; i < netif->hwaddr_len && i < NETIF_MAX_HWADDR_LEN && i < DHCP_CHADDR_LEN; i++) {
    if (netif->hwaddr[i] != reply_msg->chaddr[i]) {
      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING,
        ("netif->hwaddr[%"U16_F"]==%02"X16_F" != reply_msg->chaddr[%"U16_F"]==%02"X16_F"\n",
        (u16_t)i, (u16_t)netif->hwaddr[i], (u16_t)i, (u16_t)reply_msg->chaddr[i]));
      goto free_pbuf_and_return;
    }
  }
  /* match transaction ID against what we expected */
  if (lwip_ntohl(reply_msg->xid) != dhcp->xid) {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING,
      ("transaction id mismatch reply_msg->xid(%"X32_F")!=dhcp->xid(%"X32_F")\n",lwip_ntohl(reply_msg->xid),dhcp->xid));
    goto free_pbuf_and_return;
  }
  /* option fields could be unfold? */
  if (dhcp_parse_reply(dhcp, p) != ERR_OK) {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,
      ("problem unfolding DHCP message - too short on memory?\n"));
    goto free_pbuf_and_return;
  }

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("searching DHCP_OPTION_MESSAGE_TYPE\n"));
  /* obtain pointer to DHCP message type */
  if (!dhcp_option_given(dhcp, DHCP_OPTION_IDX_MSG_TYPE)) {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING, ("DHCP_OPTION_MESSAGE_TYPE option not found\n"));
    goto free_pbuf_and_return;
  }

  /* read DHCP message type */
  msg_type = (u8_t)dhcp_get_option_value(dhcp, DHCP_OPTION_IDX_MSG_TYPE);

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("DHCP msg type %d\n", msg_type));


  /* message type is DHCP ACK? */
  if (msg_type == DHCP_ACK) {

    dhcp_svr_hwaddr = src_hwaddr;
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("DHCP_ACK received\n"));
    /* in requesting state? */
    if (dhcp->state == DHCP_STATE_REQUESTING) {
      dhcp_handle_ack(netif);
#if DHCP_DOES_ARP_CHECK
      if ((netif->flags & NETIF_FLAG_ETHARP) != 0 && (dhcp_does_arp_check_flag == 1)) {
        /* check if the acknowledged lease address is already in use */
        dhcp_check(netif);
      } else {
        /* bind interface to the acknowledged lease address */
        dhcp_bind(netif);
      }
#else
      /* bind interface to the acknowledged lease address */
      dhcp_bind(netif);
#endif
    }
    /* already bound to the given lease address? */
    else if ((dhcp->state == DHCP_STATE_REBOOTING) || (dhcp->state == DHCP_STATE_REBINDING) ||
             (dhcp->state == DHCP_STATE_RENEWING)) {
      dhcp_handle_ack(netif);
      dhcp_bind(netif);
    }
  }
  /* received a DHCP_NAK in appropriate state? */
  else if ((msg_type == DHCP_NAK) &&
    ((dhcp->state == DHCP_STATE_REBOOTING) || (dhcp->state == DHCP_STATE_REQUESTING) ||
     (dhcp->state == DHCP_STATE_REBINDING) || (dhcp->state == DHCP_STATE_RENEWING  ))) {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("DHCP_NAK received\n"));
    dhcp_handle_nak(netif);
  }
  /* received a DHCP_OFFER in DHCP_STATE_SELECTING state? */
  else if ((msg_type == DHCP_OFFER) && (dhcp->state == DHCP_STATE_SELECTING)) {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("DHCP_OFFER received in DHCP_STATE_SELECTING state\n"));
    dhcp->request_timeout = 0;
    /* remember offered lease */
    dhcp_handle_offer(netif);
  }

free_pbuf_and_return:
  if (dhcp != NULL) {
    dhcp->msg_in = NULL;
  }
  pbuf_free(p);
}

uint32_t g_restored_ip_addr = 0;

bool dhcp_ip_addr_restore(struct netif *netif)
{
    struct dhcp *dhcp = netif_dhcp_data(netif);
    if (g_restored_ip_addr == IPADDR_ANY) {
        return false;
    }
    else {
        dhcp->offered_ip_addr.addr = g_restored_ip_addr;
        return true;
    }
}

void dhcp_ip_addr_store(struct netif *netif)
{
    struct dhcp *dhcp;
    uint32_t ip_addr;
    LWIP_ERROR("dhcp_bind: netif != NULL", (netif != NULL), return;);
    dhcp = netif_dhcp_data(netif);
    LWIP_ERROR("dhcp_bind: dhcp != NULL", (dhcp != NULL), return;);

    ip_addr = dhcp->offered_ip_addr.addr;
    if (g_restored_ip_addr != ip_addr) {
      g_restored_ip_addr = ip_addr;
    }
}

err_t dhcp_start_patch(struct netif *netif)
{
  struct dhcp *dhcp;
  err_t result;

  LWIP_ERROR("netif != NULL", (netif != NULL), return ERR_ARG;);
  LWIP_ERROR("netif is not up, old style port?", netif_is_up(netif), return ERR_ARG;);
  dhcp = netif_dhcp_data(netif);
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp_start(netif=%p) %c%c%"U16_F"\n", (void*)netif, netif->name[0], netif->name[1], (u16_t)netif->num));
  
#if OPL_DISABLE_PHY_EQU
  ctrl_wifi_reg_write(WIFI_PHY_BYP_KEEP_ADDR, WIFI_PHY_DISABLE_EQU_VAL);
#endif
  
  /* check MTU of the netif */
  if (netif->mtu < DHCP_MAX_MSG_LEN_MIN_REQUIRED) {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_start(): Cannot use this netif with DHCP: MTU is too small\n"));
    return ERR_MEM;
  }

  /* no DHCP client attached yet? */
  if (dhcp == NULL) {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_start(): mallocing new DHCP client\n"));
    dhcp = (struct dhcp *)mem_malloc(sizeof(struct dhcp));
    if (dhcp == NULL) {
      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_start(): could not allocate dhcp\n"));
      return ERR_MEM;
    }

    /* store this dhcp client in the netif */
    netif_set_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP, dhcp);
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_start(): allocated dhcp"));
  /* already has DHCP client attached */
  } else {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp_start(): restarting DHCP configuration\n"));
    LWIP_ASSERT("pbuf p_out wasn't freed", dhcp->p_out == NULL);
    LWIP_ASSERT("reply wasn't freed", dhcp->msg_in == NULL );

    if (dhcp->pcb_allocated != 0) {
      dhcp_dec_pcb_refcount(); /* free DHCP PCB if not needed any more */
    }
    /* dhcp is cleared below, no need to reset flag*/
  }

  /* clear data structure */
  memset(dhcp, 0, sizeof(struct dhcp));
  /* dhcp_set_state(&dhcp, DHCP_STATE_OFF); */

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_start(): starting DHCP configuration\n"));

  if (dhcp_inc_pcb_refcount() != ERR_OK) { /* ensure DHCP PCB is allocated */
    return ERR_MEM;
  }
  dhcp->pcb_allocated = 1;

#if LWIP_DHCP_CHECK_LINK_UP
  if (!netif_is_link_up(netif)) {
    /* set state INIT and wait for dhcp_network_changed() to call dhcp_discover() */
    dhcp_set_state(dhcp, DHCP_STATE_INIT);
    return ERR_OK;
  }
#endif /* LWIP_DHCP_CHECK_LINK_UP */


#ifdef OPL_DHCP
  // Try to restore last valid ip address obtained from DHCP server.
  // If no valid ip is available, run dhcp_discover instead.
  if(dhcp_ip_addr_restore(netif)) {
    dhcp_set_state(dhcp, DHCP_STATE_BOUND);
    dhcp_network_changed(netif);
    return ERR_OK;
  }
#endif

  /* (re)start the DHCP negotiation */
  result = dhcp_discover(netif);
  if (result != ERR_OK) {
    /* free resources allocated above */
    dhcp_stop(netif);
    return ERR_MEM;
  }
  return result;
}

/**
 * @ingroup dhcp4
 * Remove the DHCP client from the interface.
 *
 * @param netif The network interface to stop DHCP on
 */
void
dhcp_stop_patch(struct netif *netif)
{
  LWIP_ROMFN(dhcp_stop)(netif);
    
#if OPL_DISABLE_PHY_EQU
  ctrl_wifi_reg_write(WIFI_PHY_BYP_KEEP_ADDR, WIFI_PHY_ENABLE_EQU_VAL);
#endif
}

/**
 * Bind the interface to the offered IP address.
 *
 * @param netif network interface to bind to the offered address
 */
static void dhcp_bind_patch(struct netif *netif)
{
  u32_t timeout;
  struct dhcp *dhcp;
  ip4_addr_t sn_mask, gw_addr;
  LWIP_ERROR("dhcp_bind: netif != NULL", (netif != NULL), return;);
  dhcp = netif_dhcp_data(netif);
  LWIP_ERROR("dhcp_bind: dhcp != NULL", (dhcp != NULL), return;);
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_bind(netif=%p) %c%c%"U16_F"\n", (void*)netif, netif->name[0], netif->name[1], (u16_t)netif->num));

  /* reset time used of lease */
  dhcp->lease_used = 0;

  if (dhcp->offered_t0_lease != 0xffffffffUL) {
     /* set renewal period timer */
     LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_bind(): t0 renewal timer %"U32_F" secs\n", dhcp->offered_t0_lease));
     timeout = (dhcp->offered_t0_lease + DHCP_COARSE_TIMER_SECS / 2) / DHCP_COARSE_TIMER_SECS;
     if (timeout > 0xffff) {
       timeout = 0xffff;
     }
     dhcp->t0_timeout = (u16_t)timeout;
     if (dhcp->t0_timeout == 0) {
       dhcp->t0_timeout = 1;
     }
     LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp_bind(): set request timeout %"U32_F" msecs\n", dhcp->offered_t0_lease*1000));
  }

  /* temporary DHCP lease? */
  if (dhcp->offered_t1_renew != 0xffffffffUL) {
    /* set renewal period timer */
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_bind(): t1 renewal timer %"U32_F" secs\n", dhcp->offered_t1_renew));
    timeout = (dhcp->offered_t1_renew + DHCP_COARSE_TIMER_SECS / 2) / DHCP_COARSE_TIMER_SECS;
    if (timeout > 0xffff) {
      timeout = 0xffff;
    }
    dhcp->t1_timeout = (u16_t)timeout;
    if (dhcp->t1_timeout == 0) {
      dhcp->t1_timeout = 1;
    }
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp_bind(): set request timeout %"U32_F" msecs\n", dhcp->offered_t1_renew*1000));
    dhcp->t1_renew_time = dhcp->t1_timeout;
  }
  /* set renewal period timer */
  if (dhcp->offered_t2_rebind != 0xffffffffUL) {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_bind(): t2 rebind timer %"U32_F" secs\n", dhcp->offered_t2_rebind));
    timeout = (dhcp->offered_t2_rebind + DHCP_COARSE_TIMER_SECS / 2) / DHCP_COARSE_TIMER_SECS;
    if (timeout > 0xffff) {
      timeout = 0xffff;
    }
    dhcp->t2_timeout = (u16_t)timeout;
    if (dhcp->t2_timeout == 0) {
      dhcp->t2_timeout = 1;
    }
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp_bind(): set request timeout %"U32_F" msecs\n", dhcp->offered_t2_rebind*1000));
    dhcp->t2_rebind_time = dhcp->t2_timeout;
  }

  /* If we have sub 1 minute lease, t2 and t1 will kick in at the same time. */
  if ((dhcp->t1_timeout >= dhcp->t2_timeout) && (dhcp->t2_timeout > 0)) {
    dhcp->t1_timeout = 0;
  }

  if (dhcp->subnet_mask_given) {
    /* copy offered network mask */
    ip4_addr_copy(sn_mask, dhcp->offered_sn_mask);
  } else {
    /* subnet mask not given, choose a safe subnet mask given the network class */
    u8_t first_octet = ip4_addr1(&dhcp->offered_ip_addr);
    if (first_octet <= 127) {
      ip4_addr_set_u32(&sn_mask, PP_HTONL(0xff000000UL));
    } else if (first_octet >= 192) {
      ip4_addr_set_u32(&sn_mask, PP_HTONL(0xffffff00UL));
    } else {
      ip4_addr_set_u32(&sn_mask, PP_HTONL(0xffff0000UL));
    }
  }

  ip4_addr_copy(gw_addr, dhcp->offered_gw_addr);
  /* gateway address not given? */
  if (ip4_addr_isany_val(gw_addr)) {
    /* copy network address */
    ip4_addr_get_network(&gw_addr, &dhcp->offered_ip_addr, &sn_mask);
    /* use first host address on network as gateway */
    ip4_addr_set_u32(&gw_addr, ip4_addr_get_u32(&gw_addr) | PP_HTONL(0x00000001UL));
  }

#if LWIP_DHCP_AUTOIP_COOP
  if (dhcp->autoip_coop_state == DHCP_AUTOIP_COOP_STATE_ON) {
    autoip_stop(netif);
    dhcp->autoip_coop_state = DHCP_AUTOIP_COOP_STATE_OFF;
  }
#endif /* LWIP_DHCP_AUTOIP_COOP */

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_STATE, ("dhcp_bind(): IP: 0x%08"X32_F" SN: 0x%08"X32_F" GW: 0x%08"X32_F"\n",
    ip4_addr_get_u32(&dhcp->offered_ip_addr), ip4_addr_get_u32(&sn_mask), ip4_addr_get_u32(&gw_addr)));
  /* netif is now bound to DHCP leased address - set this before assigning the address
     to ensure the callback can use dhcp_supplied_address() */
  dhcp_set_state(dhcp, DHCP_STATE_BOUND);

  netif_set_addr(netif, &dhcp->offered_ip_addr, &sn_mask, &gw_addr);
  /* interface is used by routing now that an address is set */

#ifdef OPL_DHCP
  dhcp_ip_addr_store(netif);
#endif

  /* Opulinks add start. */
  if (dhcp->cb != NULL) {
      dhcp->cb(netif);
  }
  /* Opulinks add end. */
  
#if OPL_DISABLE_PHY_EQU
  ctrl_wifi_reg_write(WIFI_PHY_BYP_KEEP_ADDR, WIFI_PHY_ENABLE_EQU_VAL);
#endif
}

/**
 * The DHCP timer that checks for lease renewal/rebind timeouts.
 * Must be called once a minute (see @ref DHCP_COARSE_TIMER_SECS).
 */
void
dhcp_coarse_tmr_patch(void)
{
  struct netif *netif = netif_list;
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_coarse_tmr()\n"));
  /* iterate through all network interfaces */
  while (netif != NULL) {
    /* only act on DHCP configured interfaces */
    struct dhcp *dhcp = netif_dhcp_data(netif);
    if ((dhcp != NULL) && (dhcp->state != DHCP_STATE_OFF)) {
      /* compare lease time to expire timeout */
      if (dhcp->t0_timeout && (++dhcp->lease_used == dhcp->t0_timeout)) {
#if OPL_DISABLE_PHY_EQU
        ctrl_wifi_reg_write(WIFI_PHY_BYP_KEEP_ADDR, WIFI_PHY_DISABLE_EQU_VAL);
#endif
        LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp_coarse_tmr(): t0 timeout\n"));
        /* this clients' lease time has expired */
        dhcp_release(netif);
        dhcp_discover(netif);
      /* timer is active (non zero), and triggers (zeroes) now? */
      } else if (dhcp->t2_rebind_time && (dhcp->t2_rebind_time-- == 1)) {
#if OPL_DISABLE_PHY_EQU
        ctrl_wifi_reg_write(WIFI_PHY_BYP_KEEP_ADDR, WIFI_PHY_DISABLE_EQU_VAL);
#endif
        LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp_coarse_tmr(): t2 timeout\n"));
        /* this clients' rebind timeout triggered */
        dhcp_t2_timeout(netif);
      /* timer is active (non zero), and triggers (zeroes) now */
      } else if (dhcp->t1_renew_time && (dhcp->t1_renew_time-- == 1)) {
#if OPL_DISABLE_PHY_EQU
        ctrl_wifi_reg_write(WIFI_PHY_BYP_KEEP_ADDR, WIFI_PHY_DISABLE_EQU_VAL);
#endif
        LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp_coarse_tmr(): t1 timeout\n"));
        /* this clients' renewal timeout triggered */
        dhcp_t1_timeout(netif);
      }
    }
    /* proceed to next netif */
    netif = netif->next;
  }
}

void lwip_load_interface_dhcp_patch(void)
{
    g_dhcp_retry_mode     = 0;
    g_dhcp_retry_interval = 2000;
    
    dhcp_discover_adpt  =  dhcp_discover_patch;
    dhcp_recv_adpt      =  dhcp_recv_patch;
    dhcp_start_adpt     =  dhcp_start_patch;
    dhcp_stop_adpt      =  dhcp_stop_patch;
    dhcp_bind_adpt      =  dhcp_bind_patch;
    dhcp_coarse_tmr_adpt = dhcp_coarse_tmr_patch;
}

#endif /* LWIP_IPV4 && LWIP_DHCP */
