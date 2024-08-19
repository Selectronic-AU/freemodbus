#
# MAKEFILE -- build rules for freemodbus/modbus/port.
#
# Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
# SPDX-License-Identifier: LicenseRef-Selectronic
#

export PROJECT ?= freemodbus

subdir = modbus/port

# Relative path to top of freeMODBUS sources.
top_mbdir = ..

# Name and type of library.
LIB = freemodbus-port
LIB_TYPE = static

# Build location of the library.
LIB_ROOT = ${top_mbdir}/${archdir}

# Based on the OS, select the appropriate port.
PORT[freertos] = freertos
# PORT[cygwin_nt] = posix
# PORT[darwin] = posix
# PORT[linux] = posix
# PORT[mingw32_nt] = windows
# PORT[mingw64_nt] = windows
# PORT[Windows_NT] = windows

HOST_OS ?= ${OS}
PORT_OS = $(patsubst freertos-%,freertos %,${HOST_OS})
PORTS = $(foreach o,${PORT_OS},${PORT[$o]})

SUBDIRS = include ${PORTS}
SUBLIB_SRC = ${PORTS:%=%/$(archdir)/lib.$a}

include makeshift.mk library.mk

build@freertos: build@include
build@posix: build@include
