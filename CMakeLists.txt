#
# CMAKELIST.TXT -- CMake configuration for the freemodbus library
#

cmake_minimum_required(VERSION 3.25)
project(freemodbus C)

if(NOT TARGET freemodbus_config)
    message(WARNING "freemodbus_config was not provided. Using defaults instead.")
    message(WARNING "Note that the default configuration does not provide any freemodbus port implementation. This must be supplied by the linking application.")
    add_library(freemodbus_config INTERFACE)
    target_compile_definitions(freemodbus_config INTERFACE
        MB_SLAVE_RTU_ENABLED=1
        MB_MASTER_RTU_ENABLED=1
        MB_FUNC_OTHER_REP_SLAVEID_ENABLED=1
        MB_FUNC_READ_HOLDING_ENABLED=1
        MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED=1
        MB_FUNC_WRITE_HOLDING_ENABLED=1
        MB_FUNC_READWRITE_HOLDING_ENABLED=1
    )
endif()

get_target_property(FREEMODBUS_C_STANDARD freemodbus_config C_STANDARD)
if(NOT FREEMODBUS_C_STANDARD)
    target_compile_features(freemodbus_config INTERFACE c_std_17)
endif()

# Basic check for defined Modbus feature macros.  Freemodbus feature
# macros are disabled by default and should be explicitly defined.
if(NOT MB_DEFINED_FEATURE_MACROS)
    message(STATUS "No Modbus feature macros defined.")
endif()

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
    modbus/mb.c
    modbus/mb_m.c
)

# Add object libraries for each modbus protocol part as dependencies of
# the main freemodbus library.

target_sources(freemodbus PRIVATE
    $<TARGET_OBJECTS:freemodbus_ascii>
    $<TARGET_OBJECTS:freemodbus_rtu>
    $<TARGET_OBJECTS:freemodbus_tcp>
    $<TARGET_OBJECTS:freemodbus_functions>
    $<TARGET_OBJECTS:freemodbus_port>
)
target_link_libraries(freemodbus PUBLIC freemodbus_config)

# Public include directories for consumers of this library
target_include_directories(freemodbus PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/modbus/include
    ${CMAKE_CURRENT_SOURCE_DIR}/modbus/port/include
    $<TARGET_PROPERTY:freemodbus_ascii,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:freemodbus_rtu,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:freemodbus_tcp,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:freemodbus_functions,INTERFACE_INCLUDE_DIRECTORIES>
)
