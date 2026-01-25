add_library(wch_log STATIC)

target_include_directories(wch_log PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_sources(wch_log PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/wch_log.c
)
