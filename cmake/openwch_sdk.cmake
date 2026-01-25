include_guard(GLOBAL)

set(OPENWCH_CMAKE_PATH ${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED OPENWCH_SDK_PATH)
    if (NOT DEFINED ENV{OPENWCH_SDK_PATH})
        message(FATAL_ERROR "OPENWCH_SDK_PATH is not defined")
    else()
        set(OPENWCH_SDK_PATH $ENV{OPENWCH_SDK_PATH})
    endif()
endif()


function(openwch_add_sources_if config)
    if(${config})
        target_sources(openwch_platform PRIVATE ${ARGN})
    endif()
endfunction()

# 简介: 为目标生成调试信息，包括反汇编、ELF头、符号表等
# 参数:
# + target_name 目标
macro(openwch_generate_debug_files target_name)
    set(_debug_script "${CMAKE_CURRENT_BINARY_DIR}/listenai_debug_${target_name}.cmake")
    file(GENERATE OUTPUT "${_debug_script}" CONTENT "file(MAKE_DIRECTORY \"$<TARGET_FILE_DIR:${target_name}>\")\nexecute_process(COMMAND \"${CMAKE_OBJDUMP}\" -d -S \"$<TARGET_FILE:${target_name}>\" OUTPUT_FILE \"$<TARGET_FILE_DIR:${target_name}>/${target_name}.lst\")\nexecute_process(COMMAND \"${CMAKE_READELF}\" -a \"$<TARGET_FILE:${target_name}>\" OUTPUT_FILE \"$<TARGET_FILE_DIR:${target_name}>/${target_name}.relf\")\nexecute_process(COMMAND \"${CMAKE_NM}\" -CSsnl -f sysv \"$<TARGET_FILE:${target_name}>\" OUTPUT_FILE \"$<TARGET_FILE_DIR:${target_name}>/${target_name}.symb\")\n")
    add_custom_command(
        TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "-- Genarating file: ${target_name}.lst"
        COMMAND ${CMAKE_COMMAND} -P "${_debug_script}"
    )
endmacro()

function(_openwch_is_enabled var out)
    if(DEFINED ${var} AND ${${var}})
        set(${out} TRUE PARENT_SCOPE)
    else()
        set(${out} FALSE PARENT_SCOPE)
    endif()
endfunction()

function(_openwch_add_objcopy target format ext)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ${format} $<TARGET_FILE:${target}> ${CMAKE_CURRENT_BINARY_DIR}/${target}.${ext}
        COMMENT "[objcopy] Generating ${target}.${ext}"
    )
endfunction()

function(openwch_generate_version_header out_file)
    if(NOT OPENWCH_SDK_DIR)
        message(FATAL_ERROR "OPENWCH_SDK_DIR is not defined")
    endif()
    if(NOT out_file)
        message(FATAL_ERROR "openwch_generate_version_header requires OUT_FILE")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND}
            -DOPENWCH_SDK_DIR=${OPENWCH_SDK_DIR}
            -DOUT_FILE=${out_file}
            -P ${OPENWCH_SDK_DIR}/cmake/gen_version_h.cmake
        WORKING_DIRECTORY ${OPENWCH_SDK_DIR}
    )
endfunction()

function(_openwch_project_ensure_setup)
    get_property(_already GLOBAL PROPERTY OPENWCH_PROJECT_SETUP_DONE)
    if(_already)
        return()
    endif()

    get_filename_component(_sdk_from_module "${CMAKE_CURRENT_LIST_DIR}/.." REALPATH)
    set(_openwch_sdk_dir "${OPENWCH_SDK_DIR}")
    if(NOT _openwch_sdk_dir)
        set(_openwch_sdk_dir "${_sdk_from_module}")
    elseif(NOT EXISTS "${_openwch_sdk_dir}/cmake/kconfig.cmake")
        set(_openwch_sdk_dir "${_sdk_from_module}")
    endif()
    set(OPENWCH_SDK_DIR "${_openwch_sdk_dir}" CACHE PATH "Path to OpenWCH SDK root" FORCE)

    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "${OPENWCH_SDK_DIR}/cmake/toolchain-riscv-none-elf.cmake" CACHE FILEPATH "" FORCE)
    endif()

    include("${OPENWCH_SDK_DIR}/cmake/kconfig.cmake")

    if(NOT TARGET openwch_config)
        if(NOT OPENWCH_CONFIG_FILE)
            set(OPENWCH_CONFIG_FILE "${OPENWCH_SDK_DIR}/.config")
        elseif(NOT EXISTS "${OPENWCH_CONFIG_FILE}")
            message(FATAL_ERROR "OPENWCH_CONFIG_FILE not found: ${OPENWCH_CONFIG_FILE}")
        endif()
        set(OPENWCH_GENERATED_CONFIG "${CMAKE_BINARY_DIR}/include/generated/config.h")
        set(OPENWCH_GENERATED_VERSION "${CMAKE_BINARY_DIR}/include/generated/version.h")
        openwch_kconfig_import(${OPENWCH_CONFIG_FILE} ${OPENWCH_GENERATED_CONFIG})
        openwch_generate_version_header(${OPENWCH_GENERATED_VERSION})

        add_library(openwch_config INTERFACE)
        target_include_directories(openwch_config INTERFACE
            ${CMAKE_BINARY_DIR}/include/generated
        )
        if(OPENWCH_CONFIG_DEFINES)
            target_compile_definitions(openwch_config INTERFACE ${OPENWCH_CONFIG_DEFINES})
        endif()
    endif()

    if(NOT TARGET openwch_platform)
        add_subdirectory(${OPENWCH_SDK_DIR}/components ${CMAKE_BINARY_DIR}/components)
    endif()

    if(NOT SIZE_TOOL AND UNIX)
        find_program(SIZE_TOOL riscv-wch-elf-size)
    endif()
    set_property(GLOBAL PROPERTY OPENWCH_PROJECT_SIZE_TOOL "${SIZE_TOOL}")
    set_property(GLOBAL PROPERTY OPENWCH_PROJECT_SETUP_DONE TRUE)
endfunction()

function(openwch_project_setup)
    _openwch_project_ensure_setup()
    get_property(_size GLOBAL PROPERTY OPENWCH_PROJECT_SIZE_TOOL)
    set(OPENWCH_SDK_PATH "${OPENWCH_SDK_PATH}" PARENT_SCOPE)
    set(SIZE_TOOL "${_size}" PARENT_SCOPE)
endfunction()

function(openwch_add_source target)
    _openwch_project_ensure_setup()

    cmake_parse_arguments(OW "" "MAP" "SOURCES;LINK_LIBS;DEFINES" ${ARGN})

    if(NOT OW_SOURCES)
        message(FATAL_ERROR "openwch_add_source requires SOURCES")
    endif()

    add_executable(${target}
        ${OW_SOURCES}
    )

    target_link_libraries(${target}
        ${OW_LINK_LIBS}
        openwch_board
        openwch_platform
    )

    if(OW_DEFINES)
        target_compile_definitions(${target} PRIVATE ${OW_DEFINES})
    endif()

    if(CONFIG_BLE_SUPPORT AND TARGET openwch_ble_handlers)
        target_sources(${target} PRIVATE $<TARGET_OBJECTS:openwch_ble_handlers>)
    endif()

    target_link_options(${target} PRIVATE
        -nostartfiles
        -Wl,--gc-sections
        -Wl,--print-memory-usage
    )

    _openwch_is_enabled(CONFIG_OPENWCH_GEN_HEX _gen_hex)
    _openwch_is_enabled(CONFIG_OPENWCH_GEN_BIN _gen_bin)
    _openwch_is_enabled(CONFIG_OPENWCH_GEN_MAP _gen_map)
    _openwch_is_enabled(CONFIG_OPENWCH_GEN_DEBUG_FILES _gen_debug_files)

    if(_gen_hex)
        _openwch_add_objcopy(${target} ihex hex)
    endif()

    if(_gen_bin)
        _openwch_add_objcopy(${target} binary bin)
    endif()

    if(_gen_map)
        set(map_file "${OW_MAP}")
        if(NOT map_file)
            set(map_file "${CMAKE_CURRENT_BINARY_DIR}/${target}.map")
        endif()
        target_link_options(${target} PRIVATE
            -Wl,-Map=${map_file}
        )
    endif()

    if(_gen_debug_files)
        openwch_generate_debug_files(${target})
    endif()

    get_property(_size GLOBAL PROPERTY OPENWCH_PROJECT_SIZE_TOOL)
    if(_size)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${_size} $<TARGET_FILE:${target}>
            COMMENT "[size] ${target}"
        )
    endif()

endfunction()
