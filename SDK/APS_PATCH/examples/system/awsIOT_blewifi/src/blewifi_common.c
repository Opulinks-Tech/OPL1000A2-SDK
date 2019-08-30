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

#include <stdio.h>
#include <string.h>
#include "blewifi_common.h"
#include "sys_common_api.h"

#if (SNTP_FUNCTION_EN == 1)
#include "cmsis_os.h"
#include "lwip/netdb.h"

uint32_t g_ulSystemSecondInit;    // System Clock Time Initialize
uint32_t g_ulSntpSecondInit;      // GMT Time Initialize
#endif

void BleWifi_HexDump(const char *title, const uint8_t *buf, size_t len)
{
	size_t i;

	printf("\n%s - hexdump(len=%lu):", title, (unsigned long)len);
	if (buf == NULL)
	{
		printf(" [NULL]");
	}
	else
	{
		for (i = 0; i < len; i++)
		{
            if (i%16 == 0)
                printf("\n");
			printf(" %02x", buf[i]);
		}
	}
	printf("\n");

}

#if (SNTP_FUNCTION_EN == 1)
uint32_t BleWifi_CurrentSystemTimeGet(void)
{
    uint32_t ulTick;
    int32_t  dwOverflow;
    uint32_t ulsecond;
    
    uint32_t ulSecPerTickOverflow;
    uint32_t ulSecModTickOverflow;
    uint32_t ulMsModTickOverflow;
    
    uint32_t ulSecPerTick;
    uint32_t ulSecModTick;
    
    osKernelSysTickEx(&ulTick, &dwOverflow);

    // the total time of TickOverflow is 4294967295 + 1 ms
    ulSecPerTickOverflow = (0xFFFFFFFF / osKernelSysTickFrequency) * dwOverflow;
    ulSecModTickOverflow = (((0xFFFFFFFF % osKernelSysTickFrequency) + 1) * dwOverflow) / osKernelSysTickFrequency;
    ulMsModTickOverflow = (((0xFFFFFFFF % osKernelSysTickFrequency) + 1) * dwOverflow) % osKernelSysTickFrequency;

    ulSecPerTick = (ulTick / osKernelSysTickFrequency);
    ulSecModTick = (ulTick % osKernelSysTickFrequency + ulMsModTickOverflow) / osKernelSysTickFrequency;

    ulsecond = (ulSecPerTickOverflow + ulSecModTickOverflow) + (ulSecPerTick + ulSecModTick);

    return ulsecond;
}

int BleWifi_SntpInit(void)
{
    int lRet = false;
    sntp_header_t sntp_h;
    
    struct sockaddr_in server; 
    struct addrinfo *res = NULL;
    struct in_addr *addr = NULL;
    struct timeval receiving_timeout;
    struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_DGRAM,
    };

    int err;
    int sockfd = 0;
    int retry = 0;
    int sntp_port = 0;
    
    time_t rawtime;
    struct tm *pSntpTime = NULL;
    
    /* get sntp server ip */
    err = getaddrinfo(SNTP_SERVER, SNTP_PORT_NUM, &hints, &res);
    if (err != 0 || res == NULL) 
    {
        goto fail;
    }

    addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
    printf("DNS lookup succeeded. IP=%s \r\n", inet_ntoa(*addr));

    memset((char*) &server, 0, sizeof(server));

    sscanf(SNTP_PORT_NUM, "%d", &sntp_port);
    server.sin_family = AF_INET;
    server.sin_port = htons(sntp_port);
    server.sin_addr.s_addr = addr->s_addr;
    
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if (sockfd < 0) {
        printf("create socket failed.\n");
        goto fail;
    }

    if (connect(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0)
    {
        printf("connecting failed.\n");
        goto fail;
    }

    receiving_timeout.tv_sec = 5;
    receiving_timeout.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout, sizeof(receiving_timeout)) < 0) 
    {
        printf("failed to set socket receiving timeout\r\n");
        goto fail;
    }

    /* init sntp header */
    sntp_h.li_vn_mode = 0x1b; /* li = 0, no warning, vn = 3, ntp version, mode = 3, client */

    while (retry < 5)
    {
       if (write(sockfd, (char*) &sntp_h, sizeof(sntp_h)) < 0) 
       {
           printf("sendto failed.\n");
       } 
       else 
       {
           if (read(sockfd, (char*) &sntp_h, sizeof(sntp_h)) < 0) 
           {
               printf("recvfrom failed.\n");
           } 
           else 
           {   
               g_ulSystemSecondInit = BleWifi_CurrentSystemTimeGet();
               g_ulSntpSecondInit = ntohl(sntp_h.trantimeint);
               rawtime = SNTP_CONVERT_TIME(g_ulSntpSecondInit);
               pSntpTime = localtime(&rawtime);
               printf("Current time: %d-%d-%d %d:%d:%d\n", pSntpTime->tm_year + 1900, pSntpTime->tm_mon + 1, pSntpTime->tm_mday, pSntpTime->tm_hour, pSntpTime->tm_min, pSntpTime->tm_sec);
               lRet = true;
               break;
           }
       }
       retry++;
   }
   
fail:
    if (res != NULL)
        freeaddrinfo(res);
    if (sockfd >= 0)
        close(sockfd);

    return lRet;
}

void BleWifi_SntpGet(struct tm *pSystemTime)
{
    time_t rawtime;
    struct tm * timeinfo;
    uint32_t ulTmpSystemSecond = 0;
    uint32_t ulDeltaSystemSecond = 0;

    ulTmpSystemSecond = BleWifi_CurrentSystemTimeGet();
    ulDeltaSystemSecond =  (ulTmpSystemSecond - g_ulSystemSecondInit);
    rawtime = SNTP_CONVERT_TIME(g_ulSntpSecondInit + ulDeltaSystemSecond);
    timeinfo = localtime(&rawtime);
    memcpy(pSystemTime, timeinfo, sizeof(struct tm));
    pSystemTime->tm_year = pSystemTime->tm_year + 1900;
    pSystemTime->tm_mon = pSystemTime->tm_mon + 1;
    // printf("Current time: %d-%d-%d %d:%d:%d\n", pSystemTime->tm_year, pSystemTime->tm_mon, pSystemTime->tm_mday, pSystemTime->tm_hour, pSystemTime->tm_min, pSystemTime->tm_sec);
}

time_t BleWifi_SntpGetRawData(void)
{
    time_t rawtime;
    uint32_t ulTmpSystemSecond = 0;
    uint32_t ulDeltaSystemSecond = 0;

    ulTmpSystemSecond = BleWifi_CurrentSystemTimeGet();
    ulDeltaSystemSecond =  (ulTmpSystemSecond - g_ulSystemSecondInit);
    rawtime = SNTP_CONVERT_TIME(g_ulSntpSecondInit + ulDeltaSystemSecond);
    
    return rawtime;
}

#endif

/* 
Set RF power (0x00 - 0xFF)
*/
void BleWifi_RFPowerSetting(uint8_t level)
{
    int ret = 0;

    ret = sys_set_config_rf_power_level(level);
    printf("RF Power Settings is = %s \n", (ret == 1 ? "successful":"false"));
}

