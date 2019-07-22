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

#ifndef _IPERF_CLI_H_
#define _IPERF_CLI_H_

#include <stdio.h>
#include <string.h>

#define IPERF_DEFAULT_PORT     5001
#define IPERF_DEFAULT_INTERVAL 3
#define IPERF_DEFAULT_TIME     30

int cmd_iperf(int argc, char** argv);

#endif /* _IPERF2_CLI_H_ */
