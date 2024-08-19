#
# MAKEFILE -- build rules for freemodbus/modbus.
#
# Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
# SPDX-License-Identifier: LicenseRef-Selectronic
#

export PROJECT ?= freemodbus

language = c
subdir = modbus

C_SRC = mb.c

SUBLIB_SRC = ascii/$(archdir)/lib.$(a) functions/$(archdir)/lib.$(a) \
    port/$(archdir)/lib.$(a) rtu/$(archdir)/lib.$(a) \
    tcp/$(archdir)/lib.$(a)

topdir = .
LIB = freemodbus
LIB_ROOT = ${topdir}/${archdir}

INCLUDE_DIRS = \
	${topdir}/include ${topdir}/port/include \
	${topdir}/include/modbus ${topdir}/port/include/modbus \
	$(LIB_ROOT)/include $(LIB_ROOT)/include/modbus

LOCAL.C_CPPFLAGS = ${INCLUDE_DIRS:%=-I%}

include makeshift.mk library.mk

build@port: build@include
build@functions: build@port
build@ascii build@rtu build@tcp: build@functions

build: build-lib-lib-root