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

#ifndef _WIFI_CLI_H_
#define _WIFI_CLI_H_

int wifi_cmd_sta(int argc, char** argv);
int wifi_cmd_scan(int argc, char** argv);
int wifi_cmd_query(int argc, char **argv);
int wifi_cmd_disconnect(int argc, char **argv);
int wifi_cmd_data_rate(int argc, char **argv);
int wifi_cmd_sta_mac_addr(int argc, char **argv);
int wifi_cmd_hl_power(int argc, char **argv);

#endif /* _WIFI_CLI_H_ */
