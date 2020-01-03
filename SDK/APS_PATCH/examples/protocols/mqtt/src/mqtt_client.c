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
*  mqtt_client.c
*
*  Project:
*  --------
*  OPL1000 blewifi_mqtt Project - mqtt client implement file
*
*  Description:
*  ------------
*  This implement file includes:
*      publish topic data regularly and subscribe topic.
*
*  Author:
*  -------
*  sh_sdk
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#include <string.h>
#include "sys_init.h"
#include "sys_init_patch.h"
#include "hal_system.h"
#include "cmsis_os.h"
#include "sys_os_config.h"
#include "lwip/sockets.h"
#include "lwip/opt.h"
#include "lwip/api.h"
#include "mqtt_client.h"
#include "tcp_client.h"
#include "mqtt_api.h"
#include "cJSON.h"
#include "blewifi_ctrl.h"
#include "event_groups.h"
#include "wifi_api.h"
#include "mw_fim_default_group12_project.h"

/* OPL1000 PWM for lighting*/
#include "hal_pwm.h"
#define MAX_LIGHT_BRIGHTNESS   95
#define MIN_LIGHT_BRIGHTNESS   0
#define DEAFAULT_PWM_FREQ      500


uint8_t LightStatus = 0;

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
uint8_t MQTT_RevBuf[256] = {0};
T_MwFim_GP12_Mqtt_Data g_tGP12MqttData = {0};
// Sec 5: declaration of global function prototype

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
static char MQTT_PubData[] = MQTT_PUB_MSG; // system level publish data; fixed data
static char MQTT_PubLightData[32] = {0};
static char OPL1_PubData[OPL1_PUB_MSG_MAX_LEN] = {0};

// Sec 7: declaration of static function prototype

/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   mqtt_client
*
* DESCRIPTION:
*   the achieve function to subscribe topic and publish topic regularly.
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/

void ConfigLightStatus( char* PayloadForControlLight)
{
    if( strncmp( PayloadForControlLight, "light:ON", strlen( "light:ON" ) ) == 0 )
    {
        Hal_Pwm_SimpleConfigSet(HAL_PWM_IDX_1,MAX_LIGHT_BRIGHTNESS,DEAFAULT_PWM_FREQ); // default bringtness level
        Hal_Pwm_Enable(HAL_PWM_IDX_1);
        LightStatus = 1 ;

        printf("\033[42;31;4m [%s , %d]  light:ON  \033[0m\n\n", __FUNCTION__,__LINE__);

    }
    
    if( strncmp( PayloadForControlLight, "light:OFF", strlen( "light:OFF" ) ) == 0 )
    {
        Hal_Pwm_SimpleConfigSet(HAL_PWM_IDX_1,MIN_LIGHT_BRIGHTNESS,DEAFAULT_PWM_FREQ);
        Hal_Pwm_Enable(HAL_PWM_IDX_1);
        Hal_Pwm_Disable(HAL_PWM_IDX_1);
        LightStatus = 0 ;

        printf("\033[42;31;4m [%s , %d]  light:OFF  \033[0m\n\n", __FUNCTION__,__LINE__);

    }
    
}

void ConfigDeviceSettingParser ( char* PayloadFromTopicPublish)
{
    if(strstr(PayloadFromTopicPublish,"light") != NULL)
    {
        ConfigLightStatus(PayloadFromTopicPublish);
    }

}


void mqtt_client(void )
{
    uint16_t i = 0;
    uint8_t ping_buf[2];
    uint8_t ack_buf[10] = {0};
    uint8_t ack_buf_len = 10;
    uint8_t ack_len = 0;
    uint16_t last_ping_time = 10;
        uint8_t wifi_alive_count = 0;
        uint8_t mqtt_pub_data_len = 0;
    char full_topic_name[256] = {0};
    uint8_t macaddr[6] = {0};
    
    unsigned char dup;
    int qos = 0;
    unsigned char retained;
    unsigned short msgid;
    int payloadlen_in;
    unsigned char* payload_in;
    int rc;
    int tmp = 0;
    MQTTString receivedTopic;
    
    int ping_buf_len = sizeof(ping_buf);
    int ping_len = MQTTSerialize_pingreq(ping_buf, ping_buf_len);
    int buflen = sizeof(MQTT_RevBuf);  
        
    struct timeval receiving_timeout;

    wifi_config_get_mac_address(WIFI_MODE_STA, macaddr);
    sprintf(full_topic_name, MQTT_SUB_TOPIC_BODY, macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
    
    
    MQTT_Connect();
    MQTT_Subscribe(full_topic_name);
    MQTT_Publish(full_topic_name, MQTT_PubData);
    MQTT_Subscribe(OPL1000_SUB_TOPIC);
    
    receiving_timeout.tv_sec = 5;
    receiving_timeout.tv_usec = 0;
    
    if(setsockopt(MQTT_Socket, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout, sizeof(receiving_timeout)) < 0) 
    {
        printf("... failed to set socket receiving timeout \n");
    }
        
        // build fixed data to be published here.
        mqtt_pub_data_len = strlen(OPL1000_PUB_MSG);
        if(mqtt_pub_data_len > (OPL1_PUB_MSG_MAX_LEN - 3))
        {
              printf("[warn]: opl1000 publish too long! would be trimmed. \n");
              mqtt_pub_data_len = OPL1_PUB_MSG_MAX_LEN - 3;
        }
        strncpy(OPL1_PubData, OPL1000_PUB_MSG, mqtt_pub_data_len);
        
    while(1)
    {
        if(MQTT_SocketStatus == 1) 
        {
            last_ping_time++;
                      wifi_alive_count = 0; // clear the wifi connection count.
            if(last_ping_time > 20)
            {            
                last_ping_time = 0;// = millis();
                rc = tcp_write_data(ping_buf, ping_len);
                              printf("send ping\r\n");
                sprintf(MQTT_PubLightData,MQTT_PUB_Light_MSG,LightStatus);
                MQTT_Publish(full_topic_name, MQTT_PubLightData);
                              if(rc == 2)
                                {
                                      miss_ping_ack_count++;
                                      if(miss_ping_ack_count > 3)
                                      {
                                            printf("Missed ping ACK greater than 3.\r\n");
                                            MQTT_SocketStatus = 0;
                                            close(MQTT_Socket);
                                      }
                                }
            }
            // update publish data by adding index count.
                        i++;
                        OPL1_PubData[mqtt_pub_data_len] = ' ';
                        OPL1_PubData[mqtt_pub_data_len + 1] = (uint8_t)((uint8_t)('0') + i%10);
                        OPL1_PubData[mqtt_pub_data_len + 2] = '\n';
                        
                        // publish OPL1000 data for testing.
                        MQTT_Publish(OPL1000_PUB_TOPIC, OPL1_PubData);
                        
                        do
                        {
                            tmp = MQTTPacket_read(MQTT_RevBuf, buflen,tcp_read_data);
                    //printf("rev pack = %d, ping counter = %d\n", tmp, last_ping_time);
                            if(tmp == PUBLISH)
                            {               
                                    rc = MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic, &payload_in, &payloadlen_in, MQTT_RevBuf, buflen);
                                    if(rc == 1)
                                    {
                        //printf("topic:%s; message:\" %.*s\"   qos:%d  msgid:%d\n", receivedTopic.lenstring.data, payloadlen_in, payload_in, qos, msgid);
                                              printf("topic:%s\n", receivedTopic.lenstring.data);
                                              printf("message:\" %.*s\"   qos:%d  msgid:%d\n", payloadlen_in, payload_in, qos, msgid);

                        ConfigDeviceSettingParser((char *)payload_in);
                                            if(qos)
                                            {
                                                    ack_len = MQTTSerialize_puback(ack_buf, ack_buf_len, msgid);
                                                    rc = tcp_write_data(ack_buf, ack_len);
                                                    if(rc == ack_len)
                                                            printf("send puback ok\n\r");
                                                    else
                                                            printf("send Puback failed\n\r");
                                            }
                                    }
                                    else
                                    {
                                            printf("message  err :%d",rc); 
                                    }                
                            }
                            else if(tmp == PINGRESP)//13
                            {
                                    printf("recv ping\r\n");
                                    miss_ping_ack_count--;
                            }
                    else
                    {
                        printf("other pack = %d skip it.\n", tmp);
                    }
                        }
                        while(tmp != -1);
        }
        else // MQTT_SocketStatus == 0
        {
            printf("Socket conn closed, retry connect\n");
                      wifi_alive_count++;
                      if(wifi_alive_count > 4) //if SocketStatus=0 for sometime, positively set g_connection_flag = false to indicate the wifi connection is broken.
                        {
                                if(true == BleWifi_Ctrl_EventStatusWait(BLEWIFI_CTRL_EVENT_BIT_GOT_IP,0xFFFFFFFF))
                                {
                                        printf("... Got IP again\n");
                                        wifi_alive_count = 0;
                                }                    
                        }
            rc = MQTT_Connect();
            if( rc == -1 )
            {
                printf("[ERR]: MQTT_Connect return fail.\n");
                continue;
            }
            MQTT_Subscribe(full_topic_name);
            MQTT_Publish(full_topic_name, MQTT_PUB_MSG);
            MQTT_Subscribe(OPL1000_SUB_TOPIC);
            
            if(MQTT_SocketStatus)
            {
                if(setsockopt(MQTT_Socket, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout, sizeof(receiving_timeout)) < 0) 
                {
                    printf("... failed to set socket receiving timeout \n");
                }
            }
        }
    }
}
