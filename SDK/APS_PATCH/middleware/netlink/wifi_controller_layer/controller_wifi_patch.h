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
 *  @file controller_wifi_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _CONTROLLER_WIFI_PATCH_H_
#define _CONTROLLER_WIFI_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "controller_wifi.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define WIFI_CFG_TOUT_TIME      5000
#define WIFI_SCAN_BY_CFG_TOUT_TIME_EXT   25000  /* SCAN_MAX_NUM_OF_DUR_TIME * WIFI_MLME_SCAN_MAX_NUM_CHANNELS +
                                                 100 ms per channel if wifi connected.
                                                 Basic scan time M0 needed : (1500 * 14) + (14 * 100) = 22400 ms */

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */



/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
void wifi_ctrl_init_patch(void);
void wifi_sta_info_init_fn_impl(void);
int ctrl_wifi_evt_cfg_patch(wifi_evt_t * pWifiEvt);

#ifdef __cplusplus
}
#endif
#endif  /* _CONTROLLER_WIFI_PATCH_H_ */
