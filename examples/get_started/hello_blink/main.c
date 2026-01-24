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
#include "version.h"
#include "wch_log.h"

int main(void)
{
    board_init();
    WCH_LOG_PRINTF("hello blink\r\n");
    WCH_LOG_PRINTF("Compiled on %s at %s\r\n", __DATE__, __TIME__);

    int counter = 0;
    while (1) {
        WCH_LOG_PRINTF("Toggling LED %d\r\n", counter++);
        board_led_toggle();
        mDelaymS(500);
    }
    return 0;
}
