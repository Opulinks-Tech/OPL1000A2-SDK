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

#include "sys_init_patch.h"

static void Main_AppInit_patch(void);


void __Patch_EntryPoint(void) __attribute__((section("ENTRY_POINT")));
void __Patch_EntryPoint(void)
{
    /* Don't remove SysInit_EntryPoint */
    SysInit_EntryPoint();
    
    /* Uncomment this function when the device is without 32k XTAL */
    //Sys_SwitchTo32kRC();
    
    /* Application initialization */
    Sys_AppInit = Main_AppInit_patch;
}


/*************************************************************************
* FUNCTION:
*   Main_AppInit_patch
*
* DESCRIPTION:
*   the initial of application
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_AppInit_patch(void)
{
    // add the application initialization from here
    printf("AppInit\n");
}
