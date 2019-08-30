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

// Sec 5: declaration of global function prototype

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
static char MQTT_PubData[] = MQTT_PUB_MSG; // system level publish data; fixed data
static char OPL1_PubData[OPL1_PUB_MSG_MAX_LEN] = {0}; // OPL1000 publish data for testing; variable data

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
    
    osDelay(1000);
    MQTT_Connect();
    osDelay(1000);
    MQTT_Publish(MQTT_PUB_TOPIC, MQTT_PubData);
    osDelay(1000);
    MQTT_Subscribe(MQTT_SUB_TOPIC);
    osDelay(1000);
    MQTT_Subscribe(OPL1000_SUB_TOPIC);
    
    osDelay(1000);
    receiving_timeout.tv_sec = 1;
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
				if(MQTT_SocketStatus == 1) //20190628-Update.
        {
            last_ping_time++;
					  wifi_alive_count = 0; // clear the wifi connection count.
            if(last_ping_time > 20)
            {            
                last_ping_time = 0;// = millis();
                rc = tcp_write_data(ping_buf, ping_len);
							  printf("send ping\r\n");
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
                osDelay(20);
            }
            // update publish data by adding index count.
						i++;
						OPL1_PubData[mqtt_pub_data_len] = ' ';
						OPL1_PubData[mqtt_pub_data_len + 1] = (uint8_t)((uint8_t)('0') + i%10);
						OPL1_PubData[mqtt_pub_data_len + 2] = '\n';
						
						// publish OPL1000 data for testing.
						MQTT_Publish(OPL1000_PUB_TOPIC, OPL1_PubData);
						osDelay(1000);
						
						do
						{
							tmp = MQTTPacket_read(MQTT_RevBuf, buflen,tcp_read_data);
			        //printf("rev pack = %d, ping counter = %d\n", tmp, last_ping_time);
							if(tmp == PUBLISH)
							{               
									rc = MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic, &payload_in, &payloadlen_in, MQTT_RevBuf, buflen);
									if(rc == 1)
									{
											printf("topic:%s; message:\" %.*s\"   qos:%d  msgid:%d\n", receivedTopic.lenstring.data, payloadlen_in, payload_in, qos, msgid);
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
									osDelay(20);
							}
			        else
			        {
			            printf("other pack = %d skip it.\n", tmp);
			        }
						}
						while(tmp != -1);
        }
        else //20190628-Update.  MQTT_SocketStatus == 0
        {
            printf("Socket conn closed, retry connect\n");
					  wifi_alive_count++;
					  if(wifi_alive_count > 4) //if SocketStatus=0 for sometime, positively set g_connection_flag = false to indicate the wifi connection is broken.
						{
								//20190628-Update.
								if(true == BleWifi_Ctrl_EventStatusWait(BLEWIFI_CTRL_EVENT_BIT_GOT_IP,0xFFFFFFFF))
								{
										printf("... Got IP again\n");
										osDelay(100);
										wifi_alive_count = 0;
								}					
						}
            osDelay(600);
            rc = MQTT_Connect();
			if( rc == -1 )
			{
			    osDelay(1000);
				printf("[ERR]: MQTT_Connect return fail.\n");
				continue;
			}
            osDelay(600);
            MQTT_Publish(MQTT_PUB_TOPIC, MQTT_PUB_MSG);
            osDelay(600);
            MQTT_Subscribe(MQTT_SUB_TOPIC);
						osDelay(1000);
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
