/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2020/08/06
 * Description        : 外设从机应用主函数及任务系统初始化
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include "config.h"
#include "hal.h"
#include "gattprofile.h"
#include "peripheral.h"

#include <openwch_platform.h>
#include "board.h"
#include "wch_log.h"

/*********************************************************************
 * GLOBAL TYPEDEFS
 */
__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];

#if (defined(BLE_MAC)) && (BLE_MAC == TRUE)
const uint8_t MacAddr[6] = {0x84, 0xC2, 0xE4, 0x03, 0x02, 0x02};
#endif

/*********************************************************************
 * @fn      Main_Circulation
 *
 * @brief   主循环
 *
 * @return  none
 */
__HIGH_CODE
__attribute__((noinline)) void Main_Circulation()
{
    while (1) {
        TMOS_SystemProcess();
    }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   主函数
 *
 * @return  none
 */
int main(void)
{
#if (defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg(ENABLE);
#endif
#if (defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
#endif

    // Initialize board (includes clock and UART for logging)
    board_init();

    WCH_LOGI("BLE Peripheral Starting...");
    WCH_LOGI("Library Version: %s", VER_LIB);

#if CONFIG_SOC_CH59X
    CH59x_BLEInit();
#endif
#if CONFIG_SOC_CH58X
    CH58x_BLEInit();
#endif

    uint8_t MacAddr[6];
    GetMACAddress(MacAddr);
    // WCH_LOG_HEXDUMP("MAC Address", MacAddr, sizeof(MacAddr));
    WCH_LOGI("BLE MAC Address: %02X:%02X:%02X:%02X:%02X:%02X",
                MacAddr[5], MacAddr[4], MacAddr[3],
                MacAddr[2], MacAddr[1], MacAddr[0]);
    WCH_LOGI("BLE Init OK");

    HAL_Init();
    WCH_LOGI("HAL Init OK");

    GAPRole_PeripheralInit();
    WCH_LOGI("GAPRole Init OK");

    Peripheral_Init();
    WCH_LOGI("Peripheral Init OK, entering main loop");

    Main_Circulation();
    return 0;
}

/******************************** endfile @ main ******************************/
