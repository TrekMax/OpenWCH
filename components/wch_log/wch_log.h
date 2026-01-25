/**
 * @file wch_log.h
 * @brief 
 * @version 0.1
 * @date 2026-01-23
 * 
 * @copyright Copyright (c) 2026 OpenWCH Team
 * 
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __WCH_LOG_H__
#define __WCH_LOG_H__

#include <stdio.h>

#if defined(CONFIG_WCH_LOG_UART)
#define WCH_LOG_UART
#elif defined(CONFIG_WCH_LOG_RTT)
#define WCH_LOG_RTT
#elif defined(CONFIG_WCH_LOG_CDC)
#define WCH_LOG_CDC
#endif

#if defined(WCH_LOG_UART)
#define WCH_LOG_PRINTF printf
#define WCH_LOG(fmt, ...) WCH_LOG_PRINTF(fmt "\n" LOG_RESET_COLOR, ##__VA_ARGS__)
#elif defined(WCH_LOG_RTT)
#define WCH_LOG_PRINTF(fmt, ...) SEGGER_RTT_printf(0, fmt, ##__VA_ARGS__)
#define WCH_LOG(fmt, ...) SEGGER_RTT_printf(0, fmt "\n" LOG_RESET_COLOR, ##__VA_ARGS__)
#elif defined(WCH_LOG_CDC)
#define WCH_LOG_PRINTF usb_printf
#define WCH_LOG(fmt, ...) usb_printf(fmt "\n" LOG_RESET_COLOR, ##__VA_ARGS__)

#else
#define WCH_LOG_PRINTF
#define WCH_LOG(fmt, ...)
#endif


#define WCH_LOG_DEBUG(msg, ...) WCH_LOG("[%s:%d|%s] " msg, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define WCH_LOG_LEVEL_NONE      0
#define WCH_LOG_LEVEL_ERROR     1
#define WCH_LOG_LEVEL_WARN      2
#define WCH_LOG_LEVEL_INFO      3
#define WCH_LOG_LEVEL_DEBUG     4
#define WCH_LOG_LEVEL_VERBOSE   5

#if defined(CONFIG_WCH_LOG_LEVEL_NONE)
#define WCH_LOG_LEVEL WCH_LOG_LEVEL_NONE
#elif defined(CONFIG_WCH_LOG_LEVEL_ERROR)
#define WCH_LOG_LEVEL WCH_LOG_LEVEL_ERROR
#elif defined(CONFIG_WCH_LOG_LEVEL_WARN)
#define WCH_LOG_LEVEL WCH_LOG_LEVEL_WARN
#elif defined(CONFIG_WCH_LOG_LEVEL_INFO)
#define WCH_LOG_LEVEL WCH_LOG_LEVEL_INFO
#elif defined(CONFIG_WCH_LOG_LEVEL_VERBOSE)
#define WCH_LOG_LEVEL WCH_LOG_LEVEL_VERBOSE
#else
#define WCH_LOG_LEVEL WCH_LOG_LEVEL_DEBUG
#endif

#ifndef CONFIG_WCH_LOG_COLORS
#define CONFIG_WCH_LOG_COLORS 1
#endif

/* clang-format off */
#if CONFIG_WCH_LOG_COLORS
#define LOG_COLOR_BLACK   "30"
#define LOG_COLOR_RED     "31"
#define LOG_COLOR_GREEN   "32"
#define LOG_COLOR_BROWN   "33"
#define LOG_COLOR_BLUE    "34"
#define LOG_COLOR_PURPLE  "35"
#define LOG_COLOR_CYAN    "36"
#define LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define LOG_RESET_COLOR   "\033[0m"
#define LOG_COLOR_E       LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W       LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I       LOG_COLOR(LOG_COLOR_GREEN)
#define LOG_COLOR_D
#define LOG_COLOR_V
#else /* CONFIG_LOG_COLORS */
#define LOG_COLOR_E
#define LOG_COLOR_W
#define LOG_COLOR_I
#define LOG_COLOR_D
#define LOG_COLOR_V
#define LOG_COLOR(COLOR)
#define LOG_RESET_COLOR
#endif /* CONFIG_LOG_COLORS */


#define WCH_LOGE(fmt, ...)     do {if (WCH_LOG_LEVEL >= WCH_LOG_LEVEL_ERROR)  { WCH_LOG(LOG_COLOR_E "ERR:"fmt,##__VA_ARGS__);}} while(0)
#define WCH_LOGW(fmt, ...)     do {if (WCH_LOG_LEVEL >= WCH_LOG_LEVEL_WARN)   { WCH_LOG(LOG_COLOR_W "WRN:"fmt,##__VA_ARGS__);}} while(0)
#define WCH_LOGI(fmt, ...)     do {if (WCH_LOG_LEVEL >= WCH_LOG_LEVEL_INFO)   { WCH_LOG(LOG_COLOR_I "INF:"fmt,##__VA_ARGS__);}} while(0)
#define WCH_LOGD(fmt, ...)     do {if (WCH_LOG_LEVEL >= WCH_LOG_LEVEL_DEBUG)  { WCH_LOG("DBG:"fmt,##__VA_ARGS__);}} while(0)
#define WCH_LOGV(fmt, ...)     do {if (WCH_LOG_LEVEL >= WCH_LOG_LEVEL_VERBOSE){ WCH_LOG("VBS:"fmt,##__VA_ARGS__);}} while(0)

void wch_log_hexdump(const char *tag, uint8_t *buf, uint32_t length);

#define WCH_LOG_HEXDUMP(tag, buf, length) wch_log_hexdump(tag, buf, length)


#endif /* __WCH_LOG_H__ */