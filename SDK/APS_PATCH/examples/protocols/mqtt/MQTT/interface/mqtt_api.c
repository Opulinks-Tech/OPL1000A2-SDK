#include <string.h>
#include "mqtt_api.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "event_loop.h"
#include "wifi_api.h"
#include "wifi_event.h"
#include "wifi_event_handler.h"
#include "lwip_helper.h"
#include "tcp_client.h"
#include "mqtt_client.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "lwip/netif.h"
#include "errno.h"

int  MQTT_Socket = -1;
int   Socket_Local;
uint16_t Socket_LocalPort;
uint8_t  Socket_SserverIP[4];
uint16_t Socket_ServerPort;

struct sockaddr_in MQTT_ServerAdd;  
char MQTT_ServerIP[32];
int MQTT_ServerPort = TCP_SERVER_PORT; 

uint8_t  MQTT_SocketStatus = 0;
uint8_t  miss_ping_ack_count = 0;

int MQTT_Begin(int p_s,uint16_t p_port)
{
    uint8_t retryNum = 0;

    MQTT_ServerAdd.sin_family = AF_INET; 
    MQTT_ServerAdd.sin_addr.s_addr = inet_addr(TCP_SERVER_ADDR);  
    MQTT_ServerAdd.sin_port = htons(TCP_SERVER_PORT); 

    strcpy(MQTT_ServerIP, TCP_SERVER_ADDR);

    printf("Connect %s at port %d \r\n", MQTT_ServerIP, MQTT_ServerPort); 

    while(1) 
    {
        p_s = socket(AF_INET, SOCK_STREAM, 0);
        if (p_s < 0) 
        {
            printf("... Failed to allocate socket. \r\n");
            osDelay(1000);
            retryNum++;
            if(retryNum > 100)
            {
                return -1;
            }
            continue;
        }
        else
        {
            return 1;
        }
    }
}


/*
@return  1:ok;
        -1:tcp connect err,check socket,server_ip,server_port;
        -2:mqtt server no ack or ack err;
*/
int  MQTT_Connect(void)
{
    uint8_t  buf[256] = {0};
    int32_t len=0;
    int rc = 0;
    int tmp;
         uint8_t  t = 5; 
    uint8_t retryNum = 0;
    uint32_t tmp_time;
    int buflen = sizeof(buf);
    uint8_t  macaddr[WIFI_MAC_ADDRESS_LENGTH];                /**< Stores the predefined device ID (MAC). */
    char  full_topic_name[128]={0};

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

    
    MQTT_ServerAdd.sin_family = AF_INET; 
    MQTT_ServerAdd.sin_addr.s_addr = inet_addr(TCP_SERVER_ADDR);  
    MQTT_ServerAdd.sin_port = htons(TCP_SERVER_PORT); 
    
    printf("Connect %s at port %d \r\n", TCP_SERVER_ADDR, TCP_SERVER_PORT); 
    printf("MQTT_ServerAdd.sin_family = %d\n", MQTT_ServerAdd.sin_family);
    printf("MQTT_ServerAdd.sin_addr = %08X\n", MQTT_ServerAdd.sin_addr.s_addr);
    printf("MQTT_ServerAdd.sin_port = %02X\n", MQTT_ServerAdd.sin_port);
    
    while(1) 
    {
        MQTT_Socket = socket(AF_INET, SOCK_STREAM, 0);
        if (MQTT_Socket < 0) 
        {
            printf("... Failed to allocate socket. \r\n");
            retryNum++;
                      osDelay(1000);
            if(retryNum > 10)
            {
                return -1;
            }
            continue;
        }
        else
        {
            break;
        }
    }
    
    while(t)
    {
        t--;
        rc = connect(MQTT_Socket, (struct sockaddr *)&MQTT_ServerAdd, sizeof(MQTT_ServerAdd));
        if( rc != 0) 
        {
            printf("... socket connect failed errno=%d \r\n", rc);
            osDelay((5 - t) * 2000);
            continue;
        }
        else
        {
            MQTT_SocketStatus = 1;
                      miss_ping_ack_count = 0; //initialize it to 0 when connection setting up.
            rc = 1;
            break;
        }
        
    }
        
    if(rc == 1)
    {
        printf("tcp connect server is ok !\r\n");
    }
    else
    {
                // update status and close socket failed to be connected. 
                MQTT_SocketStatus = 0;
                close(MQTT_Socket);    
        printf("tcp connect server is failed:%d\r\n",rc);
        return -1;
    }

    wifi_config_get_mac_address(WIFI_MODE_STA, macaddr);
    sprintf(full_topic_name, MQTT_SUB_TOPIC_BODY, macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
   
    data.clientID.cstring = DEVICE_ID;
    data.username.cstring = USERNAME;
    data.password.cstring = PASSWORD;
    data.keepAliveInterval = 120;
    data.cleansession = 1;
    data.MQTTVersion = 3;
    data.will.qos=1;
    data.will.message.cstring = "ReportNetworkStatus V1.0 \n{\"online\":false}";
    data.will.topicName.cstring = full_topic_name;
    data.willFlag = 1;
    
    len = MQTTSerialize_connect(buf, buflen, &data); /* 1 */
    /*send package*/
    tcp_write_data(buf,len);

    
    /* wait for connack */
    tmp_time = 0;//millis();
    do
    {
        tmp = MQTTPacket_read(buf, buflen, tcp_read_data);
        if(tmp == CONNACK)
        {
            unsigned char sessionPresent, connack_rc;
            if(MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
            {
                printf("Unable to connect, return code %d\n", connack_rc);
            }
            printf("connect mqtt server is ok !\r\n");
            return 1;
        }
        osDelay(100);
        tmp_time++;
    }
    while(tmp != CONNACK && (tmp_time < 20));
    return -2;
}

/*
@return  1:ok;
*/
int MQTT_Disconnect()
{
  uint8_t  buf[128] = {0};
  int32_t len=0;
  int buflen = sizeof(buf);
    
  len = MQTTSerialize_disconnect(buf, buflen ); 
  tcp_write_data(buf,len);
    
  close(MQTT_Socket);

  return 1;
}
/*
@return  1:ok;
        -1:send topick failed;
        -2:publish failed;
*/
int MQTT_Publish(char *topick, char *message)
{
//    uint16_t i = 0;
    uint8_t  buf[256] = {0};
    int32_t len=0;
    int rc = 0;
    int buflen = sizeof(buf);  
    MQTTString topicString = MQTTString_initializer;
    int msglen = strlen(message);


    topicString.cstring = topick;


    printf("----start:publish---\n");


    //build publish data.
    len = MQTTSerialize_publish(buf, buflen, 0, 1, 0, 0, topicString, (unsigned char*)message, msglen); /* 2 */
    //printf("message : %s\n",message);
    rc = tcp_write_data(buf, len);
    //printf("mqtt pub topic:%s; len:%d\n", topick, rc);
    /*for(i=0; i<rc; i++)
    {
        printf("%02X ", buf[i]);
    }
    printf("\n");*/
    if(rc > 0)
    {
        printf("topick  : %s\r\n",topick);
        printf("message : %s\r\n",message);
        printf("trans is ok,len = %d\r\n",rc);
    }
    else
    {
        printf("trans is failed,len = %x\n",rc);
        return -1;
    }
    if(rc == len)
    {
        printf("Successfully published\n");
    }
    else
    {
        printf("Publish failed\n");
        return -2;
    }
    return 1;
}

int MQTT_Subscribe(char *topick)
{
    uint8_t  buf[128] = {0};
    int32_t len=0;
    int tmp = 0;
    uint32_t tmp_time;
    int buflen = sizeof(buf);  
      int msgid = 1;
      int req_qos = 1;

      char* payload = "mypayload";
    MQTTString topicString = MQTTString_initializer;
      int payloadlen = strlen(payload);
    topicString.cstring = topick;


      /* subscribe */
      topicString.cstring = topick;
      len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);

      tcp_write_data(buf, len);
    tmp_time = 0;//retry 5 times at most here;
    do{
              tmp_time++;
        tmp = MQTTPacket_read(buf, buflen, tcp_read_data);
        if (tmp == SUBACK)     /* wait for suback */
        {
            unsigned short submsgid;
            int subcount;
            int granted_qos;

            MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
            if (granted_qos != 0)
            {
                printf("granted qos != 0, %d\n", granted_qos);
            }
            printf("subscribe:ok!\r\n");
            printf("topick   :\"%s\"\r\n",topick);
            printf("submsgid:%d\r\n",submsgid);
            printf("subcount:%d\r\n",subcount);
            printf("granted_qos:%d\r\n",granted_qos);
            return 0;
        }
                osDelay(100);
    }
    while(tmp != SUBACK && (tmp_time < 5));
      return -1;
}

