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

#ifndef _LE_PTS_APP_H_
#define _LE_PTS_APP_H_

#include "le.h"
#include "le_host.h"
#include "le_gap_if.h"
#include "le_smp.h"
#include "le_pts_app_fn.h"


#ifdef INCLUDE_PTS_APP

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)


enum
{
	LE_APP_USR_MSG_STOP_ADVERTISING = LE_APP_USR_MSG_BASE,
	LE_APP_USR_MSG_START_ADVERTISING,
	LE_APP_USR_MSG_STOP_SCANNING,
	LE_APP_USR_MSG_START_SCANNING,
    
	LE_APP_USR_MSG_DISCONNECTION,
	
	LE_APP_USR_MSG_SHOW_ONLY,
	LE_APP_USR_MSG_TEST_NOTIFY,
	LE_APP_USR_MSG_TEST_INDICATION,

	LE_APP_USR_MSG_TEST_HOST_CMD,
	LE_APP_USR_MSG_DUMP_MM,

	LE_APP_USR_MSG_TEST_TIMER,

	LE_APP_USR_MSG_RPA_TIMEOUT,
    
	LE_APP_USR_MSG_TOP
};

enum
{
	LE_APP_STATE_IDLE = 0,
	LE_APP_STATE_ADVERTISING = 1,
	LE_APP_STATE_SCANNING = 2,
	LE_APP_STATE_CONNECTING = 4,
	LE_APP_STATE_CONNECTED = 8,
};

enum
{
	LE_APP_ACTION_START_ADVERTISING 	= (1 << 0),
	LE_APP_ACTION_END_ADVERTISING 		= (1 << 1),
	LE_APP_ACTION_START_CONNECTING 		= (1 << 2),
	LE_APP_ACTION_CANCEL_CONNECTING 	= (1 << 3),
	LE_APP_ACTION_START_SCANNING 		= (1 << 4),
	LE_APP_ACTION_END_SCANNING          = (1 << 5),
};

typedef struct
{
	UINT8 enable;
	UINT8 duplicate;
} LE_APP_USR_MSG_SCANNING_T;

typedef struct
{
	UINT16			handle;
	UINT16			type;
    UINT16			len;
	UINT8			val[32];
} LE_TEST_PEER_ATTR_T;

typedef struct _LE_TEST_GPA_BV_11_T
{
	UINT8					action;
    UINT16					start_hdl;
    UINT16					end_hdl;
    UINT16					count;
    UINT16					curr_idx;
    LE_TEST_PEER_ATTR_T		*attr;

} LE_TEST_GPA_BV_11_T;

typedef struct
{
	UINT16			len;
	UINT8			data[31];
} LE_PTS_ADV_DATA_T;

typedef struct _LE_HOST_PTS_APP_DATA_T
{
	TASKPACK		task;
	UINT16			state;
    UINT16			action;
    UINT16			flag;
	UINT8			wl_size;
	UINT16			rpa_timeout;

    UINT16			default_mtu;
	UINT16			curr_mtu;
    UINT16			ucp_id;
    
	UINT16			len;
	UINT8			buf[256];

	UINT16			conn_hdl;
    LE_BT_ADDR_T	btaddr;
    UINT8			oob_data[16];
	BOOL			oob_present;
    
    LE_SMP_SC_OOB_DATA_T	peer_oob;
    LE_SMP_SC_OOB_DATA_T	our_oob;

	BOOL			author_enable;
	BOOL			author_enable2;
	BOOL			author_enable3;
	BOOL			author_enable4;
	BOOL			author_enable5;

	LE_TEST_GPA_BV_11_T		gpa_bv_11;

	UINT8			cla_count;
    UINT8			cla_idx;
    UINT8			client_action[10];

    UINT8			bonded;
    UINT8			encrypted;
    UINT8			privacy;

    UINT8			find_name;
    UINT16			peer_name_hdl;

    LE_BT_ADDR_T	pts_addr;
    UINT8			dir_adv;

	LE_PTS_ADV_DATA_T adv;
	LE_PTS_ADV_DATA_T scan;

    UINT8			gap_sec_aut_bv_14;
    UINT8			gap_gat_bv_08;
   
} LE_HOST_PTS_APP_DATA_T;


#endif

#endif
