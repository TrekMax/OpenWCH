# Board-specific sources/includes for CH59x EVB

target_include_directories(openwch_board PUBLIC
    ${OPENWCH_BOARD_DIR}/
    ${OPENWCH_BOARD_DIR}/hal/include/
    ${OPENWCH_SOC_DIR}/include/
    ${OPENWCH_SOC_DIR}/rvmsis/
)

target_sources(openwch_board PRIVATE
    ${OPENWCH_BOARD_DIR}/board.c
)

# HAL sources are only needed when BLE support is enabled
if(CONFIG_BLE_SUPPORT)
    target_sources(openwch_board PRIVATE
        ${OPENWCH_BOARD_DIR}/hal/mcu.c
        ${OPENWCH_BOARD_DIR}/hal/led.c
        ${OPENWCH_BOARD_DIR}/hal/key.c
        ${OPENWCH_BOARD_DIR}/hal/rtc.c
        ${OPENWCH_BOARD_DIR}/hal/sleep.c
    )
    target_link_libraries(openwch_board PUBLIC openwch_config openwch_ble wch_log)
else()
    target_link_libraries(openwch_board PUBLIC openwch_config wch_log)
endif()
