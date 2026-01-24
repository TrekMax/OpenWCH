/********************************** (C) COPYRIGHT *******************************
 * File Name          : ble_utils.h
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2026/01/18
 * Description        : BLE advertising data utilities (Zephyr-compatible)
 *                      Provides Zephyr-style API for defining BLE advertising data
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#ifndef __BLE_UTILS_H
#define __BLE_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/*********************************************************************
 * BLE Advertising Data Structures (Zephyr-compatible)
 * These structures and macros provide a clean way to define BLE advertising data
 * 
 * Usage example:
 * 
 * static const struct bt_data ad[] = {
 *     BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
 *     BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1),
 * };
 * 
 * uint8_t adv_buf[31];
 * size_t adv_len = bt_data_to_ltv(adv_buf, sizeof(adv_buf), ad, ARRAY_SIZE(ad));
 */

/**
 * @brief Bluetooth advertising data element (Zephyr-compatible)
 * @details Describes a single AD Type element in advertising/scan response data.
 *          Compatible with Zephyr RTOS bt_data structure.
 */
struct bt_data {
    /** Type of advertising data field */
    uint8_t type;
    /** Length of data field */
    uint8_t data_len;
    /** Pointer to data field payload */
    const uint8_t *data;
};

/**
 * @brief Helper to declare bt_data array elements (Zephyr-compatible)
 * @param _type Type of advertising data field
 * @param _data Pointer to the data field payload (can be string literal)
 * @param _data_len Number of octets in the data field
 * 
 * Example:
 *   BT_DATA(GAP_ADTYPE_LOCAL_NAME_COMPLETE, "Device Name", 11)
 */
#define BT_DATA(_type, _data, _data_len) \
    { \
        .type = (_type), \
        .data_len = (_data_len), \
        .data = (const uint8_t *)(_data), \
    }

/**
 * @brief Helper to declare bt_data with inline bytes (Zephyr-compatible)
 * @param _type Type of advertising data field
 * @param _bytes Variable number of single-byte parameters
 * 
 * Example:
 *   BT_DATA_BYTES(GAP_ADTYPE_FLAGS, 0x06)
 *   BT_DATA_BYTES(GAP_ADTYPE_16BIT_MORE, 0x0D, 0x18, 0x0F, 0x18)
 */
#define BT_DATA_BYTES(_type, _bytes...) \
    BT_DATA(_type, ((uint8_t[]){_bytes}), sizeof((uint8_t[]){_bytes}))

/**
 * @brief Helper macro to get array size
 * @param arr Array variable
 * @return Number of elements in the array
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/**
 * @brief Convert bt_data array to LTV byte array
 * @details This function converts a Zephyr-style bt_data structure array
 *          to LTV (Length-Type-Value) byte format suitable for BLE controllers.
 *          This is what Zephyr's bt_le_adv_start() does internally.
 * 
 * @param dest Destination buffer for LTV formatted data
 * @param dest_size Size of destination buffer (typically 31 bytes max)
 * @param src Source bt_data array
 * @param src_count Number of elements in source array
 * @return Number of bytes written to destination buffer
 * 
 * Example:
 *   uint8_t adv_buf[31];
 *   size_t len = bt_data_to_ltv(adv_buf, sizeof(adv_buf), ad, ARRAY_SIZE(ad));
 *   GAPRole_SetParameter(GAPROLE_ADVERT_DATA, len, adv_buf);
 */
static inline size_t bt_data_to_ltv(uint8_t *dest, size_t dest_size,
                                     const struct bt_data *src, size_t src_count)
{
    size_t pos = 0;

    for (size_t i = 0; i < src_count; i++) {
        // Check if there's enough space: Length(1) + Type(1) + Data
        if (pos + 2 + src[i].data_len > dest_size) {
            break;
        }

        // Write Length (Type + Data)
        dest[pos++] = src[i].data_len + 1;

        // Write Type
        dest[pos++] = src[i].type;

        // Write Data
        for (size_t j = 0; j < src[i].data_len; j++) {
            dest[pos++] = src[i].data[j];
        }
    }

    return pos;
}

#ifdef __cplusplus
}
#endif

#endif /* __BLE_UTILS_H */
