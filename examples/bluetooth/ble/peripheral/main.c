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

#if CONFIG_CUSTOM_BOARD_HAS_RF_PA
blePaControlConfig_t  blePaControlConfigIO; //必须是全局变量
#endif

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

#if CONFIG_CUSTOM_BOARD_HAS_RF_PA
    WCH_LOGI("Custom Board RF PA Control Init");
    // PA控制IO初始化(GPIOB4作为RX使能，GPIOB5作为TX使能)

    GPIOB_ModeCfg(GPIO_Pin_4|GPIO_Pin_5, GPIO_ModeOut_PP_5mA) ;//设置对应的 GPIO 为对应的拉高拉低寄存器 //对应的 Pin 是 bit
    
    blePaControlConfigIO.txEnableGPIO = (uint32_t)&R32_PB_OUT;
    blePaControlConfigIO.txDisableGPIO = (uint32_t)&R32_PB_CLR;
    blePaControlConfigIO.tx_pin = GPIO_Pin_5;

    blePaControlConfigIO.rxEnableGPIO = (uint32_t)&R32_PB_OUT;
    blePaControlConfigIO.rxDisableGPIO = (uint32_t)&R32_PB_CLR;
    blePaControlConfigIO.rx_pin = GPIO_Pin_4;
    
    BLE_PAControlInit(&blePaControlConfigIO);
#endif
    Main_Circulation();
    return 0;
}

/******************************** endfile @ main ******************************/
