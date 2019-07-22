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

#ifndef _WIFI_MAC_FRAME_H_
#define _WIFI_MAC_FRAME_H_

//#include "wifi_mac_dcf.h"
//#include "wifi_mac_types.h"
//#include "wifi_mac_common.h"
//#include "ieee80211_mac_header.h"
//#include "linked_list.h"

//#include "ipc.h"


enum nl80211_auth_type {
	NL80211_AUTHTYPE_OPEN_SYSTEM,
	NL80211_AUTHTYPE_SHARED_KEY,
	NL80211_AUTHTYPE_FT,
	NL80211_AUTHTYPE_NETWORK_EAP,
	NL80211_AUTHTYPE_SAE,
	/* keep last */
	__NL80211_AUTHTYPE_NUM,
	NL80211_AUTHTYPE_MAX = __NL80211_AUTHTYPE_NUM - 1,
	NL80211_AUTHTYPE_AUTOMATIC
};

#define MAX_SSID_SIZE               32          /* Wi-Fi SSID size */
#define MAX_CHANNEL_NUM             14          /* Wi-Fi channel number */
#define CHALLENGE_TEXT_SIZE         128         /* Challenge Text size */

#define DOT11B_SIFS                 10          /* Wi-Fi (default) SIFS : 10 us */
#define DOT11B_PIFS	                30          /* Wi-Fi (default) PIFS : 30 us */
#define DOT11B_DIFS                 50          /* Wi-Fi (default) DIFS : 50 us */
#define DOT11B_SLOT_TIME            20          /* Wi-Fi slot time : 20 us */
#define TIME_UNIT                   1024        /* Wi-Fi time unit (1TU) : 1024 us */

#define LONG_PREAMBLE_TIME          192         /* Long Preamble : 192us @ 1Mbps */
#define SHORT_PREAMBLE_TIME         96          /* Short Preamble : 96us @ 1Mbps */

#define RATE_BASIC 0x80


#define WIFI_SUPPORT_RATE_1M    (PHY_RATE_1M_REG/5)   //0x02     // 1Mbps
#define WIFI_SUPPORT_RATE_2M    (PHY_RATE_2M_REG/5)   //0x04     // 2Mbps
#define WIFI_SUPPORT_RATE_5_5M  (PHY_RATE_5_5M_REG/5) //0x0B     // 5.5Mbps
#define WIFI_SUPPORT_RATE_11M   (PHY_RATE_11M_REG/5)  //0x16     // 11Mbps
//#define TX_FRAME_MAX 128   //IEEE80211_MAX_FRAME_LEN

// should be move to others??
#ifdef ENHANCE_IPC
#define SHARE_MEMERY_MAX        IPC_WIFI_SHARED_MEM_LEN
#else
#define SHARE_MEMERY_MAX 64
#endif

typedef struct{
	u16 capabilities;
	u16 listen_interval;
} association_request_t;
#define ASSOCIATION_REQUEST_LEN 4

typedef struct{
	u16 capabilities;
	u16 listen_interval;
	u8 current_ap[MAC_ADDR_LEN];
} reassociation_request_t;
#define REASSOCIATION_REQUEST_LEN 4

typedef struct{
	//u8 timestamp[8];
	u32 timestamp_low;
	u32 timestamp_high;
	u16 beacon_interval;
	u16 capabilities;
} probe_response_t;
#define PROBE_RESPONSE_LEN 12

typedef struct{
	u16 capabilities;
	u16 status;
	u16 aid;
} association_response_t;
#define ASSOCIATION_RESPONSE_LEN 6

typedef struct {
	u16 auth_algorithm;
	u16 auth_seq_num;
	u16 auth_status;
} auth_req_resp_t;
#define AUTH_REQ_RESP_LEN 6


typedef struct{
	u8 element_id;
	u8 element_length;
} element_header_t;
#define ELEMENT_HEADER_LEN 2

typedef struct{
	element_header_t header;
	u8				data[256];
} infomation_element_t;

typedef enum {
	MAC_STATE_IDLE,
	MAC_STATE_0,
	MAC_STATE_1,
	MAC_STATE_2,
	MAC_STATE_3,
	MAC_STATE_4
} mac_state_t;

typedef enum {
	MAC_SUB_STATE_IDLE,
	MAC_SUB_STATE_TX1,
	MAC_SUB_STATE_RX1,
	MAC_SUB_STATE_TX2,
	MAC_SUB_STATE_RX2,
	MAC_SUB_STATE_TX3,
	MAC_SUB_STATE_RX4
} mac_sub_state_t;

typedef enum {
	MAC_IDLE,
	MAC_SCAN_REQ,
	MAC_SCAN_RESP,
	MAC_JOIN,
	MAC_AUTH_REQ,
	MAC_AUTH_RESP,
	MAC_ASSOC_REQ,
	MAC_ASSOC_RESP,
	DATA_TX
} mlme_event_t;
//

typedef enum
{
    WIFI_RATE_1M = 0,
    WIFI_RATE_2M = 1,
    WIFI_RATE_5_5M = 2,
    WIFI_RATE_11M = 3,
    WIFI_SUPPORTED_RATE_NUM
}wifi_rate_mode_t;

typedef struct
{
	/**
	 * psk - WPA pre-shared key (256 bits)
	 */
	u8 psk[32];

	/**
	 * psk_set - Whether PSK field is configured
	 */
	int psk_set;

	/**
	 * passphrase - WPA ASCII passphrase
	 *
	 * If this is set, psk will be generated using the SSID and passphrase
	 * configured for the network. ASCII passphrase must be between 8 and
	 * 63 characters (inclusive).
	 */
	char *passphrase;

	/**
	 * pairwise_cipher - Bitfield of allowed pairwise ciphers, WPA_CIPHER_*
	 */
	int pairwise_cipher;

	/**
	 * group_cipher - Bitfield of allowed group ciphers, WPA_CIPHER_*
	 */
	int group_cipher;

	/**
	 * key_mgmt - Bitfield of allowed key management protocols
	 *
	 * WPA_KEY_MGMT_*
	 */
	int key_mgmt;

	int mgmt_group_cipher;	//for 802.11w?
	/**
	 * proto - Bitfield of allowed protocols, WPA_PROTO_*
	 */
	int proto;

#if 1//def IEEE8021X_EAPOL
#define EAPOL_FLAG_REQUIRE_KEY_UNICAST BIT(0)
#define EAPOL_FLAG_REQUIRE_KEY_BROADCAST BIT(1)
	/**
	 * eapol_flags - Bit field of IEEE 802.1X/EAPOL options (EAPOL_FLAG_*)
	 */
	int eapol_flags;

	/**
	 * eap - EAP peer configuration for this network
	 */
	//struct eap_peer_config eap;
	/**
	 * leap - Number of EAP methods using LEAP
	 *
	 * This field should be set to 1 if LEAP is enabled. This is used to
	 * select IEEE 802.11 authentication algorithm.
	 */
	int leap;

	/**
	 * non_leap - Number of EAP methods not using LEAP
	 *
	 * This field should be set to >0 if any EAP method other than LEAP is
	 * enabled. This is used to select IEEE 802.11 authentication
	 * algorithm.
	 */
	int non_leap;

	/**
	 * eap_workaround - EAP workarounds enabled
	 *
	 * wpa_supplicant supports number of "EAP workarounds" to work around
	 * interoperability issues with incorrectly behaving authentication
	 * servers. This is recommended to be enabled by default because some
	 * of the issues are present in large number of authentication servers.
	 *
	 * Strict EAP conformance mode can be configured by disabling
	 * workarounds with eap_workaround = 0.
	 */
	unsigned int eap_workaround;


#endif /* IEEE8021X_EAPOL */

}wpa_ssid_t;

typedef struct
{
    //u8 * frame_buffer;
    u8 frame_buffer[384];
    unsigned int frame_length;
}rx_eapol_report;

typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	u16 frame_length;
} rts_info_t;

/*
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
} ack_info_t;
*/

/*
MLME-SCAN Request
	BSSType, BSSID, SSID, ScanType, ProbeDelay, ChannelList, MinChannelTime, MaxChannelTime,
	VendorSpecificInfo
MLME-SCAN Confirm
	BBSDescriptionSet, ResultCode, VendorSpecificInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	//u8 bssid[MAC_ADDR_LEN];
	//u8 ssid[IEEE80211_MAX_SSID_LEN + 1];
} probe_req_info_t;

/*
MLME-AUTHENTICATE Request
	PeerSTAAddress, AuthenticationType, AuthenticateFailureTimeout, VendorSpecificInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	//auth_req_resp_t auth_request;
	//u16 auth_algorithm;
	//u16 auth_seq_num;
	//u16 auth_status;
} auth_req_info_t;

/*
MLME-SCAN Confirm
	BBSDescriptionSet, ResultCode, VendorSpecificInfo
*/
typedef struct {
	//u8 management_frame_type;
	probe_response_t probe_response;
	u8 bssid[MAC_ADDR_LEN];
	u8 ssid[IEEE80211_MAX_SSID_LEN + 1];
	u8 supported_rates[IEEE80211_MAX_SUPPORTED_RATES];
} probe_resp_info_t;

/*
MLME-AUTHENTICATE Response
	PeerSTAAddress, ResultCode, VendorSpecificInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	auth_req_resp_t auth_response;
} auth_resp_info_t;

/*
MLME-ASSOCIATE Request
	PeerSTAAddress, AssociationFailureTimeout, CapabilityInformation, ListenInterval, Supported Channels, RSN, QoSCapability, VendorSpeciticInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	//association_request_t association_request;
} assoc_req_info_t;

/*
MLME-REASSOCIATE Response
	PeerSTAAddress, ResultCode, CapabilityInformation, AssociationID, EDCAParameterSet,	VendorSpeciticInfo
*/
/*
MLME-ASSOCIATE Response
	PeerSTAAddress, ResultCode, CapabilityInformation, AssociationID, EDCAParameterSet, VendorSpeciticInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	association_response_t association_response;
	u8 supported_rates[IEEE80211_MAX_SUPPORTED_RATES];
} assoc_resp_info_t;

/*
MLME-DEAUTHENTICATE Request
	PeerSTAAdress, ReasonCode, VendorSpecificInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	u32 reason_code;
} deauth_req_info_t;

/*
MLME-Disassociate Request
	PeerSTAAddress, ReasonCode, VendorSpecifinInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	u16 reason_code;
} disassoc_req_info_t;

/*
MLME-REASSOCIATE Request
	NewAPAddress, ReassociationFailureTimeout, CapabilityInformation, ListenInterval, Supported Channels, RSN, QoSCapability, VendorSpeciticInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	//reassociation_request_t reassociation_request;
} reassoc_req_info_t;

u16 calculate_duration_impl(u16 frame_len, u8 preamble, u8 pbcc, u8 rate);
void wifi_sta_create_data_mac_header_impl(u8 *header, u16 data_len, u16 encrypt, const u8* eth_dest);
void wifi_create_data_mac_header(u8 *header, u16 data_len, u8* eth_dest);
void wifi_create_rts_frame(rts_info_t *parameter);
void wifi_create_probe_req_frame(probe_req_info_t *parameter);
void wifi_create_auth_req_frame(auth_req_info_t *parameter);
void wifi_create_assoc_req_frame(assoc_req_info_t *parameter);
void wifi_create_reassoc_req_frame(reassoc_req_info_t *parameter);
void wifi_create_disassoc_req_frame(disassoc_req_info_t *parameter);
void wifi_create_deauth_req_frame(deauth_req_info_t *parameter);
void wifi_create_beacon_frame(u8 *mpdu_buf);
#ifdef WIFI_DEBUG_MODE
//void wifi_create_ack_frame(ack_info_t *parameter);
void wifi_create_ack_frame(u8 *mpdu_buf);
#endif // WIFI_DEBUG_MODE

void bss_reset(void);


typedef u16 (*calculate_duration_fp)(u16 frame_len, u8 preamble, u8 pbcc, u8 rate);
typedef void (*wifi_sta_create_data_mac_header_fp)(u8 *header, u16 data_len, u16 encrypt, const u8* eth_dest);

extern calculate_duration_fp calculate_duration;
extern wifi_sta_create_data_mac_header_fp wifi_sta_create_data_mac_header;

#endif /* _WIFI_MAC_FRAME_H_ */

