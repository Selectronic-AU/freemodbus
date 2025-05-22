# freemodbus/modbus/port/freertos/CMakeLists.txt

file(GLOB FREERTOS_PORT_SRC
    *.c
)

add_library(freemodbus-port-freertos STATIC ${FREERTOS_PORT_SRC})

target_include_directories(freemodbus-port-freertos PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
