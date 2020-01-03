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

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
//#include "mw_fim_default_group12.h"
#include "mw_fim_default_group12_project.h"
#include "mqtt_client.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable

// the default value of MQTT data.
const T_MwFim_GP12_Mqtt_Data g_tMwFimDefaultGp12MqttData =
{
    .ubaClientId           = DEVICE_ID,
    .ubaUserName           = USERNAME,
    .ubaPassword           = PASSWORD,
    .ubKeepAliveInterval   = MQTT_KEEP_ALIVE_INTERVAL,
    .ubCleanSession        = MQTT_CLEAN_SESSION,
    .ubMqttVersion         = MQTT_VERSION,
    .ubQos                 = MQTT_QOS,
    .ubaServerIpAddr       = TCP_SERVER_ADDR,
    .ubServerIpPort        = TCP_SERVER_PORT,
    
    .ubaReserved            = {0xFF, 0xFF}
};
// the address buffer of MQTT data.
uint32_t g_ulaMwFimAddrBufferGp12MqttData[MW_FIM_GP12_MQTT_DATA_NUM];


// the information table of group 12
const T_MwFimFileInfo g_taMwFimGroupTable12_project[] =
{
    {MW_FIM_IDX_GP12_PROJECT_MQTT_DATA,          MW_FIM_GP12_MQTT_DATA_NUM,           MW_FIM_GP12_MQTT_DATA_SIZE,          (uint8_t*)&g_tMwFimDefaultGp12MqttData,          g_ulaMwFimAddrBufferGp12MqttData},

    // the end, don't modify and remove it
    {0xFFFFFFFF,            0x00,              0x00,               NULL,                            NULL}
};


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
