/* --------------------------------------------------------------------------
 * Copyright (c) 2013-2022 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *      Name:    cmsis_os2.c
 *      Purpose: CMSIS RTOS2 wrapper for FreeRTOS
 *
 *---------------------------------------------------------------------------*/

#include <string.h>

#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "cmsis_compiler.h"             // Compiler agnostic definitions

#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "semphr.h"                     // ARM.FreeRTOS::RTOS:Core
#include "timers.h"                     // ARM.FreeRTOS::RTOS:Timers

#include "freertos_mpool.h"             // osMemoryPool definitions
#include "freertos_os2.h"               // Configuration check and setup

/*---------------------------------------------------------------------------*/
#ifndef __ARM_ARCH_6M__
  #define __ARM_ARCH_6M__         0
#endif
#ifndef __ARM_ARCH_7M__
  #define __ARM_ARCH_7M__         0
#endif
#ifndef __ARM_ARCH_7EM__
  #define __ARM_ARCH_7EM__        0
#endif
#ifndef __ARM_ARCH_8M_MAIN__
  #define __ARM_ARCH_8M_MAIN__    0
#endif
#ifndef __ARM_ARCH_7A__
  #define __ARM_ARCH_7A__         0
#endif

#if   ((__ARM_ARCH_7M__      == 1U) || \
       (__ARM_ARCH_7EM__     == 1U) || \
       (__ARM_ARCH_8M_MAIN__ == 1U))
#define IS_IRQ_MASKED()           ((__get_PRIMASK() != 0U) || (__get_BASEPRI() != 0U))
#elif  (__ARM_ARCH_6M__      == 1U)
#define IS_IRQ_MASKED()           (__get_PRIMASK() != 0U)
#elif (__ARM_ARCH_7A__       == 1U)
/* CPSR mask bits */
#define CPSR_MASKBIT_I            0x80U

#define IS_IRQ_MASKED()           ((__get_CPSR() & CPSR_MASKBIT_I) != 0U)
#else
#define IS_IRQ_MASKED()           (__get_PRIMASK() != 0U)
#endif

#if    (__ARM_ARCH_7A__      == 1U)
/* CPSR mode bitmasks */
#define CPSR_MODE_USER            0x10U
#define CPSR_MODE_SYSTEM          0x1FU

#define IS_IRQ_MODE()             ((__get_mode() != CPSR_MODE_USER) && (__get_mode() != CPSR_MODE_SYSTEM))
#else
#define IS_IRQ_MODE()             (__get_IPSR() != 0U)
#endif

/*
  Determine if CPU executes from interrupt context or if interrupts are masked.
*/
__STATIC_INLINE uint32_t IRQ_Context (void) {
  uint32_t irq;
  BaseType_t state;

  irq = 0U;

  if (IS_IRQ_MODE()) {
    /* Called from interrupt context */
    irq = 1U;
  }
  else {
    /* Get FreeRTOS scheduler state */
    state = xTaskGetSchedulerState();

    if (state != taskSCHEDULER_NOT_STARTED) {
      /* Scheduler was started */
      if (IS_IRQ_MASKED()) {
        /* Interrupts are masked */
        irq = 1U;
      }
    }
  }

  /* Return context, 0: thread context, 1: IRQ context */
  return (irq);
}

/*
  Pass control to next thread that is in state READY.
*/
osStatus_t osThreadYield (void) {
  osStatus_t stat;

  if (IRQ_Context() != 0U) {
    stat = osErrorISR;
  } else {
    stat = osOK;
    taskYIELD();
  }

  /* Return execution status */
  return (stat);
}

/* ==== Generic Wait Functions ==== */

/*
  Wait for Timeout (Time Delay).
*/
osStatus_t osDelay (uint32_t ticks) {
  osStatus_t stat;

  if (IRQ_Context() != 0U) {
    stat = osErrorISR;
  }
  else {
    stat = osOK;

    if (ticks != 0U) {
      vTaskDelay(ticks);
    }
  }

  /* Return execution status */
  return (stat);
}

/*
/* ==== Mutex Management Functions ==== */

#if (configUSE_OS2_MUTEX == 1)
/*
  Create and Initialize a Mutex object.

  Limitations:
  - Priority inherit protocol is used by default, osMutexPrioInherit attribute is ignored.
  - Robust mutex is not supported, NULL is returned if used.
*/
osMutexId_t osMutexNew (const osMutexAttr_t *attr) {
  SemaphoreHandle_t hMutex;
  uint32_t type;
  uint32_t rmtx;
  int32_t  mem;

  hMutex = NULL;

  if (IRQ_Context() == 0U) {
    if (attr != NULL) {
      type = attr->attr_bits;
    } else {
      type = 0U;
    }

    if ((type & osMutexRecursive) == osMutexRecursive) {
      rmtx = 1U;
    } else {
      rmtx = 0U;
    }

    if ((type & osMutexRobust) != osMutexRobust) {
      mem = -1;

      if (attr != NULL) {
        if ((attr->cb_mem != NULL) && (attr->cb_size >= sizeof(StaticSemaphore_t))) {
          /* The memory for control block is provided, use static object */
          mem = 1;
        }
        else {
          if ((attr->cb_mem == NULL) && (attr->cb_size == 0U)) {
            /* Control block will be allocated from the dynamic pool */
            mem = 0;
          }
        }
      }
      else {
        mem = 0;
      }

      if (mem == 1) {
        #if (configSUPPORT_STATIC_ALLOCATION == 1)
          if (rmtx != 0U) {
            #if (configUSE_RECURSIVE_MUTEXES == 1)
            hMutex = xSemaphoreCreateRecursiveMutexStatic (attr->cb_mem);
            #endif
          }
          else {
            hMutex = xSemaphoreCreateMutexStatic (attr->cb_mem);
          }
        #endif
      }
      else {
        if (mem == 0) {
          #if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
            if (rmtx != 0U) {
              #if (configUSE_RECURSIVE_MUTEXES == 1)
              hMutex = xSemaphoreCreateRecursiveMutex ();
              #endif
            } else {
              hMutex = xSemaphoreCreateMutex ();
            }
          #endif
        }
      }

      #if (configQUEUE_REGISTRY_SIZE > 0)
      if (hMutex != NULL) {
        if ((attr != NULL) && (attr->name != NULL)) {
          /* Only non-NULL name objects are added to the Queue Registry */
          vQueueAddToRegistry (hMutex, attr->name);
        }
      }
      #endif

      if ((hMutex != NULL) && (rmtx != 0U)) {
        /* Set LSB as 'recursive mutex flag' */
        hMutex = (SemaphoreHandle_t)((uint32_t)hMutex | 1U);
      }
    }
  }

  /* Return mutex ID */
  return ((osMutexId_t)hMutex);
}

/*
  Acquire a Mutex or timeout if it is locked.
*/
osStatus_t osMutexAcquire (osMutexId_t mutex_id, uint32_t timeout) {
  SemaphoreHandle_t hMutex;
  osStatus_t stat;
  uint32_t rmtx;

  hMutex = (SemaphoreHandle_t)((uint32_t)mutex_id & ~1U);

  /* Extract recursive mutex flag */
  rmtx = (uint32_t)mutex_id & 1U;

  stat = osOK;

  if (IRQ_Context() != 0U) {
    stat = osErrorISR;
  }
  else if (hMutex == NULL) {
    stat = osErrorParameter;
  }
  else {
    if (rmtx != 0U) {
      #if (configUSE_RECURSIVE_MUTEXES == 1)
      if (xSemaphoreTakeRecursive (hMutex, timeout) != pdPASS) {
        if (timeout != 0U) {
          stat = osErrorTimeout;
        } else {
          stat = osErrorResource;
        }
      }
      #endif
    }
    else {
      if (xSemaphoreTake (hMutex, timeout) != pdPASS) {
        if (timeout != 0U) {
          stat = osErrorTimeout;
        } else {
          stat = osErrorResource;
        }
      }
    }
  }

  /* Return execution status */
  return (stat);
}

/*
  Release a Mutex that was acquired by osMutexAcquire.
*/
osStatus_t osMutexRelease (osMutexId_t mutex_id) {
  SemaphoreHandle_t hMutex;
  osStatus_t stat;
  uint32_t rmtx;

  hMutex = (SemaphoreHandle_t)((uint32_t)mutex_id & ~1U);

  /* Extract recursive mutex flag */
  rmtx = (uint32_t)mutex_id & 1U;

  stat = osOK;

  if (IRQ_Context() != 0U) {
    stat = osErrorISR;
  }
  else if (hMutex == NULL) {
    stat = osErrorParameter;
  }
  else {
    if (rmtx != 0U) {
      #if (configUSE_RECURSIVE_MUTEXES == 1)
      if (xSemaphoreGiveRecursive (hMutex) != pdPASS) {
        stat = osErrorResource;
      }
      #endif
    }
    else {
      if (xSemaphoreGive (hMutex) != pdPASS) {
        stat = osErrorResource;
      }
    }
  }

  /* Return execution status */
  return (stat);
}

/*
  Get Thread which owns a Mutex object.
*/
osThreadId_t osMutexGetOwner (osMutexId_t mutex_id) {
  SemaphoreHandle_t hMutex;
  osThreadId_t owner;

  hMutex = (SemaphoreHandle_t)((uint32_t)mutex_id & ~1U);

  if ((IRQ_Context() != 0U) || (hMutex == NULL)) {
    owner = NULL;
  } else {
    owner = (osThreadId_t)xSemaphoreGetMutexHolder (hMutex);
  }

  /* Return owner thread ID */
  return (owner);
}

/*
  Delete a Mutex object.
*/
osStatus_t osMutexDelete (osMutexId_t mutex_id) {
  osStatus_t stat;
#ifndef USE_FreeRTOS_HEAP_1
  SemaphoreHandle_t hMutex;

  hMutex = (SemaphoreHandle_t)((uint32_t)mutex_id & ~1U);

  if (IRQ_Context() != 0U) {
    stat = osErrorISR;
  }
  else if (hMutex == NULL) {
    stat = osErrorParameter;
  }
  else {
    #if (configQUEUE_REGISTRY_SIZE > 0)
    vQueueUnregisterQueue (hMutex);
    #endif
    stat = osOK;
    vSemaphoreDelete (hMutex);
  }
#else
  stat = osError;
#endif

  /* Return execution status */
  return (stat);
}
#endif /* (configUSE_OS2_MUTEX == 1) */


/* ==== Semaphore Management Functions ==== */

/*
  Create and Initialize a Semaphore object.
*/
osSemaphoreId_t osSemaphoreNew (uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr) {
  SemaphoreHandle_t hSemaphore;
  int32_t mem;

  hSemaphore = NULL;

  if ((IRQ_Context() == 0U) && (max_count > 0U) && (initial_count <= max_count)) {
    mem = -1;

    if (attr != NULL) {
      if ((attr->cb_mem != NULL) && (attr->cb_size >= sizeof(StaticSemaphore_t))) {
        /* The memory for control block is provided, use static object */
        mem = 1;
      }
      else {
        if ((attr->cb_mem == NULL) && (attr->cb_size == 0U)) {
          /* Control block will be allocated from the dynamic pool */
          mem = 0;
        }
      }
    }
    else {
      mem = 0;
    }

    if (mem != -1) {
      if (max_count == 1U) {
        if (mem == 1) {
          #if (configSUPPORT_STATIC_ALLOCATION == 1)
            hSemaphore = xSemaphoreCreateBinaryStatic ((StaticSemaphore_t *)attr->cb_mem);
          #endif
        }
        else {
          #if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
            hSemaphore = xSemaphoreCreateBinary();
          #endif
        }

        if ((hSemaphore != NULL) && (initial_count != 0U)) {
          if (xSemaphoreGive (hSemaphore) != pdPASS) {
            vSemaphoreDelete (hSemaphore);
            hSemaphore = NULL;
          }
        }
      }
      else {
        if (mem == 1) {
          #if (configSUPPORT_STATIC_ALLOCATION == 1)
            hSemaphore = xSemaphoreCreateCountingStatic (max_count, initial_count, (StaticSemaphore_t *)attr->cb_mem);
          #endif
        }
        else {
          #if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
            hSemaphore = xSemaphoreCreateCounting (max_count, initial_count);
          #endif
        }
      }

      #if (configQUEUE_REGISTRY_SIZE > 0)
      if (hSemaphore != NULL) {
        if ((attr != NULL) && (attr->name != NULL)) {
          /* Only non-NULL name objects are added to the Queue Registry */
          vQueueAddToRegistry (hSemaphore, attr->name);
        }
      }
      #endif
    }
  }

  /* Return semaphore ID */
  return ((osSemaphoreId_t)hSemaphore);
}

/*
  Acquire a Semaphore token or timeout if no tokens are available.
*/
osStatus_t osSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout) {
  SemaphoreHandle_t hSemaphore = (SemaphoreHandle_t)semaphore_id;
  osStatus_t stat;
  BaseType_t yield;

  stat = osOK;

  if (hSemaphore == NULL) {
    stat = osErrorParameter;
  }
  else if (IRQ_Context() != 0U) {
    if (timeout != 0U) {
      stat = osErrorParameter;
    }
    else {
      yield = pdFALSE;

      if (xSemaphoreTakeFromISR (hSemaphore, &yield) != pdPASS) {
        stat = osErrorResource;
      } else {
        portYIELD_FROM_ISR (yield);
      }
    }
  }
  else {
    if (xSemaphoreTake (hSemaphore, (TickType_t)timeout) != pdPASS) {
      if (timeout != 0U) {
        stat = osErrorTimeout;
      } else {
        stat = osErrorResource;
      }
    }
  }

  /* Return execution status */
  return (stat);
}

/*
  Release a Semaphore token up to the initial maximum count.
*/
osStatus_t osSemaphoreRelease (osSemaphoreId_t semaphore_id) {
  SemaphoreHandle_t hSemaphore = (SemaphoreHandle_t)semaphore_id;
  osStatus_t stat;
  BaseType_t yield;

  stat = osOK;

  if (hSemaphore == NULL) {
    stat = osErrorParameter;
  }
  else if (IRQ_Context() != 0U) {
    yield = pdFALSE;

    if (xSemaphoreGiveFromISR (hSemaphore, &yield) != pdTRUE) {
      stat = osErrorResource;
    } else {
      portYIELD_FROM_ISR (yield);
    }
  }
  else {
    if (xSemaphoreGive (hSemaphore) != pdPASS) {
      stat = osErrorResource;
    }
  }

  /* Return execution status */
  return (stat);
}

/*
  Get current Semaphore token count.
*/
uint32_t osSemaphoreGetCount (osSemaphoreId_t semaphore_id) {
  SemaphoreHandle_t hSemaphore = (SemaphoreHandle_t)semaphore_id;
  uint32_t count;

  if (hSemaphore == NULL) {
    count = 0U;
  }
  else if (IRQ_Context() != 0U) {
    count = (uint32_t)uxSemaphoreGetCountFromISR (hSemaphore);
  } else {
    count = (uint32_t)uxSemaphoreGetCount (hSemaphore);
  }

  /* Return number of tokens */
  return (count);
}

/*
  Delete a Semaphore object.
*/
osStatus_t osSemaphoreDelete (osSemaphoreId_t semaphore_id) {
  SemaphoreHandle_t hSemaphore = (SemaphoreHandle_t)semaphore_id;
  osStatus_t stat;

#ifndef USE_FreeRTOS_HEAP_1
  if (IRQ_Context() != 0U) {
    stat = osErrorISR;
  }
  else if (hSemaphore == NULL) {
    stat = osErrorParameter;
  }
  else {
    #if (configQUEUE_REGISTRY_SIZE > 0)
    vQueueUnregisterQueue (hSemaphore);
    #endif

    stat = osOK;
    vSemaphoreDelete (hSemaphore);
  }
#else
  stat = osError;
#endif

  /* Return execution status */
  return (stat);
}


/* ==== Message Queue Management Functions ==== */

/*
  Create and Initialize a Message Queue object.

  Limitations:
  - The memory for control block and and message data must be provided in the
    osThreadAttr_t structure in order to allocate object statically.
*/
osMessageQueueId_t osMessageQueueNew (uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr) {
  QueueHandle_t hQueue;
  int32_t mem;

  hQueue = NULL;

  if ((IRQ_Context() == 0U) && (msg_count > 0U) && (msg_size > 0U)) {
    mem = -1;

    if (attr != NULL) {
      if ((attr->cb_mem != NULL) && (attr->cb_size >= sizeof(StaticQueue_t)) &&
          (attr->mq_mem != NULL) && (attr->mq_size >= (msg_count * msg_size))) {
        /* The memory for control block and message data is provided, use static object */
        mem = 1;
      }
      else {
        if ((attr->cb_mem == NULL) && (attr->cb_size == 0U) &&
            (attr->mq_mem == NULL) && (attr->mq_size == 0U)) {
          /* Control block will be allocated from the dynamic pool */
          mem = 0;
        }
      }
    }
    else {
      mem = 0;
    }

    if (mem == 1) {
      #if (configSUPPORT_STATIC_ALLOCATION == 1)
        hQueue = xQueueCreateStatic (msg_count, msg_size, attr->mq_mem, attr->cb_mem);
      #endif
    }
    else {
      if (mem == 0) {
        #if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
          hQueue = xQueueCreate (msg_count, msg_size);
        #endif
      }
    }

    #if (configQUEUE_REGISTRY_SIZE > 0)
    if (hQueue != NULL) {
      if ((attr != NULL) && (attr->name != NULL)) {
        /* Only non-NULL name objects are added to the Queue Registry */
        vQueueAddToRegistry (hQueue, attr->name);
      }
    }
    #endif

  }

  /* Return message queue ID */
  return ((osMessageQueueId_t)hQueue);
}

/*
  Put a Message into a Queue or timeout if Queue is full.

  Limitations:
  - Message priority is ignored
*/
osStatus_t osMessageQueuePut (osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout) {
  QueueHandle_t hQueue = (QueueHandle_t)mq_id;
  osStatus_t stat;
  BaseType_t yield;

  (void)msg_prio; /* Message priority is ignored */

  stat = osOK;

  if (IRQ_Context() != 0U) {
    if ((hQueue == NULL) || (msg_ptr == NULL) || (timeout != 0U)) {
      stat = osErrorParameter;
    }
    else {
      yield = pdFALSE;

      if (xQueueSendToBackFromISR (hQueue, msg_ptr, &yield) != pdTRUE) {
        stat = osErrorResource;
      } else {
        portYIELD_FROM_ISR (yield);
      }
    }
  }
  else {
    if ((hQueue == NULL) || (msg_ptr == NULL)) {
      stat = osErrorParameter;
    }
    else {
      if (xQueueSendToBack (hQueue, msg_ptr, (TickType_t)timeout) != pdPASS) {
        if (timeout != 0U) {
          stat = osErrorTimeout;
        } else {
          stat = osErrorResource;
        }
      }
    }
  }

  /* Return execution status */
  return (stat);
}

/*
  Get a Message from a Queue or timeout if Queue is empty.

  Limitations:
  - Message priority is ignored
*/
osStatus_t osMessageQueueGet (osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout) {
  QueueHandle_t hQueue = (QueueHandle_t)mq_id;
  osStatus_t stat;
  BaseType_t yield;

  (void)msg_prio; /* Message priority is ignored */

  stat = osOK;

  if (IRQ_Context() != 0U) {
    if ((hQueue == NULL) || (msg_ptr == NULL) || (timeout != 0U)) {
      stat = osErrorParameter;
    }
    else {
      yield = pdFALSE;

      if (xQueueReceiveFromISR (hQueue, msg_ptr, &yield) != pdPASS) {
        stat = osErrorResource;
      } else {
        portYIELD_FROM_ISR (yield);
      }
    }
  }
  else {
    if ((hQueue == NULL) || (msg_ptr == NULL)) {
      stat = osErrorParameter;
    }
    else {
      if (xQueueReceive (hQueue, msg_ptr, (TickType_t)timeout) != pdPASS) {
        if (timeout != 0U) {
          stat = osErrorTimeout;
        } else {
          stat = osErrorResource;
        }
      }
    }
  }

  /* Return execution status */
  return (stat);
}

/*
  Get maximum number of messages in a Message Queue.
*/
uint32_t osMessageQueueGetCapacity (osMessageQueueId_t mq_id) {
  StaticQueue_t *mq = (StaticQueue_t *)mq_id;
  uint32_t capacity;
  /* Return maximum number of messages */
  return (capacity);
}

/*
  Get maximum message size in a Message Queue.
*/
uint32_t osMessageQueueGetMsgSize (osMessageQueueId_t mq_id) {
  StaticQueue_t *mq = (StaticQueue_t *)mq_id;
  uint32_t size;
  /* Return maximum message size */
  return (size);
}

/*
  Get number of queued messages in a Message Queue.
*/
uint32_t osMessageQueueGetCount (osMessageQueueId_t mq_id) {
  QueueHandle_t hQueue = (QueueHandle_t)mq_id;
  UBaseType_t count;

  if (hQueue == NULL) {
    count = 0U;
  }
  else if (IRQ_Context() != 0U) {
    count = uxQueueMessagesWaitingFromISR (hQueue);
  }
  else {
    count = uxQueueMessagesWaiting (hQueue);
  }

  /* Return number of queued messages */
  return ((uint32_t)count);
}

/*
  Get number of available slots for messages in a Message Queue.
*/
uint32_t osMessageQueueGetSpace (osMessageQueueId_t mq_id) {
    return NULL;
}

/*
  Reset a Message Queue to initial empty state.
*/
osStatus_t osMessageQueueReset (osMessageQueueId_t mq_id) {
  QueueHandle_t hQueue = (QueueHandle_t)mq_id;
  osStatus_t stat;

  if (IRQ_Context() != 0U) {
    stat = osErrorISR;
  }
  else if (hQueue == NULL) {
    stat = osErrorParameter;
  }
  else {
    stat = osOK;
    (void)xQueueReset (hQueue);
  }

  /* Return execution status */
  return (stat);
}

/*
  Delete a Message Queue object.
*/
osStatus_t osMessageQueueDelete (osMessageQueueId_t mq_id) {
  QueueHandle_t hQueue = (QueueHandle_t)mq_id;
  osStatus_t stat;

#ifndef USE_FreeRTOS_HEAP_1
  if (IRQ_Context() != 0U) {
    stat = osErrorISR;
  }
  else if (hQueue == NULL) {
    stat = osErrorParameter;
  }
  else {
    #if (configQUEUE_REGISTRY_SIZE > 0)
    vQueueUnregisterQueue (hQueue);
    #endif

    stat = osOK;
    vQueueDelete (hQueue);
  }
#else
  stat = osError;
#endif

  /* Return execution status */
  return (stat);
}
