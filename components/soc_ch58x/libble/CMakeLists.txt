# CH58x BLE Library
add_library(ch58x_ble INTERFACE)

target_include_directories(ch58x_ble INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link the pre-compiled BLE library with whole-archive to ensure all symbols are included
if(CONFIG_BLE_PERIPHERAL_ONLY)
    set(BLE_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/libCH58xBLE_PERI.a")
else()
    set(BLE_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/libCH58xBLE.a")
endif()

target_link_libraries(ch58x_ble INTERFACE
    -Wl,--whole-archive
    ${BLE_LIB_PATH}
    -Wl,--no-whole-archive
)
