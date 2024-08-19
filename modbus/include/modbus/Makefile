#
# MAKEFILE -- build rules for freemodbus/modbus/ascii.
#
# Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
# SPDX-License-Identifier: LicenseRef-Selectronic
#

export PROJECT ?= freemodbus

language = c
subdir = modbus

H_SRC = mb.h mbconfig.h mbframe.h mbfunc.h mbport.h mbproto.h mbutils.h

topdir = ../..
LIB_ROOT = ${topdir}/${archdir}
LIB_TYPE = header

INCLUDE_DIRS = \
	${topdir}/include ${topdir}/port/include \
	${topdir}/include/modbus ${topdir}/port/include/modbus \
	$(LIB_ROOT)/include $(LIB_ROOT)/include/modbus

LOCAL.C_CPPFLAGS = ${INCLUDE_DIRS:%=-I%}

include makeshift.mk library.mk
