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
 * @file wpa_at_if.h
 * @author Michael Liao
 * @date 20 Mar 2018
 * @brief File containing declaration of wpa at command interface api & definition of structure for reference.
 *
 */

#ifndef __WPA_AT_IF_H__
#define __WPA_AT_IF_H__

enum wifi_wpa_mode
{
  WPA_MODE_STA = 1,
  WPA_MODE_AP,
  WPA_MODE_STA_AP
};

extern int g_wpa_mode;

typedef void (*wpa_set_wpa_mode_fp_t)(int mode);
extern wpa_set_wpa_mode_fp_t wpa_set_wpa_mode;

int is_sorting(void);
void set_sorting(int enable, int mask);
int get_sorting_mask(void);

void wpa_at_if_func_init(void);

#endif

