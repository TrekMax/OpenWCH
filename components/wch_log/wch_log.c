/**
 * @file wch_log.c
 * @brief 
 * @version 0.1
 * @date 2026-01-23
 * 
 * @copyright Copyright (c) 2026 OpenWCH Team
 * 
 * SPDX-License-Identifier: Apache-2.0
 */
#include "wch_log.h"

#define __IS_ASCII(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')

void wch_log_hexdump(const char *tag, uint8_t *buf, uint32_t length)
{
    uint32_t i, j;
    WCH_LOG_PRINTF("%s | addr: 0x%08lX, length: %lu\r\n", tag, (unsigned long)buf, (unsigned long)length);

    for (i = 0; i < length; i += 16)
    {
        char line[96];
        int pos = 0;

        pos += snprintf(line + pos, sizeof(line) - pos, "\t%08lX: ", (unsigned long)i);

        for (j = 0; j < 16 && pos < (int)sizeof(line); j++)
        {
            if (i + j < length)
                pos += snprintf(line + pos, sizeof(line) - pos, "%02X ", buf[i + j]);
            else
                pos += snprintf(line + pos, sizeof(line) - pos, "   ");
        }

        pos += snprintf(line + pos, sizeof(line) - pos, LOG_COLOR(LOG_COLOR_GREEN) " | ");
        for (j = 0; j < 16 && pos < (int)sizeof(line); j++)
        {
            if (i + j < length)
                pos += snprintf(line + pos, sizeof(line) - pos, "%c", __IS_ASCII(buf[i + j]) ? buf[i + j] : '.');
        }
        pos += snprintf(line + pos, sizeof(line) - pos, LOG_RESET_COLOR " |\r\n");

        WCH_LOG_PRINTF("%s", line);
    }
}
 