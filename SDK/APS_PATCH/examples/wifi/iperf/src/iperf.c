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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"
#include "sockets.h"
#include "iperf.h"
#include "opulinks_log.h"
#include "ftoa_util.h"
#include "strerror_util.h"
#include "driver_netlink.h"

static const char *TAG="iperf";

extern int dbg_mode;

osThreadId iperf_client_task_id = NULL;
osThreadId iperf_server_task_id = NULL;
osThreadId iperf_report_task_id = NULL;

static uint16_t cfg_mode       = 0x00;
static iperf_cfg_t s_iperf_ctrl;
static iperf_ctrl_t iperf_client_ctrl;
static iperf_ctrl_t iperf_server_ctrl;

#define IS_DUAL_TEST        ((cfg_mode & IPERF_FLAG_DUAL) != 0)
#define IS_CLIENT           ((cfg_mode & IPERF_FLAG_CLIENT) != 0)
#define IS_CLIENT_DUAL_TEST ((cfg_mode & IPERF_FLAG_DUAL) != 0 && IS_CLIENT)
#define IS_TCP_CLIENT       ((cfg_mode & IPERF_FLAG_CLIENT) != 0 && (cfg_mode & IPERF_FLAG_TCP) != 0)
#define IS_TCP_SERVER       ((cfg_mode & IPERF_FLAG_SERVER) != 0 && (cfg_mode & IPERF_FLAG_TCP) != 0)
#define IS_UDP_CLIENT       ((cfg_mode & IPERF_FLAG_CLIENT) != 0 && (cfg_mode & IPERF_FLAG_UDP) != 0)
#define IS_UDP_SERVER       ((cfg_mode & IPERF_FLAG_SERVER) != 0 && (cfg_mode & IPERF_FLAG_UDP) != 0)
#define IS_AMOUNT           (s_iperf_ctrl.mAmount != 0)
#define IS_ABORT            ((cfg_mode & IPERF_FLAG_ABORT) != 0)

static int iperf_task_create(char *name, uint32_t size, uint32_t pri, os_pthread fn, osThreadId *thread);

static int iperf_show_socket_error_reason(const char *str, int sockfd)
{
    if (errno != 0) {
        LOGE(TAG, "%s error, error code: %d, reason: %s", str, errno, util_strerr(errno));
    }

    return errno;
}

void iperf_report_task(void* arg)
{
    iperf_ctrl_t *p_ctrl;
    TickType_t delay_interval;
    uint32_t interval;
    uint32_t time;
    uint32_t cur = 0;
    char bw_string[16] = {0};
    char tran_string[16] = {0};
    char bidr_str[10] = {0};
    int8_t rssi;
    
    delay_interval = (s_iperf_ctrl.interval * 1000);
    interval = s_iperf_ctrl.interval;
    time = s_iperf_ctrl.time;
    
    if (dbg_mode == 0) {
        printf("\n%12s      %s        %s\n", "Interval", "Transfer", "Bandwidth");
    }
    else {
        printf("\n%12s      %s        %s          %s\n", "Interval", "Transfer", "Bandwidth", "RSSI");
    }

    if (IS_CLIENT) {
        p_ctrl = &iperf_client_ctrl;
        strcpy(bidr_str, "Sender");
    } else {
        p_ctrl = &iperf_server_ctrl;
        strcpy(bidr_str, "Receiver");
    }

    while(1) {
        vTaskDelay(delay_interval);
        
        if (dbg_mode == 1) {
            rssi = wpa_driver_netlink_get_rssi();
        }
        
        if (IS_DUAL_TEST) {
            if (dbg_mode == 0) {
                printf("%4d-%4d sec     %s MBytes    %s Mbits/sec    %s\n", cur, cur+interval,
                        ftoa((double)((iperf_client_ctrl.total_len - iperf_client_ctrl.last_len))/1048576, tran_string, 3),
                        ftoa((double)((iperf_client_ctrl.total_len - iperf_client_ctrl.last_len))/1000000/interval*8, bw_string, 3),
                        "Sender");
            }
            else {
                printf("%4d-%4d sec     %s MBytes    %s Mbits/sec    %s    %d\n", cur, cur+interval,
                        ftoa((double)((iperf_client_ctrl.total_len - iperf_client_ctrl.last_len))/1048576, tran_string, 3),
                        ftoa((double)((iperf_client_ctrl.total_len - iperf_client_ctrl.last_len))/1000000/interval*8, bw_string, 3),
                        "Sender",
                        rssi);
            }
            
            iperf_client_ctrl.last_len = iperf_client_ctrl.total_len;

            if (dbg_mode == 0) {
                printf("%4d-%4d sec     %s MBytes    %s Mbits/sec    %s\n", cur, cur+interval, 
                        ftoa((double)((iperf_server_ctrl.total_len - iperf_server_ctrl.last_len))/1048576, tran_string, 3),
                        ftoa((double)((iperf_server_ctrl.total_len - iperf_server_ctrl.last_len))/1000000/interval*8, bw_string, 3),
                        "Receiver"); 
            }
            else {
                printf("%4d-%4d sec     %s MBytes    %s Mbits/sec    %s    %d\n", cur, cur+interval, 
                        ftoa((double)((iperf_server_ctrl.total_len - iperf_server_ctrl.last_len))/1048576, tran_string, 3),
                        ftoa((double)((iperf_server_ctrl.total_len - iperf_server_ctrl.last_len))/1000000/interval*8, bw_string, 3),
                        "Receiver",
                        rssi);            
            }

            iperf_server_ctrl.last_len = iperf_server_ctrl.total_len;

        } else {
            if (dbg_mode == 0) {
                printf("%4d-%4d sec     %s MBytes    %s Mbits/sec\n", cur, cur+interval,
                        ftoa((double)((p_ctrl->total_len - p_ctrl->last_len))/1048576, tran_string, 3),
                        ftoa((double)((p_ctrl->total_len - p_ctrl->last_len))/1000000/interval*8, bw_string, 3)); 
            }
            else {
                printf("%4d-%4d sec     %s MBytes    %s Mbits/sec    %d\n", cur, cur+interval,
                        ftoa((double)((p_ctrl->total_len - p_ctrl->last_len))/1048576, tran_string, 3),
                        ftoa((double)((p_ctrl->total_len - p_ctrl->last_len))/1000000/interval*8, bw_string, 3), rssi); 
            }

           p_ctrl->last_len = p_ctrl->total_len;
        }
        
        cur += interval;
        
        //Timer base
        if (!IS_AMOUNT) {
            if (cur >= time) {
                iperf_client_ctrl.finish = true;
                iperf_server_ctrl.finish = true;
                break;
            }
        }
        else { //Amount base
            if (IS_DUAL_TEST) { 
                if (iperf_client_ctrl.finish && iperf_server_ctrl.finish) {
                    break;
                }
            }
            else {
                if (p_ctrl->finish) {
                    break;
                }
            }
        }
        
        //Abort
        if (IS_ABORT) {
            break; 
        }
    }
   
    if (IS_DUAL_TEST) {
        printf("%4d-%4d sec     %s MBytes    %s Mbits/sec    %s\n", 0, cur,
                ftoa((double)(iperf_client_ctrl.total_len)/1048576, tran_string, 3),
                ftoa((double)(iperf_client_ctrl.total_len)/1000000/cur*8, bw_string, 3),
                "Sender total");

        printf("%4d-%4d sec     %s MBytes    %s Mbits/sec    %s\n", 0, cur,
                ftoa((double)(iperf_server_ctrl.total_len)/1048576, tran_string, 3),
                ftoa((double)(iperf_server_ctrl.total_len)/1000000/cur*8, bw_string, 3),
                "Recever total");
    } else {
        printf("%4d-%4d sec     %s MBytes    %s Mbits/sec\n", 0, cur, 
                ftoa((double)(p_ctrl->total_len)/1048576, tran_string, 3),
                ftoa((double)(p_ctrl->total_len)/1000000/cur*8, bw_string, 3));
    }
    
    LOGI(TAG, "iperf report exit");
    iperf_report_task_id = NULL;
    vTaskDelete(NULL);
}

static int iperf_report_task_create(void)
{
    osThreadDef_t task_def;
    
    task_def.name = IPERF_REPORT_TASK_NAME;
    task_def.stacksize = IPERF_REPORT_TASK_STACK;
    task_def.tpriority = (osPriority)IPERF_REPORT_TASK_PRIORITY;
    task_def.pthread = iperf_report_task;
    iperf_report_task_id = osThreadCreate(&task_def, (void*)NULL);
    if(iperf_report_task_id == NULL)
    {
        LOGE_DRCT(TAG, IPERF_REPORT_TASK_NAME"Task create fail \r\n");
        return -1;
    }
    else
    {
        LOGI(TAG, IPERF_REPORT_TASK_NAME"Task create successful");
    }
    
    return 0;
}

static int iperf_start_report(void)
{
    int ret;
    
    ret = iperf_report_task_create();

    if (ret != 0)  {
        LOGE(TAG, "create task %s failed", IPERF_REPORT_TASK_NAME);
        return -1;
    }
 
    return 0;
}

static int client_init_to_server(int sockfd, iperf_client_hdr_t * udp_buf)
{
    int actual_send;
    char buffer[sizeof(iperf_client_hdr_t)] = {0};
    iperf_client_hdr_t *client_hdr;

    if (IS_TCP_CLIENT) 
        client_hdr = (iperf_client_hdr_t *)buffer;
    else 
        client_hdr = udp_buf;

#if 1 //Iperf 2.0.10
    client_hdr->base.flags = htonl(HEADER_VERSION1);
    client_hdr->base.flags |= htonl(1);
    client_hdr->base.numThreads = htonl(1);
    if (IS_AMOUNT) {
        client_hdr->base.mAmount = htonl((long)(s_iperf_ctrl.mAmount * 1.2));
        client_hdr->base.mAmount &= htonl( 0x7FFFFFFF );
    } else {
        client_hdr->base.mAmount = htonl(-(long)(s_iperf_ctrl.time * 100));
    }
    client_hdr->base.mPort = htonl(5001);
#else //Iperf 2.0.9
    client_hdr->flags = htonl(HEADER_VERSION1);
    client_hdr->flags |= htonl(1);
    client_hdr->numThreads = htonl(1);
    client_hdr->mAmount = htonl((int)(s_iperf_ctrl.time * 100));
    client_hdr->mPort = htonl(5001);
#endif

    if (IS_TCP_CLIENT) {
        actual_send = send(sockfd, buffer, sizeof(iperf_client_hdr_t), 0);
        if (actual_send <= 0) {
            iperf_show_socket_error_reason("tcp client init send", sockfd);
            return -1;
        }
    } 
    else {
#if 1 //Iperf 2.0.10
        if (!IS_AMOUNT) {
            client_hdr->base.flags |= htonl(HEADER_EXTEND);
            client_hdr->extend.mRate = htonl(IPERF_UDP_TX_MAX_RATE_DEF);
        }
#else //Iperf 2.0.9
        client_hdr->mRate = htonl(IPERF_UDP_TX_MAX_RATE_DEF);
#endif
    }

    return 0;
}

static int malloc_service_buffer(iperf_ctrl_t *p_ctrl, uint32_t len)
{
    if (p_ctrl == NULL) {
        LOGE(TAG, "p_ctrl is null.");
        return -1;
    }
    
    p_ctrl->buffer_len = len;

    p_ctrl->buffer = (uint8_t *)malloc(p_ctrl->buffer_len);
    if (!p_ctrl->buffer) {
        LOGE(TAG, "create buffer: out of memory");
        return -1;
    }
    memset(p_ctrl->buffer, 0, p_ctrl->buffer_len);

    return 0;
}

static int check_dual_test_by_client(iperf_client_hdr_t *client_hdr, struct sockaddr_in *addr)
{
#if 1 //Iperf 2.0.10
    if ((ntohl(client_hdr->base.flags) & HEADER_VERSION1) != 0) {
        cfg_mode |= IPERF_FLAG_DUAL;
        s_iperf_ctrl.dip = addr->sin_addr.s_addr;
        s_iperf_ctrl.dport = ntohs(client_hdr->base.mPort>>16);
#else //Iperf 2.0.9
    if ((ntohl(client_hdr->flags) & HEADER_VERSION1) != 0) {
        cfg_mode |= IPERF_FLAG_DUAL;
        s_iperf_ctrl.dip = addr->sin_addr.s_addr;
        s_iperf_ctrl.dport = ntohs(client_hdr->mPort>>16);
#endif
        if (IS_UDP_SERVER) {
            iperf_task_create(IPERF_CLIENT_TASK_NAME, IPERF_CLIENT_TASK_STACK, 
                                IPERF_CLIENT_TASK_PRIORITY, iperf_run_udp_client, &iperf_client_task_id);
        }
        else {
            iperf_task_create(IPERF_CLIENT_TASK_NAME, IPERF_CLIENT_TASK_STACK, 
                                IPERF_CLIENT_TASK_PRIORITY, iperf_run_tcp_client, &iperf_client_task_id);
        }
        
        return true;
    }
    return false;
}

void iperf_run_udp_server(void *arg)
{
    socklen_t addr_len = sizeof(struct sockaddr_in);
    struct sockaddr_in addr;
    int actual_recv = 0;
    struct timeval t;
    int want_recv = 0;
    uint8_t *buffer;
    int sockfd;
    int opt;
    int is_dual = false;
    int report_created = false;
    int ret;
    fd_set read_fd;
    
    malloc_service_buffer(&iperf_server_ctrl, IPERF_UDP_RX_LEN);
    
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        iperf_show_socket_error_reason("udp server create", sockfd);
        goto done;
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(s_iperf_ctrl.sport);
    addr.sin_addr.s_addr = s_iperf_ctrl.sip;
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        iperf_show_socket_error_reason("udp server bind", sockfd);
        goto done;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(s_iperf_ctrl.sport);
    addr.sin_addr.s_addr = s_iperf_ctrl.sip;

    buffer = iperf_server_ctrl.buffer;
    want_recv = iperf_server_ctrl.buffer_len;
    LOGI(TAG, "want recv=%d", want_recv);
    
    if (dbg_mode == 1 && !report_created) {
        iperf_start_report();
        report_created = true;
    }
    
    while (1) {
        FD_ZERO(&read_fd);
        FD_SET(sockfd, &read_fd);
        t.tv_sec = 0;
        t.tv_usec = 500000;
        
        ret = select(sockfd + 1, &read_fd, NULL, NULL, &t);
        if (ret < 0) {
            goto done;
        }
        
        if (iperf_server_ctrl.finish) { //abort
            goto done;
        }
        
        if (FD_ISSET(sockfd, &read_fd)) { 
            break;
        }
    }
    
    while (!iperf_server_ctrl.finish) {
        actual_recv = recvfrom(sockfd, buffer, want_recv, 0, (struct sockaddr *)&addr, &addr_len);
 
        if (actual_recv < 0) {
            iperf_show_socket_error_reason("udp server recv", sockfd);
            goto done;
        } else {
            if (!report_created) {
                if (!IS_CLIENT_DUAL_TEST)
                    iperf_start_report();
                
                t.tv_sec = IPERF_SOCKET_RX_TIMEOUT;
                t.tv_usec = 0;
                setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t));
                report_created = true;
            }

            if (!is_dual) {
                iperf_udp_pkt_t *udp = (iperf_udp_pkt_t *)buffer;
                is_dual = check_dual_test_by_client((iperf_client_hdr_t *)(udp+1), &addr);
            }

            iperf_server_ctrl.total_len += actual_recv;
            if (IS_AMOUNT) {
                if (iperf_server_ctrl.total_len >= s_iperf_ctrl.mAmount) {
                    goto done;
                }
            }
        }
    }

done:
    FD_CLR(sockfd, &read_fd);
    close(sockfd);
    iperf_server_ctrl.finish = true;
    
    if (iperf_server_ctrl.buffer) {
        free(iperf_server_ctrl.buffer);
        iperf_server_ctrl.buffer = 0;
    }

    LOGI(TAG, "iperf server exit");
    iperf_server_task_id = NULL;
    vTaskDelete(NULL);
}

void iperf_run_udp_client(void *arg)
{
    struct sockaddr_in addr;
    iperf_udp_pkt_t *udp;
    int actual_send = 0;
    bool retry = false;
    uint32_t delay = 1;
    int want_send = 0;
    uint8_t *buffer;
    int sockfd;
    int opt;
    int id;
    
    malloc_service_buffer(&iperf_client_ctrl, IPERF_UDP_TX_LEN);
    
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        iperf_show_socket_error_reason("udp server create", sockfd);
        goto done;
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = s_iperf_ctrl.sip;
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        iperf_show_socket_error_reason("udp server bind", sockfd);
        goto done;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(s_iperf_ctrl.dport);
    addr.sin_addr.s_addr = s_iperf_ctrl.dip;

    buffer = iperf_client_ctrl.buffer;
    udp = (iperf_udp_pkt_t *)buffer;
    want_send = iperf_client_ctrl.buffer_len;
    id = 0;
    iperf_start_report();

    if (IS_CLIENT_DUAL_TEST) {
        client_init_to_server(NULL, (iperf_client_hdr_t *)(udp+1));
    }

    while (!iperf_client_ctrl.finish) {
        if (false == retry) {
            id ++;
            udp->id = htonl(id);
            delay = 1;
        }
        
        retry = false;
        actual_send = sendto(sockfd, buffer, want_send, 0, (struct sockaddr *)&addr, sizeof(addr));

        if (actual_send != want_send) {
            if (errno == ENOMEM) {
                LOGI(TAG, "socket queue full, waiting %d ...", delay);
                vTaskDelay(delay);
                if (delay < IPERF_MAX_DELAY) {
                    delay <<= 1;
                }
                retry = true;
                continue;
            } else {
                iperf_show_socket_error_reason("udp client send abort", sockfd);
                goto done;
            }
        } else {
            iperf_client_ctrl.total_len += actual_send;
            if (IS_AMOUNT) {
                if (iperf_client_ctrl.total_len >= s_iperf_ctrl.mAmount) {
                    goto done;
                }
            }
        }
    }

done:
    close(sockfd);
    iperf_client_ctrl.finish = true;
    
    if (iperf_client_ctrl.buffer) {
        free(iperf_client_ctrl.buffer);
        iperf_client_ctrl.buffer = 0;
    }

    LOGI(TAG, "iperf client exit");
    iperf_client_task_id = NULL;
    vTaskDelete(NULL);
}

void iperf_run_tcp_server(void *arg)
{
    socklen_t addr_len = sizeof(struct sockaddr);
    struct sockaddr_in remote_addr;
    struct sockaddr_in addr;
    int actual_recv = 0;
    int want_recv = 0;
    uint8_t *buffer;
    int listen_socket;
    struct timeval t;
    int sockfd;
    int opt;
    int is_dual = false;
    int ret;
    fd_set read_fd;
    
    malloc_service_buffer(&iperf_server_ctrl, IPERF_TCP_RX_LEN);

    listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket < 0) {
        iperf_show_socket_error_reason("tcp server create", listen_socket);
        goto done;
    }

    setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(s_iperf_ctrl.sport);
    addr.sin_addr.s_addr = s_iperf_ctrl.sip;
    if (bind(listen_socket, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        iperf_show_socket_error_reason("tcp server bind", listen_socket);
        goto done;
    }
    
    if (listen(listen_socket, 5) < 0) {
        iperf_show_socket_error_reason("tcp server listen", listen_socket);
        goto done;
    }
            
    while (1) {
        FD_ZERO(&read_fd);
        FD_SET(listen_socket, &read_fd);
        t.tv_sec = 0;
        t.tv_usec = 500000;
        
        ret = select(listen_socket + 1, &read_fd, NULL, NULL, &t);
        if (ret < 0) {
            goto done;
        }
        
        if (iperf_server_ctrl.finish) { //abort
            goto done;
        }

        if (FD_ISSET(listen_socket, &read_fd)) { 
            break;
        }
    }

    buffer = iperf_server_ctrl.buffer;
    want_recv = iperf_server_ctrl.buffer_len;

    while (!iperf_server_ctrl.finish) {
        sockfd = accept(listen_socket, (struct sockaddr*)&remote_addr, &addr_len);
        if (sockfd < 0) {
            iperf_show_socket_error_reason("tcp server listen", listen_socket);
            goto done;
        } else {
            LOGI(TAG, "server accept: %s,%d\n", inet_ntoa(remote_addr.sin_addr), htons(remote_addr.sin_port));

            t.tv_sec = IPERF_SOCKET_RX_TIMEOUT;
            t.tv_usec = 0;
            setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t));
            
            if (!IS_CLIENT_DUAL_TEST)
                iperf_start_report();
        }

        while (!iperf_server_ctrl.finish) {
            actual_recv = recv(sockfd, buffer, want_recv, 0);
            if (actual_recv < 0) {
                iperf_show_socket_error_reason("tcp server recv", listen_socket);
                goto done;
            } else {
                if (!is_dual) {
                    is_dual = check_dual_test_by_client((iperf_client_hdr_t *)buffer, &remote_addr);
                }
                iperf_server_ctrl.total_len += actual_recv;
                if (IS_AMOUNT) {
                    if (iperf_server_ctrl.total_len >= s_iperf_ctrl.mAmount) {
                        goto done;
                    }
                }
            }
        }
        
        close(sockfd);
    }

done:
    FD_CLR(listen_socket, &read_fd);
    close(sockfd);
    close(listen_socket);
    iperf_server_ctrl.finish = true;
    
    if (iperf_server_ctrl.buffer) {
        free(iperf_server_ctrl.buffer);
        iperf_server_ctrl.buffer = 0;
    }

    LOGI(TAG, "iperf server exit");
    iperf_server_task_id = NULL;
    vTaskDelete(NULL);
}


void iperf_run_tcp_client(void *arg)
{
    struct sockaddr_in local_addr;
    struct sockaddr_in remote_addr;
    static int actual_send = 0;
    static int want_send = 0;
    uint8_t *buffer;
    int sockfd;
    int opt;
    
    malloc_service_buffer(&iperf_client_ctrl, IPERF_TCP_TX_LEN);

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        iperf_show_socket_error_reason("tcp client create", sockfd);
        goto done;
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = 0;
    local_addr.sin_addr.s_addr = s_iperf_ctrl.sip;
    if (bind(sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr)) != 0) {
        iperf_show_socket_error_reason("tcp client bind", sockfd);
        goto done;
    }

    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(s_iperf_ctrl.dport);
    remote_addr.sin_addr.s_addr = s_iperf_ctrl.dip;
    if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(remote_addr)) < 0) {
        iperf_show_socket_error_reason("tcp client connect", sockfd);
        goto done;
    } else {
        printf("client connecting to %s, %d\n", inet_ntoa(remote_addr.sin_addr), htons(remote_addr.sin_port));
    }

    if (IS_CLIENT_DUAL_TEST) {
        client_init_to_server(sockfd, NULL);
    }
    
    buffer = iperf_client_ctrl.buffer;
    want_send = iperf_client_ctrl.buffer_len;
   
    iperf_start_report();

    while (!iperf_client_ctrl.finish) {
        actual_send = send(sockfd, buffer, want_send, 0);
        if (actual_send <= 0) {
            iperf_show_socket_error_reason("tcp client send", sockfd);
            goto done;
        } else {
            iperf_client_ctrl.total_len += actual_send;
            if (IS_AMOUNT) {
                if (iperf_client_ctrl.total_len >= s_iperf_ctrl.mAmount) {
                    goto done;
                }
            }
        }
    }

done:
    close(sockfd);
    iperf_client_ctrl.finish = true;
    
    if (iperf_client_ctrl.buffer) {
        free(iperf_client_ctrl.buffer);
        iperf_client_ctrl.buffer = 0;
    }

    LOGI(TAG, "iperf client exit");
    iperf_client_task_id = NULL;
    vTaskDelete(NULL);
}

static int iperf_task_create(char *name, uint32_t size, uint32_t pri, os_pthread fn, osThreadId *thread)
{
    osThreadDef_t task_def = {0};
    
    task_def.name = name;
    task_def.stacksize = size;
    task_def.tpriority = (osPriority)pri;
    task_def.pthread = fn;
    
    *thread = osThreadCreate(&task_def, (void*)NULL);
    if(*thread == NULL) {
        LOGE(TAG, "%s Task create fail \r\n", name);
        return -1;
    }
    else {
        LOGI(TAG, "%s Task create successful", name);
    }

    return 0;
}

static int tcp_client_ft(void)
{
    iperf_server_ctrl.finish = true;
    iperf_task_create(IPERF_CLIENT_TASK_NAME, IPERF_CLIENT_TASK_STACK, 
                        IPERF_CLIENT_TASK_PRIORITY, iperf_run_tcp_client, &iperf_client_task_id);
    return 0;
}

static int tcp_server_ft(void)
{
    iperf_client_ctrl.finish = true;
    iperf_task_create(IPERF_SERVER_TASK_NAME, IPERF_SERVER_TASK_STACK, 
                        IPERF_SERVER_TASK_PRIORITY, iperf_run_tcp_server, &iperf_server_task_id);
    return 0;
}

static int tcp_client_dual_ft(void)
{
    iperf_task_create(IPERF_CLIENT_TASK_NAME, IPERF_CLIENT_TASK_STACK, 
                        IPERF_CLIENT_TASK_PRIORITY, iperf_run_tcp_client, &iperf_client_task_id);
    iperf_task_create(IPERF_SERVER_TASK_NAME, IPERF_SERVER_TASK_STACK, 
                        IPERF_SERVER_TASK_PRIORITY, iperf_run_tcp_server, &iperf_server_task_id);    
    return 0;
}

static int udp_client_dual_ft(void)
{
    iperf_task_create(IPERF_CLIENT_TASK_NAME, IPERF_CLIENT_TASK_STACK, 
                        IPERF_CLIENT_TASK_PRIORITY, iperf_run_udp_client, &iperf_client_task_id);
    iperf_task_create(IPERF_SERVER_TASK_NAME, IPERF_SERVER_TASK_STACK, 
                        IPERF_SERVER_TASK_PRIORITY, iperf_run_udp_server, &iperf_server_task_id);
    return 0;
}

static int udp_client_ft(void)
{
    iperf_server_ctrl.finish = true;
    iperf_task_create(IPERF_CLIENT_TASK_NAME, IPERF_CLIENT_TASK_STACK, 
                        IPERF_CLIENT_TASK_PRIORITY, iperf_run_udp_client, &iperf_client_task_id);
    return 0;
}

static int udp_server_ft(void)
{
    iperf_client_ctrl.finish = true;
    iperf_task_create(IPERF_SERVER_TASK_NAME, IPERF_SERVER_TASK_STACK, 
                        IPERF_SERVER_TASK_PRIORITY, iperf_run_udp_server, &iperf_server_task_id);

    return 0;
}

static iperf_func_fp_t iperf_fun_table[] =
{
    {IPERF_TCP_CLIENT_FT_IDX,      tcp_client_ft},
    {IPERF_TCP_SERVER_FT_IDX,      tcp_server_ft},
    {IPERF_TCP_CLIENT_DUAL_FT_IDX, tcp_client_dual_ft},
    {IPERF_UDP_CLIENT_DUAL_FT_IDX, udp_client_dual_ft},
    {IPERF_UDP_CLIENT_FT_IDX,      udp_client_ft},
    {IPERF_UDP_SERVER_FT_IDX,      udp_server_ft},
    {NULL, NULL},
};

int iperf_start(iperf_cfg_t *cfg)
{
    iperf_func_fp_t *p_ft = NULL;
    
    if (!cfg) {
        return -1;
    }

    if (iperf_client_task_id || iperf_server_task_id) {
        LOGW_DRCT(TAG, "iperf is running");
        return -1;
    }
    
    cfg_mode = cfg->flag;

    memcpy(&s_iperf_ctrl, cfg, sizeof(iperf_cfg_t));
    memset(&iperf_client_ctrl, 0, sizeof(iperf_ctrl_t));
    memset(&iperf_server_ctrl, 0, sizeof(iperf_ctrl_t));

    for (p_ft=&iperf_fun_table[0]; p_ft->idx; p_ft++) {
        if (p_ft->idx == cfg_mode) {
            p_ft->func();
            break;
        }
    }

    return 0;
}

int iperf_stop(void)
{
    iperf_client_ctrl.finish = true;
    iperf_server_ctrl.finish = true;

    cfg_mode |= IPERF_FLAG_ABORT;
    
    printf("\n");
    LOGI_DRCT(TAG, "Aborted iperf");

    return 0;
}
