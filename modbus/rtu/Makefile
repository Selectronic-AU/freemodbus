#
# MAKEFILE -- build rules for freemodbus/modbus/rtu.
#
# Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
# SPDX-License-Identifier: LicenseRef-Selectronic
#

export PROJECT ?= freemodbus

language = c
subdir = modbus

H_SRC = mbcrc.h mbrtu.h
C_SRC = mbcrc.c mbrtu.c

# Relative path to top of freeMODBUS sources.
top_mbdir = ..

# Name and type of library.
LIB = freemodbus-rtu
LIB_TYPE = static

# Build location of the library.
LIB_ROOT = ${top_mbdir}/${archdir}

INCLUDE_DIRS = \
	${top_mbdir}/include ${top_mbdir}/port/include \
	${top_mbdir}/include/modbus ${top_mbdir}/port/include/modbus \
	$(LIB_ROOT)/include $(LIB_ROOT)/include/modbus

LOCAL.C_CPPFLAGS = ${INCLUDE_DIRS:%=-I%}

include makeshift.mk library.mk
