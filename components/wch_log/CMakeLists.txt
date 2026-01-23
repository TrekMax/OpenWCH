add_library(wch_log INTERFACE)

target_include_directories(wch_log INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_sources(wch_log INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/wch_log.c
)
