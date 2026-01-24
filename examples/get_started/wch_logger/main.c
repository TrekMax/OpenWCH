#include <openwch_platform.h>
#include "board.h"
#include "version.h"
#include "wch_log.h"

int main(void)
{
    board_init();
    WCH_LOGI("Compiled on %s at %s", __DATE__, __TIME__);
    
    // WCH_LOGI("Build commit: " TAG_VERSION);
    // WCH_LOGI("Author: " COMMIT_AUTHOR " <" AUTHOR_EMAIL ">");
    // WCH_LOGI("Commit hash: " COMMIT_HASH);
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
