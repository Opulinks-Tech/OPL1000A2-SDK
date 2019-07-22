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

#ifndef __BLEWIFI_COMMON_H__
#define __BLEWIFI_COMMON_H__

#include "msg.h"
#include "blewifi_configuration.h"
#if (SNTP_FUNCTION_EN == 1)
#include <time.h>
#endif

//#define BLEWIFI_SHOW_INFO
//#define BLEWIFI_SHOW_DUMP

#ifdef BLEWIFI_SHOW_INFO
#define BLEWIFI_INFO(fmt, ...)      tracer_log(LOG_LOW_LEVEL, fmt, ##__VA_ARGS__)
#define BLEWIFI_WARN(fmt, ...)      tracer_log(LOG_MED_LEVEL, fmt, ##__VA_ARGS__)
#define BLEWIFI_ERROR(fmt, ...)     tracer_log(LOG_HIGH_LEVEL, fmt, ##__VA_ARGS__)
#else
#define BLEWIFI_INFO(fmt, ...)
#define BLEWIFI_WARN(fmt, ...)
#define BLEWIFI_ERROR(fmt, ...)
#endif  // end of BLEWIFI_SHOW_INFO

#ifdef BLEWIFI_SHOW_DUMP
#define BLEWIFI_DUMP(a, b, c)    BleWifi_HexDump(a, b, c)
#else
#define BLEWIFI_DUMP(a, b, c)
#endif  // end of BLEWIFI_SHOW_DUMP

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

void BleWifi_HexDump(const char *title, const uint8_t *buf, size_t len);

#if (SNTP_FUNCTION_EN == 1)
/*
NTP data struct
*/
typedef struct {
    uint8_t li_vn_mode;
    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;
    int32_t rootdelay;
    int32_t rootdispersion;
    int8_t  refid[4];
    int32_t reftimeint;
    int32_t reftimefraction;
    int32_t oritimeint;
    int32_t oritimefraction;
    int32_t recvtimeint;
    int32_t recvtimefraction;
    int32_t trantimeint;
    int32_t trantimefraction;
} sntp_header_t;

#define SNTP_SEC_1970           (2208988800)                                    /* 1970 - 1900 in seconds */
#define SNTP_SEC_2019           (3755260800)                                    /* 2019 - 1900 in seconds */
#define SNTP_CONVERT_TIME(s)    ((s) - SNTP_SEC_1970 + 3600 * SNTP_TIME_ZONE)   /* Current time - 1970 + time zone */ 

/*
tm Data Structure
+----------+------+---------------------------+-------+
|  Member  | Type |          Meaning          | Range |
+----------+------+---------------------------+-------+
| tm_sec   | int  | seconds after the minute  | 0-61* |
| tm_min   | int  | minutes after the hour    | 0-59  |
| tm_hour  | int  | hours since midnight      | 0-23  |
| tm_mday  | int  | day of the month          | 1-31  |
| tm_mon   | int  | months since January      | 1-12  |
| tm_year  | int  | years since 1900          |       |
| tm_wday  | int  | days since Sunday         | 0-6   |
| tm_yday  | int  | days since January 1      | 0-365 |
| tm_isdst | int  | Daylight Saving Time flag |       |
+----------+------+---------------------------+-------+
*/

int BleWifi_SntpInit(void);
void BleWifi_SntpGet(struct tm *pSystemTime);
time_t BleWifi_SntpGetRawData(void);

extern uint32_t g_ulSystemSecondInit;    // System Clock Time
extern uint32_t g_ulSntpSecondInit;      // GMT Time
#endif

void BleWifi_RFPowerSetting(uint8_t level);

#endif  // end of __BLEWIFI_COMMON_H__
