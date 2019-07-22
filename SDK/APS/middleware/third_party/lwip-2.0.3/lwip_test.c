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

/*
 * @brief LWIP TCP Echo example
 *
 * @note
 *
 */
#if 1
#include <stdio.h>
#include <string.h>

#include "cmsis_os.h"
#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "lwip/stats.h"
#if LWIP_DHCP
#include "lwip/dhcp.h"
#endif

#include "msg.h"
#include "wlannetif.h"
#include "tcpecho.h"
#include "lwip_helper.h"
#include "sys_os_config.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
/* NETIF data */
extern struct netif netif;
osThreadId tid_SetupIFTask;                     /* Thread id of thread: SetupIFTask      */

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
void msDelay(uint32_t ms);


/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Callback for TCPIP thread to indicate TCPIP init is done */
static void tcpip_init_done_signal(void *arg)
{
	/* Tell main thread TCP/IP init is done */
	*(s32_t *) arg = 1;
}

/* LWIP kickoff and PHY link monitor thread */
#if 1
static void SetupIFTask(void  *pvParameters)
{
	ip4_addr_t ipaddr, netmask, gw;
	volatile s32_t tcpipdone = 0;
	//uint32_t physts;
	static int prt_ip = 0;

	/* Wait until the TCP/IP thread is finished before
	   continuing or wierd things may happen */
	LWIP_DEBUGF(LWIP_DBG_ON, ("Waiting for TCPIP thread to initialize...\n"));
	tcpip_init(tcpip_init_done_signal, (void *) &tcpipdone);
	while (!tcpipdone) {
		msDelay(1);
	}

	LWIP_DEBUGF(LWIP_DBG_ON, ("Starting LWIP TCP echo server...\n"));

	/* Static IP assignment */
#if LWIP_DHCP
	IP4_ADDR(&gw, 0, 0, 0, 0);
	IP4_ADDR(&ipaddr, 0, 0, 0, 0);
	IP4_ADDR(&netmask, 0, 0, 0, 0);
#else
	IP4_ADDR(&gw, 255, 255, 255, 1);
	IP4_ADDR(&ipaddr, 190, 168, 1, 99);
	IP4_ADDR(&netmask, 255, 255, 255, 0);
#endif

	/* Add netif interface for NL1000, wlan interface */
	if (!netif_add(&netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, tcpip_input)) {
		LWIP_ASSERT("Net interface failed to initialize\r\n", 0);
	}
	netif_set_default(&netif);
	netif_set_up(&netif);

	/* Enable MAC interrupts only after LWIP is ready */
	//NVIC_SetPriority(ETHERNET_IRQn, config_ETHERNET_INTERRUPT_PRIORITY);
	//NVIC_EnableIRQ(ETHERNET_IRQn);

#if LWIP_DHCP
	dhcp_start(&netif);
#endif

	/* Initialize and start application */
	//tcpecho_init();
    //ping_init();
    //ping_init(200, "0.0.0.0", 7, 32);


	/* This loop monitors the PHY link and will handle cable events
	   via the PHY driver. */
	while (1) {
        /* Handle packets as part of this loop, not in the IRQ handler */
        //ethernetif_poll(&netif);
        

		/* Call the PHY status update state machine once in a while
		   to keep the link status up-to-date */
        //stats_display();

		/* Only check for connection state when the PHY status has changed */
     	/* Delay for link detection (250mS) */
        msDelay(50);

 		/* Print IP address info */
		if (!prt_ip) {
			//if (netif.ip_addr) {
            {
				static char tmp_buff[16];
				printf("IP_ADDR    : %s\n", ipaddr_ntoa_r((const ip_addr_t *) &netif.ip_addr, tmp_buff, 16));
				printf("NET_MASK   : %s\n", ipaddr_ntoa_r((const ip_addr_t *) &netif.netmask, tmp_buff, 16));
				printf("GATEWAY_IP : %s\n", ipaddr_ntoa_r((const ip_addr_t *) &netif.gw, tmp_buff, 16));
				prt_ip = 1;
			}
		}
 	}
}
#else

#include "lwip_helper.h"
static void SetupIFTask(void  *pvParameters)
{
    lwip_network_init(WIFI_MODE_STA_ONLY);
    vTaskDelete(NULL);
}    
#endif
/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	MilliSecond delay function based on FreeRTOS
 * @param	ms	: Number of milliSeconds to delay
 * @return	Nothing
 * Needed for some functions, do not use prior to FreeRTOS running
 */
void msDelay(uint32_t ms)
{
	vTaskDelay((configTICK_RATE_HZ * ms) / 1000);
}


/**
 * @brief	main routine for example_lwip_tcpecho_freertos
 * @return	Function should not exit
 */
osThreadDef(setup_task, SetupIFTask, OS_TASK_PRIORITY_SETUP_IF, 1, OS_TASK_STACK_SIZE_SETUP_IF);

void lwip_task_create(void)
{
	/* Add another thread for initializing physical interface. This
	   is delayed from the main LWIP initialization. */
    //tid_SetupIFTask = osThreadCreate(osThread(setup_task), NULL);
}

#endif
