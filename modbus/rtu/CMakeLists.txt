#
# CMAKELIST.TXT -- CMake configuration for the freemodbus_rtu library
#

add_library(freemodbus_rtu OBJECT
    mbcrc.c
    mbrtu.c
    mbrtu_m.c
)

target_link_libraries(freemodbus_rtu PRIVATE freemodbus_port)

target_include_directories(freemodbus_rtu PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/modbus
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include/modbus
)
