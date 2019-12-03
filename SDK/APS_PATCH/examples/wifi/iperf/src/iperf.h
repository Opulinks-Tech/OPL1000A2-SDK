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

#ifndef _IPER_H_
#define _IPER_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IPERF_FLAG_CLIENT (1)
#define IPERF_FLAG_SERVER (1<<1)
#define IPERF_FLAG_DUAL   (1<<2)
#define IPERF_FLAG_TCP    (1<<3)
#define IPERF_FLAG_UDP    (1<<4)
#define IPERF_FLAG_ABORT  (1<<15)

#define IPERF_CLIENT_TASK_NAME       "iperf_traf_client"
#define IPERF_CLIENT_TASK_PRIORITY   0
#define IPERF_CLIENT_TASK_STACK      512
#define IPERF_SERVER_TASK_NAME       "iperf_traf_server"
#define IPERF_SERVER_TASK_PRIORITY   0
#define IPERF_SERVER_TASK_STACK      512
#define IPERF_REPORT_TASK_NAME       "iperf_report"
#define IPERF_REPORT_TASK_PRIORITY    1
#define IPERF_REPORT_TASK_STACK       256

//for iperf 2.0.10 default udp maximum size is 1460, including IPV4 header:20 and UDP header: 8
//for iperf 2.0.09 you can set 1472
#define IPERF_UDP_TX_LEN     (1432)
#define IPERF_UDP_RX_LEN     (16<<7)
#define IPERF_TCP_TX_LEN     (16<<8)
#define IPERF_TCP_RX_LEN     (16<<6)

#define IPERF_MAX_DELAY      64
#define IPERF_UDP_TX_MAX_RATE_DEF  1024 * 1024 * 2 //2Mbps

#define IPERF_SOCKET_RX_TIMEOUT      10
#define IPERF_SOCKET_ACCEPT_TIMEOUT  5

#define HEADER_VERSION1 0x80000000
#define HEADER_EXTEND   0x40000000 //Iperf 2.0.10

#define IPERF_TCP_CLIENT_FT_IDX 0x09
#define IPERF_TCP_SERVER_FT_IDX 0x0A
#define IPERF_TCP_CLIENT_DUAL_FT_IDX   0x0D
#define IPERF_UDP_CLIENT_FT_IDX 0x11
#define IPERF_UDP_SERVER_FT_IDX 0x12
#define IPERF_UDP_CLIENT_DUAL_FT_IDX   0x15

typedef struct {
    bool     finish;
    uint32_t total_len;
    uint32_t last_len;
    uint32_t buffer_len;
    uint8_t  *buffer;
    int      sockfd;
} iperf_ctrl_t;

typedef struct {
    uint32_t id;
    uint32_t sec;
    uint32_t usec;
    uint32_t id2; //Iperf 2.0.10
} iperf_udp_pkt_t;

typedef struct {
    uint32_t flag;
    uint32_t dip;
    uint32_t sip;
    uint16_t dport;
    uint16_t sport;
    uint32_t interval;
    uint32_t time;
    uint32_t mAmount;
} iperf_cfg_t;

#if 0 //Iperf 2.0.9
/*
 * The client_hdr structure is sent from clients
 * to servers to alert them of things that need
 * to happen. Order must be perserved in all 
 * future releases for backward compatibility.
 * 1.7 has flags, numThreads, mPort, and bufferlen
 */
typedef struct client_hdr {
    /*
     * flags is a bitmap for different options
     * the most significant bits are for determining
     * which information is available. So 1.7 uses
     * 0x80000000 and the next time information is added
     * the 1.7 bit will be set and 0x40000000 will be
     * set signifying additional information. If no 
     * information bits are set then the header is ignored.
     * The lowest order diferentiates between dualtest and
     * tradeoff modes, wheither the speaker needs to start 
     * immediately or after the audience finishes.
     */
    int flags;
    int numThreads;
    int mPort;
    int bufferlen;
    int mWindowSize;
    int mAmount;
    int mRate;
    int mUDPRateUnits;
    int mRealtime;
} iperf_client_hdr_t;
#endif

#if 1
/********* Iperf 2.0.10 *********/

/*
 * The client_hdr structure is sent from clients
 * to servers to alert them of things that need
 * to happen. Order must be perserved in all
 * future releases for backward compatibility.
 * 1.7 has flags, numThreads, mPort, and bufferlen
 */
typedef struct client_hdr_v1 {
    /*
     * flags is a bitmap for different options
     * the most significant bits are for determining
     * which information is available. So 1.7 uses
     * 0x80000000 and the next time information is added
     * the 1.7 bit will be set and 0x40000000 will be
     * set signifying additional information. If no
     * information bits are set then the header is ignored.
     * The lowest order diferentiates between dualtest and
     * tradeoff modes, wheither the speaker needs to start
     * immediately or after the audience finishes.
     */
    int flags;
    int numThreads;
    int mPort;
    int bufferlen;
    int mWinBand;
    int mAmount;
} client_hdr_v1;

typedef struct hdr_typelen {
    int type;
    int length;
} hdr_typelen;

typedef struct client_hdrext {
    hdr_typelen typelen;
    int flags;
    int version_u;
    int version_l;
    int reserved;
    int mRate;
    int mUDPRateUnits;
    int mRealtime;
} client_hdrext;

typedef struct client_hdr {
    client_hdr_v1 base;
    client_hdrext extend;
} iperf_client_hdr_t;

#endif

typedef int (*iperf_handle_fp_t) (void);
typedef struct iperf_func_fp {
    const uint32_t idx;
    iperf_handle_fp_t func;
}iperf_func_fp_t;


int iperf_start(iperf_cfg_t *cfg);
int iperf_stop(void);
void iperf_run_udp_client(void *arg);
void iperf_run_tcp_client(void *arg);

#ifdef __cplusplus
}
#endif

#endif /* _IPER_H_ */
