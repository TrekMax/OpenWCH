#include "board.h"

#if defined(CONFIG_DEBUG_UART0) || defined(CONFIG_DEBUG_UART1) || defined(CONFIG_DEBUG_UART2) || defined(CONFIG_DEBUG_UART3)
#define PRINTF_ENABLE 1
#else
#define PRINTF_ENABLE 0
#endif

#if !PRINTF_ENABLE
#define printf(...) do {} while(0)
#endif

#if PRINTF_ENABLE
#include <stdio.h>
#endif

/* Basic board bring-up for CH58x EVB. */

void board_clock_init(void)
{
    /* Use external crystal with default capacitance and run PLL to SYSCLK_FREQ. */
    HSECFG_Capacitance(HSECap_18p);
    SetSysClock(SYSCLK_FREQ);
}

void board_pinmux_init(void)
{
#if defined(CONFIG_DEBUG_UART0)
    /* UART0 remap to PA14/PA15 for console prints. */
    GPIOA_SetBits(GPIO_Pin_14);
    GPIOPinRemap(ENABLE, RB_PIN_UART0);
    GPIOA_ModeCfg(GPIO_Pin_15, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_14, GPIO_ModeOut_PP_5mA);
    UART0_DefInit();
#elif defined(CONFIG_DEBUG_UART1)
    /* UART1 configuration - add if needed */
#endif

    /* User LED (silkscreen D6) on PB15. Default off (set high). */
    GPIOB_SetBits(BOARD_LED_PIN);
    GPIOB_ModeCfg(BOARD_LED_PIN, GPIO_ModeOut_PP_5mA);
}

void board_init(void)
{
#if PRINTF_ENABLE
    setvbuf(stdout, NULL, _IONBF, 0);
#endif

    board_clock_init();
    board_pinmux_init();
}

void board_led_on(void)
{
    GPIOB_ResetBits(BOARD_LED_PIN);
}

void board_led_off(void)
{
    GPIOB_SetBits(BOARD_LED_PIN);
}

void board_led_toggle(void)
{
    R32_PB_OUT ^= BOARD_LED_PIN;
}
