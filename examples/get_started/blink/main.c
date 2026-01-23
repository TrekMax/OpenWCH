/**
 * @file main.c
 * @brief 
 * @version 0.1
 * @date 2026-01-23
 * 
 * @copyright Copyright (c) 2026 OpenWCH Team
 * 
 * SPDX-License-Identifier: Apache-2.0
 */
#include <openwch_platform.h>
#include "board.h"

int main(void)
{
    board_init();
    /* Blink LED at ~1 Hz. */
    while (1) {
        board_led_on();
        mDelaymS(600);
        board_led_off();
        mDelaymS(300);
    }
    return 0;
}
