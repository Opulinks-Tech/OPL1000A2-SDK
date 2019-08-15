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

#ifndef __AT_CMD_COMMOM_H__
#define __AT_CMD_COMMOM_H__

#include "hal_pin_def.h"

/**
 * @file at_cmd_common.h
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File containing declaration of at_cmd_common api & definition of structure for reference.
 *
 */

/**
 * @brief UART1 Mode Types
 *
 */
enum {
	UART1_MODE_AT = 0,       /**< AT Mode (default). */
	UART1_MODE_BLE_HCI = 1,  /**< BLE HCI Mode. */
	UART1_MODE_BLE_HOST = 2, /**< BLE HOST Mode (Need to confirm with BLE Protocol Stack owner that if it's necessary). */
	UART1_MODE_OTHERS = 3,   /**< Reserved */
	UART1_MODE_NUM
};

/**
 * @brief The Default is AT mode
 *
 */
#define UART1_MODE_DEFAULT UART1_MODE_AT

/**
 * @brief AT_VER
 *
 */
#define AT_VER "1.0"

enum {
    AT_STATE_IDLE,              // idle
    AT_STATE_PROCESSING,        // at mode, in busy
    AT_STATE_SENDING_RECV,      // UART1 ISR push receive data to cmd line and switch to this state then notify AT task
    AT_STATE_SENDING_DATA,      // sending data mode
    AT_STATE_TRANSMIT           // transmit transparently
};

typedef enum {
    AT_RESULT_CODE_OK           = 0x00,
    AT_RESULT_CODE_ERROR        = 0x01,
    AT_RESULT_CODE_FAIL         = 0x02,
    AT_RESULT_CODE_SEND_OK      = 0x03,
    AT_RESULT_CODE_SEND_FAIL    = 0x04,
    AT_RESULT_CODE_IGNORE       = 0x05,
    AT_RESULT_CODE_PROCESS_DONE = 0x06, // not response string
    AT_RESULT_CODE_MAX
} at_result_code_string_index_t;

#define at_printf(fmt,...)          printf(fmt,##__VA_ARGS__)
#define at_strlen(s)                strlen((char*)(s))
#define at_strcpy(dest,src)         strcpy((char*)(dest),(char*)(src))
#define at_strncpy(dest,src,n)      strncpy((char*)(dest),(char*)(src),n)
#define at_strcmp(s1,s2)            strcmp((char*)(s1),(char*)(s2))
#define at_strncmp(s1,s2,n)         strncmp((char*)(s1),(char*)(s2),n)
#define at_strstr(s1,s2)            strstr((char*)(s1),(char*)(s2))
#define at_sprintf(s,...)           sprintf((char*)(s), ##__VA_ARGS__)
#define at_snprintf(s,size,...)     snprintf((char*)(s),size, ##__VA_ARGS__)
#define at_memset(dest,x,n)         memset(dest,x,n)
#define at_memcpy(dest,src,n)       memcpy(dest,src,n)
#define at_memcmp(s1,s2,n)          memcmp(s1,s2,n)
#define at_malloc(size)             malloc(size)
#define at_free(ptr)                free(ptr)
#define at_output                   msg_print_uart1


/**
 * @brief Function Pointer Type for API msg_print_uart1
 *
 */
typedef void (*msg_print_uart1_fp_t)(char *fmt,...);

/**
 * @brief Function Pointer Type for API uart1_mode_get
 *
 */
typedef int (*uart1_mode_get_fp_t)(void);

/**
 * @brief Function Pointer Type for API uart1_mode_set
 *
 */
typedef int (*uart1_mode_set_fp_t)(int argc, char *argv[]);

/**
 * @brief Function Pointer Type for API uart1_mode_set_at
 *
 */
typedef void (*uart1_mode_set_at_fp_t)(void);

/**
 * @brief Function Pointer Type for API uart1_mode_set_ble_hci
 *
 */
typedef void (*uart1_mode_set_ble_hci_fp_t)(void);

/**
 * @brief Function Pointer Type for API uart1_mode_set_ble_host
 *
 */
typedef void (*uart1_mode_set_ble_host_fp_t)(void);

/**
 * @brief Function Pointer Type for API uart1_mode_set_others
 *
 */
typedef void (*uart1_mode_set_others_fp_t)(void);

/**
 * @brief Function Pointer Type for API uart1_mode_set_default
 *
 */
typedef void (*uart1_mode_set_default_fp_t)(void);

/**
 * @brief Function Pointer Type for API uart1_rx_int_do_at
 *
 */
typedef void (*uart1_rx_int_do_at_fp_t)(uint32_t u32Data);

/**
 * @brief Function Pointer Type for API uart1_rx_int_do_ble_hci
 *
 */
typedef void (*uart1_rx_int_do_ble_hci_fp_t)(uint32_t u32Data);

/**
 * @brief Function Pointer Type for API uart1_rx_int_do_ble_host
 *
 */
typedef void (*uart1_rx_int_do_ble_host_fp_t)(uint32_t u32Data);

/**
 * @brief Function Pointer Type for API uart1_rx_int_do_others
 *
 */
typedef void (*uart1_rx_int_do_others_fp_t)(uint32_t u32Data);

/**
 * @brief Function Pointer Type for API uart1_rx_int_handler_default
 *
 */
typedef void (*uart1_rx_int_handler_default_fp_t)(uint32_t u32Data);

/**
 * @brief Function Pointer Type for API at_clear_uart_buffer
 *
 */
typedef void (*at_clear_uart_buffer_fp_t)(void);

/**
 * @brief Function Pointer Type for API set_echo_on
 *
 */
typedef void (*set_echo_on_fp_t)(int on);

/**
 * @brief Function Pointer Type for API uart1_rx_int_at_data_receive_ble
 *
 */
typedef void (*uart1_rx_int_at_data_receive_ble_fp_t)(uint32_t u32Data);

/**
 * @brief Function Pointer Type for API uart1_rx_int_at_data_receive_tcpip
 *
 */
typedef void (*uart1_rx_int_at_data_receive_tcpip_fp_t)(uint32_t u32Data);

/**
 * @brief Function Pointer Type for API at_cmd_get_para_as_digital
 *
 */
typedef int (*at_cmd_get_para_as_digital_fp_t)(char *para, int *result);

typedef void (*at_cmd_switch_uart1_dbguart_fp_t)(void);
typedef void (*at_io01_uart_mode_set_fp)(E_IO01_UART_MODE eMode);

/**
 * @brief Extern Function msg_print_uart1
 *
 */
extern msg_print_uart1_fp_t msg_print_uart1;

/**
 * @brief Extern Function uart1_mode_get
 *
 */
extern uart1_mode_get_fp_t uart1_mode_get;

/**
 * @brief Extern Function uart1_mode_set
 *
 */
extern uart1_mode_set_fp_t uart1_mode_set;

/**
 * @brief Extern Function uart1_mode_set_at
 *
 */
extern uart1_mode_set_at_fp_t uart1_mode_set_at;

/**
 * @brief Extern Function uart1_mode_set_ble_hci
 *
 */
extern uart1_mode_set_ble_hci_fp_t uart1_mode_set_ble_hci;

/**
 * @brief Extern Function uart1_mode_set_ble_host
 *
 */
extern uart1_mode_set_ble_host_fp_t uart1_mode_set_ble_host;

/**
 * @brief Extern Function uart1_mode_set_others
 *
 */
extern uart1_mode_set_others_fp_t uart1_mode_set_others;

/**
 * @brief Extern Function uart1_mode_set_default
 *
 */
extern uart1_mode_set_default_fp_t uart1_mode_set_default;

/**
 * @brief Extern Function uart1_rx_int_do_at
 *
 */
extern uart1_rx_int_do_at_fp_t uart1_rx_int_do_at;

/**
 * @brief Extern Function uart1_rx_int_do_ble_hci
 *
 */
extern uart1_rx_int_do_ble_hci_fp_t uart1_rx_int_do_ble_hci;

/**
 * @brief Extern Function uart1_rx_int_do_ble_host
 *
 */
extern uart1_rx_int_do_ble_host_fp_t uart1_rx_int_do_ble_host;

/**
 * @brief Extern Function uart1_rx_int_do_others
 *
 */
extern uart1_rx_int_do_others_fp_t uart1_rx_int_do_others;

/**
 * @brief Extern Function uart1_rx_int_handler_default
 *
 */
extern uart1_rx_int_handler_default_fp_t uart1_rx_int_handler_default;

/**
 * @brief Extern Function at_clear_uart_buffer
 *
 */
extern at_clear_uart_buffer_fp_t at_clear_uart_buffer;

/**
 * @brief Extern Function set_echo_on
 *
 */
extern set_echo_on_fp_t set_echo_on;

/**
 * @brief Extern Function uart1_rx_int_at_data_receive_ble
 *
 */
extern uart1_rx_int_at_data_receive_ble_fp_t uart1_rx_int_at_data_receive_ble;

/**
 * @brief Extern Function uart1_rx_int_at_data_receive_tcpip
 *
 */
extern uart1_rx_int_at_data_receive_tcpip_fp_t uart1_rx_int_at_data_receive_tcpip;


extern at_io01_uart_mode_set_fp at_io01_uart_mode_set;

/**
 * @brief Extern Function at_cmd_get_para_as_digital
 *
 */
extern at_cmd_get_para_as_digital_fp_t at_cmd_get_para_as_digital;

/*
 * @brief at_cmd_common_func_init - AT Common Interface Initialization (AT Common)
 *
 */
void at_cmd_common_func_init(void);

extern at_cmd_switch_uart1_dbguart_fp_t at_cmd_switch_uart1_dbguart;

char *at_cmd_param_trim(char *sParam);
int at_cmd_buf_to_argc_argv(char *pbuf, int *argc, char *argv[], int iArgvNum);

void at_uart1_write_buffer(char *buf, int len);
void at_uart1_printf(char *str);
void at_response_result(uint8_t result_code);

int wpas_get_state(void);
int wpas_get_assoc_freq(void);

#endif /* __AT_CMD_COMMOM_H__ */

