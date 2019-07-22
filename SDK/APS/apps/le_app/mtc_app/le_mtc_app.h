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

#ifndef _LE_MTC_APP_H_
#define _LE_MTC_APP_H_

#include "le.h"
#include "le_host.h"
#include "le_gap_if.h"
#include "le_smp.h"
#include "le_mtc_app_fn.h"


#ifdef INCLUDE_MTC_APP

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)


enum
{
	LE_APP_USR_MSG_STOP_ADVERTISING = LE_APP_USR_MSG_BASE,
	LE_APP_USR_MSG_START_ADVERTISING,
    
	LE_APP_USR_MSG_DISCONNECTION,
    
	LE_APP_USR_MSG_TOP
};

enum
{
	LE_APP_STATE_IDLE,
	LE_APP_STATE_ADVERTISING 	= (1 << 0),
	LE_APP_STATE_SCANNING 		= (1 << 1),
	LE_APP_STATE_CONNECTING 	= (1 << 2),
	LE_APP_STATE_CONNECTED 		= (1 << 3),
};

enum
{
	LE_APP_ACTION_START_ADVERTISING 	= (1 << 0),
	LE_APP_ACTION_END_ADVERTISING 		= (1 << 1),
	LE_APP_ACTION_START_SCANNING 		= (1 << 2),
	LE_APP_ACTION_END_SCANNING          = (1 << 3),
	LE_APP_ACTION_START_CONNECTING 		= (1 << 4),
	LE_APP_ACTION_END_CONNECTING 	    = (1 << 5),
};


typedef struct
{
	UINT16				conn_hdl;

} LE_APP_USR_MSG_DISCONNECTION_T;

typedef struct _LE_MTC_APP_PEER_T
{
    LE_BT_ADDR_T		btaddr;
	UINT16				conn_hdl;
    UINT16				devid;
	UINT16				mtu;
    UINT16				ucp_id;

	BOOL				role;				
   
} LE_MTC_APP_PEER_T;

typedef struct
{
	UINT16				len;
	UINT8				data[31];
} LE_MTC_ADV_DATA_T;

typedef struct _LE_HOST_MTC_APP_DATA_T
{
	TASKPACK			task;
	UINT16				state;
    UINT16				action;
    UINT16				flag;
	UINT8				wl_size;
	UINT16				rpa_timeout;
    
	UINT16				curr_mtu;
    UINT16				ucp_id;
    
	UINT16				len;
	UINT8				buf[256];

	UINT16				conn_hdl;
    LE_BT_ADDR_T		btaddr;

	UINT16				peer_count;
	LE_MTC_APP_PEER_T	peer[8];

	LE_MTC_ADV_DATA_T 	adv;
	LE_MTC_ADV_DATA_T 	scan;

} LE_HOST_MTC_APP_DATA_T;

#endif

#endif
