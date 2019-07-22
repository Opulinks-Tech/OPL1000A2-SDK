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
#ifndef __ARCH_SYS_ARCH_IF_H__
#define __ARCH_SYS_ARCH_IF_H__

#include "lwip/opt.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/* Mutex functions: */
typedef err_t (*sys_mutex_new_fp_t)(sys_mutex_t *mutex);
typedef void  (*sys_mutex_lock_fp_t)(sys_mutex_t *mutex);
typedef void  (*sys_mutex_unlock_fp_t)(sys_mutex_t *mutex);
typedef void  (*sys_mutex_free_fp_t)(sys_mutex_t *mutex);
#ifndef sys_mutex_valid
typedef int   (*sys_mutex_valid_fp_t)(sys_mutex_t *mutex);
#endif
#ifndef sys_mutex_set_invalid
typedef void  (*sys_mutex_set_invalid_fp_t)(sys_mutex_t *mutex);
#endif

extern sys_mutex_new_fp_t      sys_mutex_new_adpt;
extern sys_mutex_lock_fp_t     sys_mutex_lock_adpt;
extern sys_mutex_unlock_fp_t   sys_mutex_unlock_adpt;
extern sys_mutex_free_fp_t     sys_mutex_free_adpt;


/* Semaphore functions: */
typedef err_t (*sys_sem_new_fp_t)(sys_sem_t *sem, u8_t count);
typedef void  (*sys_sem_signal_fp_t)(sys_sem_t *sem);
typedef u32_t (*sys_arch_sem_wait_fp_t)(sys_sem_t *sem, u32_t timeout);
typedef void  (*sys_sem_free_fp_t)(sys_sem_t *sem);

extern sys_sem_new_fp_t         sys_sem_new_adpt;
extern sys_sem_signal_fp_t      sys_sem_signal_adpt;
extern sys_arch_sem_wait_fp_t   sys_arch_sem_wait_adpt;
extern sys_sem_free_fp_t        sys_sem_free_adpt;

#ifndef sys_sem_valid
int   sys_sem_valid(sys_sem_t *sem);
#endif
#ifndef sys_sem_set_invalid
void  sys_sem_set_invalid(sys_sem_t *sem);
#endif


#ifndef sys_msleep
typedef void  (*sys_msleep_fp_t)(u32_t ms);
extern sys_msleep_fp_t sys_msleep_adpt;
#endif


/* Mailbox functions. */
typedef err_t (*sys_mbox_new_fp_t)(sys_mbox_t *mbox, int size);
typedef void  (*sys_mbox_free_fp_t)(sys_mbox_t *mbox);
typedef void  (*sys_mbox_post_fp_t)(sys_mbox_t *mbox, void *msg);
typedef err_t (*sys_mbox_trypost_fp_t)(sys_mbox_t *mbox, void *msg);
typedef u32_t (*sys_arch_mbox_fetch_fp_t)(sys_mbox_t *mbox, void **msg, u32_t timeout);
#ifndef sys_arch_mbox_tryfetch
typedef u32_t (*sys_arch_mbox_tryfetch_fp_t)(sys_mbox_t *mbox, void **msg);
#endif

extern sys_mbox_new_fp_t            sys_mbox_new_adpt;
extern sys_mbox_free_fp_t           sys_mbox_free_adpt;
extern sys_mbox_post_fp_t           sys_mbox_post_adpt;
extern sys_mbox_trypost_fp_t        sys_mbox_trypost_adpt;
extern sys_arch_mbox_fetch_fp_t     sys_arch_mbox_fetch_adpt;
#ifndef sys_arch_mbox_tryfetch
extern sys_arch_mbox_tryfetch_fp_t  sys_arch_mbox_tryfetch_adpt;
#endif

#ifndef sys_mbox_valid
int   sys_mbox_valid(sys_mbox_t *mbox);
#endif
#ifndef sys_mbox_set_invalid
void  sys_mbox_set_invalid(sys_mbox_t *mbox);
#endif


typedef sys_prot_t (*sys_arch_protect_fp_t)( void );
typedef void (*sys_arch_unprotect_fp_t)(sys_prot_t xValue);

extern sys_arch_protect_fp_t    sys_arch_protect_adpt;
extern sys_arch_unprotect_fp_t  sys_arch_unprotect_adpt;

typedef sys_thread_t (*sys_thread_new_fp_t)(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio);

extern sys_thread_new_fp_t sys_thread_new_adpt;

/* sys_init() must be called before anything else. */
typedef void (*sys_init_fp_t)(void);

extern sys_init_fp_t sys_init_adpt;

#ifndef sys_jiffies
/**
 * Ticks/jiffies since power up.
 */
typedef u32_t (*sys_jiffies_fp_t)(void);
#endif

/**
 * @ingroup sys_time
 * Returns the current time in milliseconds,
 * may be the same as sys_jiffies or at least based on it.
 */
typedef u32_t (*sys_now_fp_t)(void);
typedef void  (*assert_loop_fp_t)(void);
typedef void  (*assert_printf_fp_t)(char *msg, int line, char *file);
typedef void  (*sys_arch_assert_fp_t)(const char *file, int line);
typedef void  (*sys_delay_ms_fp_t)(uint32_t ms);

extern sys_now_fp_t            sys_now_adpt;
extern assert_loop_fp_t        assert_loop_adpt;
extern assert_printf_fp_t      assert_printf_adpt;
extern sys_arch_assert_fp_t    sys_arch_assert_adpt;
extern sys_delay_ms_fp_t       sys_delay_ms_adpt;

#endif /* __ARCH_SYS_ARCH_IF_H__ */

