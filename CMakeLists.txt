cmake_minimum_required(VERSION 3.20)
project(OpenWCH_SDK C CXX ASM)

# Top-level options can be extended via Kconfig-generated headers.
option(OPENWCH_SDK_ENABLE_TESTS "Build tests" ON)

# Add subdirectories as they materialize.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
include(kconfig)

set(OPENWCH_CONFIG_FILE "${CMAKE_SOURCE_DIR}/.config")
set(OPENWCH_GENERATED_CONFIG "${CMAKE_BINARY_DIR}/include/generated/config.h")
openwch_kconfig_import(${OPENWCH_CONFIG_FILE} ${OPENWCH_GENERATED_CONFIG})

add_library(openwch_config INTERFACE)
target_include_directories(openwch_config INTERFACE
    ${CMAKE_BINARY_DIR}/include/generated
)
if(OPENWCH_CONFIG_DEFINES)
    target_compile_definitions(openwch_config INTERFACE ${OPENWCH_CONFIG_DEFINES})
endif()

add_subdirectory(components)

if(OPENWCH_SDK_ENABLE_TESTS)
    add_subdirectory(tests)
endif()

# Placeholder install rules can be expanded when packaging is defined.
