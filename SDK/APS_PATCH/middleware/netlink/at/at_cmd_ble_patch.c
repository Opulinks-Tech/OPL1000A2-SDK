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

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file



// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "os.h"
#include "at_cmd.h"
#include "sys_common_ctrl.h"

#include "at_cmd_patch.h"
#include "at_cmd_common_patch.h"
#include "le_ctrl_patch.h"
#include "at_cmd_ble_patch.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
/*
 * @brief Command at+mpbleaddr
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_mp_ble_addr_patch(char *buf, int len, int mode)
{
    le_cfg_t tCfg;
    int iRet = 0;
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    uint8_t i = 0;
    uint8_t bd_addr[6] = {0};
    uint8_t type = 0;
    
    if (!at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS))
    {
        goto done;
    }
    
    if (!at_strncmp("?", &(argv[0][12]), 13))
    {
        get_ble_bd_addr(bd_addr);
        at_output("\r\nOK\r\n");
        at_output("+MPBLEADDR:\"");
        for (i = 0; i < 6; i++)
        {
            if (i == 5)
                at_output("%02X\"\r\n", bd_addr[i]);
            else
                at_output("%02X:", bd_addr[i]);
        }
        iRet = 1;
    }
    else 
    {
        if (type == 0)
        {
            extern uint8_t at_cmd_UtilGetAddrFromStr(uint8_t *addr_str, uint8_t *addr);
            if (!at_cmd_UtilGetAddrFromStr((uint8_t *)argv[2], bd_addr)) goto done;
            
            // write to flash
            set_ble_bd_addr(bd_addr);
            
            // read from flash and check if addr is match
            le_read_cfg_from_flash(&tCfg);
            if (memcmp(bd_addr, &tCfg.bd_addr, 6) == 0)
            {
                at_output("\r\nOK\r\n");
                iRet = 1;
            }
        }
    }
    
done:
    if (!iRet)
    {
        at_output("+MPBLEADDR: ERROR\r\n");
    }

    return true;
}

