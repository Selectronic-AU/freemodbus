#
# CMAKELIST.TXT -- CMake configuration for the freemodbus_ascii library
#

add_library(freemodbus_ascii OBJECT
    mbascii.c
)
target_link_libraries(freemodbus_ascii PUBLIC freemodbus_port)

target_include_directories(freemodbus_ascii PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/modbus
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include/modbus
)
