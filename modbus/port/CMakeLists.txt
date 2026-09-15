#
# CMAKELIST.TXT -- CMake configuration for the freemodbus-port library
#

add_library(freemodbus-port OBJECT)

if(MB_PORT_FREERTOS)
    if(NOT DEFINED MB_PORT_FREERTOS_EVENT_QUEUE)
        message(FATAL_ERROR "MB_PORT_FREERTOS_EVENT_QUEUE has not been defined")
    endif()
    if(NOT DEFINED MB_PORT_FREERTOS_TIMERS)
        message(FATAL_ERROR "MB_PORT_FREERTOS_TIMERS has not been defined")
    endif()
    target_sources(freemodbus-port PRIVATE
        freertos/freertos_event.c
        freertos/freertos_timer.c
    )
    if(TARGET freertos_config)
        target_link_libraries(freemodbus_config INTERFACE freertos_config)
    endif()
endif()

target_link_libraries(freemodbus-port PUBLIC freemodbus_config)

target_include_directories(freemodbus-port PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos
)
