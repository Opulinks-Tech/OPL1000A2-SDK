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
/* lwIP includes. */
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"

#include <stdio.h>
#include "cmsis_os.h"

#if !NO_SYS
/* ------------------------ System architecture includes ----------------------------- */
#include "arch/sys_arch.h"

/* ------------------------ lwIP includes --------------------------------- */
#include "lwip/opt.h"
#include "lwip/stats.h"


/*-------------------------------------------------------------------------------------
 * Definitions of interface function pointer
 *------------------------------------------------------------------------------------*/
RET_DATA sys_mutex_new_fp_t           sys_mutex_new_adpt;
RET_DATA sys_mutex_lock_fp_t          sys_mutex_lock_adpt;
RET_DATA sys_mutex_unlock_fp_t        sys_mutex_unlock_adpt;
RET_DATA sys_mutex_free_fp_t          sys_mutex_free_adpt;

RET_DATA sys_sem_new_fp_t             sys_sem_new_adpt;
RET_DATA sys_sem_signal_fp_t          sys_sem_signal_adpt;
RET_DATA sys_arch_sem_wait_fp_t       sys_arch_sem_wait_adpt;
RET_DATA sys_sem_free_fp_t            sys_sem_free_adpt;

RET_DATA sys_mbox_new_fp_t            sys_mbox_new_adpt;
RET_DATA sys_mbox_free_fp_t           sys_mbox_free_adpt;
RET_DATA sys_mbox_post_fp_t           sys_mbox_post_adpt;
RET_DATA sys_mbox_trypost_fp_t        sys_mbox_trypost_adpt;
RET_DATA sys_arch_mbox_fetch_fp_t     sys_arch_mbox_fetch_adpt;
RET_DATA sys_arch_mbox_tryfetch_fp_t  sys_arch_mbox_tryfetch_adpt;

RET_DATA sys_arch_protect_fp_t        sys_arch_protect_adpt;
RET_DATA sys_arch_unprotect_fp_t      sys_arch_unprotect_adpt;

RET_DATA sys_thread_new_fp_t          sys_thread_new_adpt;
RET_DATA sys_init_fp_t                sys_init_adpt;
RET_DATA sys_now_fp_t                 sys_now_adpt;
RET_DATA assert_loop_fp_t             assert_loop_adpt;
RET_DATA assert_printf_fp_t           assert_printf_adpt;
RET_DATA sys_arch_assert_fp_t         sys_arch_assert_adpt;
RET_DATA sys_delay_ms_fp_t            sys_delay_ms_adpt;
RET_DATA sys_msleep_fp_t              sys_msleep_adpt;

/*-------------------------------------------------------------------------------------
 * Interface assignment
 *------------------------------------------------------------------------------------*/
void lwip_load_interface_sys_arch(void)
{
    sys_mutex_new_adpt              = LWIP_ROMFN(sys_mutex_new);
    sys_mutex_lock_adpt             = LWIP_ROMFN(sys_mutex_lock);
    sys_mutex_unlock_adpt           = LWIP_ROMFN(sys_mutex_unlock);
    sys_mutex_free_adpt             = LWIP_ROMFN(sys_mutex_free);

    sys_sem_new_adpt                = LWIP_ROMFN(sys_sem_new);
    sys_sem_signal_adpt             = LWIP_ROMFN(sys_sem_signal);
    sys_arch_sem_wait_adpt          = LWIP_ROMFN(sys_arch_sem_wait);
    sys_sem_free_adpt               = LWIP_ROMFN(sys_sem_free);

    sys_mbox_new_adpt               = LWIP_ROMFN(sys_mbox_new);
    sys_mbox_free_adpt              = LWIP_ROMFN(sys_mbox_free);
    sys_mbox_post_adpt              = LWIP_ROMFN(sys_mbox_post);
    sys_mbox_trypost_adpt           = LWIP_ROMFN(sys_mbox_trypost);
    sys_arch_mbox_fetch_adpt        = LWIP_ROMFN(sys_arch_mbox_fetch);
    sys_arch_mbox_tryfetch_adpt     = LWIP_ROMFN(sys_arch_mbox_tryfetch);

    sys_arch_protect_adpt           = LWIP_ROMFN(sys_arch_protect);
    sys_arch_unprotect_adpt         = LWIP_ROMFN(sys_arch_unprotect);

    sys_thread_new_adpt             = LWIP_ROMFN(sys_thread_new);
    sys_init_adpt                   = LWIP_ROMFN(sys_init);
    sys_now_adpt                    = LWIP_ROMFN(sys_now);
    assert_loop_adpt                = LWIP_ROMFN(assert_loop);
    assert_printf_adpt              = LWIP_ROMFN(assert_printf);
    sys_arch_assert_adpt            = LWIP_ROMFN(sys_arch_assert);
    sys_delay_ms_adpt               = LWIP_ROMFN(sys_delay_ms);
    sys_msleep_adpt                 = LWIP_ROMFN(sys_msleep);
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates a new mailbox
 * Inputs:
 *      int size                -- Size of elements in the mailbox
 * Outputs:
 *      sys_mbox_t              -- Handle to new mailbox
 *---------------------------------------------------------------------------*/
err_t sys_mbox_new( sys_mbox_t *pxMailBox, int iSize )
{
    return sys_mbox_new_adpt(pxMailBox, iSize);
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_free
 *---------------------------------------------------------------------------*
 * Description:
 *      Deallocates a mailbox. If there are messages still present in the
 *      mailbox when the mailbox is deallocated, it is an indication of a
 *      programming error in lwIP and the developer should be notified.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 * Outputs:
 *      sys_mbox_t              -- Handle to new mailbox
 *---------------------------------------------------------------------------*/
void sys_mbox_free( sys_mbox_t *pxMailBox )
{
    sys_mbox_free_adpt(pxMailBox);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_post
 *---------------------------------------------------------------------------*
 * Description:
 *      Post the "msg" to the mailbox.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void *data              -- Pointer to data to post
 *---------------------------------------------------------------------------*/
void sys_mbox_post( sys_mbox_t *pxMailBox, void *pxMessageToPost )
{
    sys_mbox_post_adpt(pxMailBox, pxMessageToPost);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_trypost
 *---------------------------------------------------------------------------*
 * Description:
 *      Try to post the "msg" to the mailbox.  Returns immediately with
 *      error if cannot.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void *msg               -- Pointer to data to post
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *                                  if not.
 *---------------------------------------------------------------------------*/
err_t sys_mbox_trypost( sys_mbox_t *pxMailBox, void *pxMessageToPost )
{
    return sys_mbox_trypost_adpt(pxMailBox, pxMessageToPost);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_mbox_fetch
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread until a message arrives in the mailbox, but does
 *      not block the thread longer than "timeout" milliseconds (similar to
 *      the sys_arch_sem_wait() function). The "msg" argument is a result
 *      parameter that is set by the function (i.e., by doing "*msg =
 *      ptr"). The "msg" parameter maybe NULL to indicate that the message
 *      should be dropped.
 *
 *      The return values are the same as for the sys_arch_sem_wait() function:
 *      Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
 *      timeout.
 *
 *      Note that a function with a similar name, sys_mbox_fetch(), is
 *      implemented by lwIP.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void **msg              -- Pointer to pointer to msg received
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- SYS_ARCH_TIMEOUT if timeout, else number
 *                                  of milliseconds until received.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_mbox_fetch( sys_mbox_t *pxMailBox, void **ppvBuffer, u32_t ulTimeOut )
{
    return sys_arch_mbox_fetch_adpt(pxMailBox, ppvBuffer, ulTimeOut);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_mbox_tryfetch
 *---------------------------------------------------------------------------*
 * Description:
 *      Similar to sys_arch_mbox_fetch, but if message is not ready
 *      immediately, we'll return with SYS_MBOX_EMPTY.  On success, 0 is
 *      returned.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void **msg              -- Pointer to pointer to msg received
 * Outputs:
 *      u32_t                   -- SYS_MBOX_EMPTY if no messages.  Otherwise,
 *                                  return ERR_OK.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_mbox_tryfetch( sys_mbox_t *pxMailBox, void **ppvBuffer )
{
    return sys_arch_mbox_tryfetch_adpt(pxMailBox, ppvBuffer);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates and returns a new semaphore. The "ucCount" argument specifies
 *      the initial state of the semaphore.
 *      NOTE: Currently this routine only creates counts of 1 or 0
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      u8_t ucCount            -- Initial ucCount of semaphore (1 or 0)
 * Outputs:
 *      sys_sem_t               -- Created semaphore or 0 if could not create.
 *---------------------------------------------------------------------------*/
err_t sys_sem_new( sys_sem_t *pxSemaphore, u8_t ucCount )
{
    return sys_sem_new_adpt(pxSemaphore, ucCount);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_sem_wait
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread while waiting for the semaphore to be
 *      signaled. If the "timeout" argument is non-zero, the thread should
 *      only be blocked for the specified time (measured in
 *      milliseconds).
 *
 *      If the timeout argument is non-zero, the return value is the number of
 *      milliseconds spent waiting for the semaphore to be signaled. If the
 *      semaphore wasn't signaled within the specified time, the return value is
 *      SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
 *      (i.e., it was already signaled), the function may return zero.
 *
 *      Notice that lwIP implements a function with a similar name,
 *      sys_sem_wait(), that uses the sys_arch_sem_wait() function.
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to wait on
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- Time elapsed or SYS_ARCH_TIMEOUT.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_sem_wait( sys_sem_t *pxSemaphore, u32_t ulTimeout )
{
    return sys_arch_sem_wait_adpt(pxSemaphore, ulTimeout);
}

/**
 * @brief	Create a new mutex
 * @param	pxMutex pointer to the mutex to create
 * @return	a new mutex
 */
err_t sys_mutex_new( sys_mutex_t *pxMutex )
{
    return sys_mutex_new_adpt(pxMutex);
}

/** Lock a mutex
 * @param pxMutex the mutex to lock */
void sys_mutex_lock( sys_mutex_t *pxMutex )
{
    sys_mutex_lock_adpt(pxMutex);
}

/** Unlock a mutex
 * @param pxMutex the mutex to unlock */
void sys_mutex_unlock(sys_mutex_t *pxMutex )
{
    sys_mutex_unlock_adpt(pxMutex);
}


/** Delete a semaphore
 * @param pxMutex the mutex to delete */
void sys_mutex_free( sys_mutex_t *pxMutex )
{
    sys_mutex_free_adpt(pxMutex);
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_signal
 *---------------------------------------------------------------------------*
 * Description:
 *      Signals (releases) a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to signal
 *---------------------------------------------------------------------------*/
void sys_sem_signal( sys_sem_t *pxSemaphore )
{
    sys_sem_signal_adpt(pxSemaphore);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_free
 *---------------------------------------------------------------------------*
 * Description:
 *      Deallocates a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to free
 *---------------------------------------------------------------------------*/
void sys_sem_free( sys_sem_t *pxSemaphore )
{
    sys_sem_free_adpt(pxSemaphore);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_init
 *---------------------------------------------------------------------------*
 * Description:
 *      Initialize sys arch
 *---------------------------------------------------------------------------*/
void sys_init(void)
{
    sys_init_adpt();
}

u32_t sys_now(void)
{
    return sys_now_adpt();
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_thread_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Starts a new thread with priority "prio" that will begin its
 *      execution in the function "thread()". The "arg" argument will be
 *      passed as an argument to the thread() function. The id of the new
 *      thread is returned. Both the id and the priority are system
 *      dependent.
 * Inputs:
 *      char *name              -- Name of thread
 *      void (* thread)(void *arg) -- Pointer to function to run.
 *      void *arg               -- Argument passed into function
 *      int stacksize           -- Required stack amount in bytes
 *      int prio                -- Thread priority
 * Outputs:
 *      sys_thread_t            -- Pointer to per-thread timeouts.
 *---------------------------------------------------------------------------*/
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
    return sys_thread_new_adpt(name, thread, arg, stacksize, prio);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_protect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" critical region protection and
 *      returns the previous protection level. This function is only called
 *      during very short critical regions. An embedded system which supports
 *      ISR-based drivers might want to implement this function by disabling
 *      interrupts. Task-based systems might want to implement this by using
 *      a mutex or disabling tasking. This function should support recursive
 *      calls from the same task or interrupt. In other words,
 *      sys_arch_protect() could be called while already protected. In
 *      that case the return value indicates that it is already protected.
 *
 *      sys_arch_protect() is only required if your port is supporting an
 *      operating system.
 * Outputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
sys_prot_t sys_arch_protect( void )
{
    return sys_arch_protect_adpt();
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_unprotect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" set of critical region
 *      protection to the value specified by pval. See the documentation for
 *      sys_arch_protect() for more information. This function is only
 *      required if your port is supporting an operating system.
 * Inputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
void sys_arch_unprotect( sys_prot_t xValue )
{
    sys_arch_unprotect_adpt(xValue);
}

/*
 * Prints an assertion messages and aborts execution.
 */
void sys_arch_assert(const char *file, int line)
{
    sys_arch_assert_adpt(file, line);
}

void assert_printf(char *msg, int line, char *file)
{
    assert_printf_adpt(msg, line, file);
}

void assert_loop(void)
{
    assert_loop_adpt();
}

void sys_delay_ms(uint32_t ms)
{
    sys_delay_ms_adpt(ms);
}

void sys_msleep(u32_t ms)
{
    sys_msleep_adpt(ms);
}
/*-------------------------------------------------------------------------*
 * End of File:  sys_arch.c
 *-------------------------------------------------------------------------*/

#endif

/*-----------------------------------------------------------------------------------*/
