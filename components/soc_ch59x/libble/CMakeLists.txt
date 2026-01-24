# CH58x BLE Library
add_library(openwch_ble INTERFACE)

target_include_directories(openwch_ble INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link the pre-compiled BLE library with whole-archive to ensure all symbols are included
if(CONFIG_BLE_PERIPHERAL_ONLY)
    set(BLE_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/libCH59xBLE_PERI.a")
else()
    set(BLE_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/libCH59xBLE.a")
endif()

target_link_libraries(openwch_ble INTERFACE
    -Wl,--whole-archive
    ${BLE_LIB_PATH}
    -Wl,--no-whole-archive
)
