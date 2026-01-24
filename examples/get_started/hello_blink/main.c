#include <stdio.h>
#include <openwch_platform.h>
#include "board.h"
#include "version.h"
#include "wch_log.h"

int main(void)
{
    board_init();
    WCH_LOG_PRINTF("hello blink");
    WCH_LOG_PRINTF("Compiled on %s at %s\r\n", __DATE__, __TIME__);

    int counter = 0;
    while (1) {
        WCH_LOG_PRINTF("Toggling LED %d\r\n", counter++);
        board_led_toggle();
        mDelaymS(500);
    }
    return 0;
}
