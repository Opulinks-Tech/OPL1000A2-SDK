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

#ifndef __LE_CTRL_DEF_H__
#define __LE_CTRL_DEF_H__

#include <stdint.h>

#include "le_ctrl_para.h"

/******************** MISC ********************/

#define LE_CTRL_ALL_HANDLE_FRAGMENT									0xFFFF
#define LE_CTRL_DATA_ENTRY_MAX										10
#define LE_CTRL_FIND_BUF_FRAGMENTED									0x01
#define LE_CTRL_FIND_BUF											0x02

#define LE_PHY_1M													0x01
#define LE_PHY_2M													0x02

/******************** HCI Transport Layer Const ********************/

#define HCI_COMMAND_PACKET_INDICATOR								1
#define HCI_ACL_DATA_PACKET_INDICATOR								2
#define HCI_EVENT_PACKET_INDICATOR									4

#define HCI_COMMAND_PACKET_HEADER_LEN								3
#define HCI_ACL_DATA_PACKET_HEADER_LEN								4
#define HCI_EVENT_PACKET_HEADER_LEN									2

#define UART_TRANSPORT_LAYER_PACKET_INDICATER_LEN					1
#define COMMAND_HEADER_LEN											(UART_TRANSPORT_LAYER_PACKET_INDICATER_LEN + HCI_COMMAND_PACKET_HEADER_LEN)
#define ACL_HEADER_LEN												(UART_TRANSPORT_LAYER_PACKET_INDICATER_LEN + HCI_ACL_DATA_PACKET_HEADER_LEN)
#define EVENT_HEADER_LEN											(UART_TRANSPORT_LAYER_PACKET_INDICATER_LEN + HCI_EVENT_PACKET_HEADER_LEN)

/******************** Command OpCode ********************/
#define CommandOpcode_Reset_Command                                 (0x03 << 10 | 0x0003)
#define CommandOGF_LE_Controller_Commands							0x08
#define CommandOpcode_LE_Encrypt_Command                            (CommandOGF_LE_Controller_Commands << 10 | 0x0017)
#define CommandOpcode_LE_Rand_Command                               (CommandOGF_LE_Controller_Commands << 10 | 0x0018)
#define CommandOpcode_LE_Start_Encryption_Command					(CommandOGF_LE_Controller_Commands << 10 | 0x0019)
#define CommandOpcode_LE_Long_Term_Key_Request_Reply_Command		(CommandOGF_LE_Controller_Commands << 10 | 0x001A)
#define CommandOpcode_LE_Long_Term_Key_Request_Negative_Reply		(CommandOGF_LE_Controller_Commands << 10 | 0x001B)
#define CommandOpcode_LE_Read_Local_P_256_Public_Key_Command        (CommandOGF_LE_Controller_Commands << 10 | 0x0025)
#define CommandOpcode_LE_Generate_DHKey_Command                     (CommandOGF_LE_Controller_Commands << 10 | 0x0026)

#define CommandOGF_Vendor_Commands									0x3F
#define CommandOpcode_Vendor_Specific_Exit_HCI_Mode_Command			(CommandOGF_Vendor_Commands << 10 | 0x01)
#define CommandOpcode_Vendor_Specific_Start_Encryption_Command		(CommandOGF_Vendor_Commands << 10 | 0x10)
#define CommandOpcode_Vendor_Specific_Notify_Control_PDU_Command	(CommandOGF_Vendor_Commands << 10 | 0x11)
#define CommandOpcode_Vendor_Specific_Decrypt_Fail_Command			(CommandOGF_Vendor_Commands << 10 | 0x12)
#define CommandOpcode_Vendor_Specific_Generate_Rand_Command			(CommandOGF_Vendor_Commands << 10 | 0x13)
#define CommandOpcode_Vendor_Specific_Set_LE_Cfg_Command            (CommandOGF_Vendor_Commands << 10 | 0x16)
#define CommandOpcode_Vendor_Specific_Throughput_Test_Command       (CommandOGF_Vendor_Commands << 10 | 0x17)

/******************** Event Code ********************/

#define EventCode_Disconnection_Complete_Event						0x05
#define EventCode_Command_Complete_Event							0x0E
#define EventCode_Encryption_Change_Event							0x08
#define EventCode_Command_Status_Event								0x0F
#define EventCode_Hardware_Error_Event								0x10
#define EventCode_Number_Of_Completed_Packets_Event					0x13
#define EventCode_Data_Buffer_Overflow_Event						0x1A
#define EventCode_Encryption_Key_Refresh_Complete_Event				0x30

#define EventCode_LE_Meta_Event										0x3E
#define SubEventCode_LE_Connection_Complete_Event					0x01
#define SubEventCode_LE_Long_Term_Key_Request_Event					0x05
#define SubEventCode_LE_Data_Length_Change_Event					0x07
#define SubEventCode_LE_Read_Local_P_256_Public_Key_Complete_Event  0x08
#define SubEventCode_LE_DHKey_Generation_Complete_Event             0x09 
#define SubEventCode_LE_Enhanced_Connection_Complete_Event 			0x0A
#define SubEventCode_LE_PHY_Update_Complete_Event 					0x0C

#define EventCode_Vendor_Event										0xFF
#define SubEventCode_Vendor_Specific_Start_Encryption_Event			0x10
#define SubEventCode_Vendor_Specific_Send_Control_PDU_Event			0x11
#define SubEventCode_Vendor_Specific_Number_Of_Completed_Bytes		0x12
#define SubEventCode_Vendor_Specific_No_PHY_Change_Event			0x13

/******************** Encryption Related Const ********************/

#define	PB_FLAG_START												0x00
#define	PB_FLAG_CONTINUE											0x01
#define LL_ID_START													0x02
#define LL_ID_CONTINUE												0x01
#define LL_ID_CONTROL_PDU											0x03

#define OPCODE_LL_START_ENC_RSP										0x06

#define ENC_NONCE_DIRECTION_BIT_MASTER								0x01
#define ENC_NONCE_DIRECTION_BIT_SLAVE								0x00
#define ENC_LTK_LEN													16
#define ENC_LTK_BIT_LEN												128
#define ENC_RAND_LEN												8
#define ENC_EDIV_LEN												2
#define ENC_SKD_LEN													16
#define ENC_SKD_HALF_LEN											8
#define ENC_SK_LEN													16
#define ENC_SK_BIT_LEN												128
#define ENC_IV_LEN													8
#define ENC_IV_HALF_LEN												4
#define ENC_PKT_CTR_LEN												5
#define ENC_MIC_LEN													4
#define ENC_NONCE_LEN												13
#define ENC_ADATA_LEN												1
#define ENC_P256_PUBLIC_KEY_LEN                                     64
#define ENC_DHKEY_LEN                                               32

/******************** Connection Data Definition ********************/

#define DEFAULT_MAX_RX_OCTETS										27
#define DEFAULT_MAX_RX_IIME											328
#define CONNECTION_ROLE_MASTER										0x00
#define CONNECTION_ROLE_SLAVE										0x01
#define LE_TEST_MAX_LENGTH                                          251

/******************** UART Transport Layer Struct ********************/

typedef struct {
	int cursor;
	int length;
	uint8_t	data[LE_CTRL_UART_HCI_BUF_LEN];
} uart_hci_buf_t;

/*************************** HCI Packet Format **********************************

	HCI Command Packet:
	Maximum length is 259 octets.
	Minimum length is   4 octets.
    LSB-----------------------------------------------------------------MSB
	| Indicator |        OpCode        |  Length   |      Partameters     |
	| (1 octet) |      (2 octets)      | (1 octet) |    (0-255 octets)    |
	-----------------------------------------------------------------------

	HCI ACL Data Packet:
	Maximum length is 260 octets.
	Minimum length is   5 octets.
	(data part maximum is 65535 in spec, here 255 limited in real buffer size)
    LSB-------------------------------------------------------------------------------------------------MSB
	| Indicator |  Connection Handle  |  PB Flag |  BC Flag |        Length        |         Data         |
	| (1 octet) |      (12 bits)      | (2 bits) | (2 bits) |       (2 octet)      |    (0-255 octets)    |
	-------------------------------------------------------------------------------------------------------

	HCI Event Packet:
	Maximum length is 258 octets.
	Minimum length is   3 octets.
    LSB------------------------------------------MSB
	| Indicator |  Length   |     Partameters      |
	| (1 octet) | (1 octet) |    (0-255 octets)    |
	------------------------------------------------

*********************************************************************************/

#pragma pack(push, 1)

typedef struct {
	uint16_t opcode;
	uint8_t length;
	uint8_t parameters[255];
} hci_command_packet_01;

typedef struct {
	uint16_t handle:12;
	uint16_t pb_flag:2;
	uint16_t bc_flag:2;
	uint16_t length;
	uint8_t data[255];
} hci_acl_data_packet_02;

typedef struct {
	uint16_t handle:12;
	uint16_t pb_flag:2;
	uint16_t bc_flag:2;
	uint16_t length;
	uint8_t data[251];
} hci_acl_tx_data_packet_02;

typedef struct {
	uint8_t event_code;
	uint8_t length;
	uint8_t parameters[255];
} hci_event_packet_04;

typedef struct {
	uint8_t count_lso[4];
	uint8_t count_mso:7;
	uint8_t direction_bit:1;
} packet_counter_t;

/******************** Standard HCI Command Parameters ********************/
// 7.8.22 LE Encrypt Command
typedef struct {
	uint8_t key[16];
	uint8_t plaintext_data[16];
} le_encrypt_command_parameters_t;

typedef struct {
	uint8_t status;
	uint8_t encrypted_data[16];
} le_encrypt_command_return_parameters_t;

// 7.8.23 LE Rand Command 
typedef struct {
    uint8_t status;
    uint8_t random_number[8];
} le_rand_command_return_parameters_t;

//	7.8.24 LE Start Encryption Command
typedef struct {
	uint16_t Connection_Handle;
	uint8_t Random_Number[8];
	uint8_t Encrypted_Diversifier[2];
	uint8_t Long_Term_Key[16];
} LE_Start_Encryption_Command_Parameters;

//	7.8.25 LE Long Term Key Request Reply Command
typedef struct {
	uint16_t Connection_Handle;
	uint8_t Long_Term_Key[16];
} LE_Long_Term_Key_Request_Reply_Command_Parameters;

typedef struct {
	uint8_t Status;
	uint16_t Connection_Handle;
} LE_Long_Term_Key_Request_Reply_Command_Return_Parameters;

//	7.8.26 LE Long Term Key Request Negative Reply Command
typedef struct {
	uint16_t Connection_Handle;
} LE_Long_Term_Key_Request_Negative_Reply_Command_Parameters;

typedef struct {
	uint8_t Status;
	uint16_t Connection_Handle;
} LE_Long_Term_Key_Request_Negative_Reply_Command_Return_Parameters;

// 7.8.37 LE Generate DHKey Command
typedef struct {
	uint8_t remote_p_256_public_key[64];
} le_generate_dhkey_command_parameters_t;

typedef struct {
    uint8_t X[32];
    uint8_t Y[32];
} le_p_256_key_t;

/******************** Standard HCI Event Parameters ********************/

//	7.7.5 Disconnection Complete Event
typedef struct {
	uint8_t Status;
	uint16_t Connection_Handle;
	uint8_t Reason;
} Disconnection_Complete_Event_Parameters;

//	7.7.8 Encryption Change Event
typedef struct {
	uint8_t Status;
	uint16_t Connection_Handle;
	uint8_t Encryption_Enabled;
} Encryption_Change_Event_Parameters;

//	7.7.14 Command Complete Event
typedef struct {
	uint8_t Num_HCI_Command_Packets;
	uint16_t Command_Opcode;
	uint8_t Return_Parameters[252];
} Command_Complete_Event_Parameters;

//	7.7.15 Command Status Event
typedef struct {
	uint8_t Status;
	uint8_t Num_HCI_Command_Packets;
	uint16_t Command_Opcode;
} Command_Status_Event_Parameters;

//	7.7.19 Number Of Completed Packets Event
typedef struct {
	uint8_t Number_of_Handles;
	uint16_t Connection_Handle;
	uint16_t HC_Num_Of_Completed_Packets;
} Number_Of_Completed_Packets_Event_Parameters;

//	7.7.26 Data Buffer Overflow Event
typedef struct {
	uint8_t Link_Type;
} Data_Buffer_Overflow_Event_Parameters;

//	7.7.39 Encryption Key Refresh Complete Event
typedef struct {
	uint8_t Status;
	uint16_t Connection_Handle;
} Encryption_Key_Refresh_Complete_Event_Parameters;

//	7.7.65.1 LE Connection Complete Event
typedef struct {
	uint8_t Subevent_Code;
	uint8_t Status;
	uint16_t Connection_Handle;
	uint8_t Role;
	uint8_t Peer_Address_Type;
	uint8_t Peer_Address[6];
	uint16_t Conn_Interval;
	uint16_t Conn_Latency;
	uint16_t Supervision_Timeout;
	uint8_t Master_Clock_Accuracy;
} LE_Connection_Complete_Event_Parameters;

//	7.7.65.5 LE Long Term Key Request Event
typedef struct {
	uint8_t Subevent_Code;
	uint16_t Connection_Handle;
	uint8_t Random_Number[8];
	uint8_t Encryption_Diversifier[2];
} LE_Long_Term_Key_Request_Event_Parameters;

//	7.7.65.7 LE Data Length Change Event
typedef struct {
	uint8_t Subevent_Code;
	uint16_t Connection_Handle;
	uint16_t MaxTxOctets;
	uint16_t MaxTxTime;
	uint16_t MaxRxOctets;
	uint16_t MaxRxTime;
} LE_Data_Length_Change_Event_Parameters;

// 7.7.65.8 LE Read Local P-256 Public Key Complete event
typedef struct {
	uint8_t Subevent_Code;
	uint8_t Status;
	uint8_t Local_P_256_Public_Key[64];
} LE_Read_Local_P_256_Public_Key_Complete_Event_Parameters;

// 7.7.65.9 LE DHKey Generation Complete event
typedef struct {
	uint8_t Subevent_Code;
	uint8_t Status;
	uint8_t DHKey[32];
} LE_DHKey_Generation_Complete_Event_Parameters;

//	7.7.65.10 LE Enhanced Connection Complete Event
typedef struct {
	uint8_t Subevent_Code;
	uint8_t Status;
	uint16_t Connection_Handle;
	uint8_t Role;
	uint8_t Peer_Address_Type;
	uint8_t Peer_Address[6];
	uint8_t Local_Resolvable_Private_Address[6];
	uint8_t Peer_Resolvable_Private_Address[6];
	uint16_t Conn_Interval;
	uint16_t Conn_Latency;
	uint16_t Supervision_Timeout;
	uint8_t Master_Clock_Accuracy;
} LE_Enhanced_Connection_Complete_Event_Parameters;

//	7.7.65.12 LE PHY Update Complete Event
typedef struct {
	uint8_t Subevent_Code;
	uint8_t Status;
	uint16_t Connection_Handle;
	uint8_t TX_PHY;
	uint8_t RX_PHY;
} LE_PHY_Update_Complete_Event_Parameters;

/******************** FIM Related Struct of LE ********************/
typedef struct
{
    uint16_t hci_revision;
    uint16_t manufacturer_name;
    uint16_t lmp_pal_subversion;
    uint8_t  hci_version;
    uint8_t  lmp_pal_version;
    uint8_t  bd_addr[6];
    uint8_t  reserved[241];
} le_cfg_t;

/******************** Vendor-Specific HCI Command Parameters ********************/

//	0x3F.10 Vendor-Specific Start Encryption Command
typedef struct {
	uint8_t Status;									// 0:success; others:fail to query LTK or hardware encrypt error
	uint16_t Connection_Handle;
	uint8_t Local_Sessionkey_Diversifier[8];		// SKDm(8) || SKDs(8) = SKD(16)
	uint8_t Local_Initial_Vector[4];				// IVm(4) || IVs(4) = IV(8)
	uint8_t Local_Random_Number[8];					// for slave use, instead of 7.7.65.5 LE Long Term Key Request Event
	uint8_t Local_Encrypted_Diversifier[2];			// for slave use, instead of 7.7.65.5 LE Long Term Key Request Event
} Vendor_Specific_Start_Encryption_Command_Parameters;

//	0x3F.11 Vendor-Specific Notify Control PDU Command
typedef struct {
	uint16_t Connection_Handle;
	uint8_t Payload_Length;
	uint8_t Payload[27];
} Vendor_Specific_Notify_Control_PDU_Command_Parameters;

//	0x3F.12 Vendor-Specific Decrypt Fail Command
typedef struct {
	uint16_t Connection_Handle;
} Vendor_Specific_Decrypt_Fail_Command_Parameters;

/******************** Vendor-Specific HCI Event Parameters ********************/

//	0x3F.10 Vendor-Specific Start Encryption Event
typedef struct {
	uint8_t Subevent_Code;
	uint16_t Connection_Handle;
	uint8_t Sessionkey_Diversifier[8];				// SKDm(8) || SKDs(8) = SKD(16)
	uint8_t Initial_Vector[4];						// IVm(4) || IVs(4) = IV(8)
	uint8_t Random_Number[8];						// for slave use, instead of 7.7.65.5 LE Long Term Key Request Event
	uint8_t Encrypted_Diversifier[2];				// for slave use, instead of 7.7.65.5 LE Long Term Key Request Event
} Vendor_Specific_Start_Encryption_Event_Parameters;

//	0x3F.11 Vendor-Specific Send Control PDU Event
typedef struct {
	uint8_t Subevent_Code;
	uint16_t Connection_Handle;
	uint8_t Text_Plain_Length;
	uint8_t Text_Plain[27];
} Vendor_Specific_Send_Control_PDU_Event_Parameters;

//	0x3F.12 Vendor-Specific Number of Completed Bytes Event
typedef struct {
	uint8_t Subevent_Code;
	uint16_t Connection_Handle;
	uint16_t Num_Of_Completed_Bytes;
} Vendor_Specific_Number_Of_Completed_Bytes_Event_Parameters;

//	0x3F.13 Vendor-Specific No PHY Change Event
typedef struct {
	uint8_t Subevent_Code;
	uint16_t Connection_Handle;
} Vendor_Specific_No_PHY_Change_Event_Parameters;

//	0x3F.16 Vendor-Specific Set LE Cfg Command
typedef struct {
    le_cfg_t cfg;
} Vendor_Specific_Set_LE_Cfg_Command_Parameters;

// 0x3F.17 Vendor-Specific Throughput Test Command
typedef struct {
    uint8_t length;
} Vendor_Specific_Throughput_Test_Command_Parameters;

#pragma pack(pop)

/******************** HCI Data Buf Struct ********************/

typedef struct
{
	uint32_t seq_no;	// number ordered by input sequence
	uint8_t left_len;	// how many data not fragmented;
	uint8_t cursor;		// indicate next location which to be fragmented
	uint8_t is_used;	// indicate is there any data not be fragmented
	hci_acl_tx_data_packet_02 *data_pkt;

} hci_data_buf_t;

typedef struct
{
	uint8_t cursor;
	uint8_t data[2][27];

} ll_ctrl_pdu_buf_t;

typedef struct
{
	uint8_t ll_id;
	uint8_t len;
	uint8_t is_enc;
	uint8_t rfu;
	uint8_t *data;
	uint32_t mic;

} ll_data_entry_t;

/******************** Connection Data Struct ********************/

typedef struct
{
	uint8_t is_used;
	uint16_t handle;
	uint8_t role;
	uint16_t max_tx_octets;
	uint8_t tx_queue_number;  // shared memory queue number

	uint32_t tx_queued_data_len;
	uint32_t tx_transmitted_data_len;

	uint8_t is_fragment_enable;
	uint8_t is_encrypted;
	packet_counter_t encrypted_tx_packet_counter;
	packet_counter_t encrypted_rx_packet_counter;

	uint8_t long_term_key[ENC_LTK_LEN];
	uint8_t random_number[ENC_RAND_LEN];
	uint8_t encrypted_diversifier[ENC_EDIV_LEN];
	uint8_t sessionkey_diversifier[ENC_SKD_LEN];
	uint8_t initial_vector[ENC_IV_LEN];
	uint8_t initial_vector_master_new[ENC_IV_HALF_LEN];
	uint8_t sessionkey[ENC_SK_LEN];

	// 2M PHY REVISION
	uint16_t max_tx_time;
	uint16_t used_tx_octects;
	uint8_t tx_phy;
	uint8_t used_tx_phy;

	uint8_t rfu[10];

} le_ctrl_conn_t;

/******************** BLE ALL Data in this Struct ********************/

typedef struct
{
	// hci actual buf-pool
	hci_acl_tx_data_packet_02 	hci_data_pool[LE_CTRL_HCI_BUF_NUM]; // NOTE: it need be located in 1k boundary to avoid DMA-burst issue
	uint32_t 					pad;								// NOTE: reserved for 1k alignment padding
	ll_ctrl_pdu_buf_t 			ll_ctrl_pdu_buf[LE_CTRL_CONN_NUM];	// NOTE: it need be located in 1k boundary to avoid DMA-burst issue

	// hci buffer management
	hci_data_buf_t	 			hci_data_buf[LE_CTRL_HCI_BUF_NUM];	// header of hci-buf-mgnt
	uint32_t 					hci_data_seq_no;					// current buffer sequence number

	// connection data
	le_ctrl_conn_t 				conn_data[LE_CTRL_CONN_NUM];

	// uart transport layer
	uint8_t 					is_uart_hci_mode;					// indicate whether in uart_hci_mode or internal_host_hci_mode
	uart_hci_buf_t 				uart_hci_buf;						// hci buffer for externl uart input

    // MISC
    uint8_t                     is_request_reset;
    
	// debug message switches
	uint8_t 					open_dbg_hci_standard;				// open all standard hci command/event log
	uint8_t 					open_dbg_ll_hci_hook;				// open all link layer internal handler log
	uint8_t 					open_dbg_ll_enc;					// open all encryption related log
} le_ctrl_data_t;

#endif
