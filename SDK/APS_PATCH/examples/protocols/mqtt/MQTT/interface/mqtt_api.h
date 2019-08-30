#ifndef __MQTT_API_H
#define __MQTT_API_H
#include <stdint.h>
#include "MQTTPacket.h"

#define MQTT_CALLBACK_SIGNATURE void (*callback)(char*,int, char*, int)

extern int MQTT_Socket;
extern uint8_t  MQTT_SocketStatus;
extern uint8_t  miss_ping_ack_count;

int MQTT_Connect(void);
int MQTT_Publish(char *topick, char *message);
int MQTT_Subscribe(char *topick);
#endif
