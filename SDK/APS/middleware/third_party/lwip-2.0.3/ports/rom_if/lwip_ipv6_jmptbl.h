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
#ifndef LWIP_IPV6_JMPTBL_H
#define LWIP_IPV6_JMPTBL_H

void lwip_load_interface_icmp6(void);
void lwip_load_interface_ethip6(void)
void lwip_load_interface_ip6(void);
void lwip_load_interface_ip6_frag(void);
void lwip_load_interface_ip6_mld6(void);
void lwip_load_interface_ip6_nd6(void);

#endif /* LWIP_IPV6_JMPTBL_H */

