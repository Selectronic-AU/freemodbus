#
# MAKEFILE -- build rules for freemodbus/modbus/tcp.
#
# Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
# SPDX-License-Identifier: LicenseRef-Selectronic
#

export PROJECT ?= freemodbus

language = c
subdir = modbus

H_SRC = mbtcp.h
C_SRC = mbtcp.c

topdir = ..
LIB_ROOT = ${topdir}/${archdir}

INCLUDE_DIRS = \
	${topdir}/include ${topdir}/port/include \
	${topdir}/include/modbus ${topdir}/port/include/modbus \
	$(LIB_ROOT)/include $(LIB_ROOT)/include/modbus

LOCAL.C_CPPFLAGS = ${INCLUDE_DIRS:%=-I%}

include makeshift.mk library.mk
