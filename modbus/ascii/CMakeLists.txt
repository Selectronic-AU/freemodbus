# freemodbus/modbus/ascii/CMakeLists.txt

set(ASCII_SRC
    mbascii.c
)

add_library(freemodbus-ascii STATIC ${ASCII_SRC})

target_include_directories(freemodbus-ascii PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/modbus
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include/modbus
)
