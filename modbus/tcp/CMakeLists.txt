#
# CMAKELIST.TXT -- CMake configuration for the freemodbus-tcp library
#

add_library(freemodbus-tcp OBJECT
    mbtcp.c
)

target_link_libraries(freemodbus-tcp PUBLIC freemodbus_config)

target_include_directories(freemodbus-tcp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/modbus
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include/modbus
)
