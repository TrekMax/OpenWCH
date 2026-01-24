cmake_minimum_required(VERSION 3.20)
project(wch_logger C CXX ASM)

set(OPENWCH_SDK_PATH "$ENV{OPENWCH_SDK_PATH}" CACHE PATH "Path to OpenWCH SDK root")
if(NOT OPENWCH_SDK_PATH)
    get_filename_component(OPENWCH_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/../../.." REALPATH)
endif()

include(${OPENWCH_SDK_PATH}/cmake/openwch_sdk.cmake)

openwch_add_source(wch_logger
    SOURCES main.c
)
