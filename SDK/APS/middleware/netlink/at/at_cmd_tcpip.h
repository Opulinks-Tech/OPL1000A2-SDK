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
/**
 * @file at_cmd_tcpip.h
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File containing declaration of at_cmd_tcpip api & definition of structure for reference.
 *
 */

#ifndef __AT_CMD_TCPIP_H__
#define __AT_CMD_TCPIP_H__

#include <stdint.h>
#include "lwip/sockets.h"
#include "at_cmd_nvm.h"

#define AT_DATA_TASK_QUEUE_SIZE 10
#define AT_LINK_MAX_NUM         5

#define AT_DATA_LEN_MAX     2048
#define AT_DATA_RX_BUFSIZE  1024

#define AT_DATA_TX_EVENT        0x0001
#define AT_DATA_TIMER_EVENT     0x0002

#define AT_SERVER_DEFAULT_PORT      (333)

#define IP2STR(ipaddr) ip4_addr1_16(ip_2_ip4(ipaddr)), \
    ip4_addr2_16(ip_2_ip4(ipaddr)), \
    ip4_addr3_16(ip_2_ip4(ipaddr)), \
    ip4_addr4_16(ip_2_ip4(ipaddr))

#define IPSTR "%d.%d.%d.%d"


#define IPV42STR(ipaddr) ip4_addr1_16(ipaddr), \
    ip4_addr2_16(ipaddr), \
    ip4_addr3_16(ipaddr), \
    ip4_addr4_16(ipaddr)

typedef struct {
    ip4_addr_t ip;
    ip4_addr_t netmask;
    ip4_addr_t gw;
} net_adapter_ip_info_t;

typedef enum {
    TCPIP_ADAPTER_IF_STA = 0,     /**< station interface */
    TCPIP_ADAPTER_IF_AP,          /**< soft-AP interface */
    TCPIP_ADAPTER_IF_MAX
} net_adapter_if_t;

typedef struct {
    uint32_t event;
	uint32_t length;
	uint8_t *param;
} at_event_msg_t;


/** @brief
 * This enum defines the status of the station interface
 */
typedef enum {
    AT_STA_INACTIVE = 0,
    AT_STA_IDLE,
    AT_STA_GOT_IP,             /**<station got IP from connected AP.*/
    AT_STA_LINKED,             /**<station has created a TCP or UDP transmission.*/
    AT_STA_UNLINKED,           /**<The TCP or UDP transmission of station is disconnected */
    AT_STA_DISCONNECT          /**<station does not connect to an AP.*/
} at_state_type_t;


/** @brief
 * This enum defines the types of device running as a client or server
 */
typedef enum {
    AT_LOCAL_CLIENT = 0,
    AT_REMOTE_CLIENT
} at_terminal_type;


typedef enum {
    AT_LINK_DISCONNECTED = 0,
    AT_LINK_CONNECTING,
    AT_LINK_CONNECTED,
    AT_LINK_WAIT_SENDING,
    AT_LINK_SENDING,
    AT_LINK_WAIT_SENDING_CB,
    AT_LINK_TRANSMIT_CONNECTED,
    AT_LINK_TRANSMIT_SEND,
    AT_LINK_TRANSMIT_NON_CONN,
    AT_LINK_DISCONNECTING,
    AT_LINK_CHANGED
} at_link_state;


enum {
    AT_SEND_MODE_IDLE = 0,
    AT_SEND_MODE_SEND,
    AT_SEND_MODE_SENDEX,
    AT_SEND_MODE_TRANSMIT,
};


/** @brief
 * This enum defines the TCPIP link transmission type.
 */
typedef enum {
    AT_LINK_TYPE_TCP = 0,
    AT_LINK_TYPE_UDP,
    AT_LINK_TYPE_SSL,
    AT_LINK_TYPE_INVALID
} at_link_type_t;


typedef struct {
    at_link_state link_state;
    at_terminal_type terminal_type;
    int sock;
    int link_en;
    uint8_t send_mode;
    uint8_t link_id;
    uint8_t link_type;
    uint8_t timeout;
    uint8_t repeat_time;
    uint8_t change_mode;
    ip_addr_t remote_ip;
    uint16_t remote_port;
    uint16_t local_port;

    int32_t keep_alive;
    int32_t server_timeout;
    void* task_handle;
    char* recv_buf;
} at_socket_t;

extern uint32_t g_at_trans_recreate_ms;

/*
 * @brief AT Command Interface Initialization for TCP/IP modules
 *
 */
void at_cmd_tcpip_func_init(void);

//forward declare
struct _ping_result;
typedef void * TimerHandle_t;

typedef int  (*net_adapter_get_ip_info_fp_t)(net_adapter_if_t tcpip_if, net_adapter_ip_info_t *ip_info);
typedef int  (*valid_digit_fp_t)(char *ip_str);
typedef int  (*at_cmd_is_valid_ip_fp_t)(char *sIpStr);
typedef void (*at_cmd_ping_callback_fp_t)(struct _ping_result *ptResult);
typedef int  (*at_cmd_is_tcpip_ready_fp_t)(void);
typedef int  (*at_cmd_get_param_as_str_fp_t)(char *param, char **result);
typedef int  (*at_get_socket_error_code_fp_t)(int sockfd);
typedef int  (*at_show_socket_error_reason_fp_t)(const char *str, int sockfd);
typedef void (*at_update_link_count_fp_t)(int8_t count);
typedef void (*at_link_init_fp_t)(uint32_t conn_max);
typedef at_socket_t *(*at_link_alloc_fp_t)(void);
typedef at_socket_t *(*at_link_alloc_id_fp_t)(int id);
typedef at_socket_t *(*at_link_get_id_fp_t)(int id);
typedef int (*at_resolve_dns_fp_t)(const char *host, struct sockaddr_in *ip);
typedef int (*at_create_tcp_server_fp_t)(int port, int timeout_ms);
typedef int (*at_create_tcp_client_fp_t)(at_socket_t *link, const char *host, int port, int timeout_ms);
typedef int (*at_create_udp_client_fp_t)(at_socket_t *link, const char *host, int remote_port, int local_port, int timeout_ms);
typedef int (*at_close_client_fp_t)(at_socket_t *link);
typedef int (*at_ip_send_data_fp_t)(uint8_t *pdata, int send_len);
typedef int (*at_socket_read_fp_t)(at_socket_t *link, char *buf, int min_len, int max_len, int *p_read_len);
typedef int (*at_socket_read_set_timeout_fp_t)(at_socket_t *plink, int timeout_ms);
typedef int (*at_socket_write_set_timeout_fp_t)(at_socket_t *plink, int timeout_ms);
typedef int (*at_build_fd_sets_fp_t)(at_socket_t *server, fd_set *read_fds, fd_set *write_fds, fd_set *except_fds);
typedef void (*at_receive_from_peer_fp_t)(at_socket_t *plink);
typedef int  (*at_handle_new_connection_fp_t)(int listen_sock);
typedef int  (*at_process_socket_nonblock_fp_t)(at_socket_t *plink);
typedef void (*at_process_listen_socket_fp_t)(void *arg);
typedef void (*at_process_recv_socket_fp_t)(at_socket_t *plink);
typedef int  (*at_data_task_post_fp_t)(at_event_msg_t *msg, uint32_t timeout);
typedef int  (*at_data_task_send_fp_t)(at_event_msg_t *msg);
typedef void (*at_data_tx_task_fp_t)(void *arg);
typedef void (*at_socket_process_task_fp_t)(void *arg);
typedef void (*at_create_tcpip_tx_task_fp_t)(void);
typedef void (*at_create_tcpip_data_task_fp_t)(void);
typedef int  (*at_socket_client_create_task_fp_t)(at_socket_t *link);
typedef int  (*at_socket_client_cleanup_task_fp_t)(at_socket_t* plink);
typedef void (*at_socket_server_listen_task_fp_t)(void *arg);
typedef int  (*at_socket_server_create_task_fp_t)(int sock);
typedef int  (*at_socket_server_cleanup_task_fp_t)(int sock);
typedef void (*at_server_timeout_cb_fp_t)( TimerHandle_t xTimer );
typedef void (*at_server_timeout_handler_fp_t)(void);
typedef int  (*at_create_tcp_client_trans_fp_t)(at_socket_t *link);
typedef void (*at_trans_timeout_cb_fp_t)(TimerHandle_t xTimer);
typedef int  (*at_cmd_trans_lock_fp_t)(void);
typedef int  (*at_trans_client_init_fp_t)(at_nvm_trans_config_t *trans_cfg);

extern net_adapter_get_ip_info_fp_t           net_adapter_get_ip_info;
extern valid_digit_fp_t                       valid_digit;
extern at_cmd_is_valid_ip_fp_t                at_cmd_is_valid_ip;
extern at_cmd_ping_callback_fp_t              at_cmd_ping_callback;
extern at_cmd_is_tcpip_ready_fp_t             at_cmd_is_tcpip_ready;
extern at_cmd_get_param_as_str_fp_t           at_cmd_get_param_as_str;
extern at_get_socket_error_code_fp_t          at_get_socket_error_code;
extern at_show_socket_error_reason_fp_t       at_show_socket_error_reason;
extern at_update_link_count_fp_t              at_update_link_count;
extern at_link_init_fp_t                      at_link_init;
extern at_link_alloc_fp_t                     at_link_alloc;
extern at_link_alloc_id_fp_t                  at_link_alloc_id;
extern at_link_get_id_fp_t                    at_link_get_id;
extern at_resolve_dns_fp_t                    at_resolve_dns;
extern at_create_tcp_server_fp_t              at_create_tcp_server;
extern at_create_tcp_client_fp_t              at_create_tcp_client;
extern at_create_udp_client_fp_t              at_create_udp_client;
extern at_close_client_fp_t                   at_close_client;
extern at_ip_send_data_fp_t                   at_ip_send_data;
extern at_socket_read_fp_t                    at_socket_read;
extern at_socket_read_set_timeout_fp_t        at_socket_read_set_timeout;
extern at_socket_write_set_timeout_fp_t       at_socket_write_set_timeout;
extern at_build_fd_sets_fp_t                  at_build_fd_sets;
extern at_receive_from_peer_fp_t              at_receive_from_peer;
extern at_handle_new_connection_fp_t          at_handle_new_connection;
extern at_process_socket_nonblock_fp_t        at_process_socket_nonblock;
extern at_process_listen_socket_fp_t          at_process_listen_socket;
extern at_process_recv_socket_fp_t            at_process_recv_socket;
extern at_data_task_post_fp_t                 at_data_task_post;
extern at_data_task_send_fp_t                 at_data_task_send;
extern at_data_tx_task_fp_t                   at_data_tx_task;
extern at_socket_process_task_fp_t            at_socket_process_task;
extern at_create_tcpip_tx_task_fp_t           at_create_tcpip_tx_task;
extern at_create_tcpip_data_task_fp_t         at_create_tcpip_data_task;
extern at_socket_client_create_task_fp_t      at_socket_client_create_task;
extern at_socket_client_cleanup_task_fp_t     at_socket_client_cleanup_task;
extern at_socket_server_listen_task_fp_t      at_socket_server_listen_task;
extern at_socket_server_create_task_fp_t      at_socket_server_create_task;
extern at_socket_server_cleanup_task_fp_t     at_socket_server_cleanup_task;
extern at_server_timeout_cb_fp_t              at_server_timeout_cb;
extern at_server_timeout_handler_fp_t         at_server_timeout_handler;
extern at_create_tcp_client_trans_fp_t        at_create_tcp_client_trans;
extern at_trans_timeout_cb_fp_t               at_trans_timeout_cb;
extern at_cmd_trans_lock_fp_t                 at_cmd_trans_lock;
extern at_trans_client_init_fp_t              at_trans_client_init;

#endif //__AT_CMD_TCPIP_H__

