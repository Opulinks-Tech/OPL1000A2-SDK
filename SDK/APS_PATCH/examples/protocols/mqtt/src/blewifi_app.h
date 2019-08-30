/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

/**
 * @file blewifi_app.h
 * @author Vincent Chen
 * @date 12 Feb 2018
 * @brief File include the definition & function declaration of blewifi app task.
 *
 */

#ifndef __BLEWIFI_APP_H__
#define __BLEWIFI_APP_H__

#include <stdint.h>
#include "blewifi_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	uint16_t proj_id;
	uint16_t chip_id;
	uint16_t fw_id;
	uint32_t chksum;
	uint32_t curr_chksum;

	uint32_t total;
	uint32_t curr;
	uint16_t pkt_idx;
	uint16_t rx_pkt;
	uint16_t flag;
    
	uint8_t  buf[300];
	uint16_t idx;
} blewifi_ota_t;

extern blewifi_ota_t *gTheOta;

void BleWifiAppInit(void);


#ifdef __cplusplus
}
#endif

#endif /* __BLEWIFI_APP_H__ */
