#
# CMAKELIST.TXT -- CMake configuration for the freemodbus_port library
#

add_library(freemodbus_port OBJECT
    freertos/freertos_event.c
    freertos/freertos_timer.c
)

target_link_libraries(freemodbus_port PUBLIC freemodbus_config)

target_include_directories(freemodbus_port PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
