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

#ifndef _BLE_HCI_IF_H_
#define _BLE_HCI_IF_H_


// HCI packet Type
#define LE_HCI_PACKET_COMMAND		1
#define LE_HCI_PACKET_ACL_DATA		2
#define LE_HCI_PACKET_SYNCHRONOUS	3
#define LE_HCI_PACKET_EVENT			4

/* Advertising types */
#define LE_HCI_ADV_TYPE_ADV_IND            			(0)
#define LE_HCI_ADV_TYPE_ADV_DIRECT_IND_HD  			(1)
#define LE_HCI_ADV_TYPE_ADV_SCAN_IND       			(2)
#define LE_HCI_ADV_TYPE_ADV_NONCONN_IND    			(3)
#define LE_HCI_ADV_TYPE_ADV_DIRECT_IND_LD  			(4)

/* Own address types */
#define LE_HCI_OWN_ADDR_PUBLIC         				(0)
#define LE_HCI_OWN_ADDR_RANDOM         				(1)
#define LE_HCI_OWN_ADDR_PRIV_PUB       				(2)
#define LE_HCI_OWN_ADDR_PRIV_RAND      				(3)

/* Advertisement peer address Type */
#define LE_HCI_ADV_PEER_ADDR_PUBLIC        			(0)
#define LE_HCI_ADV_PEER_ADDR_RANDOM        			(1)

// Advertiser Channels
#define LE_HCI_ADV_CHANNEL_37                       0x01
#define LE_HCI_ADV_CHANNEL_38                       0x02
#define LE_HCI_ADV_CHANNEL_39                       0x04

#define LE_HCI_ADV_CHANNEL_MAP_MASK                 (LE_HCI_ADV_CHANNEL_37 | LE_HCI_ADV_CHANNEL_38 | LE_HCI_ADV_CHANNEL_39)

// Advertiser White List Policy
#define LE_HCI_ADV_FILT_NONE               			(0)
#define LE_HCI_ADV_FILT_SCAN               			(1)
#define LE_HCI_ADV_FILT_CONN               			(2)
#define LE_HCI_ADV_FILT_BOTH               			(3)

/* Scan type */
#define LE_HCI_SCAN_PASSIVE							(0)
#define LE_HCI_SCAN_ACTIVE							(1)

// Scan White List Policy
#define LE_HCI_SCAN_FILT_NONE              			(0)
#define LE_HCI_SCAN_FILT_WHITELIST         			(1)
#define LE_HCI_SCAN_FILT_UNDIRECT_INITA_RPA    		(2)
#define LE_HCI_SCAN_FILT_WHITELIST_INITA_RPA      	(3)

// Init White List Policy
#define LE_HCI_INIT_FILT_NONE              			(0)
#define LE_HCI_INIT_FILT_WHITELIST         			(1)

#define LE_HCI_CONN_MIN_CE_LEN     					0x0010
#define LE_HCI_CONN_MAX_CE_LEN     					0x0300

#define LE_HCI_PHY_1M                   			(1)
#define LE_HCI_PHY_2M                   			(2)
#define LE_HCI_PHY_CODED                			(3)

/* --- LE set default PHY (OCF 0x0031) */
#define LE_HCI_PHY_NO_TX_PREF_MASK      			(0x01)
#define LE_HCI_PHY_NO_RX_PREF_MASK      			(0x02)
#define LE_HCI_PHY_1M_PREF_MASK         			(0x01)
#define LE_HCI_PHY_2M_PREF_MASK         			(0x02)
#define LE_HCI_PHY_CODED_PREF_MASK      			(0x04)

#define BLE_HCI_LE_PHY_PREF_MASK_ALL    			(LE_HCI_PHY_1M_PREF_MASK | LE_HCI_PHY_2M_PREF_MASK | LE_HCI_PHY_CODED_PREF_MASK)

#define LE_HCI_PHY_CODED_ANY        				(0x0000)
#define LE_HCI_PHY_CODED_S2_PREF    				(0x0001)
#define LE_HCI_PHY_CODED_S8_PREF    				(0x0002)

#endif
