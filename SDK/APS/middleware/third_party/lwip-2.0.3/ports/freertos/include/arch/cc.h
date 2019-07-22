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
#ifndef __ARCH_CC_H__
#define __ARCH_CC_H__

#include <stdio.h>
#include <stdlib.h>

#include "sys_arch.h"

#define LWIP_PROVIDE_ERRNO

/* Define platform endianness (might already be defined) */
#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif /* BYTE_ORDER */

/* Define generic types used in lwIP */
typedef unsigned   char    u8_t;
typedef signed     char    s8_t;
typedef unsigned   short   u16_t;
typedef signed     short   s16_t;
typedef unsigned   long    u32_t;
typedef signed     long    s32_t;

typedef unsigned   long    mem_ptr_t;
typedef int sys_prot_t;

/* Define (sn)printf formatters for these lwIP types */
#define X8_F  "02x"
#define U16_F "u"
#define S16_F "d"
#define X16_F "x"
#define U32_F "u"
#define S32_F "d"
#define X32_F "x"
#define SZT_F "u"

/* Compiler hints for packing structures */
#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_STRUCT __attribute__((packed))
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END

/* Plaform specific diagnostic output */
#define LWIP_PLATFORM_DIAG(x) do {printf x;} while(0)
#define LWIP_PLATFORM_ASSERT(x) {printf("Assertion \"%s\" failed at line %d in %s\n", \
									x, __LINE__, __FILE__); assert_loop();}

#define LWIP_RAND() ((u32_t)rand())


#define LWIP_ROMBUILD
#if defined(LWIP_ROMBUILD)
    #define LWIP_ROMFN(_fn)  _fn##_impl
    #define LWIP_RETDATA     RET_DATA
#else
    #define LWIP_ROMFN(_fn)  _fn
    #define LWIP_RETDATA
#endif

/*
    memp_memory_RAW_PCB_base                 0x0040f968   Data         259  memp.o(.bss)
    memp_memory_UDP_PCB_base                 0x0040fa84   Data         371  memp.o(.bss)
    memp_memory_TCP_PCB_base                 0x0040fc10   Data         771  memp.o(.bss)
    memp_memory_TCP_PCB_LISTEN_base          0x0040ff2c   Data         515  memp.o(.bss)
    memp_memory_TCP_SEG_base                 0x00410148   Data         323  memp.o(.bss)
    memp_memory_REASSDATA_base               0x004102a4   Data         323  memp.o(.bss)
    memp_memory_NETBUF_base                  0x00410400   Data          67  memp.o(.bss)
    memp_memory_NETCONN_base                 0x0041045c   Data         483  memp.o(.bss)
    memp_memory_TCPIP_MSG_API_base           0x00410658   Data         259  memp.o(.bss)
    memp_memory_TCPIP_MSG_INPKT_base         0x00410774   Data         259  memp.o(.bss)
    memp_memory_ARP_QUEUE_base               0x00410890   Data         243  memp.o(.bss)
    memp_memory_IGMP_GROUP_base              0x0041099c   Data         131  memp.o(.bss)
    memp_memory_SYS_TIMEOUT_base             0x00410a38   Data         223  memp.o(.bss)
    memp_memory_NETDB_base                   0x00410b30   Data         323  memp.o(.bss)
    memp_memory_ND6_QUEUE_base               0x00410c8c   Data         163  memp.o(.bss)
    memp_memory_IP6_REASSDATA_base           0x00410d48   Data         203  memp.o(.bss)
    memp_memory_MLD6_GROUP_base              0x00410e2c   Data         115  memp.o(.bss)
    memp_memory_PBUF_base                    0x00410eb8   Data         259  memp.o(.bss)
    memp_memory_PBUF_POOL_base               0x00410fd4   Data       24515  memp.o(.bss)
*/

extern u8_t memp_memory_RAW_PCB_base[];
extern u8_t memp_memory_UDP_PCB_base[];
extern u8_t memp_memory_TCP_PCB_base[];
extern u8_t memp_memory_TCP_PCB_LISTEN_base[];
extern u8_t memp_memory_TCP_SEG_base[];
extern u8_t memp_memory_REASSDATA_base[];
extern u8_t memp_memory_NETBUF_base[];
extern u8_t memp_memory_NETCONN_base[];
extern u8_t memp_memory_TCPIP_MSG_API_base[];
extern u8_t memp_memory_TCPIP_MSG_INPKT_base[];
extern u8_t memp_memory_ARP_QUEUE_base[];
extern u8_t memp_memory_IGMP_GROUP_base[];
extern u8_t memp_memory_SYS_TIMEOUT_base[];
extern u8_t memp_memory_NETDB_base[];
extern u8_t memp_memory_ND6_QUEUE_base[];
extern u8_t memp_memory_IP6_REASSDATA_base[];
extern u8_t memp_memory_MLD6_GROUP_base[];
extern u8_t memp_memory_PBUF_base[];
//extern __attribute__((section("LWIP.RET_REGION"))) u8_t memp_memory_PBUF_POOL_base[];


#endif /* __ARCH_CC_H__ */
