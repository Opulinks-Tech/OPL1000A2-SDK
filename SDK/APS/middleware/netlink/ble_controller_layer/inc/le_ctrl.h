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

#ifndef __LE_CTRL_H__
#define __LE_CTRL_H__

#include "opulinks.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "le_ctrl_para.h"
#include "le_ctrl_err.h"
#include "le_ctrl_dbg.h"
#include "le_ctrl_def.h"


/******************** BLE Data Declaration ********************/

extern le_ctrl_data_t le_ctrl_data;

/******************** BLE Controller Function Interface ********************/

typedef void (*t_le_ctrl_init_fp)(void);
typedef void (*t_le_ctrl_hci_tx_handler_fp)(uint8_t *data);
typedef void (*t_le_ctrl_hci_proc_tx_cmd_fp)(hci_command_packet_01 *cmd_packet);
typedef void (*t_le_ctrl_hci_proc_tx_data_fp)(hci_acl_data_packet_02 *data_packet);
typedef void (*t_le_ctrl_hci_rx_handler_fp)(uint8_t *data);
typedef void (*t_le_ctrl_hci_proc_rx_event_fp)(hci_event_packet_04 *event_packet);
typedef void (*t_le_ctrl_hci_proc_rx_data_fp)(hci_acl_data_packet_02 *data_packet);
typedef void (*t_le_ctrl_hci_send_pkt_to_host_fp)(uint8_t indicator, uint8_t *data);
typedef void (*t_le_ctrl_hci_uart_input_fp)(uint32_t data);
typedef void (*t_le_ctrl_data_hci_buf_clear_fp)(uint16_t handle);
typedef int  (*t_le_ctrl_data_hci_buf_is_enough_fp)(void);
typedef void (*t_le_ctrl_data_hci_buf_push_fp)(hci_acl_data_packet_02 *data_pkt);
typedef hci_data_buf_t *(*t_le_ctrl_data_hci_buf_get_first_in_fp)(uint16_t handle, uint8_t type);
typedef void (*t_le_ctrl_data_hci_buf_fragment_fp)(le_ctrl_conn_t *conn, hci_data_buf_t *buf);
typedef void (*t_le_ctrl_data_hci_buf_fragment_routine_fp)(uint16_t handle);
typedef uint8_t *(*t_le_ctrl_data_ll_ctrl_pdu_put_fp)(uint16_t handle, uint8_t *data, uint8_t len);
typedef le_ctrl_conn_t *(*t_le_ctrl_data_add_conn_fp)(void);
typedef le_ctrl_conn_t *(*t_le_ctrl_data_find_conn_fp)(uint16_t handle);
typedef void (*t_le_ctrl_pkt_ctr_init_fp)(packet_counter_t *ctr, uint8_t role);
typedef void (*t_le_ctrl_pkt_ctr_increment_fp)(packet_counter_t *ctr);
typedef void (*t_le_ctrl_reverse_fp)(uint8_t *data, int length);
typedef void (*t_le_ctrl_reverse_and_copy_fp)(uint8_t *data, int length, uint8_t *output);
typedef void (*t_le_ctrl_gen_rand_fp)(uint8_t *random, uint8_t length);
typedef void (*t_le_ctrl_compute_sk_fp)(uint8_t *long_term_key, uint8_t *sessionkey_diversifier, uint8_t *session_key);
typedef void (*t_le_ctrl_enc_data_fp)(le_ctrl_conn_t *conn, ll_data_entry_t *entry);
typedef int  (*t_le_ctrl_dec_data_fp)(le_ctrl_conn_t *conn_data, hci_acl_data_packet_02 *data_packet);
typedef void (*t_send_vendor_specific_start_encryption_command_fp)(le_ctrl_conn_t *conn_data, uint8_t status);
typedef void (*t_send_vendor_specific_notify_control_pdu_command_fp)(le_ctrl_conn_t *conn_data, uint8_t payload_length, uint8_t *payload);
typedef void (*t_send_vendor_specific_decrypt_fail_command_fp)(le_ctrl_conn_t *conn_data);
typedef void (*t_send_vendor_specific_set_le_cfg_command_fp)(le_cfg_t *cfg);
typedef void (*t_send_command_complete_event_fp)(uint16_t command_opcode, void *paramsters, uint8_t parameters_length);
typedef void (*t_send_command_complete_event_with_only_status_fp)(uint16_t command_opcode, uint8_t status);
typedef void (*t_send_command_status_event_fp)(uint16_t opcode, uint8_t status, uint8_t num);
typedef void (*t_send_hardware_error_event_fp)(void);
typedef void (*t_send_number_of_completed_packets_event_fp)(uint16_t handle, uint16_t num);
typedef void (*t_send_data_buffer_overflow_event_fp)(void);
typedef void (*t_send_le_long_term_key_request_event_fp)(le_ctrl_conn_t *conn_data, uint8_t *rand, uint8_t *ediv);
typedef void (*t_le_encrypt_command_handler_fp)(hci_command_packet_01 *cmd_packet);
typedef void (*t_le_rand_command_handler_fp)(hci_command_packet_01 *cmd_packet);
typedef void (*t_le_start_encryption_command_handler_fp)(hci_command_packet_01 *cmd_packet);
typedef void (*t_le_long_term_key_request_reply_command_handler_fp)(hci_command_packet_01 *cmd_packet);
typedef void (*t_le_long_term_key_request_negative_reply_command_handler_fp)(hci_command_packet_01 *cmd_packet);
typedef void (*t_le_read_local_p_256_public_key_command_handler_fp)(hci_command_packet_01 *cmd_packet);
typedef void (*t_le_generate_dhkey_command_handler_fp)(hci_command_packet_01 *cmd_packet);
typedef void (*t_disconnection_complete_event_handler_fp)(hci_event_packet_04 *event_packet);
typedef void (*t_encryption_change_event_handler_fp)(hci_event_packet_04 *event_packet);
typedef void (*t_encryption_key_refresh_complete_event_handler_fp)(hci_event_packet_04 *event_packet);
typedef void (*t_le_connection_complete_event_handler_fp)(hci_event_packet_04 *event_packet);
typedef void (*t_le_data_length_change_event_handler_fp)(hci_event_packet_04 *event_packet);
typedef void (*t_send_le_read_local_p_256_public_key_complete_event_fp)(uint8_t status, uint8_t *key);
typedef void (*t_send_le_dhkey_generation_complete_event_fp)(uint8_t status, uint8_t *key);
typedef void (*t_le_enhanced_connection_complete_event_handler_fp)(hci_event_packet_04 *event_packet);
typedef void (*t_le_phy_update_complete_event_handler_fp)(hci_event_packet_04 *event_packet);
typedef void (*t_le_ctrl_update_used_data_length_fp)(le_ctrl_conn_t * conn_data);
typedef void (*t_vendor_specific_exit_hci_mode_command_handler_fp)(hci_command_packet_01 *cmd_packet);
typedef void (*t_vendor_specific_start_encryption_event_handler_fp)(hci_event_packet_04 *event_packet);
typedef void (*t_vendor_specific_send_control_pdu_event_handler_fp)(hci_event_packet_04 *event_packet);
typedef void (*t_vendor_specific_number_of_completed_bytes_event_handler_fp)(hci_event_packet_04 *event_packet);
typedef void (*t_vendor_specific_no_phy_change_event_handler_fp)(hci_event_packet_04 *event_packet);
typedef void (*t_vendor_specific_throughput_test_command_fp)(hci_command_packet_01 *cmd_packet);
typedef void (*t_le_read_cfg_from_flash_fp)(le_cfg_t *cfg);
typedef void (*t_le_write_cfg_to_flash_fp)(le_cfg_t *cfg);
typedef void (*t_le_write_bd_addr_to_flash_fp)(uint8_t *addr);
typedef void (*t_le_ctrl_throughput_test_fp)(uint8_t tx_octets, uint8_t time);

extern t_le_ctrl_init_fp le_ctrl_init;
extern t_le_ctrl_hci_tx_handler_fp le_ctrl_hci_tx_handler;
extern t_le_ctrl_hci_proc_tx_cmd_fp le_ctrl_hci_proc_tx_cmd;
extern t_le_ctrl_hci_proc_tx_data_fp le_ctrl_hci_proc_tx_data;
extern t_le_ctrl_hci_rx_handler_fp le_ctrl_hci_rx_handler;
extern t_le_ctrl_hci_proc_rx_event_fp le_ctrl_hci_proc_rx_event;
extern t_le_ctrl_hci_proc_rx_data_fp le_ctrl_hci_proc_rx_data;
extern t_le_ctrl_hci_send_pkt_to_host_fp le_ctrl_hci_send_pkt_to_host;
extern t_le_ctrl_hci_uart_input_fp le_ctrl_hci_uart_input;
extern t_le_ctrl_data_hci_buf_clear_fp le_ctrl_data_hci_buf_clear;
extern t_le_ctrl_data_hci_buf_is_enough_fp le_ctrl_data_hci_buf_is_enough;
extern t_le_ctrl_data_hci_buf_push_fp le_ctrl_data_hci_buf_push;
extern t_le_ctrl_data_hci_buf_get_first_in_fp le_ctrl_data_hci_buf_get_first_in;
extern t_le_ctrl_data_hci_buf_fragment_fp le_ctrl_data_hci_buf_fragment;
extern t_le_ctrl_data_hci_buf_fragment_routine_fp le_ctrl_data_hci_buf_fragment_routine;
extern t_le_ctrl_data_ll_ctrl_pdu_put_fp le_ctrl_data_ll_ctrl_pdu_put;
extern t_le_ctrl_data_add_conn_fp le_ctrl_data_add_conn;
extern t_le_ctrl_data_find_conn_fp le_ctrl_data_find_conn;
extern t_le_ctrl_pkt_ctr_init_fp le_ctrl_pkt_ctr_init;
extern t_le_ctrl_pkt_ctr_increment_fp le_ctrl_pkt_ctr_increment;
extern t_le_ctrl_reverse_fp le_ctrl_reverse;
extern t_le_ctrl_reverse_and_copy_fp le_ctrl_reverse_and_copy;
extern t_le_ctrl_gen_rand_fp le_ctrl_gen_rand;
extern t_le_ctrl_compute_sk_fp le_ctrl_compute_sk;
extern t_le_ctrl_enc_data_fp le_ctrl_enc_data;
extern t_le_ctrl_dec_data_fp le_ctrl_dec_data;
extern t_send_vendor_specific_start_encryption_command_fp send_vendor_specific_start_encryption_command;
extern t_send_vendor_specific_notify_control_pdu_command_fp send_vendor_specific_notify_control_pdu_command;
extern t_send_vendor_specific_decrypt_fail_command_fp send_vendor_specific_decrypt_fail_command;
extern t_send_vendor_specific_set_le_cfg_command_fp send_vendor_specific_set_le_cfg_command;
extern t_send_command_complete_event_fp send_command_complete_event;
extern t_send_command_complete_event_with_only_status_fp send_command_complete_event_with_only_status;
extern t_send_command_status_event_fp send_command_status_event;
extern t_send_hardware_error_event_fp send_hardware_error_event;
extern t_send_number_of_completed_packets_event_fp send_number_of_completed_packets_event;
extern t_send_data_buffer_overflow_event_fp send_data_buffer_overflow_event;
extern t_send_le_long_term_key_request_event_fp send_le_long_term_key_request_event;
extern t_le_encrypt_command_handler_fp le_encrypt_command_handler;
extern t_le_rand_command_handler_fp le_rand_command_handler;
extern t_le_start_encryption_command_handler_fp le_start_encryption_command_handler;
extern t_le_long_term_key_request_reply_command_handler_fp le_long_term_key_request_reply_command_handler;
extern t_le_long_term_key_request_negative_reply_command_handler_fp le_long_term_key_request_negative_reply_command_handler;
extern t_le_read_local_p_256_public_key_command_handler_fp le_read_local_p_256_public_key_command_handler;
extern t_le_generate_dhkey_command_handler_fp le_generate_dhkey_command_handler;
extern t_disconnection_complete_event_handler_fp disconnection_complete_event_handler;
extern t_encryption_change_event_handler_fp encryption_change_event_handler;
extern t_encryption_key_refresh_complete_event_handler_fp encryption_key_refresh_complete_event_handler;
extern t_send_le_read_local_p_256_public_key_complete_event_fp send_le_read_local_p_256_public_key_complete_event;
extern t_send_le_dhkey_generation_complete_event_fp send_le_dhkey_generation_complete_event;
extern t_le_connection_complete_event_handler_fp le_connection_complete_event_handler;
extern t_le_data_length_change_event_handler_fp le_data_length_change_event_handler;
extern t_le_enhanced_connection_complete_event_handler_fp le_enhanced_connection_complete_event_handler;
extern t_le_phy_update_complete_event_handler_fp le_phy_update_complete_event_handler;
extern t_le_ctrl_update_used_data_length_fp le_ctrl_update_used_data_length;
extern t_vendor_specific_exit_hci_mode_command_handler_fp vendor_specific_exit_hci_mode_command_handler;
extern t_vendor_specific_start_encryption_event_handler_fp vendor_specific_start_encryption_event_handler;
extern t_vendor_specific_send_control_pdu_event_handler_fp vendor_specific_send_control_pdu_event_handler;
extern t_vendor_specific_number_of_completed_bytes_event_handler_fp vendor_specific_number_of_completed_bytes_event_handler;
extern t_vendor_specific_no_phy_change_event_handler_fp vendor_specific_no_phy_change_event_handler;
extern t_vendor_specific_throughput_test_command_fp vendor_specific_throughput_test_command_handler;
extern t_le_read_cfg_from_flash_fp le_read_cfg_from_flash;
extern t_le_write_cfg_to_flash_fp le_write_cfg_to_flash;
extern t_le_write_bd_addr_to_flash_fp le_write_bd_addr_to_flash;
extern t_le_ctrl_throughput_test_fp le_ctrl_throughput_test;

/******************** BLE Controller Function Implement ********************/

void le_ctrl_pre_init(void);

/********** HCI Interface **********/

void le_ctrl_init_impl(void);

void le_ctrl_hci_tx_handler_impl(uint8_t *data);
void le_ctrl_hci_proc_tx_cmd_impl(hci_command_packet_01 *cmd_packet);
void le_ctrl_hci_proc_tx_data_impl(hci_acl_data_packet_02 *data_packet);

void le_ctrl_hci_rx_handler_impl(uint8_t *data);
void le_ctrl_hci_proc_rx_event_impl(hci_event_packet_04 *event_packet);
void le_ctrl_hci_proc_rx_data_impl(hci_acl_data_packet_02 *data_packet);

void le_ctrl_hci_send_pkt_to_host_impl(uint8_t indicator, uint8_t *data);

/********** Uart Transport Layer **********/

void le_ctrl_hci_uart_input_impl(uint32_t data);

/********** Data Manipulation **********/

// hci buffer management
void le_ctrl_data_hci_buf_clear_impl(uint16_t handle);
int  le_ctrl_data_hci_buf_is_enough_impl(void);
void le_ctrl_data_hci_buf_push_impl(hci_acl_data_packet_02 *data_pkt);
hci_data_buf_t *le_ctrl_data_hci_buf_get_first_in_impl(uint16_t handle, uint8_t type);
void le_ctrl_data_hci_buf_fragment_impl(le_ctrl_conn_t *conn, hci_data_buf_t *buf);
void le_ctrl_data_hci_buf_fragment_routine_impl(uint16_t handle);
uint8_t *le_ctrl_data_ll_ctrl_pdu_put_impl(uint16_t handle, uint8_t *data, uint8_t len);

// connection data operation
le_ctrl_conn_t *le_ctrl_data_add_conn_impl(void);
le_ctrl_conn_t *le_ctrl_data_find_conn_impl(uint16_t handle);

// packet counter operation
void le_ctrl_pkt_ctr_init_impl(packet_counter_t *ctr, uint8_t role);
void le_ctrl_pkt_ctr_increment_impl(packet_counter_t *ctr);

/********** Encryption Related Operation **********/

void le_ctrl_reverse_impl(uint8_t *data, int length);
void le_ctrl_reverse_and_copy_impl(uint8_t *data, int length, uint8_t *output);
void le_ctrl_gen_rand_impl(uint8_t *random, uint8_t length);
void le_ctrl_compute_sk_impl(uint8_t *long_term_key, uint8_t *sessionkey_diversifier, uint8_t *session_key);
void le_ctrl_enc_data_impl(le_ctrl_conn_t *conn, ll_data_entry_t *entry);
int  le_ctrl_dec_data_impl(le_ctrl_conn_t *conn_data, hci_acl_data_packet_02 *data_packet);
void le_ctrl_util_encrypt(uint8_t *key, uint8_t *pData, uint8_t *eData);
uint8_t le_ctrl_util_gen_key_pair(uint8_t *pubKey, uint8_t *privKey);
uint8_t le_ctrl_util_gen_dhkey(uint8_t *pub_key_x, uint8_t *pub_key_y, uint8_t *priv_key, uint8_t *dhkey);

/********** HCI Handler **********/

//	vendor command sender
void send_vendor_specific_start_encryption_command_impl(le_ctrl_conn_t *conn_data, uint8_t status);
void send_vendor_specific_notify_control_pdu_command_impl(le_ctrl_conn_t *conn_data, uint8_t payload_length, uint8_t *payload);
void send_vendor_specific_decrypt_fail_command_impl(le_ctrl_conn_t *conn_data);
void send_vendor_specific_set_le_cfg_command_impl(le_cfg_t *cfg);

//	standard event sender
void send_command_complete_event_impl(uint16_t command_opcode, void *paramsters, uint8_t parameters_length);
void send_command_complete_event_with_only_status_impl(uint16_t command_opcode, uint8_t status);
void send_command_status_event_impl(uint16_t opcode, uint8_t status, uint8_t num);
void send_hardware_error_event_impl(void);
void send_number_of_completed_packets_event_impl(uint16_t handle, uint16_t num);
void send_data_buffer_overflow_event_impl(void);
void send_le_long_term_key_request_event_impl(le_ctrl_conn_t *conn_data, uint8_t *rand, uint8_t *ediv);

//	standard command handler
void le_encrypt_command_handler_impl(hci_command_packet_01 *cmd_packet);
void le_rand_command_handler_impl(hci_command_packet_01 *cmd_packet);
void le_start_encryption_command_handler_impl(hci_command_packet_01 *cmd_packet);
void le_long_term_key_request_reply_command_handler_impl(hci_command_packet_01 *cmd_packet);
void le_long_term_key_request_negative_reply_command_handler_impl(hci_command_packet_01 *cmd_packet);
void le_read_local_p_256_public_key_command_handler_impl(hci_command_packet_01 *md_packet);
void le_generate_dhkey_command_handler_impl(hci_command_packet_01 *cmd_packet);

//	standard event handler
void disconnection_complete_event_handler_impl(hci_event_packet_04 *event_packet);
void encryption_change_event_handler_impl(hci_event_packet_04 *event_packet);
void encryption_key_refresh_complete_event_handler_impl(hci_event_packet_04 *event_packet);
void le_connection_complete_event_handler_impl(hci_event_packet_04 *event_packet);
void le_data_length_change_event_handler_impl(hci_event_packet_04 *event_packet);
void send_le_read_local_p_256_public_key_complete_event_impl(uint8_t status, uint8_t *key);
void send_le_dhkey_generation_complete_event_impl(uint8_t status, uint8_t *key);
void le_enhanced_connection_complete_event_handler_impl(hci_event_packet_04 *event_packet);
void le_phy_update_complete_event_handler_impl(hci_event_packet_04 *event_packet);
void le_ctrl_update_used_data_length_impl(le_ctrl_conn_t * conn_data);

//	vendor command handler
void vendor_specific_exit_hci_mode_command_handler_impl(hci_command_packet_01 *cmd_packet);

//	vendor event handler
void vendor_specific_start_encryption_event_handler_impl(hci_event_packet_04 *event_packet);
void vendor_specific_send_control_pdu_event_handler_impl(hci_event_packet_04 *event_packet);
void vendor_specific_number_of_completed_bytes_event_handler_impl(hci_event_packet_04 *event_packet);
void vendor_specific_no_phy_change_event_handler_impl(hci_event_packet_04 *event_packet);
void vendor_specific_throughput_test_command_impl(hci_command_packet_01 *cmd_packet);

// MwFim related 
void le_read_cfg_from_flash_impl(le_cfg_t *cfg);
void le_write_cfg_to_flash_impl(le_cfg_t *cfg);
void le_write_bd_addr_to_flash_impl(uint8_t *addr);
void ParseLeCfgCommand_impl(char *pbuf);

// le_ctrl_cmd
void ParseLeTestCommand_impl(char *pbuf);
void le_ctrl_throughput_test_impl(uint8_t tx_octets, uint8_t time);

#endif
