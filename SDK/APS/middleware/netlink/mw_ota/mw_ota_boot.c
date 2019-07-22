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

/******************************************************************************
*  Filename:
*  ---------
*  mw_ota_boot.c
*
*  Project:
*  --------
*  OPL1000 Project - the Over The Air (OTA) implement file
*
*  Description:
*  ------------
*  This implement file is include the Over The Air (OTA) function and api.
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
#include <stdlib.h>
#include <string.h>
#include "mw_ota.h"
#include "mw_ota_boot.h"
#include "boot_sequence.h"
#include "hal_dbg_uart.h"
#include "hal_tick.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define PATCH_HEAD_LEN      8
#define PATCH_DATA_LEN      256

#define CHECK               "<CHECK>"
#define ACK                 "<ACK>"
#define NACK                "<NACK>"
#define START               "<START>"
#define ESCAPE              "<ESCAPE>"
#define AGENT               "<AGENT>"

#define BOOT_CHECK_RETRY    100     // 100ms

#define PATTERN_ENTRY       0xfe
#define PATTERN_BOOT        'u'
#define PATTERN_PATCH       'p'
#define PATTERN_ESCAPE      'e'
#define PATTERN_CHANGE      'c'     /* Change Baud rate */
#define PATTERN_WRITE       '1'
#define PATTERN_ERASE       '2'
#define PATTERN_HEADER      '3'     // OTA image header

#define CHANGE_BAUD_ACK     0x5A                /* 'Z' */
#define CHANGE_BAUD_NACK    0x00
#define DEFAULT_WAIT_UART_DELAY         1       /* unit: ms */
#define BEFORE_CHANGE_BAUD_DELAY        5       /* unit: ms */
#define CHANGE_BAUD_SEND_SYNC_DELAY    95       /* unit: ms */

#define INIT_BAUD_RATE               115200



typedef enum {
    BAUD_115200=0x41,
    BAUD_230400,
    BAUD_460800,
    BAUD_921600,
}E_BAUD_RATE;

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list
typedef struct UART_DATA_HEADER {
    uint8_t head[PATCH_HEAD_LEN];
    uint32_t number;
} UART_DATA_HEADER_t;


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
uint32_t g_u32BA_PrevSuccBaud;


// Sec 5: declaration of global function prototype
extern bool Boot_CheckPattern(uint8_t checkPattern, uint32_t retry);
extern void Boot_SpiLoadPatch(uint32_t ulStartAddr, uint32_t ulMaxSize, uint8_t ubUseQuadMode);
extern uint32_t Boot_RecvMultiData(uint8_t *data, uint32_t u32Length);
extern uint32_t Boot_SwapEndian(uint32_t num);
extern uint32_t Boot_GetCheckSum(uint8_t * data, uint32_t lenth);

RET_DATA T_MwOta_Boot_Init_Fp MwOta_Boot_Init;
RET_DATA T_MwOta_Boot_CheckUartBehavior_Fp MwOta_Boot_CheckUartBehavior;
RET_DATA T_MwOta_Boot_LoadPatchImage_Fp MwOta_Boot_LoadPatchImage;

// internal part
RET_DATA T_MwOta_Boot_HeaderPaser_Fp MwOta_Boot_HeaderPaser;
RET_DATA T_MwOta_Boot_WritePatchImage_Fp MwOta_Boot_WritePatchImage;
BootAgent_ChangeBaud_fp BootAgent_ChangeBaud;
BootAgent_DelayMs_fp BootAgent_DelayMs;

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype
void Boot_ChangeBaud_impl(void);

/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   MwOta_Boot_Init
*
* DESCRIPTION:
*   the init of OTA boot
*
* PARAMETERS
*   none
*
* RETURNS
*   MW_OTA_OK   : successful
*   MW_OTA_FAIL : fail
*
*************************************************************************/
uint8_t MwOta_Boot_Init_impl(void)
{
    T_MwOtaLayoutInfo tLayout;
    
    // give the layout information
    tLayout.ulaHeaderAddr[0] = MW_OTA_HEADER_ADDR_1;
    tLayout.ulaHeaderAddr[1] = MW_OTA_HEADER_ADDR_2;
    tLayout.ulaImageAddr[0] = MW_OTA_IMAGE_ADDR_1;
    tLayout.ulaImageAddr[1] = MW_OTA_IMAGE_ADDR_2;
    tLayout.ulImageSize = MW_OTA_IMAGE_SIZE;
    if (MW_OTA_OK != MwOta_Init(&tLayout, 1))
        return MW_OTA_FAIL;
    
    // give the fake current index
    MwOta_CurrentIdxFake();
    
    g_u32BA_PrevSuccBaud = INIT_BAUD_RATE;
    
    return MW_OTA_OK;
}

/*************************************************************************
* FUNCTION:
*   MwOta_Boot_CheckUartBehavior
*
* DESCRIPTION:
*   do the 2nd boot loader
*
* PARAMETERS
*   none
*
* RETURNS
*   0 : go to load the image from flash
*   1 : don't load the image from flash
*
*************************************************************************/
uint8_t MwOta_Boot_CheckUartBehavior_impl(void)
{
    uint32_t ulData = 0;
    bool bIsChangingBuad = false;
    
StartCheck:
    Boot_SendMultiData(AGENT);
    if (Boot_CheckPattern(PATTERN_ENTRY, BOOT_CHECK_RETRY))
    {
        bIsChangingBuad = false;
        g_u32BA_PrevSuccBaud = Hal_DbgUart_BaudRateGet();
        Hal_DbgUart_DataRecvTimeOut(&ulData, 1);

        switch(ulData)
        {
            case PATTERN_WRITE:
                if (MW_OTA_OK == MwOta_Boot_WritePatchImage())
                {
                    MwOta_DataFinish();
                }
                goto StartCheck;

            case PATTERN_HEADER:
                MwOta_Boot_HeaderPaser();
                goto StartCheck;

            case PATTERN_CHANGE:
                bIsChangingBuad = BootAgent_ChangeBaud();
                goto StartCheck;
            case PATTERN_ESCAPE:
            default:
                Boot_SendMultiData(ESCAPE);
                break;
        }
    }
    else if (bIsChangingBuad == true)
    {   /* Rx ENTRY pattern fail, recover Baud rate */
        Hal_DbgUart_BaudRateSet(g_u32BA_PrevSuccBaud);
        bIsChangingBuad = false;
        goto StartCheck;
    }
    
    return 0;
}

/**
 * @brief Change buad rate process
 * 
 * Rx Baud rate setting. 
 *  - If success, send ACK and change Baud rate. Wait delay and go back to start.
 *  - If fail, send NACK and go back to start.
 * @return Changing baud rate
 *
 */
bool BootAgent_ChangeBaud_impl(void)
{
    uint32_t u32Data=0xFF;
    uint32_t u32RxStatus;
    uint32_t u32NewBaudRate = 0;
    
    u32RxStatus = Hal_DbgUart_DataRecvTimeOut(&u32Data, DEFAULT_WAIT_UART_DELAY);
    if (u32RxStatus == HAL_DBG_UART_RX_SUCCESS)
    {
        switch (u32Data)
        {
            case BAUD_115200:
                u32NewBaudRate = 115200;
                break;
            case BAUD_230400:
                u32NewBaudRate = 230400;
                break;
            case BAUD_460800:
                u32NewBaudRate = 460800;
                break;
            case BAUD_921600:
                u32NewBaudRate = 921600;
                break;
            default:
                break;
        }
    }
    if (u32NewBaudRate > 0)
    {   /* Rx correct Baud rate */
            Hal_DbgUart_DataSend(CHANGE_BAUD_ACK);
            BootAgent_DelayMs(BEFORE_CHANGE_BAUD_DELAY);
        Hal_DbgUart_BaudRateSet(u32NewBaudRate);
            BootAgent_DelayMs(CHANGE_BAUD_SEND_SYNC_DELAY);
            return true;
        }
    else
    {
    Hal_DbgUart_DataSend(CHANGE_BAUD_NACK);
    return false;
    }
}



/*************************************************************************
* FUNCTION:
*   MwOta_Boot_LoadPatchImage
*
* DESCRIPTION:
*   load the patch image
*
* PARAMETERS
*   none
*
* RETURNS
*   MW_OTA_OK   : successful
*   MW_OTA_FAIL : fail
*
*************************************************************************/
uint8_t MwOta_Boot_LoadPatchImage_impl(void)
{
    uint32_t ulImageAddr;
    
    // get the start address of patch image
    if (MW_OTA_OK != MwOta_BootAddrGet(&ulImageAddr))
        return MW_OTA_FAIL;
    
    // load the patch image
    Boot_SpiLoadPatch(ulImageAddr, MW_OTA_IMAGE_SIZE, 0);
    
    return MW_OTA_OK;
}

/*************************************************************************
* FUNCTION:
*   MwOta_Boot_HeaderPaser
*
* DESCRIPTION:
*   get the header information and prepare the information for OTA write
*
* PARAMETERS
*   none
*
* RETURNS
*   MW_OTA_OK   : successful
*   MW_OTA_FAIL : fail
*
*************************************************************************/
uint8_t MwOta_Boot_HeaderPaser_impl(void)
{
    uint8_t ubaData[64];
    T_MwOtaFlashHeader *ptHeader;
    
    Boot_SendMultiData(START);
    
    // receive the 64 bytes data
    if (0 == Boot_RecvMultiData(ubaData, 64))
    {
        // prepare the information
        ptHeader = (T_MwOtaFlashHeader*)ubaData;
        if (MW_OTA_OK != MwOta_Prepare(ptHeader->uwProjectId, ptHeader->uwChipId, ptHeader->uwFirmwareId, ptHeader->ulImageSize, ptHeader->ulImageSum))
        {
            Boot_SendMultiData(NACK);
            return MW_OTA_FAIL;
        }
    }
    
    Boot_SendMultiData(ACK);
    return MW_OTA_OK;
}

/*************************************************************************
* FUNCTION:
*   MwOta_Boot_WritePatchImage
*
* DESCRIPTION:
*   write the image to flash
*
* PARAMETERS
*   none
*
* RETURNS
*   MW_OTA_OK   : successful
*   MW_OTA_FAIL : fail
*
*************************************************************************/
uint8_t MwOta_Boot_WritePatchImage_impl(void)
{
    uint32_t i = 0;
    UART_DATA_HEADER_t dataHeader;
    uint32_t dataAddr = 0;
    uint32_t dataLenth = 0;
    uint8_t data[PATCH_DATA_LEN] = {0};
    uint32_t dataChecksum = 0;
    uint32_t checksum = 0;
    uint8_t *pHead = dataHeader.head;

    Boot_SendMultiData(START);

    if(Boot_RecvMultiData((uint8_t *)&dataHeader, sizeof(UART_DATA_HEADER_t)) == 0) {

        if(strncmp((char *)pHead, "netlinkc", PATCH_HEAD_LEN) == 0) {

            dataHeader.number = Boot_SwapEndian(dataHeader.number);

            Boot_SendMultiData(ACK);

            for(i = 0; i < dataHeader.number; i++) {
                Boot_RecvMultiData((uint8_t *)&dataAddr, 4);
                Boot_RecvMultiData((uint8_t *)&dataLenth, 4);

                dataAddr  =  Boot_SwapEndian(dataAddr);
                dataLenth =  Boot_SwapEndian(dataLenth);

                if(dataLenth > 0 && dataLenth <= PATCH_DATA_LEN) {
                    Boot_RecvMultiData(data, dataLenth);
                    Boot_RecvMultiData((uint8_t *)&dataChecksum, 4);

                    dataChecksum =  Boot_SwapEndian(dataChecksum);

                    checksum = Boot_GetCheckSum(data, dataLenth);

                    if(checksum == dataChecksum) {
                        if (MW_OTA_OK == MwOta_DataIn(data, dataLenth)) {
                            Boot_SendMultiData(ACK);
                        } else {
                            Boot_SendMultiData(NACK);
                            return MW_OTA_FAIL;
                        }
                    } else {
                        printf("Invalid checksum %X %X size %d\r\n", dataChecksum, checksum, dataLenth);
                        Boot_SendMultiData(NACK);
                        return MW_OTA_FAIL;
                    }
                } else {
                    printf("Invalid data lenth %X\r\n", dataLenth);
                    Boot_SendMultiData(NACK);
                    return MW_OTA_FAIL;
                }
            }
        } else {
            printf("Invalid data head %s\r\n", pHead);
            Boot_SendMultiData(NACK);
            return MW_OTA_FAIL;
        }
    }
    
    return MW_OTA_OK;
}

/**
 * @brief Delay ms
 *
 */
void BootAgent_DelayMs_impl(uint32_t u32Ms)
{
    uint32_t u32StartTime, u32DiffTimes;
    uint32_t u32DelayCnt = u32Ms * SystemCoreClockGet() / 1000;
    
    u32StartTime = Hal_Tick_Diff(0);
    while (1)
    {
        u32DiffTimes = Hal_Tick_Diff(u32StartTime);
        if (u32DiffTimes > u32DelayCnt)
            break;
    }
}


/*************************************************************************
* FUNCTION:
*   MwOta_Boot_PreInitCold
*
* DESCRIPTION:
*   the previous initial for cold boot
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void MwOta_Boot_PreInitCold(void)
{
    MwOta_Boot_Init = MwOta_Boot_Init_impl;
    MwOta_Boot_CheckUartBehavior = MwOta_Boot_CheckUartBehavior_impl;
    MwOta_Boot_LoadPatchImage = MwOta_Boot_LoadPatchImage_impl;
    
    MwOta_Boot_HeaderPaser = MwOta_Boot_HeaderPaser_impl;
    MwOta_Boot_WritePatchImage = MwOta_Boot_WritePatchImage_impl;
    
    BootAgent_ChangeBaud = BootAgent_ChangeBaud_impl;
    BootAgent_DelayMs = BootAgent_DelayMs_impl;
}
