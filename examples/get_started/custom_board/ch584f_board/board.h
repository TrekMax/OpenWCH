#pragma once

#include "CH58x_common.h"

void board_clock_init(void);
void board_pinmux_init(void);
void board_init(void);

/* EVB pin assignments */
#define BOARD_LED_PIN GPIO_Pin_8

/* LED helpers */
void board_led_on(void);
void board_led_off(void);
void board_led_toggle(void);
