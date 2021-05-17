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
 *  @file driver_netlink_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _DRIVER_NETLINK_PATCH_H_
#define _DRIVER_NETLINK_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "driver_netlink.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */


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
void wpa_driver_func_init_patch(void);
int wpa_driver_netlink_reset(void);
Boolean wpa_driver_netlink_disconnect_patch(const u8 *bssid, u16 reason_code);

#ifdef __cplusplus
}
#endif
#endif  /* _DRIVER_NETLINK_PATCH_H_ */
