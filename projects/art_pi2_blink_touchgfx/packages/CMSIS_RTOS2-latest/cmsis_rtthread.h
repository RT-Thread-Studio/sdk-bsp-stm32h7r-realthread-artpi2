/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-04-12     misonyo      the first version
 */

#ifndef __RTTHREAD_OS2_H__
#define __RTTHREAD_OS2_H__

#include <rtthread.h>
#include <cmsis_os2.h>

#ifdef __cplusplus
extern "C" {
#endif

///< RT-Thread Kernel version
#if defined(RT_VERSION_CHECK) && (RTTHREAD_VERSION >= RT_VERSION_CHECK(5, 0, 2))

#define KERNEL_VERSION             (((rt_uint32_t)RT_VERSION_MAJOR * 10000000UL)    | \
                                   ((rt_uint32_t)RT_VERSION_MINOR  *    10000UL)    | \
                                   ((rt_uint32_t)RT_VERSION_PATCH  *        1UL))
#define thread_rt_list_entry(node, rt_thread)   RT_THREAD_LIST_NODE_ENTRY(node)
#define THREAD_NAME(thread_cb)  thread_cb->thread.parent.name
#define RT_WEAK                 rt_weak

#if(RTTHREAD_VERSION >= RT_VERSION_CHECK(5, 2, 0))
#define CMSIS_RT_SCHED_CTX(thread) RT_SCHED_CTX(&thread)
#else
#define CMSIS_RT_SCHED_CTX(thread) (thread)
#endif  /* RTTHREAD_VERSION >= RT_VERSION_CHECK(5, 2, 0) */

#else   /* legacy version macros (<5.0.2) */
#define KERNEL_VERSION             (((rt_uint32_t)RT_VERSION * 10000000UL)   | \
                                   ((rt_uint32_t)RT_SUBVERSION *    10000UL) | \
                                   ((rt_uint32_t)RT_REVISION *        1UL))
#define thread_rt_list_entry(node, rt_thread)   rt_list_entry(node, struct rt_thread, list)
#define THREAD_NAME(thread_cb)  thread_cb->thread.name
#define CMSIS_RT_SCHED_CTX(thread) (thread)
#endif  /* RT_VERSION_CHECK */

typedef struct
{
    rt_uint8_t flags;
    struct rt_thread thread;
    rt_sem_t joinable_sem;    ///< semaphore for joinable thread
    rt_uint8_t flag_set;      ///< thread flag set
    rt_uint8_t prio;
}thread_cb_t;

typedef struct
{
    rt_uint8_t flags;
    struct rt_timer timer;
}timer_cb_t;

#ifdef RT_USING_EVENT
typedef struct
{
    rt_uint8_t flags;
    struct rt_event event;
}event_cb_t;
#endif

#ifdef RT_USING_MUTEX
typedef struct
{
    rt_uint8_t flags;
    struct rt_mutex mutex;
}mutex_cb_t;
#endif

#ifdef RT_USING_SEMAPHORE
typedef struct
{
    rt_uint8_t flags;
    struct rt_semaphore sem;
}sem_cb_t;
#endif

#ifdef RT_USING_MEMPOOL
typedef struct
{
    rt_uint8_t flags;
    struct rt_mempool mp;
}mempool_cb_t;
#endif

#ifdef RT_USING_MESSAGEQUEUE
typedef struct
{
    rt_uint32_t init_msg_size;
    rt_uint8_t flags;
    struct rt_messagequeue mq;
}mq_cb_t;
#endif

#ifdef __cplusplus
}
#endif

#endif
