/*
 * Copyright (c) 2014-2016 Alibaba Group. All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "iot_import.h"
#include "iotx_hal_internal.h"
#include "kvmgr.h"
#include "kv.h"

#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "lwip/netif.h"
#include "lwip_helper.h"
#include "network_config.h"


#define __DEMO__

#ifdef __DEMO__
    char _product_key[PRODUCT_KEY_LEN + 1];
    char _product_secret[PRODUCT_SECRET_LEN + 1];
    char _device_name[DEVICE_NAME_LEN + 1];
    char _device_secret[DEVICE_SECRET_LEN + 1];
#endif

typedef SemaphoreHandle_t os_sem_t;
typedef SemaphoreHandle_t os_mutex_t;
typedef TaskHandle_t os_thread_t;


typedef enum {
    TCPIP_ADAPTER_IF_STA = 0,     /**< station interface */
    TCPIP_ADAPTER_IF_AP,          /**< soft-AP interface */
    TCPIP_ADAPTER_IF_MAX
} net_adapter_if_t;

typedef struct {
    ip4_addr_t ip;
    ip4_addr_t netmask;
    ip4_addr_t gw;
} net_adapter_ip_info_t;

static int net_adapter_get_ip_info(net_adapter_if_t tcpip_if, net_adapter_ip_info_t *ip_info)
{
    struct netif *p_netif = NULL;
    lwip_tcpip_config_t tcpip_config = {{0}, {0}, {0}};

    if (tcpip_if >= TCPIP_ADAPTER_IF_MAX || ip_info == NULL) {
        return -1;
    }

    p_netif = netif_find("st1");

    if (p_netif != NULL && netif_is_up(p_netif)) {
        ip4_addr_set(&ip_info->ip, ip_2_ip4(&p_netif->ip_addr));
        ip4_addr_set(&ip_info->netmask, ip_2_ip4(&p_netif->netmask));
        ip4_addr_set(&ip_info->gw, ip_2_ip4(&p_netif->gw));

        return 0;
    }

    if (0 != tcpip_config_init(&tcpip_config)) {
        printf("hal_os tcpip config init fail");
        return -1;
    }

    ip4_addr_copy(ip_info->ip, tcpip_config.sta_ip);
    ip4_addr_copy(ip_info->gw, tcpip_config.sta_gw);
    ip4_addr_copy(ip_info->netmask, tcpip_config.sta_mask);

    return 0;
}


void *HAL_MutexCreate(void)
{
    os_mutex_t *p_mutex = NULL;
    p_mutex = (os_mutex_t *)malloc(sizeof(os_mutex_t));
    if(p_mutex == NULL) {
        return NULL;
    }

    *p_mutex = xSemaphoreCreateMutex();

    return p_mutex;
}

void HAL_MutexDestroy(_IN_ void *mutex)
{
    if (!mutex) {
        return;
    }

    vSemaphoreDelete(*((os_mutex_t*)mutex));
    free(mutex);
}

void HAL_MutexLock(_IN_ void *mutex)
{
	while( xSemaphoreTake(*((os_mutex_t*)mutex), portMAX_DELAY ) != pdPASS );
}

void HAL_MutexUnlock(_IN_ void *mutex)
{
  	xSemaphoreGive(*((os_mutex_t*)mutex));
}

void *HAL_Malloc(_IN_ uint32_t size)
{
    return malloc(size);
}

void* linkkit_realloc(void* ptr, size_t size)
{
	void* new_data = NULL;
    printf("relloc size=%d\n", size);
	if(size)
	{
		if(!ptr)
		{
			return malloc(size);
		}

		new_data = malloc(size);
		if(new_data)
		{
			memcpy(new_data, ptr, size); // TODO: unsafe copy...
			free(ptr); // we always move the data. free.
		}
	}

	return new_data;
}

void *HAL_Realloc(_IN_ void *ptr, _IN_ uint32_t size)
{
    return linkkit_realloc(ptr, size);
}

void *HAL_Calloc(_IN_ uint32_t nmemb, _IN_ uint32_t size)
{
    return calloc(nmemb, size);
}

void HAL_Free(_IN_ void *ptr)
{
    free(ptr);
}

uint64_t HAL_UptimeMs(void)
{
    uint64_t time_ms;
    time_ms = (uint64_t)osKernelSysTick();

    return time_ms;
}

char *HAL_GetTimeStr(_IN_ char *buf, _IN_ int len)
{
#if 0
    struct timeval tv;
    struct tm      tm;
    int str_len    = 0;

    if (buf == NULL || len < 28) {
        return NULL;
    }
    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &tm);
    strftime(buf, 28, "%m-%d %H:%M:%S", &tm);
    str_len = strlen(buf);
    if (str_len + 3 < len) {
        snprintf(buf + str_len, len, ".%3.3d", (int)(tv.tv_usec) / 1000);
    }
    return buf;
#endif
    return NULL;
}

void HAL_SleepMs(_IN_ uint32_t ms)
{
    if ((ms > 0) && (ms < portTICK_PERIOD_MS)) {
        ms = portTICK_PERIOD_MS;
    }

    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void HAL_Srandom(uint32_t seed)
{
    srand(seed);
}

uint32_t HAL_Random(uint32_t region)
{
    return (region > 0) ? (rand() % region) : 0;
}

int HAL_Snprintf(_IN_ char *str, const int len, const char *fmt, ...)
{
    va_list args;
    int     rc;

    va_start(args, fmt);
    rc = vsnprintf(str, len, fmt, args);
    va_end(args);

    return rc;
}

int HAL_Vsnprintf(_IN_ char *str, _IN_ const int len, _IN_ const char *format, va_list ap)
{
    return vsnprintf(str, len, format, ap);
}

void HAL_Printf(_IN_ const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    //fflush(stdout);
}

int HAL_GetPartnerID(char *pid_str)
{
    memset(pid_str, 0x0, PID_STRLEN_MAX);
#ifdef __DEMO__
    strcpy(pid_str, "opulinks");
#endif
    return strlen(pid_str);
}

int HAL_GetModuleID(char *mid_str)
{
    memset(mid_str, 0x0, MID_STRLEN_MAX);
#ifdef __DEMO__
    strcpy(mid_str, "OPL1000-DevKit");
#endif
    return strlen(mid_str);
}


char *HAL_GetChipID(_OU_ char *cid_str)
{
    memset(cid_str, 0x0, HAL_CID_LEN);
#ifdef __DEMO__
    strncpy(cid_str, "OPL1000", HAL_CID_LEN);
    cid_str[HAL_CID_LEN - 1] = '\0';
#endif
    return cid_str;
}


int HAL_GetDeviceID(_OU_ char *device_id)
{
    memset(device_id, 0x0, DEVICE_ID_LEN);
#ifdef __DEMO__
    HAL_Snprintf(device_id, DEVICE_ID_LEN, "%s.%s", _product_key, _device_name);
    device_id[DEVICE_ID_LEN - 1] = '\0';
#endif

    return strlen(device_id);
}

int HAL_SetProductKey(_IN_ char *product_key)
{
    int len = strlen(product_key);
#ifdef __DEMO__
    if (len > PRODUCT_KEY_LEN) {
        return -1;
    }
    memset(_product_key, 0x0, PRODUCT_KEY_LEN + 1);
    strncpy(_product_key, product_key, len);
#endif
    return len;
}


int HAL_SetDeviceName(_IN_ char *device_name)
{
    int len = strlen(device_name);
#ifdef __DEMO__
    if (len > DEVICE_NAME_LEN) {
        return -1;
    }
    memset(_device_name, 0x0, DEVICE_NAME_LEN + 1);
    strncpy(_device_name, device_name, len);
#endif
    return len;
}


int HAL_SetDeviceSecret(_IN_ char *device_secret)
{
    int len = strlen(device_secret);
#ifdef __DEMO__
    if (len > DEVICE_SECRET_LEN) {
        return -1;
    }
    memset(_device_secret, 0x0, DEVICE_SECRET_LEN + 1);
    strncpy(_device_secret, device_secret, len);
#endif
    return len;
}


int HAL_SetProductSecret(_IN_ char *product_secret)
{
    int len = strlen(product_secret);
#ifdef __DEMO__
    if (len > PRODUCT_SECRET_LEN) {
        return -1;
    }
    memset(_product_secret, 0x0, PRODUCT_SECRET_LEN + 1);
    strncpy(_product_secret, product_secret, len);
#endif
    return len;
}

int HAL_GetProductKey(_OU_ char *product_key)
{
    int len = strlen(_product_key);
    memset(product_key, 0x0, PRODUCT_KEY_LEN);

#ifdef __DEMO__
    strncpy(product_key, _product_key, len);
#endif

    return len;
}

int HAL_GetProductSecret(_OU_ char *product_secret)
{
    int len = strlen(_product_secret);
    memset(product_secret, 0x0, PRODUCT_SECRET_LEN);

#ifdef __DEMO__
    strncpy(product_secret, _product_secret, len);
#endif

    return len;
}

int HAL_GetDeviceName(_OU_ char *device_name)
{
    int len = strlen(_device_name);
    memset(device_name, 0x0, DEVICE_NAME_LEN);

#ifdef __DEMO__
    strncpy(device_name, _device_name, len);
#endif

    return strlen(device_name);
}

int HAL_GetDeviceSecret(_OU_ char *device_secret)
{
    int len = strlen(_device_secret);
    memset(device_secret, 0x0, DEVICE_SECRET_LEN);

#ifdef __DEMO__
    strncpy(device_secret, _device_secret, len);
#endif

    return len;
}

/*
 * This need to be same with app version as in uOTA module (ota_version.h)

    #ifndef SYSINFO_APP_VERSION
    #define SYSINFO_APP_VERSION "app-1.0.0-20180101.1000"
    #endif
 *
 */
int HAL_GetFirmwareVersion(_OU_ char *version)
{
    char *ver = "app-1.0.0-20180101.1000";
    int len = strlen(ver);
    memset(version, 0x0, FIRMWARE_VERSION_MAXLEN);
#ifdef __DEMO__
    strncpy(version, ver, len);
    version[len] = '\0';
#endif
    return strlen(version);
}

void *HAL_SemaphoreCreate(void)
{
    os_sem_t *p_sem = NULL;
    p_sem = (os_sem_t *)malloc(sizeof(os_sem_t));
    if (p_sem == NULL) {
        return NULL;
    }

  	vSemaphoreCreateBinary(*p_sem);

	if ((*p_sem) != NULL) {
       xSemaphoreTake(*p_sem, 1UL);
    }

    return p_sem;
}

void HAL_SemaphoreDestroy(_IN_ void *sem)
{
    vSemaphoreDelete(*((os_sem_t*)sem));
    free(sem);
}

void HAL_SemaphorePost(_IN_ void *sem)
{
    xSemaphoreGive(*((os_sem_t*)sem));
}

int HAL_SemaphoreWait(_IN_ void *sem, _IN_ uint32_t timeout_ms)
{
    //TickType_t xStartTime, xEndTime, xElapsed;
    long ulReturn;
    os_sem_t *p_sem = sem;

	//xStartTime = osKernelSysTick();

    if (timeout_ms == PLATFORM_WAIT_INFINITE) {
		while (xSemaphoreTake( *p_sem, portMAX_DELAY ) != pdTRUE );
		//xEndTime = osKernelSysTick();
		//xElapsed = ( xEndTime - xStartTime ) * portTICK_PERIOD_MS; //time blocked
		ulReturn = 0;
    } else {
		if (xSemaphoreTake( *p_sem, timeout_ms / portTICK_PERIOD_MS ) == pdTRUE) {
			//xEndTime = osKernelSysTick();
			//xElapsed = (xEndTime - xStartTime) * portTICK_PERIOD_MS; //time blocked
			ulReturn = 0;
		} else {
			ulReturn = -1;//timeout
		}
	}

	return ulReturn;
}

int HAL_ThreadCreate(
            _OU_ void **thread_handle,
            _IN_ void *(*work_routine)(void *),
            _IN_ void *arg,
            _IN_ hal_os_thread_param_t *hal_os_thread_param,
            _OU_ int *stack_used)
{
#if 0
    int ret = -1;

    if (stack_used) {
        *stack_used = 0;
    }

    ret = pthread_create((pthread_t *)thread_handle, NULL, work_routine, arg);

    return ret;
#endif
    return 0;
}

void HAL_ThreadDetach(_IN_ void *thread_handle)
{
    //pthread_detach((pthread_t)thread_handle);
}

void HAL_ThreadDelete(_IN_ void *thread_handle)
{
#if 0
    if (NULL == thread_handle) {
        pthread_exit(0);
    } else {
        /*main thread delete child thread*/
        pthread_cancel((pthread_t)thread_handle);
        pthread_join((pthread_t)thread_handle, 0);
    }
#endif
}

#if 0
static FILE *fp;

#define otafilename "/tmp/alinkota.bin"

void HAL_Firmware_Persistence_Start(void)
{
#ifdef __DEMO__
    fp = fopen(otafilename, "w");
    //    assert(fp);
#endif
    return;
}

int HAL_Firmware_Persistence_Write(_IN_ char *buffer, _IN_ uint32_t length)
{
#ifdef __DEMO__
    unsigned int written_len = 0;
    written_len = fwrite(buffer, 1, length, fp);

    if (written_len != length) {
        return -1;
    }
#endif
    return 0;
}

int HAL_Firmware_Persistence_Stop(void)
{
#ifdef __DEMO__
    if (fp != NULL) {
        fclose(fp);
    }
#endif

    /* check file md5, and burning it to flash ... finally reboot system */

    return 0;
}

int HAL_Config_Write(const char *buffer, int length)
{
#if 0
    FILE *fp;
    size_t written_len;
    char filepath[128] = {0};

    if (!buffer || length <= 0) {
        return -1;
    }

    snprintf(filepath, sizeof(filepath), "./%s", "alinkconf");
    fp = fopen(filepath, "w");
    if (!fp) {
        return -1;
    }

    written_len = fwrite(buffer, 1, length, fp);

    fclose(fp);

    return ((written_len != length) ? -1 : 0);
#endif
}

int HAL_Config_Read(char *buffer, int length)
{
#if 0
    FILE *fp;
    size_t read_len;
    char filepath[128] = {0};

    if (!buffer || length <= 0) {
        return -1;
    }

    snprintf(filepath, sizeof(filepath), "./%s", "alinkconf");
    fp = fopen(filepath, "r");
    if (!fp) {
        return -1;
    }

    read_len = fread(buffer, 1, length, fp);
    fclose(fp);

    return ((read_len != length) ? -1 : 0);
#endif
}

#define REBOOT_CMD "reboot"
void HAL_Sys_reboot(void)
{
    if (system(REBOOT_CMD)) {
        perror("HAL_Sys_reboot failed");
    }
}

#define ROUTER_INFO_PATH        "/proc/net/route"
#define ROUTER_RECORD_SIZE      256

char *_get_default_routing_ifname(char *ifname, int ifname_size)
{
    FILE *fp = NULL;
    char line[ROUTER_RECORD_SIZE] = {0};
    char iface[IFNAMSIZ] = {0};
    char *result = NULL;
    unsigned int destination, gateway, flags, mask;
    unsigned int refCnt, use, metric, mtu, window, irtt;

    fp = fopen(ROUTER_INFO_PATH, "r");
    if (fp == NULL) {
        perror("fopen");
        return result;
    }

    char *buff = fgets(line, sizeof(line), fp);
    if (buff == NULL) {
        perror("fgets");
        goto out;
    }

    while (fgets(line, sizeof(line), fp)) {
        if (11 !=
            sscanf(line, "%s %08x %08x %x %d %d %d %08x %d %d %d",
                   iface, &destination, &gateway, &flags, &refCnt, &use,
                   &metric, &mask, &mtu, &window, &irtt)) {
            perror("sscanf");
            continue;
        }

        /*default route */
        if ((destination == 0) && (mask == 0)) {
            strncpy(ifname, iface, ifname_size - 1);
            result = ifname;
            break;
        }
    }

out:
    if (fp) {
        fclose(fp);
    }

    return result;
}
#endif

uint32_t HAL_Wifi_Get_IP(char ip_str[NETWORK_ADDR_LEN], const char *ifname)
{

#if 0
    struct ifreq ifreq;
    int sock = -1;
    char ifname_buff[IFNAMSIZ] = {0};

    if ((NULL == ifname || strlen(ifname) == 0) &&
        NULL == (ifname = _get_default_routing_ifname(ifname_buff, sizeof(ifname_buff)))) {
        perror("get default routeing ifname");
        return -1;
    }

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    ifreq.ifr_addr.sa_family = AF_INET; //ipv4 address
    strncpy(ifreq.ifr_name, ifname, IFNAMSIZ - 1);

    if (ioctl(sock, SIOCGIFADDR, &ifreq) < 0) {
        close(sock);
        perror("ioctl");
        return -1;
    }

    close(sock);

    strncpy(ip_str,
            inet_ntoa(((struct sockaddr_in *)&ifreq.ifr_addr)->sin_addr),
            NETWORK_ADDR_LEN);

    return ((struct sockaddr_in *)&ifreq.ifr_addr)->sin_addr.s_addr;
#endif

    if (ip_str == NULL) {
        return 0;
    }

    net_adapter_ip_info_t info;
    net_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &info);
    memcpy(ip_str, inet_ntoa(info.ip.addr), NETWORK_ADDR_LEN);

    return info.ip.addr;
}

//static kv_file_t *kvfile = NULL;

int HAL_Kv_Set(const char *key, const void *val, int len, int sync)
{
#if 0
    if (!kvfile) {
        kvfile = kv_open("/tmp/kvfile.db");
        if (!kvfile) {
            return -1;
        }
    }

    return kv_set_blob(kvfile, (char *)key, (char *)val, len);
#endif
    return aos_kv_set(key, val, len, 1);
    //return -1;
}

int HAL_Kv_Get(const char *key, void *buffer, int *buffer_len)
{
#if 0
    if (!kvfile) {
        kvfile = kv_open("/tmp/kvfile.db");
        if (!kvfile) {
            return -1;
        }
    }

   return kv_get_blob(kvfile, (char *)key, buffer, buffer_len);
#endif
    printf("HAL_Kv_Get\r\n");
    return aos_kv_get(key, buffer, buffer_len);
    //return -1;

}

int HAL_Kv_Del(const char *key)
{
#if 0
    if (!kvfile) {
        kvfile = kv_open("/tmp/kvfile.db");
        if (!kvfile) {
            return -1;
        }
    }

    return kv_del(kvfile, (char *)key);
#endif
    return aos_kv_del(key);
    //return -1;
}

static uint32_t hal_get_1MHz_timer(void)
{
	return reg_read(0x40003044);
}

static void utc_clock_get(struct timeval *ptv)
{
	long long microsec = hal_get_1MHz_timer();
	ptv->tv_sec  = microsec / 1000000;
    ptv->tv_usec = microsec % 1000000;
}

static long long os_time_get(void)
{
#if 1
    struct timeval tv;
    long long ms;
    //gettimeofday(&tv, NULL);
    utc_clock_get(&tv);
    ms = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
    return ms;
#endif
}

static long long delta_time = 0;

void HAL_UTC_Set(long long ms)
{
    delta_time = ms - os_time_get();
}

long long HAL_UTC_Get(void)
{
    return delta_time + os_time_get();
}


void *HAL_Timer_Create(const char *name, void (*func)(void *), void *user_data)
{
    osTimerDef_t timer_def;
    osTimerId hal_timer;

    /* check parameter */
    if (func == NULL) {
        return NULL;
    }

    //create cmd timer
    timer_def.ptimer = func;
    hal_timer = osTimerCreate(&timer_def, osTimerOnce, NULL);
    printf("%s HAL_Timer_Create 0x%x\r\n\r\n", name, (uint32_t)(hal_timer) );

    return (void*)hal_timer;

#if 0
    timer_t *timer = NULL;

    struct sigevent ent;

    /* check parameter */
    if (func == NULL) {
        return NULL;
    }

    timer = (timer_t *)malloc(sizeof(time_t));

    /* Init */
    memset(&ent, 0x00, sizeof(struct sigevent));

    /* create a timer */
    ent.sigev_notify = SIGEV_THREAD;
    ent.sigev_notify_function = (void (*)(union sigval))func;
    ent.sigev_value.sival_ptr = user_data;

    printf("HAL_Timer_Create\n");

    if (timer_create(CLOCK_MONOTONIC, &ent, timer) != 0) {
        free(timer);
        return NULL;
    }

    return (void *)timer;
#endif
}

int HAL_Timer_Start(void *timer, int ms)
{
    osTimerStart(timer, ms);
    printf("HAL_Timer_Start 0x%x\n", (uint32_t)timer);

    return 0;
#if 0
    struct itimerspec ts;

    /* check parameter */
    if (timer == NULL) {
        return -1;
    }

    /* it_interval=0: timer run only once */
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;

    /* it_value=0: stop timer */
    ts.it_value.tv_sec = ms / 1000;
    ts.it_value.tv_nsec = (ms % 1000) * 1000;

    return timer_settime(*(timer_t *)timer, 0, &ts, NULL);
#endif
}

int HAL_Timer_Stop(void *timer)
{
    osTimerStop(timer);
    printf("HAL_Timer_Stop 0x%x\n", (uint32_t)timer);

    return 0;
#if 0
    struct itimerspec ts;

    /* check parameter */
    if (timer == NULL) {
        return -1;
    }

    /* it_interval=0: timer run only once */
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;

    /* it_value=0: stop timer */
    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = 0;

    return timer_settime(*(timer_t *)timer, 0, &ts, NULL);
#endif
}

int HAL_Timer_Delete(void *timer)
{
    osTimerDelete(timer);
    printf("HAL_Timer_Delete 0x%x\r\n", (uint32_t)timer);

    return 0;
#if 0
    int ret = 0;

    /* check parameter */
    if (timer == NULL) {
        return -1;
    }

    ret = timer_delete(*(timer_t *)timer);

    free(timer);

    return ret;
#endif
}

void HAL_Reboot(void)
{
#if 0
    reboot(0);
#endif
}

int HAL_GetNetifInfo(char *nif_str)
{
    memset(nif_str, 0x0, NIF_STRLEN_MAX);
#ifdef __DEMO__
    /* if the device have only WIFI, then list as follow, note that the len MUST NOT exceed NIF_STRLEN_MAX */
    const char *net_info = "WiFi|223344556676";
    strncpy(nif_str, net_info, strlen(net_info));
    /* if the device have ETH, WIFI, GSM connections, then list all of them as follow, note that the len MUST NOT exceed NIF_STRLEN_MAX */
    // const char *multi_net_info = "ETH|0123456789abcde|WiFi|03ACDEFF0032|Cellular|imei_0123456789abcde|iccid_0123456789abcdef01234|imsi_0123456789abcde|msisdn_86123456789ab");
    // strncpy(nif_str, multi_net_info, strlen(multi_net_info));
#endif
    return strlen(nif_str);
}

char *HAL_Wifi_Get_Mac(_OU_ char mac_str[HAL_MAC_LEN])
{
    uint8_t mac[6] = {0};

    if (mac_str == NULL) {
        return NULL;
    }

    strcpy(mac_str, "22:33:44:55:66:76");

    return mac_str;
}

int HAL_Awss_Get_Conn_Encrypt_Type()
{
    return 4;
}

