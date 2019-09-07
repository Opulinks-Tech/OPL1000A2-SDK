/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */

#ifndef BZ_TRANSPORT_H
#define BZ_TRANSPORT_H

#include <stdint.h>
#include "common.h"
#include "breeze_hal_os.h"
#include "bzopt.h"
#include "ble_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

enum {
    TX_NOTIFICATION,
    TX_INDICATION,
};

typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

typedef uint8_t		UINT8;
typedef uint16_t	UINT16;
typedef uint32_t	UINT32;


typedef LE_ERR_STATE (*transport_tx_func_t)(UINT16 conn_hdl, UINT16 hdl, UINT16 len, UINT8 *pval);

//typedef uint32_t (*transport_tx_func_t)(uint8_t *p_data, uint16_t length);

#define TX_BUFF_LEN (BZ_MAX_SUPPORTED_MTU - 3)
#define RX_BUFF_LEN BZ_MAX_PAYLOAD_SIZE
typedef struct transport_s {
    struct {
        uint8_t buff[TX_BUFF_LEN];
        uint8_t *data;
        uint16_t len;
        uint16_t bytes_sent;
        uint8_t encrypted;
        uint8_t msg_id;
        uint8_t cmd;
        uint8_t total_frame;
        uint8_t frame_seq;
        uint8_t zeroes_padded;
        uint16_t pkt_req;
        uint16_t pkt_cfm;
        os_timer_t timer;
        transport_tx_func_t active_func;
				uint16_t hdl; //Kevin add it
				uint16_t conn_hdl; //Kevin add it
    } tx;
    struct {
        uint8_t buff[RX_BUFF_LEN];
        uint16_t buff_size;
        uint16_t bytes_received;
        uint8_t msg_id;
        uint8_t cmd;
        uint8_t total_frame;
        uint8_t frame_seq;
        os_timer_t timer;
    } rx;
    uint16_t max_pkt_size;
    void *p_key;
    uint16_t timeout;
    void *p_aes_ctx;
} transport_t;

struct rx_cmd_post_t{
    uint8_t cmd;
    uint8_t frame_seq;
    uint8_t *p_rx_buf;
    uint16_t buf_sz;
};

ret_code_t transport_init(ali_init_t const *p_init);
void transport_reset(void);
ret_code_t transport_tx(uint8_t tx_type, uint8_t cmd,
                        uint8_t const *const p_data, uint16_t length, transport_tx_func_t active_func, int conn_hdl, int hdl);
void transport_txdone(uint8_t tx_type, uint16_t pkt_sent);
void transport_rx(uint8_t *p_data, uint16_t length);
uint32_t transport_update_key(uint8_t *p_key);
void reset_tx(void);
void reset_rx(void);

#ifdef __cplusplus
}
#endif

#endif // BZ_TRANSPORT_H
