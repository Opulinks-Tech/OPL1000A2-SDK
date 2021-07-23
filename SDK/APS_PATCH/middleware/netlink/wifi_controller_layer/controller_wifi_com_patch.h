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
 *  @file controller_wifi_com_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _CONTROLLER_WIFI_COM_PATCH_H_
#define _CONTROLLER_WIFI_COM_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "controller_wifi_com.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define SCAN_PROBE_REQ_COUNTERS_DEF   1
#define SCAN_MIN_DURATION_TIME        10

#define CONNECT_AP_ERR_AUTH_FAILED    203
#define CONNECT_AP_ERR_ASSOC_FAILED   204
#define CONNECT_AP_ERR_OUT_OF_COUNTRY 205

#define CTRL_WIFI_ACT_AUTO    1
#define CTRL_WIFI_ACT_HIDDEN  2

#define CTRL_WIFI_ACT_BIT_SET(x, bit) (x |= (1 << bit))
#define CTRL_WIFI_ACT_BIT_GET(x, bit) ((x >> bit) & 1)
#define CTRL_WIFI_ACT_BIT_CLR(x, bit) (x &= ~(1 << bit))

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef void (*T_Ctrl_Wifi_EvtHandler_Fp)(uint32_t evt_type, void *data, int len);
typedef struct
{
    uint32_t ulEventId;
    T_Ctrl_Wifi_EvtHandler_Fp fpFunc;
} T_Ctrl_Wifi_EvtHandlerTbl;

typedef struct
{
    /*
     * Country code (String):
     *    "CN", "US", "JP", "EU"
     */
    char cc[3];

    /*
     * Start channel 1.
     */
    uint8_t start_chan;

    /*
     * Maximum number of Channel:
     *    "CN" : 13
     *    "US" : 11
     *    "JP" : 14
     *    "EU" : 13
     */
    uint8_t max_chan;
} ctrl_wifi_country_info_t;

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern uint32_t ctrl_wifi_act;

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
int ctrl_wifi_set_country_info(ctrl_wifi_country_info_t *info);
int ctrl_wifi_get_country_info(ctrl_wifi_country_info_t *info);

#ifdef __cplusplus
}
#endif
#endif  /* _CONTROLLER_WIFI_COM_PATCH_H_ */
