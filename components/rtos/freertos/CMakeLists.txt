add_library(freertos_kernel STATIC)

set(_freertos_kernel_dir ${CMAKE_CURRENT_LIST_DIR}/kernel)
if(NOT DEFINED CONFIG_FREERTOS_HEAP_IMPL)
    set(CONFIG_FREERTOS_HEAP_IMPL 4)
endif()
set(_freertos_heap "${_freertos_kernel_dir}/portable/MemMang/heap_${CONFIG_FREERTOS_HEAP_IMPL}.c")
if(NOT EXISTS ${_freertos_heap})
    message(FATAL_ERROR "Selected FreeRTOS heap implementation heap_${CONFIG_FREERTOS_HEAP_IMPL}.c not found")
endif()

target_include_directories(freertos_kernel PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/config
    ${_freertos_kernel_dir}/include
    ${_freertos_kernel_dir}/portable/GCC/RISC-V
)

set(_soc_root "${CMAKE_CURRENT_LIST_DIR}/../..")
if(CONFIG_SOC_CH58X)
    set(_soc_dir "${_soc_root}/soc_ch58x")
elseif(CONFIG_SOC_CH59X)
    set(_soc_dir "${_soc_root}/soc_ch59x")
endif()
if(_soc_dir)
    target_include_directories(freertos_kernel PRIVATE
        ${_soc_dir}/include
        ${_soc_dir}/rvmsis
    )
endif()

target_sources(freertos_kernel PRIVATE
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
