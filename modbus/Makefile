#
# MAKEFILE -- build rules for freemodbus/modbus.
#
# Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
# SPDX-License-Identifier: LicenseRef-Selectronic
#

export PROJECT ?= freemodbus

language = c
subdir = modbus

C_SRC = mb.c mb_m.c

SUBLIB_SRC = ascii/$(archdir)/lib.$(a) functions/$(archdir)/lib.$(a) \
    port/$(archdir)/lib.$(a) rtu/$(archdir)/lib.$(a) \
    tcp/$(archdir)/lib.$(a)

# Relative path to top of freeMODBUS sources.
top_mbdir = .

# Name and type of library.
LIB = freemodbus
LIB_TYPE = static

# Build location of the library.
LIB_ROOT = ${top_mbdir}/${archdir}

INCLUDE_DIRS = \
	${top_mbdir}/include ${top_mbdir}/port/include \
	${top_mbdir}/include/modbus ${top_mbdir}/port/include/modbus \
	$(LIB_ROOT)/include $(LIB_ROOT)/include/modbus

LOCAL.C_CPPFLAGS = ${INCLUDE_DIRS:%=-I%}

include makeshift.mk library.mk

build@port: build@include
build@functions: build@port
build@ascii build@rtu build@tcp: build@functions

build: build-lib-root-lib
