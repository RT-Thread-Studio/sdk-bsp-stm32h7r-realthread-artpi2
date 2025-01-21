/*
 * MIT License
 *
 * Copyright (c) 2024 Evlers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Change Logs:
 * Date         Author      Notes
 * 2023-12-21   Evlers      first implementation
 */

#include <stdio.h>
#include "rtthread.h"
#include "whd.h"
#include "whd_thread.h"
#include "cyabs_rtos.h"


cy_rslt_t cy_rtos_create_thread(cy_thread_t *thread, cy_thread_entry_fn_t entry_function,
                                const char *name, void *stack, uint32_t stack_size, cy_thread_priority_t priority,
                                cy_thread_arg_t arg)
{
    if ((thread == NULL) || (stack_size < CY_RTOS_MIN_STACK_SIZE))
    {
        return CY_RTOS_BAD_PARAM;
    }
    else
    {
        rt_thread_t thread;

        if ((thread = rt_thread_create(name, entry_function, arg, stack_size, priority, 2)) == RT_NULL)
        {
            return CY_RTOS_NO_MEMORY;
        }

        if (rt_thread_startup(thread) != RT_EOK)
        {
            return CY_RSLT_TYPE_ERROR;
        }
    }

    return CY_RSLT_SUCCESS;
}

/**
 * Exit the current thread
 *
 * @returns WHD_SUCCESS on success or otherwise
 */
cy_rslt_t cy_rtos_exit_thread(void)
{
    return CY_RSLT_SUCCESS;
}

/**
 * Blocks the current thread until the indicated thread is complete
 *
 * @param thread         : handle of the thread to terminate
 *
 * @returns CY_RSLT_SUCCESS on success, error otherwise
 */
cy_rslt_t cy_rtos_join_thread(cy_thread_t *thread)
{
    return CY_RSLT_SUCCESS;
}

/**
 * Creates a semaphore
 *
 * @param semaphore         : pointer to variable which will receive handle of created semaphore
 *
 * @returns CY_RSLT_SUCCESS on success, error otherwise
 */
cy_rslt_t cy_rtos_init_semaphore(cy_semaphore_t *semaphore, uint32_t maxcount, uint32_t initcount)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;

    if (semaphore == NULL)
    {
        status = CY_RTOS_BAD_PARAM;
    }
    else
    {
        *semaphore = rt_sem_create("cyw", initcount, RT_IPC_FLAG_PRIO);
        if (*semaphore == NULL)
        {
            status = CY_RTOS_NO_MEMORY;
        }
    }

    return status;
}

/**
 * Gets a semaphore
 *
 * If value of semaphore is larger than zero, then the semaphore is decremented and function returns
 * Else If value of semaphore is zero, then current thread is suspended until semaphore is set.
 * Value of semaphore should never be below zero
 *
 * Must not be called from interrupt context, since it could block, and since an interrupt is not a
 * normal thread, so could cause RTOS problems if it tries to suspend it.
 * If called from interrupt context time out value should be 0.
 *
 * @param semaphore   : Pointer to variable which will receive handle of created semaphore
 * @param timeout_ms  : Maximum period to block for. Can be passed CY_RTOS_NEVER_TIMEOUT to request no timeout
 * @param will_set_in_isr : True if the semaphore will be set in an ISR. Currently only used for NoOS/NoNS
 *
 */
cy_rslt_t cy_rtos_get_semaphore(cy_semaphore_t *semaphore, uint32_t timeout_ms,
                                bool will_set_in_isr)
{
    /* In rtthread, semaphores cannot be take in interrupts */
    if ((semaphore == NULL) || (will_set_in_isr))
    {
        return CY_RTOS_BAD_PARAM;
    }
    else
    {
        rt_tick_t ticks = rt_tick_from_millisecond(timeout_ms);
        if (rt_sem_take(*semaphore, ticks) != RT_EOK)
        {
            return CY_RTOS_TIMEOUT;
        }
    }

    return CY_RSLT_SUCCESS;
}

/**
 * Sets a semaphore
 *
 * If any threads are waiting on the semaphore, the first thread is resumed
 * Else increment semaphore.
 *
 * Can be called from interrupt context, so must be able to handle resuming other
 * threads from interrupt context.
 *
 * @param semaphore       : Pointer to variable which will receive handle of created semaphore
 * @param called_from_ISR : Value of WHD_TRUE indicates calling from interrupt context
 *                          Value of WHD_FALSE indicates calling from normal thread context
 *
 * @return cy_rslt_t : CY_RSLT_SUCCESS if semaphore was successfully set
 *                        : error if an error occurred
 *
 */
cy_rslt_t cy_rtos_set_semaphore(cy_semaphore_t *semaphore, bool called_from_ISR)
{
    if ((semaphore == NULL))
    {
        return CY_RTOS_BAD_PARAM;
    }
    else
    {
        if (rt_sem_release(*semaphore) != RT_EOK)
        {
            return CY_RTOS_GENERAL_ERROR;
        }
    }

    return CY_RSLT_SUCCESS;
}

/**
 * Deletes a semaphore
 *
 * WHD uses this function to delete a semaphore.
 *
 * @param semaphore         : Pointer to the semaphore handle
 *
 * @return cy_rslt_t : CY_RSLT_SUCCESS if semaphore was successfully deleted
 *                        : error if an error occurred
 *
 */
cy_rslt_t cy_rtos_deinit_semaphore(cy_semaphore_t *semaphore)
{
    if (semaphore == NULL)
    {
        return CY_RTOS_BAD_PARAM;
    }
    else
    {
        if (rt_sem_delete(*semaphore) != RT_EOK)
        {
            return CY_RTOS_GENERAL_ERROR;
        }
    }

    return CY_RSLT_SUCCESS;
}

/**
 * Gets time in milliseconds since RTOS start
 *
 * @Note: since this is only 32 bits, it will roll over every 49 days, 17 hours.
 *
 * @returns Time in milliseconds since RTOS started.
 */
cy_rslt_t cy_rtos_get_time(cy_time_t *tval)
{
    *tval = rt_tick_get_millisecond();

    return CY_RSLT_SUCCESS;
}

/**
 * Delay for a number of milliseconds
 *
 * @return cy_rslt_t : CY_RSLT_SUCCESS if delay was successful
 *                        : error if an error occurred
 *
 */
cy_rslt_t cy_rtos_delay_milliseconds(cy_time_t num_ms)
{
    rt_thread_mdelay(num_ms);

    return CY_RSLT_SUCCESS;
}


#ifdef WHD_USE_CUSTOM_MALLOC_IMPL
/* Use the system's built-in memory allocation */
void *whd_mem_malloc (size_t size)
{
    return rt_malloc(size);
}

void *whd_mem_calloc(size_t nitems, size_t size)
{
    return rt_calloc(nitems, size);
}

void whd_mem_free(void *ptr)
{
    rt_free(ptr);
}

#endif /* ifndef WHD_USE_CUSTOM_MALLOC_IMPL */

