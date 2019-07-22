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

#ifndef __IPC_H__
#define __IPC_H__

#include <stdio.h>
#include <stdint.h>

#define ENHANCE_IPC

//#define IPC_MSQ
//#define IPC_SUT

#include "wifi_mac_common.h"
#include "le_ctrl.h"

#ifdef IPC_MSQ
    #include "ps.h"
#else
    #include "msg.h"
    #define IPC_DEBUG
#endif

#ifdef IPC_DEBUG
    #define IPC_LOGI(...)
    #define IPC_LOGE            printf
#else
    #define IPC_LOGI(...)
    #define IPC_LOGE(...)
#endif

#ifdef IPC_SUT
    #ifdef IPC_MSQ
        #define IPC_LOGT        printf
    #else
        #define IPC_LOGT(args...)   tracer_cli(LOG_MED_LEVEL, args)
    #endif

    #undef IPC_LOGE
    #define IPC_LOGE            IPC_LOGT
#endif

#ifdef IPC_MSQ
    #define IPC_LOGC        printf
#else
    #define IPC_LOGC(args...)   tracer_cli(LOG_MED_LEVEL, args)
#endif


#ifdef __NL1000_An__
#define IPC_SHARED_MEM_ADDR         0x80000000
#define IPC_SHARED_MEM_SIZE         0x00010000
#else   // __NL1000_TC__
#define IPC_SHARED_MEM_ADDR         0x00458000
#define IPC_SHARED_MEM_SIZE         0x00008000
#endif

#define IPC_ADDR_ALIGN(addr, byte)  ((((uint32_t)(addr) + byte - 1) & ~(uint32_t)(byte - 1)))
#define IPC_VALUE_ALIGN             IPC_ADDR_ALIGN

#define IPC_DEF_BUF_NUM             (8) // must be power of 2
#define IPC_COMM_BUF_NUM            (4) // must be power of 2
#define IPC_DATA_BUF_NUM            (4) // must be power of 2

#define IPC_CMD_EVT_TYPE_LEN        (4) // for access_user

#define IPC_SUT_DATA_SIZE           IPC_VALUE_ALIGN(800 + 1, 4)

#define IPC_CMD_DATA_SIZE           64 // for common command
#define IPC_EVT_DATA_SIZE           64 // for common event

#define IPC_WIFI_CMD_DATA_SIZE      32 //IPC_VALUE_ALIGN(sizeof(wifi_cmd_t), 4)
#define IPC_WIFI_EVT_DATA_SIZE      32 //IPC_VALUE_ALIGN(sizeof(wifi_evt_t), 4)

#ifdef IPC_MSQ
#define IPC_BLE_CMD_DATA_SIZE       IPC_VALUE_ALIGN(sizeof(hci_cmd_pkt_t), 4) //(260) // 4-bytes alignment for 258
#define IPC_BLE_EVT_DATA_SIZE       IPC_VALUE_ALIGN(sizeof(hci_event_pkt_t), 4) //(260) // 4-bytes alignment for 257
#define IPC_BLE_TX_RB_NUM           BLE_LL_CONNECTION_NUM_LIMIT
#else
#define IPC_BLE_CMD_DATA_SIZE       IPC_VALUE_ALIGN(sizeof(hci_command_packet_01), 4) //(260) // 4-bytes alignment for 258
#define IPC_BLE_EVT_DATA_SIZE       IPC_VALUE_ALIGN(sizeof(hci_event_packet_04), 4) //(260) // 4-bytes alignment for 257
#define IPC_BLE_TX_RB_NUM           LE_CTRL_CONN_NUM
#endif

// For common command
//#define IPC_CMD_QUEUE_WRITE         IPC_ADDR_ALIGN(IPC_SHARED_MEM_ADDR, 4)
//#define IPC_CMD_QUEUE_READ          IPC_ADDR_ALIGN(IPC_CMD_QUEUE_WRITE + sizeof(uint32_t), 4)
//#define IPC_CMD_QUEUE_START         IPC_ADDR_ALIGN(IPC_CMD_QUEUE_READ + sizeof(uint32_t), 4)
#define IPC_CMD_BUF_SIZE            IPC_VALUE_ALIGN(sizeof(T_IpcCmdBuf) + 1, 4)
#define IPC_CMD_BUF_NUM             2
//#define IPC_CMD_QUEUE_END           (IPC_CMD_QUEUE_START + (IPC_CMD_BUF_SIZE * IPC_CMD_BUF_NUM))

// For common event
//#define IPC_EVT_QUEUE_WRITE         IPC_ADDR_ALIGN(IPC_CMD_QUEUE_END, 4)
//#define IPC_EVT_QUEUE_READ          IPC_ADDR_ALIGN(IPC_EVT_QUEUE_WRITE + sizeof(uint32_t), 4)
//#define IPC_EVT_QUEUE_START         IPC_ADDR_ALIGN(IPC_EVT_QUEUE_READ + sizeof(uint32_t), 4)
#define IPC_EVT_BUF_SIZE            IPC_VALUE_ALIGN(sizeof(T_IpcEvtBuf) + 1, 4)
#define IPC_EVT_BUF_NUM             2
//#define IPC_EVT_QUEUE_END           (IPC_EVT_QUEUE_START + (IPC_EVT_BUF_SIZE * IPC_EVT_BUF_NUM))

// For BLE_HCI command
//#define IPC_BLE_CMD_QUEUE_WRITE     IPC_ADDR_ALIGN(IPC_EVT_QUEUE_END, 4)
//#define IPC_BLE_CMD_QUEUE_READ      IPC_ADDR_ALIGN(IPC_BLE_CMD_QUEUE_WRITE + sizeof(uint32_t), 4)
//#define IPC_BLE_CMD_QUEUE_START     IPC_ADDR_ALIGN(IPC_BLE_CMD_QUEUE_READ + sizeof(uint32_t), 4)
#define IPC_BLE_CMD_BUF_SIZE        IPC_VALUE_ALIGN(sizeof(T_IpcBleCmdBuf) + 1, 4)
#define IPC_BLE_CMD_BUF_NUM         2
//#define IPC_BLE_CMD_QUEUE_END       (IPC_BLE_CMD_QUEUE_START + (IPC_BLE_CMD_BUF_SIZE * IPC_BLE_CMD_BUF_NUM))

// BLE_HCI event
//#define IPC_BLE_EVT_QUEUE_WRITE     IPC_ADDR_ALIGN(IPC_BLE_CMD_QUEUE_END, 4)
//#define IPC_BLE_EVT_QUEUE_READ      IPC_ADDR_ALIGN(IPC_BLE_EVT_QUEUE_WRITE + sizeof(uint32_t), 4)
//#define IPC_BLE_EVT_QUEUE_START     IPC_ADDR_ALIGN(IPC_BLE_EVT_QUEUE_READ + sizeof(uint32_t), 4)
#define IPC_BLE_EVT_BUF_SIZE        IPC_VALUE_ALIGN(sizeof(T_IpcBleEvtBuf) + 1, 4)
#define IPC_BLE_EVT_BUF_NUM         IPC_COMM_BUF_NUM
//#define IPC_BLE_EVT_QUEUE_END       (IPC_BLE_EVT_QUEUE_START + (IPC_BLE_EVT_BUF_SIZE * IPC_BLE_EVT_BUF_NUM))

// For BLE TX data
//#define IPC_BLE_TX_QUEUE_WRITE      IPC_ADDR_ALIGN(IPC_BLE_EVT_QUEUE_END, 4)
//#define IPC_BLE_TX_QUEUE_READ       IPC_ADDR_ALIGN(IPC_BLE_TX_QUEUE_WRITE + sizeof(uint32_t), 4)
//#define IPC_BLE_TX_QUEUE_START      IPC_ADDR_ALIGN(IPC_BLE_TX_QUEUE_READ + sizeof(uint32_t), 4)
#define IPC_BLE_TX_BUF_SIZE         IPC_VALUE_ALIGN(sizeof(ll_data_entry_t) + 1, 4)
#define IPC_BLE_TX_BUF_NUM          16 //IPC_DATA_BUF_NUM
//#define IPC_BLE_TX_QUEUE_END        (IPC_BLE_TX_QUEUE_WRITE + ((8 + (IPC_BLE_TX_BUF_SIZE * IPC_BLE_TX_BUF_NUM)) * IPC_BLE_TX_RB_NUM))

// For BLE RX data
//#define IPC_BLE_RX_QUEUE_WRITE      IPC_ADDR_ALIGN(IPC_BLE_TX_QUEUE_END, 4)
//#define IPC_BLE_RX_QUEUE_READ       IPC_ADDR_ALIGN(IPC_BLE_RX_QUEUE_WRITE + sizeof(uint32_t), 4)
//#define IPC_BLE_RX_QUEUE_START      IPC_ADDR_ALIGN(IPC_BLE_RX_QUEUE_READ + sizeof(uint32_t), 4)

#ifdef IPC_MSQ
#define IPC_BLE_RX_BUF_SIZE         IPC_VALUE_ALIGN(sizeof(hci_data_pkt_t) + 1, 4) // (260) // 4-bytes alignment for 259
#else
#define IPC_BLE_RX_BUF_SIZE         IPC_VALUE_ALIGN(sizeof(hci_acl_data_packet_02) + 1, 4) // (260) // 4-bytes alignment for 259
#endif

#define IPC_BLE_RX_BUF_NUM          IPC_DATA_BUF_NUM
//#define IPC_BLE_RX_QUEUE_END        (IPC_BLE_RX_QUEUE_START + (IPC_BLE_RX_BUF_SIZE * IPC_BLE_RX_BUF_NUM))

// For WiFi_AT command
//#define IPC_WIFI_CMD_QUEUE_WRITE    IPC_ADDR_ALIGN(IPC_BLE_RX_QUEUE_END, 4)
//#define IPC_WIFI_CMD_QUEUE_READ     IPC_ADDR_ALIGN(IPC_WIFI_CMD_QUEUE_WRITE + sizeof(uint32_t), 4)
//#define IPC_WIFI_CMD_QUEUE_START    IPC_ADDR_ALIGN(IPC_WIFI_CMD_QUEUE_READ + sizeof(uint32_t), 4)
#define IPC_WIFI_CMD_BUF_SIZE       IPC_VALUE_ALIGN(sizeof(T_IpcWifiCmdBuf) + 1, 4)
#define IPC_WIFI_CMD_BUF_NUM        IPC_COMM_BUF_NUM
//#define IPC_WIFI_CMD_QUEUE_END      (IPC_WIFI_CMD_QUEUE_START + (IPC_WIFI_CMD_BUF_SIZE * IPC_WIFI_CMD_BUF_NUM))

// For WiFi_AT event
//#define IPC_WIFI_EVT_QUEUE_WRITE    IPC_ADDR_ALIGN(IPC_WIFI_CMD_QUEUE_END, 4)
//#define IPC_WIFI_EVT_QUEUE_READ     IPC_ADDR_ALIGN(IPC_WIFI_EVT_QUEUE_WRITE + sizeof(uint32_t), 4)
//#define IPC_WIFI_EVT_QUEUE_START    IPC_ADDR_ALIGN(IPC_WIFI_EVT_QUEUE_READ + sizeof(uint32_t), 4)
#define IPC_WIFI_EVT_BUF_SIZE       IPC_VALUE_ALIGN(sizeof(T_IpcWifiEvtBuf) + 1, 4)
#define IPC_WIFI_EVT_BUF_NUM        IPC_COMM_BUF_NUM
//#define IPC_WIFI_EVT_QUEUE_END      (IPC_WIFI_EVT_QUEUE_START + (IPC_WIFI_EVT_BUF_SIZE * IPC_WIFI_EVT_BUF_NUM))

// For WiFi MSQ TX
//#define IPC_WIFI_MSQ_TX_QUEUE_WRITE     IPC_ADDR_ALIGN(IPC_WIFI_EVT_QUEUE_END, 4)
//#define IPC_WIFI_MSQ_TX_QUEUE_READ      IPC_ADDR_ALIGN(IPC_WIFI_MSQ_TX_QUEUE_WRITE + sizeof(uint32_t), 4)
//#define IPC_WIFI_MSQ_TX_QUEUE_START     IPC_ADDR_ALIGN(IPC_WIFI_MSQ_TX_QUEUE_READ + sizeof(uint32_t), 16)

#ifdef IPC_SUT
#define IPC_WIFI_MSQ_TX_BUF_SIZE        IPC_SUT_DATA_SIZE
#else
#define IPC_WIFI_MSQ_TX_BUF_SIZE        IPC_VALUE_ALIGN(sizeof(wifi_mac_msq_tx_entry_t) + 1, 16)
#endif

#define IPC_WIFI_MSQ_TX_BUF_NUM         IPC_DATA_BUF_NUM
//#define IPC_WIFI_MSQ_TX_QUEUE_END       (IPC_WIFI_MSQ_TX_QUEUE_START + (IPC_WIFI_MSQ_TX_BUF_SIZE * IPC_WIFI_MSQ_TX_BUF_NUM))

// For WiFi MSQ RX
//#define IPC_WIFI_MSQ_RX_QUEUE_WRITE     IPC_ADDR_ALIGN(IPC_WIFI_MSQ_TX_QUEUE_END, 4)
//#define IPC_WIFI_MSQ_RX_QUEUE_READ      IPC_ADDR_ALIGN(IPC_WIFI_MSQ_RX_QUEUE_WRITE + sizeof(uint32_t), 4)
//#define IPC_WIFI_MSQ_RX_QUEUE_START     IPC_ADDR_ALIGN(IPC_WIFI_MSQ_RX_QUEUE_READ + sizeof(uint32_t), 16)

#ifdef IPC_SUT
#define IPC_WIFI_MSQ_RX_BUF_SIZE        IPC_SUT_DATA_SIZE
#else
#define IPC_WIFI_MSQ_RX_BUF_SIZE        IPC_VALUE_ALIGN(sizeof(wifi_mac_msq_rx_entry_t) + 1, 16)
#endif

#define IPC_WIFI_MSQ_RX_BUF_NUM         IPC_DATA_BUF_NUM
//#define IPC_WIFI_MSQ_RX_QUEUE_END       (IPC_WIFI_MSQ_RX_QUEUE_START + (IPC_WIFI_MSQ_RX_BUF_SIZE * IPC_WIFI_MSQ_RX_BUF_NUM))

// For WiFi APS TX
//#define IPC_WIFI_APS_TX_QUEUE_WRITE     IPC_ADDR_ALIGN(IPC_WIFI_MSQ_RX_QUEUE_END, 4)
//#define IPC_WIFI_APS_TX_QUEUE_READ      IPC_ADDR_ALIGN(IPC_WIFI_APS_TX_QUEUE_WRITE + sizeof(uint32_t), 4)
//#define IPC_WIFI_APS_TX_QUEUE_START     IPC_ADDR_ALIGN(IPC_WIFI_APS_TX_QUEUE_READ + sizeof(uint32_t), 16)

#ifdef IPC_SUT
#define IPC_WIFI_APS_TX_BUF_SIZE        IPC_SUT_DATA_SIZE
#else
#define IPC_WIFI_APS_TX_BUF_SIZE        IPC_VALUE_ALIGN(sizeof(wifi_mac_aps_tx_entry_t) + 1, 16)
#endif

#define IPC_WIFI_APS_TX_BUF_NUM         IPC_DATA_BUF_NUM
//#define IPC_WIFI_APS_TX_QUEUE_END       (IPC_WIFI_APS_TX_QUEUE_START + (IPC_WIFI_APS_TX_BUF_SIZE * IPC_WIFI_APS_TX_BUF_NUM))

// For WiFi APS RX
//#define IPC_WIFI_APS_RX_QUEUE_WRITE     IPC_ADDR_ALIGN(IPC_WIFI_APS_TX_QUEUE_END, 4)
//#define IPC_WIFI_APS_RX_QUEUE_READ      IPC_ADDR_ALIGN(IPC_WIFI_APS_RX_QUEUE_WRITE + sizeof(uint32_t), 4)
//#define IPC_WIFI_APS_RX_QUEUE_START     IPC_ADDR_ALIGN(IPC_WIFI_APS_RX_QUEUE_READ + sizeof(uint32_t), 16)

#ifdef IPC_SUT
#define IPC_WIFI_APS_RX_BUF_SIZE        IPC_SUT_DATA_SIZE
#else
#define IPC_WIFI_APS_RX_BUF_SIZE        IPC_VALUE_ALIGN(sizeof(wifi_mac_aps_rx_entry_t) + 1, 16)
#endif

#define IPC_WIFI_APS_RX_BUF_NUM         IPC_DATA_BUF_NUM
//#define IPC_WIFI_APS_RX_QUEUE_END       (IPC_WIFI_APS_RX_QUEUE_START + (IPC_WIFI_APS_RX_BUF_SIZE * IPC_WIFI_APS_RX_BUF_NUM))

#ifdef IPC_SUT
// For SUT
//#define IPC_SUT_QUEUE_WRITE             IPC_ADDR_ALIGN(IPC_WIFI_APS_RX_QUEUE_END, 4)
//#define IPC_SUT_QUEUE_READ              IPC_ADDR_ALIGN(IPC_SUT_QUEUE_WRITE + sizeof(uint32_t), 4)
//#define IPC_SUT_QUEUE_START             IPC_ADDR_ALIGN(IPC_SUT_QUEUE_READ + sizeof(uint32_t), 4)
#define IPC_SUT_BUF_SIZE                sizeof(T_IpcSutBuf) //(IPC_SUT_DATA_SIZE + sizeof(uint32_t))
#define IPC_SUT_BUF_NUM                 IPC_DATA_BUF_NUM
//#define IPC_SUT_QUEUE_END               (IPC_SUT_QUEUE_START + (IPC_SUT_BUF_SIZE * IPC_SUT_BUF_NUM))

//#define IPC_QUEUE_ADDR_END              IPC_SUT_QUEUE_END
#else
//#define IPC_QUEUE_ADDR_END              IPC_WIFI_APS_RX_QUEUE_END
#endif

// For WiFi BSS Info
//#define IPC_WIFI_BSS_INFO_START         IPC_ADDR_ALIGN(IPC_QUEUE_ADDR_END, 4)
#define IPC_WIFI_BSS_INFO_START         g_u32IpcWifiBssInfoAddr
//#define IPC_WIFI_BSS_INFO_END           (IPC_WIFI_BSS_INFO_START + sizeof(bss_info_t))

// For WiFi Shared Mem
//#define IPC_WIFI_SHARED_MEM_START       IPC_ADDR_ALIGN(IPC_WIFI_BSS_INFO_END, 4)
//#define IPC_WIFI_SHARED_MEM_START       g_u32IpcWifiShmAddr
//#define IPC_WIFI_SHARED_MEM_LEN         64
//#define IPC_WIFI_SHARED_MEM_END         (IPC_WIFI_SHARED_MEM_START + IPC_WIFI_SHARED_MEM_LEN)

// For DBG
//#define IPC_DBG_TRX_PARAM_START         IPC_ADDR_ALIGN(IPC_WIFI_SHARED_MEM_END, 16)
#define IPC_DBG_TRX_PARAM_START         g_u32IpcWifiDbgParamAddr
#define IPC_DBG_TRX_PARAM_LEN           sizeof(dbg_trx_config_param_t)
//#define IPC_DBG_TRX_PARAM_END           (IPC_DBG_TRX_PARAM_START + IPC_DBG_TRX_PARAM_LEN)

// For WiFi STA Info
//#define IPC_WIFI_STA_INFO_START         IPC_ADDR_ALIGN(IPC_DBG_TRX_PARAM_END, 4)
#define IPC_WIFI_STA_INFO_START         g_u32IpcWifiStaInfoAddr
#define IPC_WIFI_STA_INFO_LEN           sizeof(WifiSta_StaInfo_s)
//#define IPC_WIFI_STA_INFO_END           (IPC_WIFI_STA_INFO_START + IPC_WIFI_STA_INFO_LEN)

// For PS module ps_conf
//#define IPC_PS_CONF_START        		IPC_ADDR_ALIGN(IPC_WIFI_STA_INFO_END, 4)
#define IPC_PS_CONF_START        		g_u32IpcPsConfAddr
#define IPC_PS_CONF_LEN         		sizeof(t_ps_conf)
//#define IPC_PS_CONF_END          		(IPC_PS_CONF_START + IPC_PS_CONF_LEN)

// For message from M0 to M3
#define IPC_M0_MSG_SIZE                 256
//#define IPC_M0_MSG_QUEUE_WRITE          IPC_ADDR_ALIGN(IPC_PS_CONF_END, 4)
//#define IPC_M0_MSG_QUEUE_READ           IPC_ADDR_ALIGN(IPC_M0_MSG_QUEUE_WRITE + sizeof(uint32_t), 4)
//#define IPC_M0_MSG_QUEUE_START          IPC_ADDR_ALIGN(IPC_M0_MSG_QUEUE_READ + sizeof(uint32_t), 4)
#define IPC_M0_MSG_BUF_SIZE             IPC_VALUE_ALIGN(IPC_M0_MSG_SIZE + 1, 4)
#define IPC_M0_MSG_BUF_NUM              4
//#define IPC_M0_MSG_QUEUE_END            (IPC_M0_MSG_QUEUE_START + (IPC_M0_MSG_BUF_SIZE * IPC_M0_MSG_BUF_NUM))

// For message from M3 to M0
#define IPC_M3_MSG_SIZE                 256
//#define IPC_M3_MSG_QUEUE_WRITE          IPC_ADDR_ALIGN(IPC_M0_MSG_QUEUE_END, 4)
//#define IPC_M3_MSG_QUEUE_READ           IPC_ADDR_ALIGN(IPC_M3_MSG_QUEUE_WRITE + sizeof(uint32_t), 4)
//#define IPC_M3_MSG_QUEUE_START          IPC_ADDR_ALIGN(IPC_M3_MSG_QUEUE_READ + sizeof(uint32_t), 4)
#define IPC_M3_MSG_BUF_SIZE             IPC_VALUE_ALIGN(IPC_M3_MSG_SIZE + 1, 4)
#define IPC_M3_MSG_BUF_NUM              4
//#define IPC_M3_MSG_QUEUE_END            (IPC_M3_MSG_QUEUE_START + (IPC_M3_MSG_BUF_SIZE * IPC_M3_MSG_BUF_NUM))

//#define IPC_SHM_AVAIL_ADDR              IPC_M3_MSG_QUEUE_END

#ifdef IPC_SUT

    #define IPC_CMD_SEND                ipc_dummy_send
    #define IPC_CMD_COUNT_GET           ipc_dummy_count_get_1

    #define IPC_EVT_SEND                ipc_dummy_send
    #define IPC_EVT_READ                ipc_dummy_get_2
    #define IPC_EVT_READ_DONE           ipc_dummy_2
    #define IPC_EVT_COUNT_GET           ipc_dummy_count_get_1

    #define IPC_BLE_CMD_SEND            ipc_dummy_send
    #define IPC_BLE_CMD_COUNT_GET       ipc_dummy_count_get_1
    
    #define IPC_BLE_EVT_SEND            ipc_dummy_send
    #define IPC_BLE_EVT_READ            ipc_dummy_get_2
    #define IPC_BLE_EVT_READ_DONE       ipc_dummy_2
    #define IPC_BLE_EVT_COUNT_GET       ipc_dummy_count_get_1
    
    #define IPC_BLE_TX_WRITE            ipc_dummy_get_1
    #define IPC_BLE_TX_WRITE_DONE       ipc_dummy_1
    #define IPC_BLE_TX_READ             ipc_dummy_get_1
    #define IPC_BLE_TX_READ_DONE        ipc_dummy_1
    #define IPC_BLE_TX_NEXT_DATA_EXIST  ipc_dummy_exist
    #define IPC_BLE_TX_COUNT_GET        ipc_dummy_count_get_2
    
    #define IPC_BLE_RX_WRITE            ipc_dummy_get_1
    #define IPC_BLE_RX_WRITE_DONE       ipc_dummy_1
    #define IPC_BLE_RX_READ             ipc_dummy_get_1
    #define IPC_BLE_RX_READ_DONE        ipc_dummy_1
    #define IPC_BLE_RX_COUNT_GET        ipc_dummy_count_get_2

    #define IPC_WIFI_CMD_SEND           ipc_dummy_send
    #define IPC_WIFI_CMD_COUNT_GET      ipc_dummy_count_get_1

    #define IPC_WIFI_EVT_SEND           ipc_dummy_send
    #define IPC_WIFI_EVT_COUNT_GET      ipc_dummy_count_get_1

    #define IPC_WIFI_MSQ_TX_RESET       ipc_dummy_2
    #define IPC_WIFI_MSQ_TX_WRITE       ipc_dummy_get_2
    #define IPC_WIFI_MSQ_TX_WRITE_DONE  ipc_dummy_2
    #define IPC_WIFI_MSQ_TX_READ        ipc_dummy_get_2
    #define IPC_WIFI_MSQ_TX_READ_DONE   ipc_dummy_2
    #define IPC_WIFI_MSQ_TX_COUNT_GET   ipc_dummy_count_get

    #define IPC_WIFI_MSQ_RX_RESET       ipc_dummy_2
    #define IPC_WIFI_MSQ_RX_WRITE       ipc_dummy_get_2
    #define IPC_WIFI_MSQ_RX_WRITE_DONE  ipc_dummy_2
    #define IPC_WIFI_MSQ_RX_READ        ipc_dummy_get_2
    #define IPC_WIFI_MSQ_RX_READ_DONE   ipc_dummy_2
    #define IPC_WIFI_MSQ_RX_COUNT_GET   ipc_dummy_count_get

    #define IPC_WIFI_APS_TX_RESET       ipc_dummy_2
    #define IPC_WIFI_APS_TX_WRITE       ipc_dummy_get_2
    #define IPC_WIFI_APS_TX_WRITE_DONE  ipc_dummy_2
    #define IPC_WIFI_APS_TX_READ        ipc_dummy_get_2
    #define IPC_WIFI_APS_TX_READ_DONE   ipc_dummy_2
    #define IPC_WIFI_APS_TX_COUNT_GET   ipc_dummy_count_get

    #define IPC_WIFI_APS_RX_RESET       ipc_dummy_2
    #define IPC_WIFI_APS_RX_WRITE       ipc_dummy_get_2
    #define IPC_WIFI_APS_RX_WRITE_DONE  ipc_dummy_2
    #define IPC_WIFI_APS_RX_READ        ipc_dummy_get_2
    #define IPC_WIFI_APS_RX_READ_DONE   ipc_dummy_2
    #define IPC_WIFI_APS_RX_COUNT_GET   ipc_dummy_count_get
#else //#ifdef IPC_SUT
    #define IPC_CMD_SEND                ipc_cmd_send
    #define IPC_CMD_COUNT_GET           ipc_cmd_count_get

    #define IPC_EVT_SEND                ipc_evt_send
    #define IPC_EVT_READ                ipc_evt_read
    #define IPC_EVT_READ_DONE           ipc_evt_read_done
    #define IPC_EVT_COUNT_GET           ipc_evt_count_get

    #define IPC_BLE_CMD_SEND            ipc_ble_cmd_send
    #define IPC_BLE_CMD_COUNT_GET       ipc_ble_cmd_count_get
    
    #define IPC_BLE_EVT_SEND            ipc_ble_evt_send
    #define IPC_BLE_EVT_READ            ipc_ble_evt_read
    #define IPC_BLE_EVT_READ_DONE       ipc_ble_evt_read_done
    #define IPC_BLE_EVT_COUNT_GET       ipc_ble_evt_count_get
    
    #define IPC_BLE_TX_WRITE            ipc_ble_tx_write
    #define IPC_BLE_TX_WRITE_DONE       ipc_ble_tx_write_done
    #define IPC_BLE_TX_READ             ipc_ble_tx_read
    #define IPC_BLE_TX_READ_DONE        ipc_ble_tx_read_done
    #define IPC_BLE_TX_NEXT_DATA_EXIST  ipc_ble_tx_next_data_exist
    #define IPC_BLE_TX_COUNT_GET        ipc_ble_tx_count_get
    
    #define IPC_BLE_RX_WRITE            ipc_ble_rx_write
    #define IPC_BLE_RX_WRITE_DONE       ipc_ble_rx_write_done
    #define IPC_BLE_RX_READ             ipc_ble_rx_read
    #define IPC_BLE_RX_READ_DONE        ipc_ble_rx_read_done
    #define IPC_BLE_RX_COUNT_GET        ipc_ble_rx_count_get

    #define IPC_WIFI_CMD_SEND           ipc_wifi_cmd_send
    #define IPC_WIFI_CMD_COUNT_GET      ipc_wifi_cmd_count_get

    #define IPC_WIFI_EVT_SEND           ipc_wifi_evt_send
    #define IPC_WIFI_EVT_COUNT_GET      ipc_wifi_evt_count_get

    #define IPC_WIFI_MSQ_TX_RESET       ipc_wifi_msq_tx_reset
    #define IPC_WIFI_MSQ_TX_WRITE       ipc_wifi_msq_tx_write
    #define IPC_WIFI_MSQ_TX_WRITE_DONE  ipc_wifi_msq_tx_write_done
    #define IPC_WIFI_MSQ_TX_READ        ipc_wifi_msq_tx_read
    #define IPC_WIFI_MSQ_TX_READ_DONE   ipc_wifi_msq_tx_read_done
    #define IPC_WIFI_MSQ_TX_COUNT_GET   ipc_wifi_msq_tx_count_get

    #define IPC_WIFI_MSQ_RX_RESET       ipc_wifi_msq_rx_reset
    #define IPC_WIFI_MSQ_RX_WRITE       ipc_wifi_msq_rx_write
    #define IPC_WIFI_MSQ_RX_WRITE_DONE  ipc_wifi_msq_rx_write_done
    #define IPC_WIFI_MSQ_RX_READ        ipc_wifi_msq_rx_read
    #define IPC_WIFI_MSQ_RX_READ_DONE   ipc_wifi_msq_rx_read_done
    #define IPC_WIFI_MSQ_RX_COUNT_GET   ipc_wifi_msq_rx_count_get

    #define IPC_WIFI_APS_TX_RESET       ipc_wifi_aps_tx_reset
    #define IPC_WIFI_APS_TX_WRITE       ipc_wifi_aps_tx_write
    #define IPC_WIFI_APS_TX_WRITE_DONE  ipc_wifi_aps_tx_write_done
    #define IPC_WIFI_APS_TX_READ        ipc_wifi_aps_tx_read
    #define IPC_WIFI_APS_TX_READ_DONE   ipc_wifi_aps_tx_read_done
    #define IPC_WIFI_APS_TX_COUNT_GET   ipc_wifi_aps_tx_count_get

    #define IPC_WIFI_APS_RX_RESET       ipc_wifi_aps_rx_reset
    #define IPC_WIFI_APS_RX_WRITE       ipc_wifi_aps_rx_write
    #define IPC_WIFI_APS_RX_WRITE_DONE  ipc_wifi_aps_rx_write_done
    #define IPC_WIFI_APS_RX_READ        ipc_wifi_aps_rx_read
    #define IPC_WIFI_APS_RX_READ_DONE   ipc_wifi_aps_rx_read_done
    #define IPC_WIFI_APS_RX_COUNT_GET   ipc_wifi_aps_rx_count_get
#endif //#ifdef IPC_SUT

#define IPC_RB_BUF_NUM          1
#define IPC_COMM_RB_BUF_NUM     16

#define IPC_RB_RESERVE(rb, buf) \
{ \
    buf = rb->pdwaBuf[*(rb->pdwWrite) & rb->dwMask]; \
}

#define IPC_RB_RESERVE_DONE(rb) \
{ \
    *(rb->pdwWrite) += 1; \
}

#define IPC_RB_PROCESS(rb, buf) \
{ \
    buf = rb->pdwaBuf[*(rb->pdwRead) & rb->dwMask]; \
}

#define IPC_RB_PROCESS_DONE(rb) \
{ \
    *(rb->pdwRead) += 1; \
}

#define IPC_RB_GET(rb, buf, idx) \
{ \
    buf = rb->pdwaBuf[(*(rb->pdwRead) + idx) & rb->dwMask]; \
}

#define IPC_RB_FULL(rb) \
    (((uint32_t)(*(rb->pdwWrite) - *(rb->pdwRead))) >= rb->dwNum)

#define IPC_RB_EMPTY(rb) \
    (*(rb->pdwWrite) == *(rb->pdwRead))

#define IPC_RB_COUNT(rb) \
    (((uint32_t)(*(rb->pdwWrite) - *(rb->pdwRead))) & rb->dwMask)


typedef int (*T_IpcHandleFp)(uint32_t dwType, uint32_t dwIdx, void *pBuf, uint32_t dwBufSize);

typedef enum
{
    IPC_TYPE_CMD = 0,
    IPC_TYPE_EVT,

    IPC_TYPE_COMMON_END = IPC_TYPE_EVT,

    IPC_TYPE_BLE_CMD,
    IPC_TYPE_BLE_EVT,
    IPC_TYPE_BLE_TX,
    IPC_TYPE_BLE_RX,

    IPC_TYPE_BLE_END = IPC_TYPE_BLE_RX,

    IPC_TYPE_WIFI_CMD,
    IPC_TYPE_WIFI_EVT,
    IPC_TYPE_WIFI_MSQ_TX,
    IPC_TYPE_WIFI_MSQ_RX,
    IPC_TYPE_WIFI_APS_TX,
    IPC_TYPE_WIFI_APS_RX,

    IPC_TYPE_WIFI_END = IPC_TYPE_WIFI_APS_RX,

    IPC_TYPE_M0_MSG,
    IPC_TYPE_M3_MSG,

    IPC_TYPE_MAX,

    #ifdef IPC_SUT
    IPC_TYPE_PROC_ENABLE,
    IPC_TYPE_PROC_DISABLE,
    IPC_TYPE_HANDLE_CLEAR,
    IPC_TYPE_HANDLE_RESTORE,
    IPC_TYPE_MEM_WRITE,
    IPC_TYPE_INT_TEST,
    #endif
} T_IpcType;

typedef enum
{
    IPC_RET_DONE = 0,
    IPC_RET_ONGOING,

    IPC_RET_MAX
} T_IpcRetCode;

typedef struct
{
    uint32_t dwType;
    uint8_t baData[IPC_CMD_DATA_SIZE];
} T_IpcCmdBuf;

typedef struct
{
    uint32_t dwType;
    uint8_t baData[IPC_EVT_DATA_SIZE];
} T_IpcEvtBuf;

typedef struct
{
    uint32_t dwType;
    uint8_t baData[IPC_BLE_CMD_DATA_SIZE];
} T_IpcBleCmdBuf;

typedef struct
{
    uint32_t dwType;
    uint8_t baData[IPC_BLE_EVT_DATA_SIZE];
} T_IpcBleEvtBuf;

typedef struct
{
    uint32_t dwType;
    uint8_t baData[IPC_WIFI_CMD_DATA_SIZE];
} T_IpcWifiCmdBuf;

typedef struct
{
    uint32_t dwType;
    uint8_t baData[IPC_WIFI_EVT_DATA_SIZE];
} T_IpcWifiEvtBuf;

typedef struct
{
    void *pRb;
    uint32_t dwRbSize;
    uint32_t dwRbNum;
    uint32_t dwWriteAddr;
    uint32_t dwReadAddr;
    uint32_t dwBufStartAddr;
    uint32_t dwBufSize;
    uint32_t dwBufNum;
    T_IpcHandleFp fpHandle;
} T_IpcCb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_COMM_RB_BUF_NUM];
} T_IpcCommRb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_DEF_BUF_NUM];
} T_IpcDefRb;

/*
typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_CMD_BUF_NUM];
} T_IpcCmdRb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_EVT_BUF_NUM];
} T_IpcEvtRb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_BLE_CMD_BUF_NUM];
} T_IpcBleCmdRb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_BLE_EVT_BUF_NUM];
} T_IpcBleEvtRb;
*/
typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_BLE_TX_BUF_NUM];
} T_IpcBleTxRb;
/*
typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_BLE_RX_BUF_NUM];
} T_IpcBleRxRb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_WIFI_CMD_BUF_NUM];
} T_IpcWifiCmdRb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_WIFI_EVT_BUF_NUM];
} T_IpcWifiEvtRb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_WIFI_MSQ_TX_BUF_NUM];
} T_IpcWifiMsqTxRb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_WIFI_MSQ_RX_BUF_NUM];
} T_IpcWifiMsqRxRb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_WIFI_APS_TX_BUF_NUM];
} T_IpcWifiApsTxRb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_WIFI_APS_RX_BUF_NUM];
} T_IpcWifiApsRxRb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_M0_MSG_BUF_NUM];
} T_IpcM0MsgRb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_M3_MSG_BUF_NUM];
} T_IpcM3MsgRb;
*/

typedef void (*T_IpcCommonFp)(void);
typedef void (*T_IpcPeerReadyFp)(uint8_t bReady, uint8_t bRsp);
typedef int (*T_IpcSendFp)(uint32_t dwType, void *pData, uint32_t dwDataLen);
typedef uint32_t (*T_IpcCountGetFp)(uint32_t *pdwTotal);
typedef void *(*T_IpcReadWriteBufGetFp)(void);
typedef void *(*T_IpcBleReadWriteBufGetFp)(uint32_t dwHandle);
typedef void (*T_IpcBleReadWriteDoneFp)(uint32_t dwHandle);
typedef uint8_t (*T_IpcBleNextDataExistFp)(uint32_t dwHandle);
typedef uint32_t (*T_IpcBleCountGetFp)(uint32_t dwHandle, uint32_t *pdwTotal);
typedef uint32_t (*T_IpcWifiCountGetFp)(void);
typedef void (*T_IpcRbSetFp)(uint32_t dwType);
typedef void (*T_IpcDumpFp)(uint8_t bMode);

typedef void (*T_IpcRbProcFp)(void *pRb);
typedef void *(*T_IpcRbReadWriteBufGetFp)(void *pRb);
typedef void (*T_IpcRbInitFp)(uint32_t u32Idx, T_IpcCb *ptIpcCb);
typedef uint32_t (*T_IpcRbCountGetFp)(void *pRb, uint32_t *pdwNum);
typedef void *(*T_IpcRbReadBufGetFp)(void *pRb, uint32_t dwIdx);
typedef void (*T_IpcRbWriteDoneFp)(void *pRb, uint8_t bInd);
typedef uint8_t (*T_IpcRbCheckFp)(void *pRb);
typedef void (*T_IpcSetFp)(uint8_t bMode);

extern uint32_t g_u32IpcWifiBssInfoAddr;
extern uint32_t g_u32IpcWifiDbgParamAddr;
extern uint32_t g_u32IpcWifiStaInfoAddr;
extern uint32_t g_u32IpcPsConfAddr;

extern T_IpcCommonFp ipc_init;
extern T_IpcPeerReadyFp ipc_peer_ready;
extern T_IpcCommonFp ipc_proc;
extern T_IpcDumpFp ipc_dump;
extern T_IpcSetFp ipc_enable;

extern T_IpcSendFp ipc_cmd_send;
extern T_IpcCountGetFp ipc_cmd_count_get;

extern T_IpcSendFp ipc_ble_cmd_send;
extern T_IpcCountGetFp ipc_ble_cmd_count_get;

extern T_IpcSendFp ipc_evt_send;
extern T_IpcReadWriteBufGetFp ipc_evt_read;
extern T_IpcCommonFp ipc_evt_read_done;
extern T_IpcCountGetFp ipc_evt_count_get;

extern T_IpcSendFp ipc_ble_evt_send;
extern T_IpcReadWriteBufGetFp ipc_ble_evt_read;
extern T_IpcCommonFp ipc_ble_evt_read_done;
extern T_IpcCountGetFp ipc_ble_evt_count_get;

extern T_IpcBleReadWriteBufGetFp ipc_ble_tx_write;
extern T_IpcBleReadWriteDoneFp ipc_ble_tx_write_done;
extern T_IpcBleNextDataExistFp ipc_ble_tx_next_data_exist;
extern T_IpcBleReadWriteBufGetFp ipc_ble_tx_read;
extern T_IpcBleReadWriteDoneFp ipc_ble_tx_read_done;
extern T_IpcBleCountGetFp ipc_ble_tx_count_get;

extern T_IpcBleReadWriteBufGetFp ipc_ble_rx_write;
extern T_IpcBleReadWriteDoneFp ipc_ble_rx_write_done;
extern T_IpcBleReadWriteBufGetFp ipc_ble_rx_read;
extern T_IpcBleReadWriteDoneFp ipc_ble_rx_read_done;
extern T_IpcBleCountGetFp ipc_ble_rx_count_get;

extern T_IpcSendFp ipc_wifi_cmd_send;
extern T_IpcCountGetFp ipc_wifi_cmd_count_get;

extern T_IpcSendFp ipc_wifi_evt_send;
extern T_IpcCountGetFp ipc_wifi_evt_count_get;

extern T_IpcCommonFp ipc_wifi_msq_tx_reset;
extern T_IpcReadWriteBufGetFp ipc_wifi_msq_tx_write;
extern T_IpcCommonFp ipc_wifi_msq_tx_write_done;
extern T_IpcReadWriteBufGetFp ipc_wifi_msq_tx_read;
extern T_IpcCommonFp ipc_wifi_msq_tx_read_done;
extern T_IpcWifiCountGetFp ipc_wifi_msq_tx_count_get;

extern T_IpcCommonFp ipc_wifi_msq_rx_reset;
extern T_IpcReadWriteBufGetFp ipc_wifi_msq_rx_write;
extern T_IpcCommonFp ipc_wifi_msq_rx_write_done;
extern T_IpcReadWriteBufGetFp ipc_wifi_msq_rx_read;
extern T_IpcCommonFp ipc_wifi_msq_rx_read_done;
extern T_IpcWifiCountGetFp ipc_wifi_msq_rx_count_get;

extern T_IpcCommonFp ipc_wifi_aps_tx_reset;
extern T_IpcReadWriteBufGetFp ipc_wifi_aps_tx_write;
extern T_IpcCommonFp ipc_wifi_aps_tx_write_done;
extern T_IpcReadWriteBufGetFp ipc_wifi_aps_tx_read;
extern T_IpcCommonFp ipc_wifi_aps_tx_read_done;
extern T_IpcWifiCountGetFp ipc_wifi_aps_tx_count_get;

extern T_IpcCommonFp ipc_wifi_aps_rx_reset;
extern T_IpcReadWriteBufGetFp ipc_wifi_aps_rx_write;
extern T_IpcCommonFp ipc_wifi_aps_rx_write_done;
extern T_IpcReadWriteBufGetFp ipc_wifi_aps_rx_read;
extern T_IpcCommonFp ipc_wifi_aps_rx_read_done;
extern T_IpcWifiCountGetFp ipc_wifi_aps_rx_count_get;

#ifdef IPC_MSQ
extern T_IpcReadWriteBufGetFp ipc_m0_msg_write;
extern T_IpcCommonFp ipc_m0_msg_write_done;
extern T_IpcReadWriteBufGetFp ipc_m3_msg_read;
extern T_IpcCommonFp ipc_m3_msg_read_done;
#else
extern T_IpcReadWriteBufGetFp ipc_m3_msg_write;
extern T_IpcCommonFp ipc_m3_msg_write_done;
extern T_IpcReadWriteBufGetFp ipc_m0_msg_read;
extern T_IpcCommonFp ipc_m0_msg_read_done;
#endif

void Ipc_PreInit(void);


#ifdef IPC_SUT

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_SUT_BUF_NUM];
} T_IpcSutRb;

typedef struct
{
    uint32_t dwType;
    uint32_t dwIdx;
    uint8_t baData[IPC_SUT_DATA_SIZE];
} T_IpcSutBuf;

void ipc_sut_proc(void);

uint8_t ipc_dummy_exist(uint32_t dwHandle);
int ipc_dummy_send(uint32_t dwType, void *pData, uint32_t dwDataLen);
void ipc_dummy_1(uint32_t dwHandle);
void ipc_dummy_2(void);
void *ipc_dummy_get_1(uint32_t dwHandle);
void *ipc_dummy_get_2(void);
uint32_t ipc_dummy_count_get(void);
uint32_t ipc_dummy_count_get_2(uint32_t dwHandle, uint32_t *pdwTotal);

#ifdef IPC_MSQ
void ipc_sut_eint_2_proc(void);
#else
int ipc_sut_1_1(uint32_t dwType, uint32_t dwCount);
int ipc_sut_1_2(uint32_t dwType, uint32_t dwCount);
int ipc_sut_1_3(uint32_t dwType, uint32_t dwCount);
int ipc_sut_2_1(uint32_t dwType, uint32_t dwCount);
int ipc_sut_2_2(uint32_t dwType, uint32_t dwCount);
int ipc_sut_2_3(uint32_t dwType, uint32_t dwCount);
int ipc_sut_3_1(uint32_t dwType, uint32_t dwCount);
int ipc_sut_3_2(uint32_t dwType, uint32_t dwCount);
int ipc_sut_3_3(uint32_t dwType, uint32_t dwCount);
int ipc_sut_4_1(uint32_t dwType, uint32_t dwCount);
#endif

#endif //#ifdef IPC_SUT

#endif //#ifndef __IPC_H__

