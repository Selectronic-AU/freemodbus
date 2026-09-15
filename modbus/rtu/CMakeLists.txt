#
# CMAKELIST.TXT -- CMake configuration for the freemodbus-rtu library
#

add_library(freemodbus-rtu OBJECT
    mbcrc.c
    mbrtu.c
    mbrtu_m.c
)

target_link_libraries(freemodbus-rtu PUBLIC freemodbus_config)

target_include_directories(freemodbus-rtu PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/modbus
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include/modbus
)
