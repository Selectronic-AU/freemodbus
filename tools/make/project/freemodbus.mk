#
# FREEMODBUS.MK -- Macros and definitions for FreeMODBUS.
#
# Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
# SPDX-License-Identifier: LicenseRef-Selectronic
#

# Include the FreeMODBUS configuration file.
include config/freemodbus.mk

# Assume using a GNU toolchain.
AS = ${CC} -x assembler-with-cpp
CC = ${CROSS_COMPILE}gcc
CXX = ${CROSS_COMPILE}g++
LD = ${CC}

# Set the C language, optimization and debugging options
CSTD ?= c17
COPT ?= $(if ${DEBUG},g,3)
CDBG ?= gdb 3

PROJECT.C_DEFS = ${FREEMODBUS.C_DEFS}
PROJECT.CFLAGS = ${CSTD:%=-std=%} ${COPT:%=-O%} ${CDBG:%=-g%} \
	-fstack-protector ${FREEMODBUS.C_FLAGS}
PROJECT.C_WARN_FLAGS = -Wall -Wextra -Wpedantic -Werror \
	-Waggregate-return -Wcast-align -Wdisabled-optimization -Wformat=2 \
	-Wimplicit -Winit-self -Wmisleading-indentation -Wmissing-declarations \
	-Wmissing-prototypes -Wnested-externs -Wold-style-definition \
	-Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion \
	-Wstrict-overflow=5 -Wstrict-prototypes -Wswitch-default \
	-Wswitch-enum -Wundef -Wunreachable-code -Wwrite-strings \
	${FREEMODBUS.C_WARN_FLAGS}
PROJECT.C_CPPFLAGS = ${FREEMODBUS.C_CPPFLAGS}
PROJECT.C_SHARED_FLAGS = ${FREEMODBUS.C_SHARED_FLAGS}

build: ${H_SRC:%.h=${archdir}/h/%.h.$o}

${H_SRC:%.h=${archdir}/h/%.h.$o}: ${archdir}/h/%.h.$o: %.h
	$(ECHO_TARGET)
	${MKDIR} ${@D}
	$(CROSS_COMPILE)$(CC) -x c $(C_ALL_FLAGS) -Wno-pedantic -c -o $@ $(abspath $<)

clean: clean-x

clean-x:
	${RM} ${H_SRC:%.h=${archdir}/h/%.h.$o}
