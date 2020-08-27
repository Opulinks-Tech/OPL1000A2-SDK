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

#include "lwip_jmptbl_patch.h"
#include "wlannetif_patch.h"
#include "lwip/tcpip_patch.h"

extern void lwip_load_interface_tcp_patch(void);
extern void lwip_load_interface_lwip_helper_patch(void);
extern void lwip_load_interface_dhcp_patch(void);
extern void lwip_load_interface_etharp_patch(void);
extern void lwip_load_interface_ethernet_patch(void);
extern void lwip_load_interface_timeouts_patch(void);
extern void lwip_load_interface_tcp_output_patch(void);

void lwip_module_interface_init_patch(void)
{
    lwip_load_interface_wlannetif_patch();
    lwip_load_interface_tcpip_patch();
    lwip_load_interface_lwip_helper_patch();
    lwip_load_interface_tcp_patch();
    lwip_load_interface_dhcp_patch();
    lwip_load_interface_etharp_patch();
    lwip_load_interface_ethernet_patch();
    lwip_load_interface_timeouts_patch();
    lwip_load_interface_tcp_output_patch();
}
