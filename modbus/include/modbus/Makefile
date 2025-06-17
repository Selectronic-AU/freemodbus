#
# MAKEFILE -- build rules for freemodbus/modbus/include/modbus.
#
# Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
# SPDX-License-Identifier: LicenseRef-Selectronic
#

export PROJECT ?= freemodbus

language = c
subdir = modbus

H_SRC = mb.h mb_m.h mbconfig.h mbframe.h mbfunc.h mbport.h \
    mbproto.h mbutils.h

# Relative path to top of freeMODBUS sources.
top_mbdir = ../..

# Type of library.
LIB_TYPE = header

# Build location of the library.
LIB_ROOT = ${top_mbdir}/${archdir}

INCLUDE_DIRS = \
	${top_mbdir}/include ${top_mbdir}/port/include \
	${top_mbdir}/include/modbus ${top_mbdir}/port/include/modbus \
	$(LIB_ROOT)/include $(LIB_ROOT)/include/modbus

LOCAL.C_CPPFLAGS = ${INCLUDE_DIRS:%=-I%}

include makeshift.mk library.mk
