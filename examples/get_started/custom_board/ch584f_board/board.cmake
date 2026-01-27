# Project-defined board setup for custom_board example

if(CONFIG_SOC_CH58X)
    set(OPENWCH_CUSTOM_BOARD_DIR "${CMAKE_CURRENT_LIST_DIR}/")
    set(OPENWCH_CUSTOM_SOC_DIR "${OPENWCH_SDK_PATH}/components/soc_ch58x")
elseif(CONFIG_SOC_CH59X)
    set(OPENWCH_CUSTOM_BOARD_DIR "${OPENWCH_SDK_PATH}/components/board/ch592_evb")
    set(OPENWCH_CUSTOM_SOC_DIR "${OPENWCH_SDK_PATH}/components/soc_ch59x")
else()
    message(FATAL_ERROR "No SoC selected: enable CONFIG_SOC_CH58X or CONFIG_SOC_CH59X in Kconfig")
endif()

target_include_directories(openwch_board PUBLIC
    ${OPENWCH_CUSTOM_BOARD_DIR}/
    ${OPENWCH_CUSTOM_BOARD_DIR}/hal/include/
    ${OPENWCH_CUSTOM_SOC_DIR}/include/
    ${OPENWCH_CUSTOM_SOC_DIR}/rvmsis/
)

target_sources(openwch_board PRIVATE
    ${OPENWCH_CUSTOM_BOARD_DIR}/board.c
)

if(CONFIG_BLE_SUPPORT)
    target_sources(openwch_board PRIVATE
        ${OPENWCH_CUSTOM_BOARD_DIR}/hal/mcu.c
        ${OPENWCH_CUSTOM_BOARD_DIR}/hal/led.c
        ${OPENWCH_CUSTOM_BOARD_DIR}/hal/key.c
        ${OPENWCH_CUSTOM_BOARD_DIR}/hal/rtc.c
        ${OPENWCH_CUSTOM_BOARD_DIR}/hal/sleep.c
    )
endif()
