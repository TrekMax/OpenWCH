add_library(openwch_board INTERFACE)

# Default board: EVB
set(OPENWCH_BOARD_DIR "${CMAKE_CURRENT_LIST_DIR}/ch585_evb")

# Provide board headers and sources.
target_include_directories(openwch_board INTERFACE
    ${OPENWCH_BOARD_DIR}
    ${OPENWCH_BOARD_DIR}/hal/include
    ${CMAKE_CURRENT_LIST_DIR}/../soc_ch58x/include
    ${CMAKE_CURRENT_LIST_DIR}/../soc_ch58x/cmsis
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
