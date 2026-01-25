cmake_minimum_required(VERSION 3.20)
project(rf_phy_hop C CXX ASM)

set(OPENWCH_SDK_PATH "$ENV{OPENWCH_SDK_PATH}" CACHE PATH "Path to OpenWCH SDK root")
if(NOT OPENWCH_SDK_PATH)
    get_filename_component(OPENWCH_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/../../.." REALPATH)
endif()

include(${OPENWCH_SDK_PATH}/cmake/openwch_sdk.cmake)

openwch_add_source(rf_phy_hop
    SOURCES
        main.c
        app/rf_phy.c
)

target_include_directories(rf_phy_hop PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/app/
)
