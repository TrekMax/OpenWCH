add_library(freertos_kernel INTERFACE)

set(_freertos_kernel_dir ${CMAKE_CURRENT_LIST_DIR}/kernel)
if(NOT DEFINED CONFIG_FREERTOS_HEAP_IMPL)
    set(CONFIG_FREERTOS_HEAP_IMPL 4)
endif()
set(_freertos_heap "${_freertos_kernel_dir}/portable/MemMang/heap_${CONFIG_FREERTOS_HEAP_IMPL}.c")
if(NOT EXISTS ${_freertos_heap})
    message(FATAL_ERROR "Selected FreeRTOS heap implementation heap_${CONFIG_FREERTOS_HEAP_IMPL}.c not found")
endif()

target_include_directories(freertos_kernel INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/config
    ${_freertos_kernel_dir}/include
    ${_freertos_kernel_dir}/portable/GCC/RISC-V
)

target_sources(freertos_kernel INTERFACE
    ${_freertos_kernel_dir}/croutine.c
    ${_freertos_kernel_dir}/event_groups.c
    ${_freertos_kernel_dir}/list.c
    ${_freertos_kernel_dir}/queue.c
    ${_freertos_kernel_dir}/stream_buffer.c
    ${_freertos_kernel_dir}/tasks.c
    ${_freertos_kernel_dir}/timers.c
    ${_freertos_kernel_dir}/portable/GCC/RISC-V/port.c
    ${_freertos_kernel_dir}/portable/GCC/RISC-V/portASM.S
    ${_freertos_heap}
)

target_link_libraries(freertos_kernel INTERFACE
    openwch_platform
)
