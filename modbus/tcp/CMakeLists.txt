#
# CMAKELIST.TXT -- CMake configuration for the freemodbus_tcp library
#

add_library(freemodbus_tcp OBJECT
    mbtcp.c
)

target_link_libraries(freemodbus_tcp PRIVATE freemodbus_port)

target_include_directories(freemodbus_tcp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/modbus
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include/modbus
)
