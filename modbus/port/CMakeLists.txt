# freemodbus/modbus/port/CMakeLists.txt

# Choose the correct port implementation here.
# You can make this configurable with a CMake option if needed.
set(PORT_IMPL freertos)

add_subdirectory(${PORT_IMPL})

add_library(freemodbus-port INTERFACE)

target_include_directories(freemodbus-port INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/${PORT_IMPL}
)
target_link_libraries(freemodbus-port INTERFACE freemodbus-port-${PORT_IMPL}) 
