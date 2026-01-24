/*
	FreeRTOS V202112.00 configuration for CH58x
*/

#pragma once

#include "CH58x_common.h"
#include <stdio.h>

/* See https://www.freertos.org/Using-FreeRTOS-on-RISC-V.html */

/* Don't use MTIME on this device. */
#define configMTIME_BASE_ADDRESS      ( 0 )
#define configMTIMECMP_BASE_ADDRESS   ( 0 )

#ifndef CONFIG_FREERTOS_TICK_RATE_HZ
#define CONFIG_FREERTOS_TICK_RATE_HZ 500
#endif

#define configUSE_PREEMPTION          1
#define configUSE_TIME_SLICING        0
#define configUSE_IDLE_HOOK           0
#define configUSE_TICK_HOOK           0
#define configCPU_CLOCK_HZ            FREQ_SYS
#define configTICK_RATE_HZ            ( ( TickType_t ) CONFIG_FREERTOS_TICK_RATE_HZ )
#define configMAX_PRIORITIES          ( 15 )
#define configMINIMAL_STACK_SIZE      ( ( unsigned short ) 128 )
#define configTOTAL_HEAP_SIZE         ( ( size_t ) ( 8 * 1024 ) )
#define configMAX_TASK_NAME_LEN       ( 16 )
#define configUSE_TRACE_FACILITY      0
#define configUSE_16_BIT_TICKS        0
#define configIDLE_SHOULD_YIELD       0
#define configUSE_MUTEXES             1
#define configQUEUE_REGISTRY_SIZE     8
#define configCHECK_FOR_STACK_OVERFLOW 0
#define configUSE_RECURSIVE_MUTEXES   1
#define configUSE_MALLOC_FAILED_HOOK  0
#define configUSE_APPLICATION_TASK_TAG 0
#define configUSE_COUNTING_SEMAPHORES 1
#define configGENERATE_RUN_TIME_STATS 0
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
/* Memory allocation options. */
#define configSUPPORT_STATIC_ALLOCATION  1
#define configSUPPORT_DYNAMIC_ALLOCATION 1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES         0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Software timer definitions. */
#define configUSE_TIMERS              1
#define configTIMER_TASK_PRIORITY     ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH      4
#define configTIMER_TASK_STACK_DEPTH  ( configMINIMAL_STACK_SIZE )

/* Include API functions. */
#define INCLUDE_vTaskPrioritySet          1
#define INCLUDE_uxTaskPriorityGet         1
#define INCLUDE_vTaskDelete               1
#define INCLUDE_vTaskCleanUpResources     1
#define INCLUDE_vTaskSuspend              1
#define INCLUDE_vTaskDelayUntil           1
#define INCLUDE_vTaskDelay                1
#define INCLUDE_eTaskGetState             1
#define INCLUDE_xTimerPendFunctionCall    1
#define INCLUDE_xTaskAbortDelay           1
#define INCLUDE_xTaskGetHandle            1
#define INCLUDE_xSemaphoreGetMutexHolder  1

/* Provide an ISR stack locally to avoid linker script edits. */
#define configISR_STACK_SIZE_WORDS    ( 256 )

/* Normal assert without relying on assert.h. */
void vAssertCalled( const char *file, int line );
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); vAssertCalled(__FILE__, __LINE__); while(1); }

/* Map to the platform printf function. */
#define configPRINT_STRING( pcString )  printf( pcString )
