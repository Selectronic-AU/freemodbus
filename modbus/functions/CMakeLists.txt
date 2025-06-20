# freemodbus/modbus/functions/CMakeLists.txt

set(FUNCTIONS_SRC
    mbfunccoils.c
    mbfuncdiag.c
    mbfuncdisc.c
    mbfuncholding.c
    mbfuncinput.c
    mbfuncother.c
    mbutils.c
)

set(MASTER_FUNCTIONS_SRC
    mbfunccoils_m.c
    mbfuncdisc_m.c
    mbfuncholding_m.c
    mbfuncinput_m.c
)

add_library(freemodbus-functions STATIC ${FUNCTIONS_SRC} ${MASTER_FUNCTIONS_SRC})

target_include_directories(freemodbus-functions PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/modbus
    ${CMAKE_CURRENT_SOURCE_DIR}/../port/include/modbus
)
