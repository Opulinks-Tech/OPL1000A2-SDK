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
 * @file wpa_at_if.c
 * @author Michael Liao
 * @date 20 Mar 2018
 * @brief File creates the API interface for AT command extend usage.
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "opulinks.h"
#include "os.h"
#include "wpa_at_if.h"

RET_DATA int g_wpa_mode;
RET_DATA wpa_set_wpa_mode_fp_t wpa_set_wpa_mode;
RET_DATA int g_at_wifi_sort_enable;
RET_DATA int g_at_wifi_show_mask;

void wpa_set_wpa_mode_impl(int mode)
{
    g_wpa_mode = mode;
}

int is_sorting(void)
{
    return g_at_wifi_sort_enable;
}

void set_sorting(int enable, int mask)
{
    g_at_wifi_sort_enable = enable;
    g_at_wifi_show_mask = mask;
}

int get_sorting_mask(void)
{
    return g_at_wifi_show_mask;
}

/*
   Interface Initialization: WPA AT Interface
 */
void wpa_at_if_func_init(void)
{
    wpa_set_wpa_mode = wpa_set_wpa_mode_impl;

    set_sorting(false, 0);
}

