# CH59x BLE Library (prebuilt)
if(CONFIG_BLE_PERIPHERAL_ONLY)
    set(BLE_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/libCH59xBLE_PERI.a")
else()
    set(BLE_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/libCH59xBLE.a")
endif()
add_library(openwch_ble STATIC IMPORTED GLOBAL)
set_target_properties(openwch_ble PROPERTIES
    IMPORTED_LOCATION "${BLE_LIB_PATH}"
)

target_include_directories(openwch_ble INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# BLE IRQ handlers (must be linked directly to override weak vectors)
add_library(openwch_ble_handlers OBJECT
    ${CMAKE_CURRENT_LIST_DIR}/ble_task_scheduler.S
)
target_include_directories(openwch_ble_handlers PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
