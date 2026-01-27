cmake_minimum_required(VERSION 3.20)
project(custom_board C CXX ASM)

set(OPENWCH_SDK_PATH "$ENV{OPENWCH_SDK_PATH}" CACHE PATH "Path to OpenWCH SDK root")
if(NOT OPENWCH_SDK_PATH)
    get_filename_component(OPENWCH_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/../../.." REALPATH)
endif()

include(${OPENWCH_SDK_PATH}/cmake/openwch_sdk.cmake)

# Use project-defined openwch_board when CONFIG_BOARD_CUSTOM is enabled.
openwch_project_setup()
include(${CMAKE_CURRENT_LIST_DIR}/ch584f_board/board.cmake)

openwch_add_source(custom_board
    SOURCES main.c
)
