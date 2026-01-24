add_library(openwch_platform INTERFACE)

set(CH59X_LD_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/ld/ch592_default.ld" CACHE FILEPATH "Linker script for CH59x")

target_include_directories(openwch_platform INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/rvmsis
    ${CMAKE_CURRENT_LIST_DIR}/startup
)

target_sources(openwch_platform INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/startup/startup_CH592.S
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_sys.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_clk.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_flash.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_pwr.c
)

openwch_add_sources_if(CONFIG_SOC_CH59X_GPIO
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_gpio.c
)

openwch_add_sources_if(CONFIG_SOC_CH59X_I2C
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_i2c.c
)

openwch_add_sources_if(CONFIG_SOC_CH59X_PWM
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_pwm.c
)

openwch_add_sources_if(CONFIG_SOC_CH59X_ADC
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_adc.c
)

openwch_add_sources_if(CONFIG_SOC_CH59X_TIMER
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_timer0.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_timer1.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_timer2.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_timer3.c
)

openwch_add_sources_if(CONFIG_SOC_CH59X_SPI
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_spi0.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_spi1.c
)

openwch_add_sources_if(CONFIG_SOC_CH59X_UART
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_uart0.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_uart1.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_uart2.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_uart3.c
)

openwch_add_sources_if(CONFIG_SOC_CH59X_USB
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_usbdev.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_usbhostBase.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/CH59x_usbhostClass.c
)

target_compile_options(openwch_platform INTERFACE
    -march=rv32imac -mabi=ilp32 -mcmodel=medlow
)

target_link_libraries(openwch_platform INTERFACE
    openwch_config
    openwch_board
    ${CMAKE_CURRENT_LIST_DIR}/drivers/libISP592.a
)

# Include BLE library subdirectory
add_subdirectory(libble)

# Auto-link BLE library when BLE support is enabled via Kconfig
if(CONFIG_BLE_SUPPORT)
    target_link_libraries(openwch_platform INTERFACE openwch_ble)
endif()

# Auto-link wch_log when any log backend is enabled via Kconfig.
if(CONFIG_WCH_LOG_UART OR CONFIG_WCH_LOG_RTT OR CONFIG_WCH_LOG_CDC)
    target_link_libraries(openwch_platform INTERFACE wch_log)
endif()

# Auto-link FreeRTOS kernel when enabled via Kconfig
if(CONFIG_FREERTOS_ENABLE)
    target_link_libraries(openwch_platform INTERFACE freertos_kernel)
endif()

target_link_options(openwch_platform INTERFACE
    -T${CH59X_LD_SCRIPT}
)
