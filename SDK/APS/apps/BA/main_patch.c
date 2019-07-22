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

/******************************************************************************
*  Filename:
*  ---------
*  main_patch.c
*
*  Project:
*  --------
*  OPL1000 Project - the main patch implement file
*
*  Description:
*  ------------
*  This implement file is include the main patch function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mw_ota.h"
#include "mw_ota_boot.h"
#include "boot_sequence.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern unsigned int Image$$RW_IRAM1$$ZI$$Length;
extern char Image$$RW_IRAM1$$ZI$$Base[];


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype
static void BA_main(void) __attribute__((section(".ARM.__at_0x00440000")));
static void BA_main(void) __attribute__((used));


/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   BA_main
*
* DESCRIPTION:
*   the entry point of boot agent
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void BA_main(void)
{
    // for cold boot only
    if (0 != Boot_CheckWarmBoot())
        return;

    // init bss section
    memset(Image$$RW_IRAM1$$ZI$$Base, 0, (unsigned int)&Image$$RW_IRAM1$$ZI$$Length);
    
    // OTA
    if (MW_OTA_OK == MwOta_Boot_Init())
    {
        // the UART action
        if (0 == MwOta_Boot_CheckUartBehavior())
        {
            // load the patch image
            MwOta_Boot_LoadPatchImage();
        }
    }
    
    // !!!
    // it is needed to rollback the SW patch
    // the memory space will be used for dynamical memory pool
    Boot_BeforeApplyPatch = Boot_BeforeApplyPatch_impl;
}
