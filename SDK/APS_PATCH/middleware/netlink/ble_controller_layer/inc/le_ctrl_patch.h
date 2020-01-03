/* *****************************************************************************
 *  Copyright 2019, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *
 *******************************************************************************
 *
 *  @file le_ctrl_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _LE_CTRL_PATCH_H_
#define _LE_CTRL_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "le_ctrl.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define PATCH_BLE_MASTER_ENABLE         1
#define CommandOpcode_Vendor_Specific_Get_Packet_Info_Statistics_Command       (CommandOGF_Vendor_Commands << 10 | 0x18)
#define CommandOpcode_Vendor_Specific_Set_AGC_Level_Command                    (CommandOGF_Vendor_Commands << 10 | 0x19)


/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
#pragma pack(push, 1)
typedef struct
{
    char *log_buf_addr;

} Vendor_Specific_Get_Packet_Info_Statisticsd_Command_Return_Parameters;
#pragma pack(pop)


/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern uint8_t le_ctrl_agc_rf_gain_level;
extern uint8_t le_ctrl_agc_digital_gain_level;


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
// le_ctrl_func_map
void le_ctrl_pre_patch_init(void);

// le_ctrl_hci_uart_tl

// le_ctrl_data

// le_ctrl_enc

// le_ctrl_hci
void le_ctrl_init_patch(void);
void le_ctrl_hci_proc_rx_event_patch(hci_event_packet_04 *event_packet);
#if PATCH_BLE_MASTER_ENABLE
void le_ctrl_hci_send_pkt_to_host_patch(uint8_t indicator, uint8_t *data);
#endif

// le_ctrl_hci_handler
void vendor_specific_get_packet_info_statistics_event_handler(hci_event_packet_04 *event_packet);

// le_ctrl_fim

// le_ctrl_cmd
int le_ctrl_packet_info_display(uint8_t enable, int interval);
int le_ctrl_set_agc_gain_level(uint8_t rf_gain_level, uint8_t digital_gain_level);


#ifdef __cplusplus
}
#endif
#endif  /* _LE_CTRL_PATCH_H_ */
