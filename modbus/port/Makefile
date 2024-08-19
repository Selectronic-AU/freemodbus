#
# MAKEFILE -- build rules for freemodbus/modbus/port.
#
# Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
# SPDX-License-Identifier: LicenseRef-Selectronic
#

export PROJECT ?= freemodbus

subdir = modbus/port

topdir = ..
LIB_ROOT = ${topdir}/${archdir}

# Based on the OS, select the appropriate port.
PORT[freertos] = freertos
PORT[cygwin_nt] = posix
PORT[darwin] = posix
PORT[linux] = posix
PORT[mingw32_nt] = windows $(error "MinGW32 port not implemented")
PORT[mingw64_nt] = windows $(error "MinGW64 port not implemented")
PORT[Windows_NT] = windows $(error "Windows port not implemented")

HOST_OS ?= ${OS}
PORT_OS = $(patsubst freertos-%,freertos %,${HOST_OS})
PORTS = $(foreach o,${PORT_OS},${PORT[$o]})

SUBDIRS = include ${PORTS}
SUBLIB_SRC = ${PORTS:%=%/$(archdir)/lib.$a}

include makeshift.mk library.mk

build@freertos: build@include
build@posix: build@include
