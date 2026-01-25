/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description        :
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include <openwch_platform.h>
#include <stdint.h>
#include "rf_phy.h"
#include "config.h"
#include "wch_log.h"

/*********************************************************************
 * GLOBAL TYPEDEFS
 */

#define RF_AUTO_MODE_EXAM 1
#define RF_MODE_TX_RX     0 // 0: TX  1: RX

uint8_t taskID;
uint8_t TX_DATA[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

volatile uint8_t tx_end_flag = 0;
volatile uint8_t rx_end_flag = 0;

/*********************************************************************
 * @fn      RF_Wait_Tx_End
 *
 * @brief
 * 手动模式等待发送完成，自动模式等待发送-接收完成，必须在RAM中等待，等待时可以执行用户代码，但需要注意执行的代码必须运行在RAM中，否则影响发送
 *
 * @return  none
 */
__HIGH_CODE
__attribute__((noinline)) void RF_Wait_Tx_End()
{
    uint32_t i = 0;
    while (!tx_end_flag) {
        i++;
        __nop();
        __nop();
        // 约5ms超时
        if (i > (FREQ_SYS / 1000)) {
            tx_end_flag = TRUE;
        }
    }
}

/*********************************************************************
 * @fn      RF_Wait_Rx_End
 *
 * @brief
 * 自动模式等待应答发送完成，必须在RAM中等待，等待时可以执行用户代码，但需要注意执行的代码必须运行在RAM中，否则影响发送
 *
 * @return  none
 */
__HIGH_CODE
__attribute__((noinline)) void RF_Wait_Rx_End()
{
    uint32_t i = 0;
    while (!rx_end_flag) {
        i++;
        __nop();
        __nop();
        // 约5ms超时
        if (i > (FREQ_SYS / 1000)) {
            rx_end_flag = TRUE;
        }
    }
}

uint32_t tx_packet_count = 0;
uint32_t rx_packet_count = 0;
uint32_t crc_error_count = 0;

/*********************************************************************
 * @fn      RF_2G4StatusCallBack
 *
 * @brief   RF 状态回调，此函数在中断中调用。注意：不可在此函数中直接调用RF接收或者发送API，需要使用事件的方式调用
 *          在此回调中直接使用或调用函数涉及到的变量需注意，此函数在中断中调用。
 *
 * @param   sta     - 状态类型
 * @param   crc     - crc校验结果
 * @param   rxBuf   - 数据buf指针
 *
 * @return  none
 */
void RF_2G4StatusCallBack(uint8_t sta, uint8_t crc, uint8_t *rxBuf)
{
    switch (sta) {
    case TX_MODE_TX_FINISH: {
#if (!RF_AUTO_MODE_EXAM)
        tx_end_flag = TRUE;
#endif
        break;
    }
    case TX_MODE_TX_FAIL: {
        tx_end_flag = TRUE;
        break;
    }
    case TX_MODE_RX_DATA: {
#if (RF_AUTO_MODE_EXAM)
        tx_end_flag = TRUE;
        if (crc == 0) {
            tx_packet_count++;
            // RSSI | LEN | DATA
            int8_t rssi = (int8_t)rxBuf[0];
            uint8_t txRecvBuffer_len = rxBuf[1];
            uint8_t *txRecvBuffer = &rxBuf[2];
            WCH_LOGI("TX Recv RSSI:%ddB, len:%d", rssi, txRecvBuffer_len);
            // WCH_LOG_HEXDUMP("TX Recv Data", txRecvBuffer, txRecvBuffer_len);
            for (uint8_t i = 0; i < rxBuf[1]; i++) {
                WCH_LOG_PRINTF("%x ", txRecvBuffer[i]);
            }
            WCH_LOG_PRINTF("\n");
        } else {
            if (crc & (1 << 0)) {
                crc_error_count++;
                WCH_LOGW("RX crc error [%d:%d]", tx_packet_count, crc_error_count);
            }

            if (crc & (1 << 1)) {
                WCH_LOGW("match type error");
            }
        }
#endif
        break;
    }
    case TX_MODE_RX_TIMEOUT: // Timeout is about 200us
    {
#if (RF_AUTO_MODE_EXAM)
        tx_end_flag = TRUE;
#endif
        break;
    }
    case RX_MODE_RX_DATA: {
        if (crc == 0) {
            rx_packet_count++;
#if (RF_AUTO_MODE_EXAM)
            RF_Wait_Rx_End();
#endif
            int8_t rssi = (int8_t)rxBuf[0];
            uint8_t rxbuffer_len = rxBuf[1];
            uint8_t *rxbuffer = &rxBuf[2];
            WCH_LOGI("RX Recv RSSI:%ddB, len:%d", rssi, rxbuffer_len);
            // WCH_LOG_HEXDUMP("RX Recv Data", rxbuffer, rxbuffer_len);
        } else {
            if (crc & (1 << 0)) {
                crc_error_count++;
                WCH_LOGW("RX crc error [%d:%d]", rx_packet_count, crc_error_count);
            }

            if (crc & (1 << 1)) {
                WCH_LOGW("match type error");
            }
        }
#if (!RF_AUTO_MODE_EXAM)
        tmos_set_event(taskID, SBP_RF_RF_RX_EVT);
#endif
        break;
    }
    case RX_MODE_TX_FINISH: {
#if (RF_AUTO_MODE_EXAM)
        rx_end_flag = TRUE;
        tmos_set_event(taskID, SBP_RF_RF_RX_EVT);
#endif
        break;
    }
    case RX_MODE_TX_FAIL: {
#if (RF_AUTO_MODE_EXAM)
        rx_end_flag = TRUE;
        tmos_set_event(taskID, SBP_RF_RF_RX_EVT);
#endif
        break;
    }
    }
}

/*********************************************************************
 * @fn      RF_ProcessEvent
 *
 * @brief   RF 事件处理
 *
 * @param   task_id - 任务ID
 * @param   events  - 事件标志
 *
 * @return  未完成事件
 */
uint16_t RF_ProcessEvent(uint8_t task_id, uint16_t events)
{
    if (events & SYS_EVENT_MSG) {
        uint8_t *pMsg;

        if ((pMsg = tmos_msg_receive(task_id)) != NULL) {
            // Release the TMOS message
            tmos_msg_deallocate(pMsg);
        }
        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }
    if (events & SBP_RF_START_DEVICE_EVT) {
        tmos_start_task(taskID, SBP_RF_PERIODIC_EVT, 1000);
        return events ^ SBP_RF_START_DEVICE_EVT;
    }
    if (events & SBP_RF_PERIODIC_EVT) {
        RF_Shut();
        tx_end_flag = FALSE;
        if (!RF_Tx(TX_DATA, 10, 0xFF, 0xFF)) {
            RF_Wait_Tx_End();
        }
        tmos_start_task(taskID, SBP_RF_PERIODIC_EVT, 1000);
        return events ^ SBP_RF_PERIODIC_EVT;
    }
    if (events & SBP_RF_RF_RX_EVT) {
        uint8_t state;
        RF_Shut();
        TX_DATA[9]++;
#if (RF_AUTO_MODE_EXAM)
        rx_end_flag = FALSE;
#endif
        state = RF_Rx(TX_DATA, 10, 0xFF, 0xFF);
        WCH_LOGI("RX mode.state = %x", state);
        return events ^ SBP_RF_RF_RX_EVT;
    }
    return 0;
}

/*********************************************************************
 * @fn      RF_Init
 *
 * @brief   RF 初始化
 *
 * @return  none
 */
void RF_Init(void)
{
    uint8_t state;
    rfConfig_t rf_Config;

    tmos_memset(&rf_Config, 0, sizeof(rfConfig_t));
    taskID = TMOS_ProcessEventRegister(RF_ProcessEvent);
    // accessAddress 禁止使用 0x55555555 以及 0xAAAAAAAA ( 建议不超过 24 次位反转，且不超过连续的 6 个 0 或 1 )
    rf_Config.accessAddress = 0x71764129;
    rf_Config.CRCInit = 0x555555;
    rf_Config.Channel = 39;
    rf_Config.Frequency = 2480000;
#if (RF_AUTO_MODE_EXAM)
    rf_Config.LLEMode = LLE_MODE_AUTO;
#else
    rf_Config.LLEMode =
        LLE_MODE_BASIC | LLE_MODE_EX_CHANNEL; // 使能 LLE_MODE_EX_CHANNEL 表示 选择 rf_Config.Frequency 作为通信频点
#endif
    rf_Config.rfStatusCB = RF_2G4StatusCallBack;
    rf_Config.RxMaxlen = 251;
    state = RF_Config(&rf_Config);
    WCH_LOGI("RF 2.4g init: %x", state);
#if (RF_MODE_TX_RX == 1)
    {
        // RX mode
#if (RF_AUTO_MODE_EXAM)
        rx_end_flag = FALSE;
#endif
        state = RF_Rx(TX_DATA, 10, 0xFF, 0xFF);
        //    WCH_LOGI("RX mode.state = %x", state);
    }
#else
    { // TX mode
        tmos_set_event(taskID, SBP_RF_PERIODIC_EVT);
    }
#endif
}

/******************************** endfile @ main ******************************/
