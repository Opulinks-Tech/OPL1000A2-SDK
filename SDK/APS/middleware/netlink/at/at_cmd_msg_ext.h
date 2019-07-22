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
 * @file at_cmd_msg_ext.h
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File containing declaration of at_cmd_msg_ext api & definition of structure for reference.
 *
 */

#ifndef __AT_CMD_MSG_EXT_H__
#define __AT_CMD_MSG_EXT_H__
#include <stddef.h>
#include <ctype.h>
#include "controller_wifi_com.h"
#include "wifi_types.h"

/**
 * @brief AT Extending Customer's Message Type
 *
 */
enum at_msg_ext_type{
	AT_MSG_EXT_AMPEC = 0,     /**< Message for APMEC AT Command. */
	AT_MSG_EXT_ESPRESSIF = 1, /**< Message for Espressif AT Command. */
	AT_MSG_EXT_OTHERS = 2,    /**< Message for others AT Command. */
	AT_MSG_EXT_NUM
};

/**
 * @brief AT Extending Error Code for CWJAP
 *
 */
typedef enum {
    ERR_WIFI_CWJAP_DONE,
    ERR_WIFI_CWJAP_TO,
    ERR_WIFI_CWJAP_PWD_INVALID,
    ERR_WIFI_CWJAP_NO_AP,
    ERR_WIFI_CWJAP_FAIL,
    ERR_WIFI_CWJAP_FAIL_OTHERS,
    ERR_WIFI_CWJAP_DISCONNECT = 10, 
} at_wifi_error_code_cwjap;

/**
 * @brief AT Extending Error Code for CWAUTOCONN
 *
 */
typedef enum {
    ERR_WIFI_CWAUTOCONN_INVALID = 1,
} at_wifi_error_code_cwautoconn;

/**
 * @brief AT Extending Error Code for CWFASTCONN
 *
 */
typedef enum {
    ERR_WIFI_CWFASTCONN_INVALID = 1,
    ERR_WIFI_CWFASTCONN_AP_NULL,
    ERR_WIFI_CWFASTCONN_PARAMETER_TOO_FEW,
} at_wifi_error_code_cwfastconn;

typedef enum {
    ERR_COMM_DONE,
    ERR_COMM_INVALID,
} at_wifi_error_common_e;

/**
 * @brief AT Extending Message for WIFI
 *
 */
typedef enum {
    MSG_WIFI_CONNECTED_OPEN,
    MSG_WIFI_CONNECTED_SECURITY,
    MSG_WIFI_DISCONNECTED,
    MSG_WIFI_NUM
} at_wifi_msg_code_connected;

/**
 * @brief Function Pointer Type for API at_msg_ext_wifi_scan
 *
 */
typedef void (*at_msg_ext_wifi_scan_fp_t)(int cusType, scan_report_t *result);

/**
 * @brief Function Pointer Type for API at_msg_ext_wifi_disconnect
 *
 */
typedef void (*at_msg_ext_wifi_disconnect_fp_t)(int cusType);

/**
 * @brief Function Pointer Type for API at_msg_ext_wifi_get_rssi
 *
 */
typedef void (*at_msg_ext_wifi_get_rssi_fp_t)(int cusType, int rssi);

/**
 * @brief Function Pointer Type for API at_msg_ext_wifi_err
 *
 */
typedef void (*at_msg_ext_wifi_err_fp_t)(int cusType, char *cmd_str, int error_code);

/**
 * @brief Function Pointer Type for API at_msg_ext_wifi_connect
 *
 */
typedef void (*at_msg_ext_wifi_connect_fp_t)(int cusType, int msg_code);

/**
 * @brief Function Pointer Type for API scan_report_sorting
 *
 */
typedef void (*at_msg_ext_wifi_sorting_fp_t)(wifi_scan_list_t *data);

/**
 * @brief Function Pointer Type for API scan list filtering
 *
 */
typedef void (*at_msg_ext_wifi_show_ap_by_filter_fp_t)(void);

/**
 * @brief Function Pointer Type for API filter one AP
 *
 */
typedef void (*at_msg_ext_wifi_show_one_ap_fp_t)(int argc, char *argv[]);

/**
 * @brief Function Pointer Type for API list show all AP
 *
 */
typedef void (*at_msg_ext_wifi_show_all_fp_t)(int argc, char *argv[]);

/**
 * @brief Function Pointer Type for API scan sorting
 *
 */
typedef void (*at_msg_ext_wifi_scan_sorting_fp_t)(u8 num, wifi_scan_info_t *data);

/**
 * @brief Function Pointer Type for API scan by option
 *
 */
typedef void (*at_msg_ext_wifi_scan_by_option_fp_t)(u8 num, wifi_scan_info_t *scan_list);

/**
 * @brief Function Pointer Type for dispatch connect reason
 *
 */
typedef void (*at_msg_ext_wifi_dispatch_connect_reason_fp_t) (bool connected, int reason);

/**
 * @brief Extern Function at_msg_ext_wifi_scan
 *
 */
//extern at_msg_ext_wifi_scan_fp_t at_msg_ext_wifi_scan;

/**
 * @brief Extern Function at_msg_ext_wifi_disconnect
 *
 */
extern at_msg_ext_wifi_disconnect_fp_t at_msg_ext_wifi_disconnect;

/**
 * @brief Extern Function at_msg_ext_wifi_get_rssi
 *
 */
extern at_msg_ext_wifi_get_rssi_fp_t at_msg_ext_wifi_get_rssi;

/**
 * @brief Extern Function at_msg_ext_wifi_err
 *
 */
extern at_msg_ext_wifi_err_fp_t at_msg_ext_wifi_err;

/**
 * @brief Extern Function at_msg_ext_wifi_connect
 *
 */
extern at_msg_ext_wifi_connect_fp_t at_msg_ext_wifi_connect;

/**
 * @brief Extern Function scan_report_sorting
 *
 */
extern at_msg_ext_wifi_sorting_fp_t at_msg_ext_wifi_sorting;

/**
 * @brief Extern Function scan list filtering
 *
 */
extern at_msg_ext_wifi_show_ap_by_filter_fp_t at_msg_ext_wifi_show_ap_by_filter;

/*
 * @brief AT Command extending message interface Initialization
 *
 */
void at_msg_ext_init(void);

/**
 * @brief Extern Function scan list filtering one ap
 *
 */
extern at_msg_ext_wifi_show_one_ap_fp_t at_msg_ext_wifi_show_one_ap;

/**
 * @brief Function Pointer Type for API list show all AP
 *
 */
extern at_msg_ext_wifi_show_all_fp_t at_msg_ext_wifi_show_all;

/**
 * @brief Function Pointer Type for API scan sorting
 *
 */
extern at_msg_ext_wifi_scan_sorting_fp_t at_msg_ext_wifi_scan_sorting;

/**
 * @brief Function Pointer Type for API scan by option
 *
 */
extern at_msg_ext_wifi_scan_by_option_fp_t at_msg_ext_wifi_scan_by_option;

/**
 * @brief Function Pointer Type for dispatch connect reason
 *
 */
extern at_msg_ext_wifi_dispatch_connect_reason_fp_t at_msg_ext_wifi_dispatch_connect_reason;
#endif //__AT_CMD_MSG_EXT_H__
