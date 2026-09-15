#
# CMAKELIST.TXT -- CMake configuration for the freemodbus library
#

cmake_minimum_required(VERSION 3.25)
project(freemodbus C)

if(NOT TARGET freemodbus_config)
    message(WARNING "freemodbus_config was not provided. Using defaults instead.")
    add_library(freemodbus_config INTERFACE)
    target_compile_definitions(freemodbus_config INTERFACE
        MB_SLAVE_RTU_ENABLED=1
        MB_FUNC_READ_HOLDING_ENABLED=1
        MB_FUNC_WRITE_HOLDING_ENABLED=1
        MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED=1
        MB_FUNC_READWRITE_HOLDING_ENABLED=1
    )
endif()

get_target_property(freemodbus_defines freemodbus_config INTERFACE_COMPILE_DEFINITIONS)
foreach(def IN LISTS freemodbus_defines)
    if(def MATCHES "^(MB_[A-Za-z0-9_]+)(=([^ ]+))?$")
        if(CMAKE_MATCH_3 STREQUAL "")
            message(FATAL_ERROR "Explicit value required for ${CMAKE_MATCH_1}. E.g.: ${CMAKE_MATCH_1}=1")
        else()
            set(${CMAKE_MATCH_1} "${CMAKE_MATCH_3}")
        endif()
    endif()
endforeach()

# Freemodbus core source
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

# Add port subdirectory first to allow configuration to propagate to
# other subdirectories.
add_subdirectory(modbus/port)

# Add subdirectories for each modbus protocol part
add_subdirectory(modbus/ascii)
add_subdirectory(modbus/functions)
add_subdirectory(modbus/rtu)
add_subdirectory(modbus/tcp)

# Create the main freemodbus static library
add_library(freemodbus STATIC
    ${FREEMODBUS_SRC}
    $<TARGET_OBJECTS:freemodbus-ascii>
    $<TARGET_OBJECTS:freemodbus-rtu>
    $<TARGET_OBJECTS:freemodbus-tcp>
    $<TARGET_OBJECTS:freemodbus-functions>
)

get_target_property(FREEMODBUS_PORT_SOURCES freemodbus-port SOURCES)
if(FREEMODBUS_PORT_SOURCES)
    target_sources(freemodbus PRIVATE $<TARGET_OBJECTS:freemodbus-port>)
endif()

target_link_libraries(freemodbus PUBLIC freemodbus_config)

# Public include directories for consumers of this library
target_include_directories(freemodbus PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/modbus/include
    $<TARGET_PROPERTY:freemodbus-ascii,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:freemodbus-rtu,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:freemodbus-tcp,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:freemodbus-functions,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:freemodbus-port,INTERFACE_INCLUDE_DIRECTORIES>
)
