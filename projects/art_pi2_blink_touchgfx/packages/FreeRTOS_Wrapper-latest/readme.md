# FreeRTOS Wrapper for RT-Tread
## FreeRTOS Application Compatibility Layer (ACL) for RT-Thread
## Allow Seamless Migration of FreeRTOS Applications to RT-Thread

[中文](readme_zh.md) | English

## 1 Overview
This is a FreeRTOS application compatibility layer (ACL) for RT-Thread operating system. It allows developers to quickly and seamlessly migrate their existing FreeRTOS applications  to RT-Thread. Developers can use the same FreeRTOS API on their new RT-Thread applications, and take advantage of the feature-rich software components and packages offered by RT-Thread. The ACL is based on FreeRTOS V10.4.6. As of now it has supported migrations of multiple FreeRTOS-based SDK to RT-Thread.

### 1.1 RT-Thread Application Compatibility Layer (ACL) for Other RTOS

- μCOS-III ACL for RT-Thread：https://github.com/mysterywolf/RT-Thread-wrapper-of-uCOS-III
- μCOS-II ACL for RT-Thread：https://github.com/mysterywolf/RT-Thread-wrapper-of-uCOS-II
- RTX(CMSIS-RTOS1) ACL for RT-Thread：https://github.com/RT-Thread-packages/CMSIS_RTOS1
- RTX5(CMSIS-RTOS2) ACL for RT-Thread：https://github.com/RT-Thread-packages/CMSIS_RTOS2
- Arduino Ecosystem Compatibility Layer for RT-Thread：https://github.com/RTduino/RTduino

## 2 FreeRTOS API Support

Below is the current status of FreeRTOS API support offered by the FreeRTOS wrapper：

### 2.1 Task Control
- [x] xTaskCreate
- [x] xTaskCreateStatic
- [ ] [xTaskCreateRestrictedStatic](https://www.freertos.org/xtaskcreaterestrictedstaticfreertos-mpu-specific.html)
- [x] vTaskDelete
- [x] vTaskDelay
- [x] vTaskDelayUntil
- [x] xTaskDelayUntil 
- [x] uxTaskPriorityGet
- [x] vTaskPrioritySet
- [x] vTaskSuspend (Only support suspending the currently running task)
- [x] vTaskResume
- [x] xTaskResumeFromISR
- [x] xTaskAbortDelay
- [ ] [uxTaskGetSystemState](https://www.freertos.org/uxTaskGetSystemState.html)
- [ ] [vTaskGetInfo](https://www.freertos.org/vTaskGetInfo.html)
- [ ] [vTaskList](https://www.freertos.org/a00021.html#vTaskList)
- [ ] [vTaskGetRunTimeStats](https://www.freertos.org/a00021.html#vTaskGetRunTimeStats)
- [ ] [vTaskStartTrace](https://www.freertos.org/a00021.html#vTaskStartTrace)
- [ ] [ulTaskEndTrace](https://www.freertos.org/a00021.html#usTaskEndTrace)
- [ ] [SetThreadLocalStoragePointer](https://www.freertos.org/vTaskSetThreadLocalStoragePointer.html)
- [ ] [GetThreadLocalStoragePointer](https://www.freertos.org/pvTaskGetThreadLocalStoragePointer.html)
- [x] xTaskGetApplicationTaskTag 
- [x] xTaskGetCurrentTaskHandle
- [x] xTaskGetIdleTaskHandle
- [x] uxTaskGetStackHighWaterMark
- [x] eTaskGetState
- [x] pcTaskGetName
- [x] xTaskGetTickCount
- [x] xTaskGetTickCountFromISR
- [x] xTaskGetSchedulerState
- [x] uxTaskGetNumberOfTasks
- [x] vTaskSetApplicationTaskTag
- [x] xTaskCallApplicationTaskTag
- [x] vTaskSetTimeoutState
- [x] xTaskGetCheckForTimeout
### 2.2 RTOS Kernel Control
- [x] [taskYIELD](https://www.freertos.org/a00020.html#taskYIELD)
- [x] [taskENTER_CRITICAL](https://www.freertos.org/taskENTER_CRITICAL_taskEXIT_CRITICAL.html)
- [x] [taskEXIT_CRITICAL](https://www.freertos.org/taskENTER_CRITICAL_taskEXIT_CRITICAL.html)
- [x] [taskENTER_CRITICAL_FROM_ISR](https://www.freertos.org/taskENTER_CRITICAL_FROM_ISR_taskEXIT_CRITICAL_FROM_ISR.html)
- [x] [taskEXIT_CRITICAL_FROM_ISR](https://www.freertos.org/taskENTER_CRITICAL_FROM_ISR_taskEXIT_CRITICAL_FROM_ISR.html)
- [x] [taskDISABLE_INTERRUPTS](https://www.freertos.org/a00020.html#taskDISABLE_INTERRUPTS)
- [x] [taskENABLE_INTERRUPTS](https://www.freertos.org/a00020.html#taskENABLE_INTERRUPTS)
- [x] [vTaskStartScheduler](https://www.freertos.org/a00132.html)
- [x] [vTaskEndScheduler](https://www.freertos.org/a00133.html)
- [x] [vTaskSuspendAll](https://www.freertos.org/a00134.html)
- [x] [xTaskResumeAll](https://www.freertos.org/a00135.html)
- [ ] [vTaskStepTick](https://www.freertos.org/vTaskStepTick.html)
- [ ] [xTaskCatchUpTicks](https://www.freertos.org/xTaskCatchUpTicks.html)
### 2.3 Direct to Task Notifications
- [x] [xTaskNotifyGive](https://www.freertos.org/xTaskNotifyGive.html)
- [x] [vTaskNotifyGiveFromISR](https://www.freertos.org/vTaskNotifyGiveFromISR.html)
- [x] [ulTaskNotifyTake](https://www.freertos.org/ulTaskNotifyTake.html)
- [x] [xTaskNotify](https://www.freertos.org/xTaskNotify.html)
- [x] [xTaskNotifyAndQuery](https://www.freertos.org/xTaskNotifyAndQuery.html)
- [x] [xTaskNotifyAndQueryFromISR](https://www.freertos.org/xTaskNotifyAndQueryFromISR.html)
- [x] [xTaskNotifyFromISR](https://www.freertos.org/xTaskNotifyFromISR.html)
- [x] [xTaskNotifyWait](https://www.freertos.org/xTaskNotifyWait.html)
- [x] [xTaskNotifyStateClear](https://www.freertos.org/xTaskNotifyStateClear.html)
- [x] [ulTaskNotifyValueClear](https://www.freertos.org/ulTasknotifyValueClear.html)
### 2.4 Queues
- [x] [xQueueCreate](https://www.freertos.org/a00116.html)
- [x] [xQueueCreateStatic](https://www.freertos.org/xQueueCreateStatic.html)
- [x] [vQueueDelete](https://www.freertos.org/a00018.html#vQueueDelete)
- [x] [xQueueSend](https://www.freertos.org/a00117.html)
- [x] [xQueueSendFromISR](https://www.freertos.org/a00119.html)
- [x] [xQueueSendToBack](https://www.freertos.org/xQueueSendToBack.html)
- [x] [xQueueSendToBackFromISR](https://www.freertos.org/xQueueSendToBackFromISR.html)
- [x] [xQueueSendToFront](https://www.freertos.org/xQueueSendToFront.html) (Time out not supported)
- [x] [xQueueSendToFrontFromISR](https://www.freertos.org/xQueueSendToFrontFromISR.html)
- [x] [xQueueReceive](https://www.freertos.org/a00118.html)
- [x] [xQueueReceiveFromISR](https://www.freertos.org/a00120.html)
- [x] [uxQueueMessagesWaiting](https://www.freertos.org/a00018.html#ucQueueMessagesWaiting)
- [x] [uxQueueMessagesWaitingFromISR](https://www.freertos.org/a00018.html#ucQueueMessagesWaitingFromISR)
- [x] [uxQueueSpacesAvailable](https://www.freertos.org/a00018.html#uxQueueSpacesAvailable)
- [x] [xQueueReset](https://www.freertos.org/a00018.html#xQueueReset)
- [ ] [xQueueOverwrite](https://www.freertos.org/xQueueOverwrite.html)
- [ ] [xQueueOverwriteFromISR](https://www.freertos.org/xQueueOverwriteFromISR.html)
- [ ] [xQueuePeek](https://www.freertos.org/xQueuePeek.html)
- [ ] [xQueuePeekFromISR](https://www.freertos.org/xQueuePeekFromISR.html)
- [x] [xQueueIsQueueFullFromISR](https://www.freertos.org/a00018.html#xQueueIsQueueFullFromISR)
- [x] [xQueueIsQueueEmptyFromISR](https://www.freertos.org/a00018.html#xQueueIsQueueEmptyFromISR)
- [ ] [vQueueAddToRegistry](https://www.freertos.org/vQueueAddToRegistry.html)
- [ ] [vQueueUnregisterQueue](https://www.freertos.org/vQueueUnregisterQueue.html)
- [ ] [pcQueueGetName](https://www.freertos.org/pcQueueGetName.html)
### 2.5 Semaphore / Mutexes
- [x] [xSemaphoreCreateBinary](https://www.freertos.org/xSemaphoreCreateBinary.html)
- [x] [xSemaphoreCreateBinaryStatic](https://www.freertos.org/xSemaphoreCreateBinaryStatic.html)
- [x] [vSemaphoreCreateBinary](https://www.freertos.org/a00121.html)
- [x] [xSemaphoreCreateCounting](https://www.freertos.org/CreateCounting.html)
- [x] [xSemaphoreCreateCountingStatic](https://www.freertos.org/xSemaphoreCreateCountingStatic.html)
- [x] [xSemaphoreCreateMutex](https://www.freertos.org/CreateMutex.html)
- [x] [xSemaphoreCreateMutexStatic](https://www.freertos.org/xSemaphoreCreateMutexStatic.html)
- [x] [xSem'CreateRecursiveMutex](https://www.freertos.org/xSemaphoreCreateRecursiveMutex.html)
- [x] [xSem'CreateRecursiveMutexStatic](https://www.freertos.org/xSemaphoreCreateRecursiveMutexStatic.html)
- [x] [vSemaphoreDelete](https://www.freertos.org/a00113.html#vSemaphoreDelete)
- [x] [xSemaphoreGetMutexHolder](https://www.freertos.org/xSemaphoreGetMutexHolder.html)
- [x] [uxSemaphoreGetCount](https://www.freertos.org/uxSemaphoreGetCount.html)
- [x] [xSemaphoreTake](https://www.freertos.org/a00122.html)
- [x] [xSemaphoreTakeFromISR](https://www.freertos.org/xSemaphoreTakeFromISR.html)
- [x] [xSemaphoreTakeRecursive](https://www.freertos.org/xSemaphoreTakeRecursive.html)
- [x] [xSemaphoreGive](https://www.freertos.org/a00123.html)
- [x] [xSemaphoreGiveRecursive](https://www.freertos.org/xSemaphoreGiveRecursive.html)
- [x] [xSemaphoreGiveFromISR](https://www.freertos.org/a00124.html)
### 2.6 Software Timers
- [x] [xTimerCreate](https://www.freertos.org/FreeRTOS-timers-xTimerCreate.html)
- [x] [xTimerCreateStatic](https://www.freertos.org/xTimerCreateStatic.html)
- [x] [xTimerIsTimerActive](https://www.freertos.org/FreeRTOS-timers-xTimerIsTimerActive.html)
- [x] [xTimerStart](https://www.freertos.org/FreeRTOS-timers-xTimerStart.html)
- [x] [xTimerStop](https://www.freertos.org/FreeRTOS-timers-xTimerStop.html)
- [x] [xTimerChangePeriod](https://www.freertos.org/FreeRTOS-timers-xTimerChangePeriod.html)
- [x] [xTimerDelete](https://www.freertos.org/FreeRTOS-timers-xTimerDelete.html)
- [x] [xTimerReset](https://www.freertos.org/FreeRTOS-timers-xTimerReset.html)
- [x] [xTimerStartFromISR](https://www.freertos.org/FreeRTOS-timers-xTimerStartFromISR.html)
- [x] [xTimerStopFromISR](https://www.freertos.org/FreeRTOS-timers-xTimerStopFromISR.html)
- [x] [xTimerChangePeriodFromISR](https://www.freertos.org/FreeRTOS-timers-xTimerChangePeriodFromISR.html)
- [x] [xTimerResetFromISR](https://www.freertos.org/FreeRTOS-timers-xTimerResetFromISR.html)
- [x] [pvTimerGetTimerID](https://www.freertos.org/FreeRTOS-timers-pvTimerGetTimerID.html)
- [x] [vTimerSetReloadMode](https://www.freertos.org/FreeRTOS-Timers-vTimerSetReloadMode.html)
- [x] [vTimerSetTimerID](https://www.freertos.org/FreeRTOS-timers-vTimerSetTimerID.html)
- [x] [xTimerGetTimerDaemonTaskHandle](https://www.freertos.org/FreeRTOS-Software-Timer-API-Functions.html#xTimerGetTimerDaemonTaskHandle)
- [ ] [xTimerPendFunctionCall](https://www.freertos.org/xTimerPendFunctionCall.html)
- [ ] [xTimerPendFunctionCallFromISR](https://www.freertos.org/xTimerPendFunctionCallFromISR.html)
- [x] [pcTimerGetName](https://www.freertos.org/FreeRTOS-timers-pcTimerGetName.html)
- [x] [xTimerGetPeriod](https://www.freertos.org/FreeRTOS-timers-xTimerGetPeriod.html)
- [x] [xTimerGetExpiryTime](https://www.freertos.org/FreeRTOS-timers-xTimerGetExpiryTime.html)
- [x] [uxTimerGetReloadMode](https://www.freertos.org/uxTimerGetReloadMode.html)
### 2.7 Event Groups
- [x] [xEventGroupCreate](https://www.freertos.org/xEventGroupCreate.html)
- [x] [xEventGroupCreateStatic](https://www.freertos.org/xEventGroupCreateStatic.html)
- [x] [vEventGroupDelete](https://www.freertos.org/vEventGroupDelete.html)
- [x] [xEventGroupWaitBits](https://www.freertos.org/xEventGroupWaitBits.html)
- [x] [xEventGroupSetBits](https://www.freertos.org/xEventGroupSetBits.html)
- [x] [xEventGroupSetBitsFromISR](https://www.freertos.org/xEventGroupSetBitsFromISR.html)
- [x] [xEventGroupClearBits](https://www.freertos.org/xEventGroupClearBits.html)
- [x] [xEventGroupClearBitsFromISR](https://www.freertos.org/xEventGroupClearBitsFromISR.html)
- [x] [xEventGroupGetBits](https://www.freertos.org/xEventGroupGetBits.html)
- [x] [xEventGroupGetBitsFromISR](https://www.freertos.org/xEventGroupGetBitsFromISR.html)
- [ ] [xEventGroupSync](https://www.freertos.org/xEventGroupSync.html)
### 2.8 Unsupported Features
- [ ] [Queue Sets](https://www.freertos.org/RTOS-queue-sets.html)
- [ ] [Stream Buffers](https://www.freertos.org/RTOS-stream-buffer-API.html)
- [ ] [Message Buffers](https://www.freertos.org/RTOS-message-buffer-API.html)
- [ ] [MPU](https://www.freertos.org/FreeRTOS-MPU-specific.html)
- [ ] [Co-routines](https://www.freertos.org/croutineapi.html)
- [ ] [Hook Functions](https://www.freertos.org/a00016.html)
- [ ] [Trace Hook Macros](https://www.freertos.org/rtos-trace-macros.html)

## 3 Differences between FreeRTOS and RT-Thread
Similar to FreeRTOS, RT-Thread is a real-time operating system supporting multiple architectures, including ARM, RISC-V, etc. RT-Thread offers the typical task control APIs, software timer, and various task synchronization mechanisms including semaphores, mutexes, message queues and event groups. However, there are some subtle differences between implementations of similar features in FreeRTOS and RT-Thread. These differences are detailed below and care needs to be taken when using the following APIs.

### 3.1 Tasks, Queues and Mutexes

#### 3.1.1 vTaskSuspend
`vTaskSuspend` only supports suspending the currently-running task. When using `xTaskToSuspend`, the `xTaskToSuspend` must be `NULL`. Otherwise an assertion will be triggered.

#### 3.1.2 xQueueSendToFront
`xQueueSendToFront` does not support specifying a time out. When calling the API the `xTicksToWait` parameter is ignored. If the queue is full `errQUEUE_FULL` will be returned immediately.

#### 3.1.3 xQueueCreateStatic
Please follow the example below to create a static queue. This will ensure enough memory is allocated to store the specified number of queue items.
```c
#define QUEUE_LENGTH 10
#define ITEM_SIZE sizeof( uint32_t )

/* This is how the memory of a static queue is allocated in a FreeRTOS application. Because of differences between implementation details of queues in RT-Thread and FreeRTOS, memory allocated using this approach is not sufficient to store QUEUE_LENGTH number of elements. */
//uint8_t ucQueueStorage[ QUEUE_LENGTH * ITEM_SIZE ];
/* Need to use the QUEUE_BUFFER_SIZE macro to allocate memory */
uint8_t ucQueueStorage[ QUEUE_BUFFER_SIZE(QUEUE_LENGTH, ITEM_SIZE)];
StaticQueue_t xQueueBuffer;
QueueHandle_t xQueue1;
xQueue1 = xQueueCreate( QUEUE_LENGTH, ITEM_SIZE, &( ucQueueStorage[ 0 ] ), &xQueueBuffer );
```
#### 3.1.4 Mutex and Recursive Mutex
FreeRTOS offers two types of Mutexes: Mutex and Recursive Mutex. Recursive Mutexes can be taken repeatedly by the same task, while Mutexes cannot. All Mutexes in RT-Thread can be taken repeatedly. Therefore, the FreeRTOS wrapper does not distinguish between Mutexes and Recursive Mutexes. Mutexes created using either `xSemaphoreCreateMutex` or `xSemaphoreCreateRecursiveMutex` can be taken repeatedly.
### 3.2 Timers
Unlike FreeRTOS, RT-Thread does not send timer commands to the timer task using a message queue. When using any timer APIs that requires setting a time out in the FreeRTOS wrapper, such as `xTimerStart( xTimer, xTicksToWait )`, the `xTicksToWait` parameter is ignored and all such functions return immediately.
### 3.3 FromISR Functions
FreeRTOS distinguish those APIs that can be used from ISR and those that cannot. Those can be used from ISR has the word `FromISR` in their names. Is using these APIs from ISR wakes up a higher priority task, a FreeRTOS application normally needs to invoke the scheuler manually, as shown in the following example:
```c
BaseType_t xHigherPrioritTaskWoken = pdFALSE;
xQueueSendToFrontFromISR( xRxQueue, &cIn, &xHigherPriorityTaskWoken );
if( xHigherPriorityTaskWoken )
{
  taskYIELD ();
}
```
RT-Thread does not provide a `FromISR` version for its APIs. RT-Thread APIs can be used from ISR and they invoke the scheduler automatically. Therefore when using the FreeRTOS wrapper, you do not need to manually invoke the scheduler after using FromISR APIs. `xHigherPriorityTaskWoken` is always set to `pdFALSE`.
### 3.4 Heap
The FreeRTOS wrapper preserves the five heap allocation algorithms of FreeRTOS. By default `heap_3` is used, and `pvPortMalloc/vPortFree` invokes `RT_KERNEL_MALLOC/RT_KERNEL_FREE` to allocate memory from the system heap maintained by RT-Thread. When using `heap_3` the heap size is controlled by RT-Thread BSP configurations and you cannot change it by setting `configTOTAL_HEAP_SIZE` in `FreeRTOSConfig.h`.
If you want to use other heap allocation algorithms you need to modify `FreeRTOS/sSConscript` and choose the source file accordingly

```c
#You can replace heap_3 with heap_1, etc
src += Glob(os.path.join("portable", "MemMang", "heap_3.c"))
```
When using algorithms other than `heap_3` you can set the heap size using `configTOTAL_HEAP_SIZE` in `FreeRTOS/portable/rt-thread/FreeRTOSConfig.h`. When using `pvPortMalloc/vPortFree`, memory is allocated from a heap maintained by the FreeRTOS wrapper, separate from the RT-Thread system heap, with a size of `configTOTAL_HEAP_SIZE`. Other memory allocations without invoking `pvPortMalloc/vPortFree`, including the allocations made by the FreeRTOS wrapper APIs internally, are satisfied using the RT-Thread system heap.

### 3.5 Task Priority
In RT-Thread a smaller numerical value indicates a higher task priority. On the contrary, in FreeRTOS a larger numerical value indicates a higher priority. When using the FreeRTOS wrapper APIs such as `xTaskCreate`, task priority is specified using FreeRTOS rules.Care needs to be taken when both RT-Thread and FreeRTOS task APIs are used in the same application. You can use the following two macros to convert between RT-Thread and FreeRTOS task priority:
```c
#define FREERTOS_PRIORITY_TO_RTTHREAD(priority)    ( configMAX_PRIORITIES - 1 - ( priority ) )
#define RTTHREAD_PRIORITY_TO_FREERTOS(priority)    ( RT_THREAD_PRIORITY_MAX - 1 - ( priority ) )
```

### 3.6 Task Stack Size
The unit of FreeRTOS task stack size is `sizeof(StackType_t)`, while it is `sizeof(rt_uint8_t)` for RT-Thread. Do not confuse between the two and stick to FreeRTOS rules when creating tasks using FreeRTOS APIs.

### 3.7 vTaskStartScheduler
The startup procedure of RT-Thread is different from FreeRTOS. When using the FreeRTOS wrapper, the `main` function is run in the context of a task, whose priority is `CONFIG_RT_MAIN_THREAD_PRIORITY`. (This is specified using RT-Thread SCons configuration. A smaller numerical value indicates a higher priority.) At this time the scheduler is already started. The code for creating a task and starting the scheduler normally looks like the following in a FreeRTOS application：

```c
xTaskCreate(pxTask1Code, ......);
xTaskCreate(pxTask2Code, ......);
......
vTaskStartScheduler();
```

When using the application compatibility layer, if you use `xTaskCreate` to create any task that has a priority higher than `CONFIG_RT_MAIN_THREAD_PRIORITY`, it will execute immediately. `vTaskStartScheduler` is only a dummy function. Care needs to be taken when creating tasks using the FreeRTOS wrapper. You need to make sure all resources needed for the task are initialized and the task can execute normally when creating tasks using  `xTaskCreate`.

## 4 Usage

First use Env tool or RT-Thread Studio to add the FreeRTOS wrapper to your project:

```shell
RT-Thread online packages
    system packages --->
        [*] FreeRTOS Wrapper --->
            Version (latest)
```

These configuration operations will impact the FreeRTOS wrapper

```c
RT_USING_TIMER_SOFT /* Must enable when using FreeRTOS timers */
RT_TIMER_THREAD_PRIO  /* Timer task priority. Smaller numerical value indicates higher priority. */
RT_TIMER_THREAD_STACK_SIZE  /* Timer thread stack size. Unit is sizeof(rt_uint8_t). */
RT_USING_MUTEX  /* Must enable when using FreeRTOS mutexes */
RT_USING_SEMAPHORE  /* Must enable when using FreeRTOS semaphores */
RT_USING_HEAP /* Must enable when using FreeRTOS heap or dynamic allocation */
RT_TICK_PER_SECOND  /* equivalent to FreeRTOS configTICK_RATE_HZ */
RT_THREAD_PRIORITY_MAX /* equivalent to FreeRTOS configMAX_PRIORITIES */
RT_NAME_MAX /* equivalent to FreeRTOS configMAX_TASK_NAME_LEN */
```

You can find a `FreeRTOSConfig.h` template in `FreeRTOS/portable/rt-thread`. Most configuration options are read-only or depend on RT-Thread configurations. Those can be modified are detailed below:

```c
/* You can choose not to use recursive mutex */
#ifdef RT_USING_MUTEX
    #define configUSE_RECURSIVE_MUTEXES         1
    #define configUSE_MUTEXES                   1
#endif

/* You can choose not to use counting semaphore */
#ifdef RT_USING_SEMAPHORE
    #define configUSE_COUNTING_SEMAPHORES       1
#endif

/* If not using heap_3, you can configure heap size using configTOTAL_HEAP_SIZE */
#define configSUPPORT_STATIC_ALLOCATION         1
#ifdef RT_USING_HEAP
    #define configSUPPORT_DYNAMIC_ALLOCATION    1
    #define configTOTAL_HEAP_SIZE               10240
    #define configAPPLICATION_ALLOCATED_HEAP    0
#endif

#define configMINIMAL_STACK_SIZE                128

/* Optional functions or features */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetIdleTaskHandle          1
#define INCLUDE_xTaskAbortDelay                 1
#define INCLUDE_xSemaphoreGetMutexHolder        1
#define INCLUDE_xTaskGetHandle                  1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_uxTaskGetStackHighWaterMark2    1
#define INCLUDE_eTaskGetState                   1
#define INCLUDE_xTaskResumeFromISR              1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define configUSE_APPLICATION_TASK_TAG          1
#define configUSE_TASK_NOTIFICATIONS            1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES   3
```
Some examples are provided under the `test` directory. You can copy them to the `application` folder under the BSP directory. After building with SCons and downloading the application, you can enter relevant msh commands from the serial monitor and observe the behavior of these examples:
```shell
msh />queue_dynamic
Task 1 receive data 0 from queue
Task 1 receive data 1 from queue
Task 1 receive data 2 from queue
Task 1 receive data 3 from queue
Task 1 receive data 4 from queue
Task 1 receive data 5 from queue
Task 1 receive data 6 from queue
Task 1 receive data 7 from queue
Task 1 receive data 8 from queue
Task 1 receive data 9 from queue
Task 1 receive data 10 from queue
```

## 5 Reference
RT-Thread documentation [https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/README](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/README)

FreeRTOS documentation [https://www.freertos.org/a00106.html](https://www.freertos.org/a00106.html)

## 6 Maintaining

Homepage：https://github.com/RT-Thread-packages/FreeRTOS-Wrapper

Maintainer：[Zhaozhou Tang](https://github.com/tangzz98)
