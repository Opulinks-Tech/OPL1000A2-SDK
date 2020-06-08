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
*  SH SDK
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file
/******************************************************************************
*  Test code brief
*  These examples show how to configure spi settings and use spi driver APIs.
*
*  spi_flash_test() is an example of using SPIx to access SPI flash. The operation is 
*   (1) Erase test range
*   (2) Read and check the content erased
*   (3) write a block data to certain area.
*   (4) then read it back and compare with original data.  
*   Flash area address begins from FLASH_START_ADDR(0), size BLOCK_DATA_SIZE(1024) bytes. 
*       
*  
*  SPI1 and SPI2 signal pin and parameters are defined by OPL1000_periph.spi
* 
******************************************************************************/


// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "sys_init.h"
#include "sys_init_patch.h"
#include "hal_system.h"
#include "mw_fim.h"
#include "cmsis_os.h"
#include "sys_os_config.h"
#include "Hal_pinmux_spi.h"
#include "hal_flash_patch.h"
#include "hal_pin.h"
#include "hal_pin_def.h"
#include "hal_pin_config_project.h"
#include "at_cmd_common_patch.h"
#include "hal_dbg_uart.h"
#include "hal_vic.h"
#include "boot_sequence.h"
//#include "hal_wdt.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define DUMMY          0x00
#define SPI1_IDX       0
#define SPI2_IDX       0   // it is corresponding to the index in spi
#define TEST_SPI       SPI2_IDX

#define FLASH_START_SECTOR  0x00   // 0x000000
#define BLOCK_DATA_SIZE     1024    // bytes
#define MIN_DATA_VALUE      50 
#define MAX_DATA_VALUE      200    // note:(MAX_DATA_VALUE + MIN_DATA_VALUE) < 255 

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype
typedef void (*T_Main_AppInit_fp)(void);
extern T_Main_AppInit_fp Main_AppInit;


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable

static E_IO01_UART_MODE g_eAppIO01UartMode;
// Sec 7: declaration of static function prototype
void __Patch_EntryPoint(void) __attribute__((section("ENTRY_POINT")));
void __Patch_EntryPoint(void) __attribute__((used));
static void Main_PinMuxUpdate(void);
static void Main_FlashLayoutUpdate(void);

void Main_AppInit_patch(void);
void spi_flash_test(E_SpiIdx_t eSpiIdx);
void spi_read_device_id_test(E_SpiIdx_t eSpiIdx);

static void Main_MiscModulesInit(void);
static void Main_MiscDriverConfigSetup(void);
static void Main_AtUartDbgUartSwitch(void);
static void Main_ApsUartRxDectecConfig(void);
static void Main_ApsUartRxDectecCb(E_GpioIdx_t tGpioIdx);


/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   __Patch_EntryPoint
*
* DESCRIPTION:
*   the entry point of SW patch
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void __Patch_EntryPoint(void)
{
    // don't remove this code
    SysInit_EntryPoint();

#ifdef SWITCH_TO_32K_RC
    // Uncomment this function when the device is without 32k XTAL.
    Sys_SwitchTo32kRC();
#endif 
    
    // update the pin mux
    Hal_SysPinMuxAppInit = Main_PinMuxUpdate;
    
    // update the flash layout
    MwFim_FlashLayoutUpdate = Main_FlashLayoutUpdate;

    // the initial of driver part for cold and warm boot
    Sys_MiscModulesInit = Main_MiscModulesInit;
    Sys_MiscDriverConfigSetup = Main_MiscDriverConfigSetup;

    // update the switch AT UART / dbg UART function
    at_cmd_switch_uart1_dbguart = Main_AtUartDbgUartSwitch;
    
    Sys_SetUnsuedSramEndBound(0x440000);
    // application init
    Sys_AppInit = Main_AppInit_patch;
}


/*************************************************************************
* FUNCTION:
*   Main_PinMuxUpdate
*
* DESCRIPTION:
*   update the pin-mux setting
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_PinMuxUpdate(void)
{
    Hal_Pin_ConfigSet(0, HAL_PIN_TYPE_IO_0, HAL_PIN_DRIVING_IO_0);
    Hal_Pin_ConfigSet(1, HAL_PIN_TYPE_IO_1, HAL_PIN_DRIVING_IO_1);
    Hal_Pin_ConfigSet(2, HAL_PIN_TYPE_IO_2, HAL_PIN_DRIVING_IO_2);
    Hal_Pin_ConfigSet(3, HAL_PIN_TYPE_IO_3, HAL_PIN_DRIVING_IO_3);
    Hal_Pin_ConfigSet(4, HAL_PIN_TYPE_IO_4, HAL_PIN_DRIVING_IO_4);
    Hal_Pin_ConfigSet(5, HAL_PIN_TYPE_IO_5, HAL_PIN_DRIVING_IO_5);
    Hal_Pin_ConfigSet(6, HAL_PIN_TYPE_IO_6, HAL_PIN_DRIVING_IO_6);
    Hal_Pin_ConfigSet(7, HAL_PIN_TYPE_IO_7, HAL_PIN_DRIVING_IO_7);
    Hal_Pin_ConfigSet(8, HAL_PIN_TYPE_IO_8, HAL_PIN_DRIVING_IO_8);
    Hal_Pin_ConfigSet(9, HAL_PIN_TYPE_IO_9, HAL_PIN_DRIVING_IO_9);
    Hal_Pin_ConfigSet(10, HAL_PIN_TYPE_IO_10, HAL_PIN_DRIVING_IO_10);
    Hal_Pin_ConfigSet(11, HAL_PIN_TYPE_IO_11, HAL_PIN_DRIVING_IO_11);
    Hal_Pin_ConfigSet(12, HAL_PIN_TYPE_IO_12, HAL_PIN_DRIVING_IO_12);
    Hal_Pin_ConfigSet(13, HAL_PIN_TYPE_IO_13, HAL_PIN_DRIVING_IO_13);
    Hal_Pin_ConfigSet(14, HAL_PIN_TYPE_IO_14, HAL_PIN_DRIVING_IO_14);
    Hal_Pin_ConfigSet(15, HAL_PIN_TYPE_IO_15, HAL_PIN_DRIVING_IO_15);
    Hal_Pin_ConfigSet(16, HAL_PIN_TYPE_IO_16, HAL_PIN_DRIVING_IO_16);
    Hal_Pin_ConfigSet(17, HAL_PIN_TYPE_IO_17, HAL_PIN_DRIVING_IO_17);
    Hal_Pin_ConfigSet(18, HAL_PIN_TYPE_IO_18, HAL_PIN_DRIVING_IO_18);
    Hal_Pin_ConfigSet(19, HAL_PIN_TYPE_IO_19, HAL_PIN_DRIVING_IO_19);
    Hal_Pin_ConfigSet(20, HAL_PIN_TYPE_IO_20, HAL_PIN_DRIVING_IO_20);
    Hal_Pin_ConfigSet(21, HAL_PIN_TYPE_IO_21, HAL_PIN_DRIVING_IO_21);
    Hal_Pin_ConfigSet(22, HAL_PIN_TYPE_IO_22, HAL_PIN_DRIVING_IO_22);
    Hal_Pin_ConfigSet(23, HAL_PIN_TYPE_IO_23, HAL_PIN_DRIVING_IO_23);
    at_io01_uart_mode_set(HAL_PIN_0_1_UART_MODE);
}

/*************************************************************************
* FUNCTION:
*   Main_FlashLayoutUpdate
*
* DESCRIPTION:
*   update the flash layout
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_FlashLayoutUpdate(void)
{
    // update here
}

/*************************************************************************
* FUNCTION:
*   Main_MiscModulesInit
*
* DESCRIPTION:
*   the initial of driver part for cold and warm boot
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_MiscModulesInit(void)
{
      
}

/*************************************************************************
* FUNCTION:
*   Main_MiscDriverConfigSetup
*
* DESCRIPTION:
*   the initial of driver part for cold and warm boot
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_MiscDriverConfigSetup(void)
{
    //Hal_Wdt_Stop();   //disable watchdog here.

    // IO 1 : detect the GPIO high level if APS UART Rx is connected to another UART Tx port.
    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        Hal_DbgUart_RxIntEn(0);
        
        if (HAL_PIN_TYPE_IO_1 == PIN_TYPE_UART_APS_RX)
        {
            Main_ApsUartRxDectecConfig();
        }
    }
}

/*************************************************************************
* FUNCTION:
*   Main_AtUartDbgUartSwitch
*
* DESCRIPTION:
*   switch the UART1 and dbg UART
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_AtUartDbgUartSwitch(void)
{
    if (g_eAppIO01UartMode == IO01_UART_MODE_AT)
    {
        Hal_Pin_ConfigSet(0, PIN_TYPE_UART_APS_TX, PIN_DRIVING_FLOAT);
        Hal_Pin_ConfigSet(1, PIN_TYPE_UART_APS_RX, PIN_DRIVING_LOW);

        Hal_Pin_ConfigSet(8, PIN_TYPE_UART1_TX, PIN_DRIVING_FLOAT);
        Hal_Pin_ConfigSet(9, PIN_TYPE_UART1_RX, PIN_DRIVING_HIGH);

        Hal_DbgUart_RxIntEn(1);
    }
    else
    {
        Hal_DbgUart_RxIntEn(0);

        Hal_Pin_ConfigSet(0, PIN_TYPE_UART1_TX, PIN_DRIVING_FLOAT);
        Hal_Pin_ConfigSet(1, PIN_TYPE_UART1_RX, PIN_DRIVING_LOW);
        
        Hal_Pin_ConfigSet(8, PIN_TYPE_UART_APS_TX, PIN_DRIVING_FLOAT);
        Hal_Pin_ConfigSet(9, PIN_TYPE_UART_APS_RX, PIN_DRIVING_HIGH);
    }
    
    g_eAppIO01UartMode = (E_IO01_UART_MODE)!g_eAppIO01UartMode;
}

/*************************************************************************
* FUNCTION:
*   App_SPI_InitConfig
*
* DESCRIPTION:
*   init the pin assignment. PinMux is determined by OPL1000_periph 
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void App_SPI_InitConfig(void)
{    
    printf("SPI%d initialization  \r\n", OPL1000_periph.spi[TEST_SPI].index);
    Hal_Pinmux_Spi_Init(&OPL1000_periph.spi[TEST_SPI]);    
    
    // Init SPIx
    if (Hal_Spi_Init(OPL1000_periph.spi[TEST_SPI].index, SystemCoreClockGet()/2,
        SPI_CLK_PLOAR_HIGH_ACT, SPI_CLK_PHASE_START, SPI_FMT_MOTOROLA, SPI_DFS_08_bit, 1) !=0)
    {
        printf("SPI%d init fail\r\n", OPL1000_periph.spi[TEST_SPI].index);
        return;
    }    
    
    // Init flash on SPIx
    if (Hal_Flash_Init(OPL1000_periph.spi[TEST_SPI].index) != 0)
    {
        printf("Flash init fail\r\n", OPL1000_periph.spi[TEST_SPI].index);
        return;
    }
    
    printf("SPI%d Flash init done\r\n", OPL1000_periph.spi[TEST_SPI].index);
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
void Main_AppInit_patch(void)
{
    // init the pin assignment and SPI flash init
    App_SPI_InitConfig();    
    
    // Run SPI flash test
    spi_flash_test(OPL1000_periph.spi[TEST_SPI].index);
    
    // If flash init fail, try to check read flash ID success or not
    //spi_read_device_id_test(OPL1000_periph.spi[TEST_SPI].index);
}

/*************************************************************************
* FUNCTION:
*   spi_flash_test
*
* DESCRIPTION:
*   an example that read and write data on SPIx to access on board spi flash.
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI
*
* RETURNS
*   none
*
*************************************************************************/
void spi_flash_test(E_SpiIdx_t eSpiIdx)
{
    uint8_t u8aBlockData[BLOCK_DATA_SIZE], u8aReadData[BLOCK_DATA_SIZE] = {0};
    uint32_t i, u32Length, u32SectorAddr32bit;
    bool match_flag = true; 
    
    u32Length = BLOCK_DATA_SIZE;
      
    // prepare test block data 
    for (i=0; i<u32Length; i++)
        u8aBlockData[i] = (i%MAX_DATA_VALUE) + MIN_DATA_VALUE;
      
    
    u32SectorAddr32bit = (((uint32_t)FLASH_START_SECTOR) << 12) & 0x000ff000;
    // Erase flash
    if (Hal_Flash_4KSectorAddrErase(eSpiIdx, u32SectorAddr32bit) != 0)
    {
        printf("SPI%d flash sector erase fail\r\n", eSpiIdx);
        return;
    }
    
    // Read flash and check erased
    if (Hal_Flash_AddrRead(eSpiIdx, u32SectorAddr32bit, OPL1000_periph.spi[TEST_SPI].qMode, u32Length, u8aReadData) != 0)
    {
        printf("SPI%d read flash fail\r\n", eSpiIdx);
        return;
    }
    for(i=0; i<u32Length; i++)
    {
        if(u8aReadData[i] != 0xFF)
        {
            printf("No.%d data erase fails. read %02x \r\n",i+1, u8aReadData[i]);
            match_flag = false;
        }
    }
    if (match_flag == false)
    {
        return;
    }
    
        
    // Write u8aBlockData into flash   
    if (Hal_Flash_AddrProgram(eSpiIdx, u32SectorAddr32bit, OPL1000_periph.spi[TEST_SPI].qMode, u32Length, u8aBlockData) != 0)
    {
        printf("SPI%d program fails\r\n", eSpiIdx);
        return;
    }
    // Read flash content from SectorAddr32bit
    if (Hal_Flash_AddrRead(eSpiIdx, u32SectorAddr32bit, OPL1000_periph.spi[TEST_SPI].qMode, u32Length, u8aReadData ) != 0)
    {
        printf("SPI%d read flash fails\r\n", eSpiIdx);
        return;
    }
    for(i=0; i<u32Length; i++)
    {
        if(u8aBlockData[i] != u8aReadData[i] )
        {
            printf("No.%d data error. write %x, read %x \r\n",i+1, u8aBlockData[i],u8aReadData[i]);
            match_flag = false;
        }
    }

    if (match_flag == true)
    {
        printf("Write and read %d bytes data on flash @0x%06x successfully \r\n",u32Length, u32SectorAddr32bit); 
    }    
        
}

/*************************************************************************
* FUNCTION:
*   spi_read_device_id_test
*
* DESCRIPTION:
*   Read flash ID test. Only for debug use, be care of using in thread because this function is not protected by semaphore
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI
*
* RETURNS
*   none
*
*************************************************************************/
void spi_read_device_id_test(E_SpiIdx_t eSpiIdx)
{
    uint32_t u32aTxData[4] = {0x9F, 0, 0, 0};
    uint32_t u32aRxData[4];

    if(eSpiIdx >= SPI_IDX_MAX)
        return;
    
    Hal_Spi_Data_Send(eSpiIdx, TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | u32aTxData[0]);
    Hal_Spi_Data_Send(eSpiIdx, TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | u32aTxData[1]);
    Hal_Spi_Data_Send(eSpiIdx, TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | u32aTxData[2]);
    Hal_Spi_Data_Send(eSpiIdx, TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_WRITE | u32aTxData[3]);
    Hal_Spi_Data_Recv(eSpiIdx, &u32aRxData[0]);
    Hal_Spi_Data_Recv(eSpiIdx, &u32aRxData[1]);
    Hal_Spi_Data_Recv(eSpiIdx, &u32aRxData[2]);
    Hal_Spi_Data_Recv(eSpiIdx, &u32aRxData[3]);
    printf("Tx command 9F\n");
    printf("Rx flash ID %02X %02X %02X\n\n", u32aRxData[1], u32aRxData[2], u32aRxData[3]);
    
}

/*************************************************************************
* FUNCTION:
*   Main_ApsUartRxDectecConfig
*
* DESCRIPTION:
*   detect the GPIO high level if APS UART Rx is connected to another UART Tx port.
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_ApsUartRxDectecConfig(void)
{
    E_GpioLevel_t eGpioLevel;

    Hal_Pin_ConfigSet(1, PIN_TYPE_GPIO_INPUT, PIN_DRIVING_LOW);
    eGpioLevel = Hal_Vic_GpioInput(GPIO_IDX_01);
    if (GPIO_LEVEL_HIGH == eGpioLevel)
    {
        // it is connected
        Hal_Pin_ConfigSet(1, HAL_PIN_TYPE_IO_1, HAL_PIN_DRIVING_IO_1);
        Hal_DbgUart_RxIntEn(1);
    }
    else //if (GPIO_LEVEL_LOW == eGpioLevel)
    {
        // it is not conncected, set the high level to trigger the GPIO interrupt
        Hal_Vic_GpioCallBackFuncSet(GPIO_IDX_01, Main_ApsUartRxDectecCb);
        //Hal_Vic_GpioDirection(GPIO_IDX_01, GPIO_INPUT);
        Hal_Vic_GpioIntTypeSel(GPIO_IDX_01, INT_TYPE_LEVEL);
        Hal_Vic_GpioIntInv(GPIO_IDX_01, 0);
        Hal_Vic_GpioIntMask(GPIO_IDX_01, 0);
        Hal_Vic_GpioIntEn(GPIO_IDX_01, 1);
    }
}

/*************************************************************************
* FUNCTION:
*   Main_ApsUartRxDectecCb
*
* DESCRIPTION:
*   detect the GPIO high level if APS UART Rx is connected to another UART Tx port.
*
* PARAMETERS
*   1. tGpioIdx : Index of call-back GPIO
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_ApsUartRxDectecCb(E_GpioIdx_t tGpioIdx)
{
    // disable the GPIO interrupt
    Hal_Vic_GpioIntEn(GPIO_IDX_01, 0);

    // it it connected
    Hal_Pin_ConfigSet(1, HAL_PIN_TYPE_IO_1, HAL_PIN_DRIVING_IO_1);
    Hal_DbgUart_RxIntEn(1);
}

