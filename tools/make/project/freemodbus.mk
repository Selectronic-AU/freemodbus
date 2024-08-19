#
# FREEMODBUS.MK	--Macros and definitions for FreeMODBUS.
#

CC = gcc
LD = ${CC}

PROJECT.C_FLAGS = -std=c17 -Og -g
PROJECT.C_WARN_FLAGS = -Wall -Wextra -Wpedantic -Werror \
	-Waggregate-return -Wcast-align -Wdisabled-optimization -Wformat=2 \
	-Wimplicit -Winit-self -Wmisleading-indentation -Wmissing-declarations \
	-Wmissing-prototypes -Wnested-externs -Wold-style-definition \
	-Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion \
	-Wstack-protector -Wstrict-overflow=5 -Wstrict-prototypes \
	-Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code \
	-Wwrite-strings

ifneq ($(filter %linux,${OS}),)
export MB_PORT_POSIX_TIMERS = 0
export MB_PORT_POSIX_EVENT_QUEUE = 0
export MB_PORT_POSIX_SERIAL = 1
export MB_PORT_POSIX_SOCKETS = 1
endif

ifneq ($(filter freertos%,${OS}),)
export MB_PORT_FREERTOS_TIMERS = 1
export MB_PORT_FREERTOS_EVENT_QUEUE = 1
endif

export MB_ASCII_ENABLED = 1
export MB_RTU_ENABLED = 1
export MB_TCP_ENABLED = 1

_MB_VARIABLES ?= $(filter MB_%,${.VARIABLES})
_MB_ENABLED ?= $(foreach v,${_MB_VARIABLES},$v=${$v})

PROJECT.C_DEFS = ${_MB_ENABLED:%=-D%}


build: ${H_SRC:%.h=${archdir}/h/%.h.$o}

${H_SRC:%.h=${archdir}/h/%.h.$o}: ${archdir}/h/%.h.$o: %.h
	$(ECHO_TARGET)
	${MKDIR} ${@D}
	$(CROSS_COMPILE)$(CC) -x c $(C_ALL_FLAGS) -Wno-pedantic -c -o $@ $(abspath $<)

clean: clean-x

clean-x:
	${RM} ${H_SRC:%.h=${archdir}/h/%.h.$o}
