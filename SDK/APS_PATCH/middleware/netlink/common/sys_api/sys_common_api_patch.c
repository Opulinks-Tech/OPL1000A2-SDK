/******************************************************************************
*  Copyright 2017 - 2019, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2019
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "sys_common_types.h"
#include "sys_common_api_patch.h"
#include "sys_common_ctrl.h"

#define API_SYS_COMMON_LOGE(fmt,arg...)             printf(("E [API]: "fmt"\r\n"), ##arg)
#define API_SYS_COMMON_LOGI(fmt,arg...)             printf(("I [API]: "fmt"\r\n"), ##arg)

extern int at_cmd_rf_test_mode(char *buf, int len, int mode);

int sys_set_wifi_lowpower_tx_vdd_rf(uint8_t level)
{
    int ret;
    char strCmd[] = "at+rftm=4,0";    

    if( level > 3){
        API_SYS_COMMON_LOGE("The parameter is illegal.");
        return -1;
    }

    strCmd[10] += level;
    ret = at_cmd_rf_test_mode( strCmd, 0xB, 2);
    if( ret )
        return 0;
    else
        return -1;
}

int sys_set_rf_temp_cal_mode(uint8_t mode)
{
    int ret;
    char strCmd[] = "at+rftm=5,00";    

    if( mode == 1 ){
        strCmd[11] = '7';
    }
    else if( mode == 2 ){
        // 55 = 0x37
        strCmd[10] = '5';
        strCmd[11] = '5';
    }

    ret = at_cmd_rf_test_mode( strCmd, 0xB, 2);
    if( ret )
        return 0;
    else
        return -1;
}
