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

#ifndef _WIFI_MAC_DCF_H_
#define _WIFI_MAC_DCF_H_

#pragma anon_unions

//#include "wifi_mac_common.h"

//#include "wifi_mac_types.h"
//#include "ieee80211_mac_header.h"
//#include "wifi_mac_queue.h"

#define WIFI_DEBUG_MODE
#ifdef __NL1000_An__
    //#define WIFI_OVERFLOW_RESET
    //#define WIFI_EXT_RX_TOUT
#else
    #define WIFI_EXT_RX_TOUT
#endif // __NL1000_An__

#define SIFS_LEN            10   // SIFS (us)
#define DIFS_LEN            50   // DIFS (us)
#define EIFS_LEN            364  // EIFS (us) EIFS = aSIFSTime + DIFS + ACKTxTime
#define SLOT_LEN            20   // SLOT (us)
#define S2TR_EN_START_LEN   8
#define MA_EN_START_LEN     12  // 1
#define TRX_EN_LEN          2
#define MAC_DEC_LEN         (SLOT_LEN - MA_EN_START_LEN - TRX_EN_LEN)
#define TIME_UNIT 1024 // TU 1024 microseconds.

typedef int (*wifi_mac_tx_drop_t)(u8 result);

typedef enum {
    WIFI_MAC_IDLE,
	WIFI_MAC_INIT,
	WIFI_MAC_INIT_RX_BUF,
	WIFI_MAC_READY,
	WIFI_MAC_TERMINATING,

	WIFI_MAC_MSQ_STATE_MAX
} wifi_msq_state_t;

typedef enum {
	TX_IN_QUEUE_SUCCESS,
	TX_IN_QUEUE_FAIL,
	TX_QUEUE_FULL
} tx_data_frame_t;

typedef enum {
    TX_WAIT_NONE    = 0,
    TX_WAIT_ACK     = 1 << 0,
    TX_WAIT_CTS     = 1 << 1,
    TX_WAIT_PSPOLL  = 1 << 2,
    TX_WAIT_FRAME   = TX_WAIT_ACK | TX_WAIT_CTS | TX_WAIT_PSPOLL,

    TX_WAIT_STATE_MAX = 0xff
} tx_wait_state_t;

typedef enum {
    RX_RESP_NONE    = 0,
    RX_RESP_ACK     = 1 << 0,
    RX_RESP_CTS     = 1 << 1,
    RX_RESP_UNKNOWN = 1 << 2,
    RX_RESP_FRAME   = RX_RESP_ACK | RX_RESP_CTS | RX_RESP_UNKNOWN,

    RX_RESP_STATE_MAX = 0xff
} rx_resp_state_t;

typedef enum {
	TX_SHORT_FRAME,
	TX_LONG_FRAME,

	TX_FRM_MODE_MAX = 0xff
} tx_frame_mode_t;

typedef enum {
	LONG_PREAMBLE,
	SHORT_PREAMBLE,
	PHY_MODE_NUM,
} phy_preamble_mode_t;

#define    PHY_RATE_1M_REG        (0x0A)
#define    PHY_RATE_2M_REG        (0x14)
#define    PHY_RATE_5_5M_REG      (0x37)
#define    PHY_RATE_11M_REG       (0x6E)

/* For HW reason, the size of tx_frame_info_t should be the power of 16. */
typedef struct {
    u8  tx_wait_state;      // tx_wait_state_t, ACK/CTS/NONE
    u8  u8Channel;          // Tx frame at specific channel
    u8  tx_frame_mode;      // tx_frame_mode_t, Short/Long
    u8  curr_rate;          // Setting start from bss_info.wifi_rate_mode, it could be speed down every retry
    u8  tx_phy_mode;        // phy_preamble_mode_t, LONG_PREAMBLE/SHORT_PREAMBLE
    u8  phy_hdr_err;        // Log PHY header error count for checking RX ACK status
    u16 frame_length;       // Number of bytes in MAC packet, including MAC header, without FCS
    wifi_mac_tx_drop_t fpCallback;  // Callback function for TX done/drop
    void *frame_buf_addr;   // tx frame buffer start address
    u8  reserved[16];       // Reserved for extension
} tx_frame_info_t;
#define TX_FRAME_INFO_LEN   sizeof(tx_frame_info_t)

typedef struct {
    /* For HW reason, the structure size should be the power of 16. */
    struct {
        void    *frame_buf_addr;        // rx frame buffer start address
        u32     rssi;                   // RSSI
        u32     timestamp_l;            // Time snap of 7th word of MAC header, low 32-bit
        u32     timestamp_h;            // Time snap of 7th word of MAC header, high 32-bit
        u32     rx_status;
        u8      reserved[12];
    };

    union {
        void    *hw_desc_entry;      // HW descriptor entry

        /* TRX RX descriptor
         * For MSQ, HW will fill the following parameters automatically
         */
        struct {
            u8  rate;               // 0x0A=1M, 0x14=2M, 0x37=5.5M, 0x6E=11M
            u8  preamble_type:1;    // 0:Long, 1:Short
            u8  unused_1:7;
            u16 frame_length;       // Number of bytes in MAC packet, including MAC header, without FCS
        };
    };
} rx_frame_info_t;
#define RX_FRAME_INFO_LEN   sizeof(rx_frame_info_t)

typedef struct {
    u8  rx_resp_state;              // rx_resp_state_t, ACK/CTS/NONE
} wifi_rx_frame_info_t;

typedef enum {
    // Call by interrupt
    WIFI_EVT_TX_MA_GRANT_PROCESS,       // 0
    WIFI_EVT_TX_EN_PROCESS,             // 1
    WIFI_EVT_TX_DONE_PROCESS,           // 2
    WIFI_EVT_TX_TIMEOUT_PROCESS,        // 3
    WIFI_EVT_RX_STATUS_DONE_PROCESS,    // 4
    WIFI_EVT_RX_SUCC_PROCESS,           // 5
    WIFI_EVT_RX_FAIL_PROCESS,           // 6
    WIFI_EVT_TX_UNDERFLW_PROCESS,       // 7
    WIFI_EVT_RX_OVERFLOW_PROCESS,       // 8

    // Call by task
    WIFI_EVT_START,                     // 9
    WIFI_EVT_TERMINATING,               // 10

    WIFI_EVT_RESERVED1,                 // 11
    WIFI_EVT_RESERVED2,                 // 12
    WIFI_EVT_MAX
} wifi_fsm_event_t;

typedef enum {
    WIFI_STS_IDLE,              // 0
    WIFI_STS_TX_IDLE,           // 1
    WIFI_STS_TX_GRANTED,        // 2
    WIFI_STS_TX_IN_PROC,        // 3
    WIFI_STS_WAIT_FOR_RESP,     // 4
    WIFI_STS_WAIT_FOR_RESP_D,   // 5
    WIFI_STS_WAIT_FOR_FRM_D,    // 6
    WIFI_STS_READY_TO_TX_RESP,  // 7
    WIFI_STS_RESP_IN_PROC,      // 8

    WIFI_STS_RESERVED1,         // 9
    WIFI_STS_RESERVED2,         // 10
    WIFI_STS_MAX,

    /* Error states
     * When FSM catch the following states,
     * FSM will do HW reset or other handle then reset state to WIFI_STS_TX_IDLE
     */
    WIFI_STS_ERR,
    WIFI_STS_ERR_MSG,
    WIFI_STS_ERR_KEEP_STATE,
} wifi_fsm_state_t;

typedef enum {
	WIFI_STA_MODE,
	WIFI_AP_MODE,
	WIFI_AP_STA_MODE,
	WIFI_RF_MODE,
	BLE_DTM_MODE,


	CMD_MODE = 99,

} wifi_device_mode_t;

typedef wifi_fsm_state_t (*wifi_mac_fsm_func)(void *data);

typedef enum {
    WIFI_FLG_ERRMSG     = (1U << 0),

    WIFI_FLG_MSK        = WIFI_FLG_ERRMSG
} WifiDcf_Flags_e;

typedef struct {
    u8      u8CurrChnnNum;
    u8      u8RetryCnt;
    u8      u8RetryLimitThr;
    u8      u8CwExp;
    bool    bRateAdaptEn;
    wifi_msq_state_t tMsqState;
    u32     u32FsmStateLog;
    u32     u32FsmEventLog;
    u32     u32Flags;
} WifiDcf_DcfInfo_s;

#ifdef WIFI_DEBUG_MODE
typedef enum {
	WIFI_DATA_FRAME,
	WIFI_AUTH_FRAME,
	WIFI_RTS_FRAME,
	WIFI_ACK_FRAME,
	WIFI_BEACON_FRAME,
	WIFI_PROBE_REQ,
	WIFI_ASSOC_REQ,
	WIFI_REASSOC_REQ,
	WIFI_DISASSOC_REQ,
	WIFI_DEAUTH_REQ
} wifi_tx_type_t;

#define DBG_PARAM_ADDRESS       0x0045FF90

typedef struct
{
    bool    dbg_tx_enable;                  // [0]:Disable Tx test, 1:Enable Tx mode for test
    bool    dbg_tx_wait_ack;                // [1]:wait ack after tx data  0:tx only, don't wait/Rx ack
    bool    dbg_tx_with_dcf;                // [1]:DCF rule, 0:no DCF(CSMA, dbg_tx_wait_ack = 0 is MUST)
    bool    dbg_tx_retry;                   // [1]:enable re-transmitted TX frame, 0:re-transmitted TX frame
/* dbg_tx_data_len:
   data frame length, unit is byte, content is 0xFF
   it shoult not greater than WIFI_MAC_MSQ_TX_BUF_SIZE
   if dbg_tx_data_len = 0, send auth request frame.
 */
    u32     dbg_tx_data_len;                // [10]:data_type frame. range: 0 ~ 104 (WIFI_MAC_APS_TX_BUF_SIZE - 24)
    u32     dbg_tx_data_type;               // [WIFI_DATA_FRAME]:auth frame. (dbg_tx_data_len = 0 is MUST)
    u32     dbg_tx_rts_nav;                 // [2000]:rts nav value (dbg_tx_data_type = WIFI_RTS_FRAME is MUST)
    u32     dbg_tx_count_set;               // [1]:total TX frame will be sent , 0 is unlimited (dbg_tx_retry = 0 is MUST)
    u32     dbg_tx_interval;                // [0]:set SIFS, range: 5 ~ 63 (dbg_tx_wait_ack = 0,  dbg_tx_with_dcf = 0 is MUST)
    u32     dbg_tx_broken_frame;            // [0]:good frame, 1:broken frame (dbg_tx_wait_ack = 0 is MUST)
    u32     dbg_tx_count;                   // read only, total frames were tx
    u8      dbg_tx_phy_mode;                // [0]: LONG_PREAMBLE, [1]: SHORT_PREAMBLE
    u8      dbg_tx_data_rate;               // [0]: 1M, [1]: 2M, [2]: 5.5M, [3]: 11M

    bool    dbg_rx_response_ack;            // [1]:response ack after rx data  0:no response
    bool    dbg_rx_mgmt_fsm;                // [1]:run mgmt FSM, 0:disable mgmt FSM when rx mgmt frame (dbg_tx_retry = 1 is MUST)
    u32     dbg_rx_uc_count;                // read only, total unicast frames were rx (CRC ok)
    u32     dbg_rx_bc_count;                // read only, total broadcast frames were rx (CRC ok)
    u32     dbg_rx_er_count;                // read only, total error frames were rx
    u32     dbg_rx_tout_count;              // read only, total frames without ack response time expired
    u32     dbg_rx_tout_count_crc_ok;       // read only, expect receive an ack but get a frame CRC ok
    u32     dbg_rx_tout_count_crc_nok;      // read only, expect receive an ack but get a frame CRC fail
    u32     dbg_rx_status;
    u32     dbg_rssi;                       // Read only. Update value when INT of TRX_RX_FRAME_DONE happen

    u32     dbg_fsm_err_count;              // Read only. Increment 1 when enter undefined cases

    bool    dbg_msq_buffer_full;            // [0]:MSQ RX buffer is not full, [1]: MSQ RX buffer is full
    bool    dbg_aps_buffer_full;            // [0]:APS RX buffer is not full, [1]: APS RX buffer is full
} dbg_trx_config_param_t;

//#define dbg_trx_param       ((dbg_trx_config_param_t *) DBG_PARAM_ADDRESS)
//extern dbg_trx_config_param_t dbg_trx_param;

void wifi_print(void);
#endif // WIFI_DEBUG_MODE

extern u32 dbg_spi_late_t2r_count;
extern u32 dbg_spi_late_t2r_max;
extern u32 dbg_spi_late_r2t_count[2];
extern u32 dbg_spi_late_r2t_max[2];

//extern u32 run_wifi_mac_sta_mgmt_fsm;
extern tx_frame_info_t *g_tx_frame_info;
extern wifi_rx_frame_info_t g_wifi_rx_frame_info;

void wifi_mac_dcf_init(void);
void wifi_mac_dcf_reset(void);
void wifi_mac_dcf_start(void);
void wifi_mac_dcf_stop(void);

int wifi_fsm(wifi_fsm_event_t current_event, void *data);
void wifi_mac_service(void);
void wifi_mac_sta_init(void);
void wifi_mac_ap_init(void);

void wifi_mac_clr_silence_count(void);
void wifi_mac_beacon_schedule(bool enable, bool reset);

#endif /* _WIFI_MAC_DCF_H_ */

