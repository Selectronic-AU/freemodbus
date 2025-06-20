cmake_minimum_required(VERSION 3.10)
project(freemodbus C CXX)


# freemodbus core source
set(FREEMODBUS_SRC
    modbus/mb.c
    modbus/mb_m.c
)

# Include modbus include directories
include_directories(
    ${COMPONENT_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/modbus/include
    ${CMAKE_CURRENT_SOURCE_DIR}/modbus/port/include
    ${CMAKE_CURRENT_SOURCE_DIR}/modbus/include/modbus
    ${CMAKE_CURRENT_SOURCE_DIR}/modbus/port/include/modbus
    ${CMAKE_CURRENT_SOURCE_DIR}/modbus/ascii
    ${CMAKE_CURRENT_SOURCE_DIR}/modbus/functions
    ${CMAKE_CURRENT_SOURCE_DIR}/modbus/rtu
    ${CMAKE_CURRENT_SOURCE_DIR}/modbus/tcp
)

add_definitions(
    -D__freertos__
    -DMB_PORT_FREERTOS
    -DMB_PORT_FREERTOS_EVENT_QUEUE
    -DMB_PORT_FREERTOS_TIMERS
    #-DMB_SLAVE_RTU_ENABLED
)

# Add subdirectories for each modbus protocol part

add_subdirectory(modbus/ascii)
add_subdirectory(modbus/functions)
add_subdirectory(modbus/port)
add_subdirectory(modbus/rtu)
add_subdirectory(modbus/tcp)


# Create the main freemodbus static library
add_library(freemodbus STATIC ${FREEMODBUS_SRC})

# # Link protocol component libraries
 target_link_libraries(freemodbus PUBLIC
     freemodbus-ascii
     freemodbus-functions
     freemodbus-port
     freemodbus-rtu
     freemodbus-tcp
 )

# Public include directories for consumers of this library
target_include_directories(freemodbus PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/modbus/include
)
