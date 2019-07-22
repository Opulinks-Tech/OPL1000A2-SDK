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

#ifndef _WIFI_MAC_COMMAN_H_
#define _WIFI_MAC_COMMAN_H_

#define TRUE        1
#define FLASE       0

#define RESULT_SUCCESS        0
#define RESULT_FAIL           1

#ifndef MAX
#define MAX(a, b)                (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b)                (((a) < (b)) ? (a) : (b))
#endif

#define MAX_ADD16(a, b)          (((a) > (0xFFFF - (b))) ? 0xFFFF : ((a) + (b)))
#define MAX_ADD32(a, b)          (((a) > (0xFFFFFFFF - (b))) ? 0xFFFFFFFF : ((a) + (b)))

#define IEEE80211_SUPPORT_WPA

#define CHIP_DEMO_WIFI
#include <stdbool.h>
#include "ieee80211_mac_header.h"
#include "linked_list.h"
#include "memory_pool_ctrl.h"
//#include "wifi_mac_common.h"
//#include "wifi_mac_eth.h"
#include "wifi_mac_mgmt_fsm.h"
#include "wifi_mac_dcf.h"
#include "wifi_mac_queue.h"
#include "wifi_mac_types.h"
#include "wifi_mac_frame.h"
#include "wifi_mac_sta.h"

#define DBGPRINT(fmt, args...)        printf(fmt, ##args)
#define DBGDUMP(txt, data, len) {                                   \
  int i, j ;                                                        \
                                                                    \
  DBGPRINT("----<%s(length: %d)>----\r\n", txt, len);               \
                                                                    \
	for( i=0 ; i<len ; i+=16 )                                        \
	{                                                                 \
		DBGPRINT("%05x: ", i);                                          \
		for( j=i ; (j<i+16) && (j<len) ; j++ )                          \
    {                                                               \
		  DBGPRINT("%02x ", (unsigned char)*(data+j));                  \
    }                                                               \
		DBGPRINT("  ");                                                 \
		for( j=i ; (j<i+16) && (j<len) ; j++ )                          \
		{                                                               \
			if( (*(data+j) >= 32) && (*(data+j) <= 127) )                 \
			DBGPRINT("%c", *(data+j));                                    \
			else                                                          \
			DBGPRINT(".");                                                \
		}                                                               \
		DBGPRINT("\r\n");                                               \
	}                                                                 \
	if (len%16 == 0) {                                                \
    DBGPRINT("\r\n");                                               \
  }                                                                 \
  else {                                                            \
    DBGPRINT("-");                                                  \
  }                                                                 \
  DBGPRINT("----<end %s>----\r\n", txt);                            \
}

#endif /* _WIFI_MAC_COMMAN_H_ */

