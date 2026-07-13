#
# CMAKELIST.TXT -- CMake configuration for the freemodbus-ascii library
#

add_library(freemodbus-ascii OBJECT
    mbascii.c
)
target_link_libraries(freemodbus-ascii PUBLIC freemodbus_config)

target_include_directories(freemodbus-ascii PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/modbus
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include/modbus
)
