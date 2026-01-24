#include <openwch_platform.h>
#include "board.h"
#include "version.h"
#include "wch_log.h"

int main(void)
{
    board_init();
    WCH_LOGI("Compiled on %s@%s", _COMPILE_DATE_, _COMPILE_TIME_);
    WCH_LOGI("Build Version: %s", BUILD_VERSION);
    WCH_LOGI("SDK Version: %s(%d.%d.%d)", SDK_VERSION_STRING, SDK_VERSION_MAJOR, SDK_VERSION_MINOR, SDK_PATCHLEVEL);
    WCH_LOGI("System initialized successfully");

    WCH_LOG_HEX_DUMP("Example", (uint8_t *)"Hello, world!", 13);

    int counter = 0;
    while (1) {
        WCH_LOGI("Toggling LED %d", counter++);
        board_led_toggle();
        mDelaymS(500);
    }
    return 0;
}
