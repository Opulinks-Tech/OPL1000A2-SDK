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

#ifndef __MQTT_CLIENT_H__
#define __MQTT_CLIENT_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>

// MQTT server ip & port.
#define  TCP_SERVER_ADDR      "192.168.43.44" 

#define  TCP_SERVER_PORT      1883

// Topic names & msgs.
#define  MQTT_SUB_TOPIC_BODY    "Nexless/v1/down/actioncode/monomer/%02x:%02x:%02x:%02x:%02x:%02x"
#define  MQTT_PUB_TOPIC_BODY    "Nexless/v1/down/actioncode/monomer/%02x:%02x:%02x:%02x:%02x:%02x"
#define  OPL1000_SUB_TOPIC      "OPL1_Sub_T"
#define  OPL1000_PUB_TOPIC      "OPL1_Pub_T"
#define  MQTT_PUB_MSG           "Hello,MQTT!"
#define  OPL1000_PUB_MSG        "Hi,MQTT"
#define  OPL1_PUB_MSG_MAX_LEN   30
#define  USERNAME               "admin"
#define  PASSWORD               "123456"
#define  DEVICE_ID              "DEVICE_001"
#define  MQTT_PUB_Light_MSG     "Light status is %d"

//add some more macros for mqtt_data in FIM group12.
#define  MQTT_KEEP_ALIVE_INTERVAL (180)
#define  MQTT_CLEAN_SESSION       (1)
#define  MQTT_VERSION             (3)
#define  MQTT_QOS                 (1)
#define  MQTT_DEVICE_ID_LEN       (32)
#define  MQTT_USER_NAME_LEN       (32)
#define  MQTT_PASSWORD_LEN        (64)
#define  MQTT_SERVER_IP_ADDR_LEN  (16)

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

void WifiAppInit(void);
void mqtt_client(void);
void Iot_Data_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __MQTT_CLIENT_H__ */
