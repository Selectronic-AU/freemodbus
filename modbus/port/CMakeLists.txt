#
# CMAKELIST.TXT -- CMake configuration for the freemodbus_port library
#

add_library(freemodbus_port OBJECT)

if(MB_PORT_FREERTOS)
    # Ensure that MB_PORT_FREERTOS_EVENT_QUEUE & MB_PORT_FREERTOS_TIMERS
    # have been explicitly defined.
    if(NOT DEFINED MB_PORT_FREERTOS_EVENT_QUEUE OR "${MB_PORT_FREERTOS_EVENT_QUEUE}" STREQUAL "")
        message(FATAL_ERROR "MB_PORT_FREERTOS_EVENT_QUEUE has not been explicitly defined")
    endif()
    if(NOT DEFINED MB_PORT_FREERTOS_TIMERS OR "${MB_PORT_FREERTOS_TIMERS}" STREQUAL "")
        message(FATAL_ERROR "MB_PORT_FREERTOS_TIMERS has not been explicitly defined")
    endif()

    # Implementation sources always added regardless of the value of the
    # corresponding feature macros.
    target_sources(freemodbus_port PRIVATE
        freertos/freertos_event.c
        freertos/freertos_timer.c
    )

    # If the freertos_config target exists, link freertos it to the
    # freemodbus_config library.
    if(TARGET freertos_kernel)
        target_link_libraries(freemodbus_config INTERFACE freertos_kernel)
    endif()
endif()

target_link_libraries(freemodbus_port PUBLIC freemodbus_config)

target_include_directories(freemodbus_port PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
