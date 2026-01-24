cmake_minimum_required(VERSION 3.20)
project(ble_peripheral C CXX ASM)

set(OPENWCH_SDK_PATH "$ENV{OPENWCH_SDK_PATH}" CACHE PATH "Path to OpenWCH SDK root")
if(NOT OPENWCH_SDK_PATH)
    get_filename_component(OPENWCH_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/../../.." REALPATH)
endif()

include(${OPENWCH_SDK_PATH}/cmake/openwch_sdk.cmake)

openwch_add_source(ble_peripheral
    SOURCES
        main.c
        app/peripheral.c
        profile/devinfoservice.c
        profile/gattprofile.c
)

target_include_directories(ble_peripheral PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/app/
    ${CMAKE_CURRENT_SOURCE_DIR}/profile/
    ${OPENWCH_SDK_PATH}/components/soc_ch58x/libs
)
