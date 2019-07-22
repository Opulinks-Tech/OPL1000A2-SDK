/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#ifndef __TCPECHO_H__
#define __TCPECHO_H__


typedef void (*tcp_echo_server_test_fp_t)(void *arg);
typedef void (*udp_echo_server_test_fp_t)(void *arg);
typedef int  (*tcp_client_test_fp_t)(void *arg);
typedef int  (*udp_client_test_fp_t)(void *arg);
typedef void (*tcp_echo_server_thread_fp_t)(void *arg);
typedef void (*udp_echo_server_thread_fp_t)(void *arg);
typedef void (*client_test_thread_fp_t)(void *arg);

typedef void (*tcp_server_thread_fp_t)(void *arg);
typedef void (*tcp_client_thread_fp_t)(void *arg);

typedef void (*tcp_run_client_fp_t)(const char *ipaddr, uint16_t port);
typedef void (*tcp_run_server_fp_t)(uint16_t port);
typedef void (*socket_test_entry_fp_t)(char *param[]);
typedef int  (*socket_server_demo_fp_t)(void);

extern tcp_echo_server_test_fp_t       tcp_echo_server_test;
extern udp_echo_server_test_fp_t       udp_echo_server_test;
extern tcp_client_test_fp_t            tcp_client_test;
extern udp_client_test_fp_t            udp_client_test;
extern tcp_echo_server_thread_fp_t     tcp_echo_server_thread;
extern udp_echo_server_thread_fp_t     udp_echo_server_thread;
extern client_test_thread_fp_t         client_test_thread;

extern tcp_server_thread_fp_t          tcp_server_thread;
extern tcp_client_thread_fp_t          tcp_client_thread;

extern tcp_run_client_fp_t             tcp_run_client;
extern tcp_run_server_fp_t             tcp_run_server;
extern socket_test_entry_fp_t          socket_test_entry;
extern socket_server_demo_fp_t         socket_server_demo;



#endif /* __TCPECHO_H__ */
