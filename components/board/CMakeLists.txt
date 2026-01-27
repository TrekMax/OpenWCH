if(CONFIG_BOARD_CUSTOM)
    if(NOT TARGET openwch_board)
        add_library(openwch_board STATIC)
    endif()

    # User project should configure sources/include dirs for openwch_board.
    if(CONFIG_BLE_SUPPORT)
        target_link_libraries(openwch_board PUBLIC openwch_config openwch_ble wch_log)
    else()
        target_link_libraries(openwch_board PUBLIC openwch_config wch_log)
    endif()
    return()
endif()

if(TARGET openwch_board)
    message(FATAL_ERROR "openwch_board target already defined; enable CONFIG_BOARD_CUSTOM to use a project-defined board")
endif()

add_library(openwch_board STATIC)

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

include(${OPENWCH_BOARD_DIR}/board.cmake)
