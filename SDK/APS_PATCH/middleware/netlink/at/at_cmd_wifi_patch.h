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
 *  @file at_cmd_wifi_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _AT_CMD_WIFI_PATCH_H_
#define _AT_CMD_WIFI_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "at_cmd_wifi.h"
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
typedef struct {
    uint8_t sort_en;
    uint8_t mask;
    uint8_t times;
    uint8_t probe_counters;
} at_lap_opt_t;


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
void at_cmd_wifi_func_init_patch(void);

#ifdef __cplusplus
}
#endif
#endif  /* _AT_CMD_WIFI_PATCH_H_ */
