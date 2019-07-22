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
#ifndef LWIP_IPV4_JMPTBL_H
#define LWIP_IPV4_JMPTBL_H

void lwip_load_interface_icmp(void);
void lwip_load_interface_igmp(void);
void lwip_load_interface_etharp(void);
void lwip_load_interface_ip4(void);
void lwip_load_interface_ip4_frag(void);
void lwip_load_interface_dhcp(void);
void lwip_load_interface_autoip(void);

#endif /* LWIP_IPV4_JMPTBL_H */

