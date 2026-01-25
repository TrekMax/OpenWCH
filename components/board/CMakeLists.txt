add_library(openwch_board INTERFACE)

# Select board by Kconfig
if(CONFIG_BOARD_CH58X_EVB)
    set(OPENWCH_BOARD_DIR "${CMAKE_CURRENT_LIST_DIR}/ch585_evb")
    set(OPENWCH_SOC_DIR "${CMAKE_CURRENT_LIST_DIR}/../soc_ch58x")
elseif(CONFIG_BOARD_CH59X_EVB)
    set(OPENWCH_BOARD_DIR "${CMAKE_CURRENT_LIST_DIR}/ch592_evb")
    set(OPENWCH_SOC_DIR "${CMAKE_CURRENT_LIST_DIR}/../soc_ch59x")
else()
    message(FATAL_ERROR "No board selected: enable CONFIG_BOARD_CH58X_EVB or CONFIG_BOARD_CH59X_EVB in Kconfig")
endif()

# Provide board headers and sources.
target_include_directories(openwch_board INTERFACE
    ${OPENWCH_BOARD_DIR}/
    ${OPENWCH_BOARD_DIR}/hal/include/
    ${OPENWCH_SOC_DIR}/include/
    ${OPENWCH_SOC_DIR}/rvmsis/
)

target_sources(openwch_board INTERFACE
    ${OPENWCH_BOARD_DIR}/board.c
)

# HAL sources are only needed when BLE support is enabled
if(CONFIG_BLE_SUPPORT)
    target_sources(openwch_board INTERFACE
        ${OPENWCH_BOARD_DIR}/hal/mcu.c
        ${OPENWCH_BOARD_DIR}/hal/led.c
        ${OPENWCH_BOARD_DIR}/hal/key.c
        ${OPENWCH_BOARD_DIR}/hal/rtc.c
        ${OPENWCH_BOARD_DIR}/hal/sleep.c
    )
endif()
