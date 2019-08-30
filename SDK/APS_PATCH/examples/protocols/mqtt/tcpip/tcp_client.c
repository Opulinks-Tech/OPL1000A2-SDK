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
#include "mqtt_api.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "lwip/netif.h"
#include "errno.h"

/*
@return  sended data size for success else 0.
*/
int tcp_write_data(unsigned char* buf, int buflen)
{
	int rc = 0;
    rc = write(MQTT_Socket, buf, buflen);
	  printf("rc = %d\n", rc);
    if(rc < 0) 
    {
        printf("... socket send failed rc = %d\n", rc);
        MQTT_SocketStatus = 0;
        close(MQTT_Socket);

    }
	return rc;
}

/*
@return  received data size for success else 0.
*/
int tcp_read_data(unsigned char* buf, int count)
{
    int res = 0;
    res = read(MQTT_Socket, buf, count);
    if(res == -1)
        res = 0;
	return res;
}
