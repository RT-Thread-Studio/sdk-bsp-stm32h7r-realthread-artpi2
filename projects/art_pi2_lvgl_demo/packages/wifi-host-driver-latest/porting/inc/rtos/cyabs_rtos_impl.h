/***************************************************************************//**
* \file cyabs_rtos_impl.h
*
* \brief
* Defines the Cypress RTOS Interface. Provides prototypes for functions that
* allow Cypress libraries to use RTOS resources such as threads, mutexes & timing
* functions in an abstract way. The APIs are implemented
* in the Port Layer RTOS interface which is specific to the RTOS in use.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "rtthread.h"

/******************************************************
*                 Constants
******************************************************/
#define CY_RTOS_MIN_STACK_SIZE      300            /**< Minimum stack size in bytes */


/* RTOS thread priority */
typedef enum
{
    CY_RTOS_PRIORITY_MIN            = RT_THREAD_PRIORITY_MAX,
    CY_RTOS_PRIORITY_LOW            = (RT_THREAD_PRIORITY_MAX - 1),
    CY_RTOS_PRIORITY_BELOWNORMAL    = (RT_THREAD_PRIORITY_MAX - 2),
    CY_RTOS_PRIORITY_NORMAL         = (RT_THREAD_PRIORITY_MAX - 2),
    CY_RTOS_PRIORITY_ABOVENORMAL    = (RT_THREAD_PRIORITY_MAX - 3),
    CY_RTOS_PRIORITY_HIGH           = (RT_THREAD_PRIORITY_MAX - 4),
    CY_RTOS_PRIORITY_REALTIME       = (RT_THREAD_PRIORITY_MAX - 5),
    CY_RTOS_PRIORITY_MAX            = (RT_THREAD_PRIORITY_MAX - 6)
} cy_thread_priority_t;

typedef volatile rt_sem_t cy_semaphore_t;    /** RTOS definition of a semaphore */
typedef volatile rt_thread_t cy_thread_t;       /** RTOS definition of a thread handle */
typedef int32_t cy_rtos_error_t;                      /** RTOS definition of a error status */
typedef uint32_t cy_time_t;
typedef void * cy_thread_arg_t;
typedef uint32_t cy_event_t;        /** RTOS definition of a event */

