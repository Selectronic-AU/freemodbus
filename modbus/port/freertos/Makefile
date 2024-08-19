#
# MAKEFILE -- build rules for freemodbus/modbus/port/freertos.
#
# Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
# SPDX-License-Identifier: LicenseRef-Selectronic
#

export PROJECT ?= freemodbus

language = c
subdir = modbus/port

C_SRC = freertos_event.c freertos_timer.c

topdir = ../..
LIB_ROOT = ${topdir}/${archdir}

INCLUDE_DIRS = \
	${topdir}/include ${topdir}/port/include \
	${topdir}/include/modbus ${topdir}/port/include/modbus \
	$(LIB_ROOT)/include $(LIB_ROOT)/include/modbus

LOCAL.C_CPPFLAGS = ${INCLUDE_DIRS:%=-I%}

include makeshift.mk library.mk
