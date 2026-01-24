# SPDX-License-Identifier: Apache-2.0
# OpenWCH SDK 版本头文件生成脚本
# 此脚本由主 CMakeLists.txt 通过 execute_process 调用

# 基础参数校验
if(NOT OPENWCH_SDK_DIR)
    message(FATAL_ERROR "OPENWCH_SDK_DIR is not defined")
endif()
if(NOT OUT_FILE)
    message(FATAL_ERROR "OUT_FILE is not defined")
endif()

# 载入版本信息（SDK_VERSION_* 等）
include(${OPENWCH_SDK_DIR}/cmake/version.cmake)

# 确保输出目录存在
get_filename_component(_version_out_dir "${OUT_FILE}" DIRECTORY)
file(MAKE_DIRECTORY "${_version_out_dir}")

# 构建时间与日期（本地时区）
string(TIMESTAMP COMPILE_DATE "%Y-%m-%d")
string(TIMESTAMP COMPILE_TIME "%H:%M:%S")

# 尝试获取 git 提交哈希
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS ${OPENWCH_SDK_DIR}/.git)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --abbrev=12 --always
        WORKING_DIRECTORY ${OPENWCH_SDK_DIR}
        OUTPUT_VARIABLE BUILD_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
endif()

# 如果未获取到 git 信息，设置为默认值
if(NOT BUILD_VERSION)
    set(BUILD_VERSION "unknown")
endif()

# 从模板生成头文件
configure_file(${OPENWCH_SDK_DIR}/sdk_version.h.in ${OUT_FILE} @ONLY)

message(STATUS "生成版本头文件: ${OUT_FILE}")
message(STATUS "  BUILD_VERSION: ${BUILD_VERSION}")
