function(openwch_generate_version_header OUT_HEADER)
    if(DEFINED OPENWCH_SDK_DIR)
        set(_sdk_root "${OPENWCH_SDK_DIR}")
    else()
        get_filename_component(_sdk_root "${CMAKE_CURRENT_LIST_DIR}/../.." REALPATH)
    endif()
    set(_work_dir "${_sdk_root}")

    set(TAG_VERSION "unknown")
    set(COMMIT_AUTHOR "unknown")
    set(AUTHOR_EMAIL "unknown")
    set(COMMIT_HASH "unknown")

    execute_process(
        COMMAND git describe --long --tag --dirty --always
        WORKING_DIRECTORY ${_work_dir}
        OUTPUT_VARIABLE TAG_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    execute_process(
        COMMAND git log -1 --format=%an
        WORKING_DIRECTORY ${_work_dir}
        OUTPUT_VARIABLE COMMIT_AUTHOR
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    execute_process(
        COMMAND git log -1 --format=%ae
        WORKING_DIRECTORY ${_work_dir}
        OUTPUT_VARIABLE AUTHOR_EMAIL
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    execute_process(
        COMMAND git rev-parse HEAD
        WORKING_DIRECTORY ${_work_dir}
        OUTPUT_VARIABLE COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    set(_template "${_sdk_root}/sdk_version.h.in")
    if(NOT EXISTS "${_template}")
        message(FATAL_ERROR "sdk_version.h.in not found: ${_template}")
    endif()

    get_filename_component(_out_dir "${OUT_HEADER}" DIRECTORY)
    file(MAKE_DIRECTORY "${_out_dir}")

    configure_file(
        ${_template}
        ${OUT_HEADER}
        @ONLY
    )
endfunction()
