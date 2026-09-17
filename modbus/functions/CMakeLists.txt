#
# CMAKELIST.TXT -- CMake configuration for the freemodbus_functions library
#

add_library(freemodbus_functions OBJECT
    # Modbus slave/server support functions.
    mbfunccoils.c
    mbfuncdiag.c
    mbfuncdisc.c
    mbfuncholding.c
    mbfuncinput.c
    mbfuncother.c
    mbutils.c

    # Modbus master/client support functions.
    mbfunccoils_m.c
    mbfuncdisc_m.c
    mbfuncholding_m.c
    mbfuncinput_m.c
    mbfuncother_m.c
)

target_link_libraries(freemodbus_functions PUBLIC freemodbus_port)

target_include_directories(freemodbus_functions PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/modbus
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include/modbus
)
