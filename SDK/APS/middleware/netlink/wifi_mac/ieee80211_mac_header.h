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

#ifndef _IEEE80211_MAC_HEADER_H_
#define _IEEE80211_MAC_HEADER_H_

#include "wifi_mac_types.h"


/*
 * DS bit usage
 *
 * TA = transmitter address
 * RA = receiver address
 * DA = destination address
 * SA = source address
 *
 * ToDS    FromDS  A1(RA)  A2(TA)  A3      A4      Use
 * -----------------------------------------------------------------
 *  0       0       DA      SA      BSSID   -       IBSS/DLS
 *  0       1       DA      BSSID   SA      -       AP -> STA
 *  1       0       BSSID   SA      DA      -       AP <- STA
 *  1       1       RA      TA      DA      SA      unspecified (WDS)
 */

//To DS and From DS fields
#define FC_DS_IBSS			0
#define FC_DS_TOAP			1
#define FC_DS_FROMAP        2
#define FC_DS_WDS			3

#define FCS_LEN         4

#define IEEE80211_FCTRL_VERS            0x0003
#define IEEE80211_FCTRL_FTYPE           0x000c
#define IEEE80211_FCTRL_STYPE           0x00f0
#define IEEE80211_FCTRL_TODS            0x0100
#define IEEE80211_FCTRL_FROMDS          0x0200
#define IEEE80211_FCTRL_MOREFRAGS       0x0400
#define IEEE80211_FCTRL_RETRY           0x0800
#define IEEE80211_FCTRL_PM              0x1000
#define IEEE80211_FCTRL_MOREDATA        0x2000
#define IEEE80211_FCTRL_PROTECTED       0x4000
#define IEEE80211_FCTRL_ORDER           0x8000
#define IEEE80211_FCTRL_CTL_EXT         0x0f00

#define IEEE80211_SCTRL_FRAG            0x000F
#define IEEE80211_SCTRL_SEQ             0xFFF0

#define IEEE80211_FTYPE_NUM             4
#define IEEE80211_STYPE_NUM             16
#define IEEE80211_FTYPE_SFT             2
#define IEEE80211_STYPE_SFT             4

/* Frame types */
#define IEEE80211_FTYPE_MGMT            0x0000
#define IEEE80211_FTYPE_CTRL            0x0004
#define IEEE80211_FTYPE_DATA            0x0008
#define IEEE80211_FTYPE_EXT             0x000c

/* Frame SubType: Management Frames */
#define IEEE80211_STYPE_ASSOC_REQ       0x0000
#define IEEE80211_STYPE_ASSOC_RESP      0x0010
#define IEEE80211_STYPE_REASSOC_REQ     0x0020
#define IEEE80211_STYPE_REASSOC_RESP    0x0030
#define IEEE80211_STYPE_PROBE_REQ       0x0040
#define IEEE80211_STYPE_PROBE_RESP      0x0050
#define IEEE80211_STYPE_BEACON          0x0080
#define IEEE80211_STYPE_ATIM            0x0090
#define IEEE80211_STYPE_DISASSOC        0x00A0
#define IEEE80211_STYPE_AUTH            0x00B0
#define IEEE80211_STYPE_DEAUTH          0x00C0
#define IEEE80211_STYPE_ACTION          0x00D0
#define IEEE80211_STYPE_ACTION_NO_ACK   0x00E0

/* Frame SubType: Control */
#define IEEE80211_STYPE_CTRL_EXT        0x0070
#define IEEE80211_STYPE_BACK_REQ        0x0080
#define IEEE80211_STYPE_BACK            0x0090
#define IEEE80211_STYPE_PSPOLL          0x00A0
#define IEEE80211_STYPE_RTS             0x00B0
#define IEEE80211_STYPE_CTS             0x00C0
#define IEEE80211_STYPE_ACK             0x00D0
#define IEEE80211_STYPE_CFEND           0x00E0
#define IEEE80211_STYPE_CFENDACK        0x00F0

/* Frame SubType: Data Frames  */
#define IEEE80211_STYPE_DATA                    0x0000
#define IEEE80211_STYPE_DATA_CFACK              0x0010
#define IEEE80211_STYPE_DATA_CFPOLL             0x0020
#define IEEE80211_STYPE_DATA_CFACKPOLL          0x0030
#define IEEE80211_STYPE_NULLFUNC                0x0040
#define IEEE80211_STYPE_CFACK                   0x0050
#define IEEE80211_STYPE_CFPOLL                  0x0060
#define IEEE80211_STYPE_CFACKPOLL               0x0070
#define IEEE80211_STYPE_QOS_DATA                0x0080
#define IEEE80211_STYPE_QOS_DATA_CFACK          0x0090
#define IEEE80211_STYPE_QOS_DATA_CFPOLL         0x00A0
#define IEEE80211_STYPE_QOS_DATA_CFACKPOLL      0x00B0
#define IEEE80211_STYPE_QOS_NULLFUNC            0x00C0
#define IEEE80211_STYPE_QOS_CFACK               0x00D0
#define IEEE80211_STYPE_QOS_CFPOLL              0x00E0
#define IEEE80211_STYPE_QOS_CFACKPOLL           0x00F0

#define WIFI_FC_GET_TYPE(fc)    ((fc) & IEEE80211_FCTRL_FTYPE)
#define WIFI_FC_GET_STYPE(fc)   ((fc) & IEEE80211_FCTRL_STYPE)

#define WIFI_GET_SEQ_FRAG(seq)  ((seq) & IEEE80211_SCTRL_FRAG)
#define WIFI_GET_SEQ_SEQ(seq)   ((seq) & IEEE80211_SCTRL_SEQ)

#define IEEE80211_SN_MASK               ((IEEE80211_SCTRL_SEQ) >> 4)
#define IEEE80211_MAX_SN                IEEE80211_SN_MASK
#define IEEE80211_SN_MODULO             (IEEE80211_MAX_SN + 1)



/** 
 * @defgroup IEEE_802dot11_FrameType
 * 
 * Define IEEE 802.11 Frame Type in Frame control, including Type and Subtype filed
 */
/* Management */
#define WLAN_FT_ASSOC_REQ           (IEEE80211_FTYPE_MGMT|IEEE80211_STYPE_ASSOC_REQ)            /*!< Frame type and subtype of Association Request */
#define WLAN_FT_ASSOC_RESP          (IEEE80211_FTYPE_MGMT|IEEE80211_STYPE_ASSOC_RESP)           /*!< Frame type and subtype of Association Response */
#define WLAN_FT_REASSOC_REQ         (IEEE80211_FTYPE_MGMT|IEEE80211_STYPE_REASSOC_REQ)          /*!< Frame type and subtype of Reassociation request */
#define WLAN_FT_REASSOC_RESP        (IEEE80211_FTYPE_MGMT|IEEE80211_STYPE_REASSOC_RESP)         /*!< Frame type and subtype of Reassociation response */
#define WLAN_FT_PROBE_REQ           (IEEE80211_FTYPE_MGMT|IEEE80211_STYPE_PROBE_REQ)            /*!< Frame type and subtype of Probe request */
#define WLAN_FT_PROBE_RESP          (IEEE80211_FTYPE_MGMT|IEEE80211_STYPE_PROBE_RESP)           /*!< Frame type and subtype of Probe response */
#define WLAN_FT_BEACON              (IEEE80211_FTYPE_MGMT|IEEE80211_STYPE_BEACON)               /*!< Frame type and subtype of Beacon */
#define WLAN_FT_ATIM                (IEEE80211_FTYPE_MGMT|IEEE80211_STYPE_ATIM)                 /*!< Frame type and subtype of Announcement traffic indication message (ATIM) */
#define WLAN_FT_DISASSOC            (IEEE80211_FTYPE_MGMT|IEEE80211_STYPE_DISASSOC)             /*!< Frame type and subtype of Disassociation */
#define WLAN_FT_AUTH                (IEEE80211_FTYPE_MGMT|IEEE80211_STYPE_AUTH)                 /*!< Frame type and subtype of Authentication */
#define WLAN_FT_DEAUTH              (IEEE80211_FTYPE_MGMT|IEEE80211_STYPE_DEAUTH)               /*!< Frame type and subtype of Deauthentication */
#define WLAN_FT_ACTION              (IEEE80211_FTYPE_MGMT|IEEE80211_STYPE_ACTION)               /*!< Frame type and subtype of Action */
#define WLAN_FT_ACTION_NO_ACK       (IEEE80211_FTYPE_MGMT|IEEE80211_STYPE_ACTION_NO_ACK)        /*!< Frame type and subtype of Action No Ack */
/* Control */
#define WLAN_FT_PS_POLL             (IEEE80211_FTYPE_CTRL|IEEE80211_STYPE_PSPOLL)               /*!< Frame type and subtype of Power Save (PS)-Poll */
#define WLAN_FT_RTS                 (IEEE80211_FTYPE_CTRL|IEEE80211_STYPE_RTS)                  /*!< Frame type and subtype of Request To Send (RTS) */
#define WLAN_FT_CTS                 (IEEE80211_FTYPE_CTRL|IEEE80211_STYPE_CTS)                  /*!< Frame type and subtype of Clear To Send (CTS) */
#define WLAN_FT_ACK                 (IEEE80211_FTYPE_CTRL|IEEE80211_STYPE_ACK)                  /*!< Frame type and subtype of Acknowledgment (ACK) */
#define WLAN_FT_CFEND               (IEEE80211_FTYPE_CTRL|IEEE80211_STYPE_CFEND)                /*!< Frame type and subtype of Contention-Free (CF)-End*/
#define WLAN_FT_CFENDACK            (IEEE80211_FTYPE_CTRL|IEEE80211_STYPE_CFENDACK)             /*!< Frame type and subtype of CF-End + CF-Ack */
/* Data */
#define WLAN_FT_DATA                (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_DATA)                 /*!< Frame type and subtype of Data */
#define WLAN_FT_DATA_CFACK          (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_DATA_CFACK)           /*!< Frame type and subtype of Data + CF-Ack */
#define WLAN_FT_DATA_CFPOLL         (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_DATA_CFPOLL)          /*!< Frame type and subtype of Data + CF-Poll */
#define WLAN_FT_DATA_CFACKPOLL      (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_DATA_CFACKPOLL)       /*!< Frame type and subtype of Data + CF-Ack + CF-Poll */
#define WLAN_FT_NULLFUNC            (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_NULLFUNC)             /*!< Frame type and subtype of Null (no data) */
#define WLAN_FT_CFACK               (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_CFACK)                /*!< Frame type and subtype of CF-Ack (no data) */
#define WLAN_FT_CFPOLL              (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_CFPOLL)               /*!< Frame type and subtype of CF-Poll (no data) */
#define WLAN_FT_CFACKPOLL           (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_CFACKPOLL)            /*!< Frame type and subtype of CF-Ack + CF-Poll (no data) */
#define WLAN_FT_QOS_DATA            (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_QOS_DATA)             /*!< Frame type and subtype of QoS Data */
#define WLAN_FT_QOS_DATA_CFACK      (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_QOS_DATA_CFACK)       /*!< Frame type and subtype of QoS Data + CF-Ack */
#define WLAN_FT_QOS_DATA_CFPOLL     (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_QOS_DATA_CFPOLL)      /*!< Frame type and subtype of QoS Data + CF-Poll */
#define WLAN_FT_QOS_DATA_CFACKPOLL  (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_QOS_DATA_CFACKPOLL)   /*!< Frame type and subtype of QoS Data + CF-Ack + CF-Poll */
#define WLAN_FT_QOS_NULLFUNC        (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_QOS_NULLFUNC)         /*!< Frame type and subtype of QoS Null (no data) */
#define WLAN_FT_QOS_CFACK           (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_QOS_CFACK)            /*!< Frame type and subtype of QoS CF-Ack */
#define WLAN_FT_QOS_CFPOLL          (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_QOS_CFPOLL)           /*!< Frame type and subtype of QoS CF-Poll (no data) */
#define WLAN_FT_QOS_CFACKPOLL       (IEEE80211_FTYPE_DATA|IEEE80211_STYPE_QOS_CFACKPOLL)        /*!< Frame type and subtype of QoS CF-Ack + CF-Poll (no data) */

#define WLAN_FT_MASK                (IEEE80211_FCTRL_FTYPE|IEEE80211_FCTRL_STYPE)               /*!< Frame type and subtype mask in frame control field */
#define WLAN_GET_FT(fc)             ((fc) & WLAN_FT_MASK)                                       /*!< Get frame tyep and subtype field in frame control value */
#define IS_WLAN_FT_BEACON(fc)       (WLAN_GET_FT(fc) == WLAN_FT_BEACON)                         /*!< Check the frame type and subytpe value is beacon or not */
#define IS_WLAN_FT_PROBE_REQ(fc)    (WLAN_GET_FT(fc) == WLAN_FT_PROBE_REQ)                      /*!< Check the frame type and subytpe value is probe request or not */
#define IS_WLAN_FT_NULL_DATA(fc)    (WLAN_GET_FT(fc) == WLAN_FT_NULLFUNC)                       /*!< Check the frame type and subytpe value is Null function data or not */





/* miscellaneous IEEE 802.11 constants */
#define IEEE80211_MAX_FRAG_THRESHOLD    2352
#define IEEE80211_MAX_RTS_THRESHOLD     2353
#define IEEE80211_MAX_AID               2007
#define IEEE80211_MAX_MESH_PEERINGS     63
/* Maximum size for the MA-UNITDATA primitive, 802.11 standard section
   6.2.1.1.2.

   802.11e clarifies the figure in section 7.1.2. The frame body is
   up to 2304 octets long (maximum MSDU size) plus any crypt overhead. */
#define IEEE80211_MAX_DATA_LEN          2304

#define IEEE80211_1ADDR_LEN 10
#define IEEE80211_2ADDR_LEN 16
#define IEEE80211_3ADDR_LEN 24
#define IEEE80211_4ADDR_LEN 30
#define IEEE80211_FCS_LEN    4
#define IEEE80211_HLEN			        IEEE80211_4ADDR_LEN
#define IEEE80211_FRAME_LEN             (IEEE80211_MAX_DATA_LEN + IEEE80211_HLEN)
#define IEEE80211_MGMT_HDR_LEN 24
#define IEEE80211_DATA_HDR3_LEN 24
#define IEEE80211_DATA_HDR4_LEN 30


/* 802.11ad extends maximum MSDU size for DMG (freq > 40Ghz) networks
 * to 7920 bytes, see 8.2.3 General frame format
 */
#define IEEE80211_MAX_DATA_LEN_DMG      7920

/* 30 byte 4 addr hdr, 2 byte QoS, 2304 byte MSDU, 12 byte crypt, 4 byte FCS */
#define IEEE80211_MAX_FRAME_LEN         2352

/* Maximal size of an A-MSDU */
#define IEEE80211_MAX_MPDU_LEN_HT_3839          3839
#define IEEE80211_MAX_MPDU_LEN_HT_7935          7935

#define IEEE80211_MAX_MPDU_LEN_VHT_3895         3895
#define IEEE80211_MAX_MPDU_LEN_VHT_7991         7991
#define IEEE80211_MAX_MPDU_LEN_VHT_11454        11454

#define IEEE80211_MAX_SSID_LEN                  32
#define IEEE80211_MIN_SUPPORTED_RATES           1
#define IEEE80211_MAX_SUPPORTED_RATES           8
#define IEEE80211_MAX_DS_PARAMS                 1
#define IEEE80211_MIN_TIM_LEN                   4
#define IEEE80211_MAX_TIM_LEN                   254
#define IEEE80211_TIM_PARTLBMP_OFS              3
#define IEEE80211_MIN_RSN_LEN                   18
#define IEEE80211_MAX_RSN_LEN                   254
#define IEEE80211_MIN_WPA_LEN                   1
#define IEEE80211_MAX_WPA_LEN                   255

#define MAC_ADDR_LEN            6

// -----
// AUTH Transaction Sequence Number
#define AUTH_SEQ_REQ 0x01
#define AUTH_SEQ_RESP 0x02


/* Authentication algorithms */
#define WLAN_AUTH_OPEN 0
#define WLAN_AUTH_SHARED_KEY 1
#define WLAN_AUTH_FT 2
#define WLAN_AUTH_SAE 3
#define WLAN_AUTH_LEAP 128

#define WLAN_AUTH_CHALLENGE_LEN 128

#define WLAN_CAPABILITY_ESS		(1<<0)
#define WLAN_CAPABILITY_IBSS		(1<<1)

/*
 * A mesh STA sets the ESS and IBSS capability bits to zero.
 * however, this holds true for p2p probe responses (in the p2p_find
 * phase) as well.
 */
#define WLAN_CAPABILITY_IS_STA_BSS(cap)	\
	(!((cap) & (WLAN_CAPABILITY_ESS | WLAN_CAPABILITY_IBSS)))

#define WLAN_CAPABILITY_CF_POLLABLE	(1<<2)
#define WLAN_CAPABILITY_CF_POLL_REQUEST	(1<<3)
#define WLAN_CAPABILITY_PRIVACY		(1<<4)
#define WLAN_CAPABILITY_SHORT_PREAMBLE	(1<<5)
#define WLAN_CAPABILITY_PBCC		(1<<6)
#define WLAN_CAPABILITY_CHANNEL_AGILITY	(1<<7)

// ------ Henry add below --------------------------------
#define WLAN_CAPABILITY_SPECTRUM_MGMT           0x0100
#define WLAN_CAPABILITY_QOS                                 0x200
#define WLAN_CAPABILITY_SHORT_TIMESLOT		0x0400  //802.11g
#define WLAN_CAPABILITY_APSD					0x0800
#define WLAN_CAPABILITY_RADIO_MEASUREMENT           0x1000
#define WLAN_CAPABILITY_DSSS_OFDM				0x2000  //802.11g
#define WLAN_CAPABILITY_DELAYED_BLOCK_ACK		0x4000
#define WLAN_CAPABILITY_IMMEDIATE_BLOCK_ACK	0x8000
// -----------------------------------------------------

typedef struct {
    u16 version		:2;
    u16 type		:2;
    u16 sub_type	:4;
    u16 ds		    :2;
    u16 more_frag 	:1;
    u16 retry		:1;
    u16 pwr_mgt		:1;
    u16 more_date 	:1;
    u16 protect		:1;
    u16 order		:1;
} frame_control_t;

//Sequence
typedef struct {
    u16 fragment	:4;
    u16 sequence	:12;
} sequence_control_t;

//802.1h & RFC1042 logical link control
typedef struct {
    u8  dsap;
    u8  ssap;
    u8  control;
    u8  encap[3];
} dot11_llc_header_t;
#define DOT11_LLC_HEADER_LEN	6

//MAC header
typedef struct {
    u16	fc;
    u16	durid;
    u8	addr1[MAC_ADDR_LEN];
    u8	addr2[MAC_ADDR_LEN];
    u8	addr3[MAC_ADDR_LEN];
    u16	seqctrl;
} common_mac_header_t;
#define COMMON_MAC_HEADER_LEN    24

//Control frames
//RTS
typedef struct {
    u16	fc;
    u16	durid;
    u8	addr1[MAC_ADDR_LEN];
    u8	addr2[MAC_ADDR_LEN];
} rts_frame_t;
#define	RTS_LEN		16

//CTS
typedef struct {
    u16	fc;
    u16	durid;
    u8	addr1[MAC_ADDR_LEN];
} cts_frame_t;
#define	CTS_LEN		10

//ACK
typedef struct {
    u16	fc;
    u16	durid;
    u8	addr1[MAC_ADDR_LEN];
} ack_frame_t;
#define	ACK_LEN		10

//PS-Poll
typedef struct {
    u16	fc;
    u16	durid;
    u8	addr1[MAC_ADDR_LEN];
    u8	addr2[MAC_ADDR_LEN];
} ps_poll_frame_t;
#define	PS_POLL_LEN	16

//CF-END
typedef struct {
    u16	fc;
    u16	durid;
    u8	addr1[MAC_ADDR_LEN];
    u8	addr2[MAC_ADDR_LEN];
} cf_end_frame_t;
#define	CF_END_LEN	16

//CF-End + CF-Ack
typedef struct {
    u16	fc;
    u16	durid;
    u8	addr1[MAC_ADDR_LEN];
    u8	addr2[MAC_ADDR_LEN];
} cf_end_ack_frame_t;
#define	CF_END_ACK_LEN	16

//Control header
typedef struct  {
    u16	fc;
    u16	durid;
    u8	addr1[MAC_ADDR_LEN];
    u8	addr2[MAC_ADDR_LEN];
} ctrl_mac_header_t;
#define CTRL_HEADER_LEN	16

//Data frames
//Common data header
typedef struct  {
    u16 fc;
    u16 durid;
    u8  addr1[MAC_ADDR_LEN];
    u8  addr2[MAC_ADDR_LEN];
    u8  addr3[MAC_ADDR_LEN];
    u16 seqctrl;
} common_data_mac_header_t;
#define COMMON_DATA_HEADER_LEN    24

//Management header
typedef struct  {
	u16 fc;
	u16 durid;
	u8  addr1[MAC_ADDR_LEN];
	u8  addr2[MAC_ADDR_LEN];
	u8  addr3[MAC_ADDR_LEN];
	u16 seqctrl;
} mgmt_mac_header_t;
#define MGMT_MAC_HEADER_LEN   24

// TIM element
typedef struct
{
    u8 u8Eid;
    u8 u8Length;
    u8 u8DtimCount;
    u8 u8DtimPeriod;
    u8 u8BitmapCtrl;
    u8 u8PartialBitmap[IEEE80211_MAX_TIM_LEN - IEEE80211_TIM_PARTLBMP_OFS];
} Wlan_EidTim_s;
#define TIM_BMPCTRL_GRPDATA_SFT     0
#define TIM_BMPCTRL_GRPDATA_MSK     (1 << TIM_BMPCTRL_GRPDATA_SFT)
#define TIM_BMPCTRL_BMPOFS_SFT      1
#define TIM_BMPCTRL_BMPOFS_MSK      (0x7FU << TIM_BMPCTRL_BMPOFS_SFT)

// Status Codes: Table 8-37 in 802.11-2012
typedef enum
{
    STATUS_SUCCESS = 0,
    STATUS_AUTH_REJECT_UNSPECIFIED = 1,
    STATUS_AUTH_REJECT_OUTSIDE_SCOPE = 12,
    STATUS_AUTH_REJECT_ALGO_NOT_SUPPORT = 13,   //Algorithm is not supported
    STATUS_AUTH_REJECT_CHALLENGE_FAILURE = 15,
    STATUS_AUTH_REJECT_TIMEOUT = 16,
    STATUS_REJECT_TOO_MANY_ASSOCIATIONS = 17,
    STATUS_ASSOC_REJECTED_TEMPORARILY = 30,
    STATUS_REJECT_UNAUTHENTICATED_ACCESS = 68,
    STATUS_RESERVED = 103,
} status_code_e;


/* Status codes */
enum ieee80211_statuscode {
	WLAN_STATUS_SUCCESS = 0,
	WLAN_STATUS_UNSPECIFIED_FAILURE = 1,
	WLAN_STATUS_CAPS_UNSUPPORTED = 10,
	WLAN_STATUS_REASSOC_NO_ASSOC = 11,
	WLAN_STATUS_ASSOC_DENIED_UNSPEC = 12,
	WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG = 13,
	WLAN_STATUS_UNKNOWN_AUTH_TRANSACTION = 14,
	WLAN_STATUS_CHALLENGE_FAIL = 15,
	WLAN_STATUS_AUTH_TIMEOUT = 16,
	WLAN_STATUS_AP_UNABLE_TO_HANDLE_NEW_STA = 17,
	WLAN_STATUS_ASSOC_DENIED_RATES = 18,
	/* 802.11b */
	WLAN_STATUS_ASSOC_DENIED_NOSHORTPREAMBLE = 19,
	WLAN_STATUS_ASSOC_DENIED_NOPBCC = 20,
	WLAN_STATUS_ASSOC_DENIED_NOAGILITY = 21,
	/* 802.11h */
	WLAN_STATUS_ASSOC_DENIED_NOSPECTRUM = 22,
	WLAN_STATUS_ASSOC_REJECTED_BAD_POWER = 23,
	WLAN_STATUS_ASSOC_REJECTED_BAD_SUPP_CHAN = 24,
	/* 802.11g */
	WLAN_STATUS_ASSOC_DENIED_NOSHORTTIME = 25,
	WLAN_STATUS_ASSOC_DENIED_NODSSSOFDM = 26,
	/* 802.11w */
	WLAN_STATUS_ASSOC_REJECTED_TEMPORARILY = 30,
	WLAN_STATUS_ROBUST_MGMT_FRAME_POLICY_VIOLATION = 31,
	/* 802.11i */
	WLAN_STATUS_INVALID_IE = 40,
	WLAN_STATUS_INVALID_GROUP_CIPHER = 41,
	WLAN_STATUS_INVALID_PAIRWISE_CIPHER = 42,
	WLAN_STATUS_INVALID_AKMP = 43,
	WLAN_STATUS_UNSUPP_RSN_VERSION = 44,
	WLAN_STATUS_INVALID_RSN_IE_CAP = 45,
	WLAN_STATUS_CIPHER_SUITE_REJECTED = 46,
	/* 802.11e */
	WLAN_STATUS_UNSPECIFIED_QOS = 32,
	WLAN_STATUS_ASSOC_DENIED_NOBANDWIDTH = 33,
	WLAN_STATUS_ASSOC_DENIED_LOWACK = 34,
	WLAN_STATUS_ASSOC_DENIED_UNSUPP_QOS = 35,
	WLAN_STATUS_REQUEST_DECLINED = 37,
	WLAN_STATUS_INVALID_QOS_PARAM = 38,
	WLAN_STATUS_CHANGE_TSPEC = 39,
	WLAN_STATUS_WAIT_TS_DELAY = 47,
	WLAN_STATUS_NO_DIRECT_LINK = 48,
	WLAN_STATUS_STA_NOT_PRESENT = 49,
	WLAN_STATUS_STA_NOT_QSTA = 50,
};


/* Reason codes */
enum ieee80211_reasoncode {
	WLAN_REASON_UNSPECIFIED = 1,
	WLAN_REASON_PREV_AUTH_NOT_VALID = 2,
	WLAN_REASON_DEAUTH_LEAVING = 3,
	WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY = 4,
	WLAN_REASON_DISASSOC_AP_BUSY = 5,
	WLAN_REASON_CLASS2_FRAME_FROM_NONAUTH_STA = 6,
	WLAN_REASON_CLASS3_FRAME_FROM_NONASSOC_STA = 7,
	WLAN_REASON_DISASSOC_STA_HAS_LEFT = 8,
	WLAN_REASON_STA_REQ_ASSOC_WITHOUT_AUTH = 9,
	/* 802.11h */
	WLAN_REASON_DISASSOC_BAD_POWER = 10,
	WLAN_REASON_DISASSOC_BAD_SUPP_CHAN = 11,
	/* 802.11i */
	WLAN_REASON_INVALID_IE = 13,
	WLAN_REASON_MIC_FAILURE = 14,
	WLAN_REASON_4WAY_HANDSHAKE_TIMEOUT = 15,
	WLAN_REASON_GROUP_KEY_HANDSHAKE_TIMEOUT = 16,
	WLAN_REASON_IE_DIFFERENT = 17,
	WLAN_REASON_INVALID_GROUP_CIPHER = 18,
	WLAN_REASON_INVALID_PAIRWISE_CIPHER = 19,
	WLAN_REASON_INVALID_AKMP = 20,
	WLAN_REASON_UNSUPP_RSN_VERSION = 21,
	WLAN_REASON_INVALID_RSN_IE_CAP = 22,
	WLAN_REASON_IEEE8021X_FAILED = 23,
	WLAN_REASON_CIPHER_SUITE_REJECTED = 24,
	/* TDLS (802.11z) */
	WLAN_REASON_TDLS_TEARDOWN_UNREACHABLE = 25,
	WLAN_REASON_TDLS_TEARDOWN_UNSPECIFIED = 26,
	/* 802.11e */
	WLAN_REASON_DISASSOC_UNSPECIFIED_QOS = 32,
	WLAN_REASON_DISASSOC_QAP_NO_BANDWIDTH = 33,
	WLAN_REASON_DISASSOC_LOW_ACK = 34,
	WLAN_REASON_DISASSOC_QAP_EXCEED_TXOP = 35,
	WLAN_REASON_QSTA_LEAVE_QBSS = 36,
	WLAN_REASON_QSTA_NOT_USE = 37,
	WLAN_REASON_QSTA_REQUIRE_SETUP = 38,
	WLAN_REASON_QSTA_TIMEOUT = 39,
	WLAN_REASON_QSTA_CIPHER_NOT_SUPP = 45,
	/* 802.11s */
	WLAN_REASON_MESH_PEER_CANCELED = 52,
	WLAN_REASON_MESH_MAX_PEERS = 53,
	WLAN_REASON_MESH_CONFIG = 54,
	WLAN_REASON_MESH_CLOSE = 55,
	WLAN_REASON_MESH_MAX_RETRIES = 56,
	WLAN_REASON_MESH_CONFIRM_TIMEOUT = 57,
	WLAN_REASON_MESH_INVALID_GTK = 58,
	WLAN_REASON_MESH_INCONSISTENT_PARAM = 59,
	WLAN_REASON_MESH_INVALID_SECURITY = 60,
	WLAN_REASON_MESH_PATH_ERROR = 61,
	WLAN_REASON_MESH_PATH_NOFORWARD = 62,
	WLAN_REASON_MESH_PATH_DEST_UNREACHABLE = 63,
	WLAN_REASON_MAC_EXISTS_IN_MBSS = 64,
	WLAN_REASON_MESH_CHAN_REGULATORY = 65,
	WLAN_REASON_MESH_CHAN = 66,
};

/* Information Element IDs */
enum ieee80211_eid {
	WLAN_EID_SSID = 0,
	WLAN_EID_SUPP_RATES = 1,
	WLAN_EID_FH_PARAMS = 2, /* reserved now */
	WLAN_EID_DS_PARAMS = 3,
	WLAN_EID_CF_PARAMS = 4,
	WLAN_EID_TIM = 5,
	WLAN_EID_IBSS_PARAMS = 6,
	WLAN_EID_COUNTRY = 7,
	/* 8, 9 reserved */
	WLAN_EID_REQUEST = 10,
	WLAN_EID_QBSS_LOAD = 11,
	WLAN_EID_EDCA_PARAM_SET = 12,
	WLAN_EID_TSPEC = 13,
	WLAN_EID_TCLAS = 14,
	WLAN_EID_SCHEDULE = 15,
	WLAN_EID_CHALLENGE = 16,
	/* 17-31 reserved for challenge text extension */
	WLAN_EID_PWR_CONSTRAINT = 32,
	WLAN_EID_PWR_CAPABILITY = 33,
	WLAN_EID_TPC_REQUEST = 34,
	WLAN_EID_TPC_REPORT = 35,
	WLAN_EID_SUPPORTED_CHANNELS = 36,
	WLAN_EID_CHANNEL_SWITCH = 37,
	WLAN_EID_MEASURE_REQUEST = 38,
	WLAN_EID_MEASURE_REPORT = 39,
	WLAN_EID_QUIET = 40,
	WLAN_EID_IBSS_DFS = 41,
	WLAN_EID_ERP_INFO = 42,
	WLAN_EID_TS_DELAY = 43,
	WLAN_EID_TCLAS_PROCESSING = 44,
	WLAN_EID_HT_CAPABILITY = 45,
	WLAN_EID_QOS_CAPA = 46,
	/* 47 reserved for Broadcom */
	WLAN_EID_RSN = 48,
	WLAN_EID_802_15_COEX = 49,
	WLAN_EID_EXT_SUPP_RATES = 50,
	WLAN_EID_AP_CHAN_REPORT = 51,
	WLAN_EID_NEIGHBOR_REPORT = 52,
	WLAN_EID_RCPI = 53,
	WLAN_EID_MOBILITY_DOMAIN = 54,
	WLAN_EID_FAST_BSS_TRANSITION = 55,
	WLAN_EID_TIMEOUT_INTERVAL = 56,
	WLAN_EID_RIC_DATA = 57,
	WLAN_EID_DSE_REGISTERED_LOCATION = 58,
	WLAN_EID_SUPPORTED_REGULATORY_CLASSES = 59,
	WLAN_EID_EXT_CHANSWITCH_ANN = 60,
	WLAN_EID_HT_OPERATION = 61,
	WLAN_EID_SECONDARY_CHANNEL_OFFSET = 62,
	WLAN_EID_BSS_AVG_ACCESS_DELAY = 63,
	WLAN_EID_ANTENNA_INFO = 64,
	WLAN_EID_RSNI = 65,
	WLAN_EID_MEASUREMENT_PILOT_TX_INFO = 66,
	WLAN_EID_BSS_AVAILABLE_CAPACITY = 67,
	WLAN_EID_BSS_AC_ACCESS_DELAY = 68,
	WLAN_EID_TIME_ADVERTISEMENT = 69,
	WLAN_EID_RRM_ENABLED_CAPABILITIES = 70,
	WLAN_EID_MULTIPLE_BSSID = 71,
	WLAN_EID_BSS_COEX_2040 = 72,
	WLAN_EID_BSS_INTOLERANT_CHL_REPORT = 73,
	WLAN_EID_OVERLAP_BSS_SCAN_PARAM = 74,
	WLAN_EID_RIC_DESCRIPTOR = 75,
	WLAN_EID_MMIE = 76,
	WLAN_EID_ASSOC_COMEBACK_TIME = 77,
	WLAN_EID_EVENT_REQUEST = 78,
	WLAN_EID_EVENT_REPORT = 79,
	WLAN_EID_DIAGNOSTIC_REQUEST = 80,
	WLAN_EID_DIAGNOSTIC_REPORT = 81,
	WLAN_EID_LOCATION_PARAMS = 82,
	WLAN_EID_NON_TX_BSSID_CAP =  83,
	WLAN_EID_SSID_LIST = 84,
	WLAN_EID_MULTI_BSSID_IDX = 85,
	WLAN_EID_FMS_DESCRIPTOR = 86,
	WLAN_EID_FMS_REQUEST = 87,
	WLAN_EID_FMS_RESPONSE = 88,
	WLAN_EID_QOS_TRAFFIC_CAPA = 89,
	WLAN_EID_BSS_MAX_IDLE_PERIOD = 90,
	WLAN_EID_TSF_REQUEST = 91,
	WLAN_EID_TSF_RESPOSNE = 92,
	WLAN_EID_WNM_SLEEP_MODE = 93,
	WLAN_EID_TIM_BCAST_REQ = 94,
	WLAN_EID_TIM_BCAST_RESP = 95,
	WLAN_EID_COLL_IF_REPORT = 96,
	WLAN_EID_CHANNEL_USAGE = 97,
	WLAN_EID_TIME_ZONE = 98,
	WLAN_EID_DMS_REQUEST = 99,
	WLAN_EID_DMS_RESPONSE = 100,
	WLAN_EID_LINK_ID = 101,
	WLAN_EID_WAKEUP_SCHEDUL = 102,
	/* 103 reserved */
	WLAN_EID_CHAN_SWITCH_TIMING = 104,
	WLAN_EID_PTI_CONTROL = 105,
	WLAN_EID_PU_BUFFER_STATUS = 106,
	WLAN_EID_INTERWORKING = 107,
	WLAN_EID_ADVERTISEMENT_PROTOCOL = 108,
	WLAN_EID_EXPEDITED_BW_REQ = 109,
	WLAN_EID_QOS_MAP_SET = 110,
	WLAN_EID_ROAMING_CONSORTIUM = 111,
	WLAN_EID_EMERGENCY_ALERT = 112,
	WLAN_EID_MESH_CONFIG = 113,
	WLAN_EID_MESH_ID = 114,
	WLAN_EID_LINK_METRIC_REPORT = 115,
	WLAN_EID_CONGESTION_NOTIFICATION = 116,
	WLAN_EID_PEER_MGMT = 117,
	WLAN_EID_CHAN_SWITCH_PARAM = 118,
	WLAN_EID_MESH_AWAKE_WINDOW = 119,
	WLAN_EID_BEACON_TIMING = 120,
	WLAN_EID_MCCAOP_SETUP_REQ = 121,
	WLAN_EID_MCCAOP_SETUP_RESP = 122,
	WLAN_EID_MCCAOP_ADVERT = 123,
	WLAN_EID_MCCAOP_TEARDOWN = 124,
	WLAN_EID_GANN = 125,
	WLAN_EID_RANN = 126,
	WLAN_EID_EXT_CAPABILITY = 127,
	/* 128, 129 reserved for Agere */
	WLAN_EID_PREQ = 130,
	WLAN_EID_PREP = 131,
	WLAN_EID_PERR = 132,
	/* 133-136 reserved for Cisco */
	WLAN_EID_PXU = 137,
	WLAN_EID_PXUC = 138,
	WLAN_EID_AUTH_MESH_PEER_EXCH = 139,
	WLAN_EID_MIC = 140,
	WLAN_EID_DESTINATION_URI = 141,
	WLAN_EID_UAPSD_COEX = 142,
	WLAN_EID_WAKEUP_SCHEDULE = 143,
	WLAN_EID_EXT_SCHEDULE = 144,
	WLAN_EID_STA_AVAILABILITY = 145,
	WLAN_EID_DMG_TSPEC = 146,
	WLAN_EID_DMG_AT = 147,
	WLAN_EID_DMG_CAP = 148,
	/* 149 reserved for Cisco */
	WLAN_EID_CISCO_VENDOR_SPECIFIC = 150,
	WLAN_EID_DMG_OPERATION = 151,
	WLAN_EID_DMG_BSS_PARAM_CHANGE = 152,
	WLAN_EID_DMG_BEAM_REFINEMENT = 153,
	WLAN_EID_CHANNEL_MEASURE_FEEDBACK = 154,
	/* 155-156 reserved for Cisco */
	WLAN_EID_AWAKE_WINDOW = 157,
	WLAN_EID_MULTI_BAND = 158,
	WLAN_EID_ADDBA_EXT = 159,
	WLAN_EID_NEXT_PCP_LIST = 160,
	WLAN_EID_PCP_HANDOVER = 161,
	WLAN_EID_DMG_LINK_MARGIN = 162,
	WLAN_EID_SWITCHING_STREAM = 163,
	WLAN_EID_SESSION_TRANSITION = 164,
	WLAN_EID_DYN_TONE_PAIRING_REPORT = 165,
	WLAN_EID_CLUSTER_REPORT = 166,
	WLAN_EID_RELAY_CAP = 167,
	WLAN_EID_RELAY_XFER_PARAM_SET = 168,
	WLAN_EID_BEAM_LINK_MAINT = 169,
	WLAN_EID_MULTIPLE_MAC_ADDR = 170,
	WLAN_EID_U_PID = 171,
	WLAN_EID_DMG_LINK_ADAPT_ACK = 172,
	/* 173 reserved for Symbol */
	WLAN_EID_MCCAOP_ADV_OVERVIEW = 174,
	WLAN_EID_QUIET_PERIOD_REQ = 175,
	/* 176 reserved for Symbol */
	WLAN_EID_QUIET_PERIOD_RESP = 177,
	/* 178-179 reserved for Symbol */
	/* 180 reserved for ISO/IEC 20011 */
	WLAN_EID_EPAC_POLICY = 182,
	WLAN_EID_CLISTER_TIME_OFF = 183,
	WLAN_EID_INTER_AC_PRIO = 184,
	WLAN_EID_SCS_DESCRIPTOR = 185,
	WLAN_EID_QLOAD_REPORT = 186,
	WLAN_EID_HCCA_TXOP_UPDATE_COUNT = 187,
	WLAN_EID_HL_STREAM_ID = 188,
	WLAN_EID_GCR_GROUP_ADDR = 189,
	WLAN_EID_ANTENNA_SECTOR_ID_PATTERN = 190,
	WLAN_EID_VHT_CAPABILITY = 191,
	WLAN_EID_VHT_OPERATION = 192,
	WLAN_EID_EXTENDED_BSS_LOAD = 193,
	WLAN_EID_WIDE_BW_CHANNEL_SWITCH = 194,
	WLAN_EID_VHT_TX_POWER_ENVELOPE = 195,
	WLAN_EID_CHANNEL_SWITCH_WRAPPER = 196,
	WLAN_EID_AID = 197,
	WLAN_EID_QUIET_CHANNEL = 198,
	WLAN_EID_OPMODE_NOTIF = 199,

	WLAN_EID_VENDOR_SPECIFIC = 221,
	WLAN_EID_QOS_PARAMETER = 222,
};

#endif /* _IEEE80211_MAC_HEADER_H_ */
