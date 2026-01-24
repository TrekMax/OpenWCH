#include <stdio.h>
#include <string.h>

#include <openwch_platform.h>
#include "board.h"

#include "FreeRTOS.h"
#include "task.h"

static void led_task(void *arg)
{
    (void)arg;
    for (;;) {
        board_led_on();
        printf("LED ON\r\n");
        vTaskDelay(pdMS_TO_TICKS(250));
        board_led_off();
        printf("LED OFF\r\n");
        vTaskDelay(pdMS_TO_TICKS(750));
    }
}

__attribute__((aligned(16)))
static StackType_t led_task_stack[configMINIMAL_STACK_SIZE + 64];
__attribute__((aligned(16)))
static StaticTask_t led_task_tcb;

__attribute__((aligned(16)))
static StaticTask_t idle_task_tcb;
__attribute__((aligned(16)))
static StackType_t idle_task_stack[configMINIMAL_STACK_SIZE];

__attribute__((aligned(16)))
static StaticTask_t timer_task_tcb;
__attribute__((aligned(16)))
static StackType_t timer_task_stack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
    *ppxIdleTaskTCBBuffer = &idle_task_tcb;
    *ppxIdleTaskStackBuffer = idle_task_stack;
    *pulIdleTaskStackSize = (uint32_t)configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
    *ppxTimerTaskTCBBuffer = &timer_task_tcb;
    *ppxTimerTaskStackBuffer = timer_task_stack;
    *pulTimerTaskStackSize = (uint32_t)configTIMER_TASK_STACK_DEPTH;
}

static void uart0_putc_timeout(char ch)
{
    uint32_t timeout = 0xFFFF;
    while ((R8_UART0_TFC == UART_FIFO_SIZE) && timeout--) {
    }
    if (timeout != 0) {
        R8_UART0_THR = (uint8_t)ch;
    }
}

static void log_uart(const char *msg)
{
    while (*msg) {
        uart0_putc_timeout(*msg++);
    }
}

static void log_hex_u32(uint32_t v);
static inline uint32_t read_mepc(void);
static inline uint32_t read_mcause(void);
static inline uint32_t read_mtval(void);
static inline uint32_t read_mstatus(void);

__attribute__((interrupt("WCH-Interrupt-fast")))
__attribute__((section(".highcode")))
void HardFault_Handler(void)
{
    uint32_t sp;
    __asm volatile("mv %0, sp" : "=r"(sp));
    log_uart("[fault] HardFault\r\n");
    log_uart("mepc=0x");
    log_hex_u32(read_mepc());
    log_uart(" mcause=0x");
    log_hex_u32(read_mcause());
    log_uart(" mtval=0x");
    log_hex_u32(read_mtval());
    log_uart(" mstatus=0x");
    log_hex_u32(read_mstatus());
    log_uart(" sp=0x");
    log_hex_u32(sp);
    log_uart("\r\n");
    while (1) {
    }
}

static void log_hex_u32(uint32_t v)
{
    const char *hex = "0123456789ABCDEF";
    for (int i = 7; i >= 0; --i) {
        uart0_putc_timeout(hex[(v >> (i * 4)) & 0xF]);
    }
}

static inline uint32_t read_mepc(void)
{
    uint32_t v;
    __asm volatile("csrr %0, mepc" : "=r"(v));
    return v;
}

static inline uint32_t read_mcause(void)
{
    uint32_t v;
    __asm volatile("csrr %0, mcause" : "=r"(v));
    return v;
}

static inline uint32_t read_mtval(void)
{
    uint32_t v;
    __asm volatile("csrr %0, mtval" : "=r"(v));
    return v;
}

static inline uint32_t read_mstatus(void)
{
    uint32_t v;
    __asm volatile("csrr %0, mstatus" : "=r"(v));
    return v;
}

void vAssertCalled( const char *file, int line )
{
    log_uart("[assert] ");
    if (file != NULL) {
        log_uart(file);
    }
    log_uart(":");
    if (line >= 0) {
        char num[12];
        int idx = 0;
        if (line == 0) {
            num[idx++] = '0';
        } else {
            int v = line;
            char tmp[12];
            int t = 0;
            while (v > 0 && t < (int)sizeof(tmp)) {
                tmp[t++] = (char)('0' + (v % 10));
                v /= 10;
            }
            while (t > 0) {
                num[idx++] = tmp[--t];
            }
        }
        num[idx] = '\0';
        log_uart(num);
    }
    log_uart("\r\n");
}

int main(void)
{
    board_init();
    setvbuf(stdout, NULL, _IONBF, 0);
    log_uart("[boot] start\r\n");
    printf("Compiled on %s at %s\r\n", __DATE__, __TIME__);
    log_uart("[boot] led_task_stack @ 0x");
    log_hex_u32((uint32_t)led_task_stack);
    log_uart("\r\n");
    log_uart("[boot] led_task_tcb   @ 0x");
    log_hex_u32((uint32_t)&led_task_tcb);
    log_uart("\r\n");
    log_uart("[boot] idle_stack     @ 0x");
    log_hex_u32((uint32_t)idle_task_stack);
    log_uart("\r\n");
    log_uart("[boot] idle_tcb       @ 0x");
    log_hex_u32((uint32_t)&idle_task_tcb);
    log_uart("\r\n");
    log_uart("[boot] timer_stack    @ 0x");
    log_hex_u32((uint32_t)timer_task_stack);
    log_uart("\r\n");
    log_uart("[boot] timer_tcb      @ 0x");
    log_hex_u32((uint32_t)&timer_task_tcb);
    log_uart("\r\n");

    log_uart("[boot] before xTaskCreateStatic\r\n");
    TaskHandle_t led_handle = xTaskCreateStatic(
        led_task,
        "led",
        configMINIMAL_STACK_SIZE + 64,
        NULL,
        tskIDLE_PRIORITY + 1,
        led_task_stack,
        &led_task_tcb
    );
    log_uart("[boot] after xTaskCreateStatic\r\n");
    
    if (led_handle == NULL) {
        printf("Failed to create led_task\r\n");
        while (1);
    } else {
        printf("led_task created\r\n");
    }

    log_uart("[boot] before vTaskStartScheduler\r\n");
    vTaskStartScheduler();
    log_uart("[boot] vTaskStartScheduler returned\r\n");

    while (1) {
    }
}
