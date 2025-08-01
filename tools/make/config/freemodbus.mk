#
# CONFIG/FREEMODBUS.MK	-- Configuration macros and definitions for FreeMODBUS.
#
# Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
# SPDX-License-Identifier: LicenseRef-Selectronic
#

ifeq (${CONFIG/FREEMODBUS.MK},)
CONFIG/FREEMODBUS.MK := 1

# Favour FreeRTOS port options over alernatives.
export MB_PORT_FREERTOS ?= 1
export MB_PORT_FREERTOS_EVENT_QUEUE ?= ${MB_PORT_FREERTOS}
export MB_PORT_FREERTOS_TIMERS ?= ${MB_PORT_FREERTOS}

# # Get the underlying OS name from the OS variable.
# BASE_OS ?= $(patsubst freertos-%,%,${OS})

# # Determine if the BASE_OS is a POSIX OS.
# export _MB_PORT_POSIX[cygwin_nt] = 1
# export _MB_PORT_POSIX[darwin] = 1
# export _MB_PORT_POSIX[linux] = 1
# export MB_PORT_POSIX ?= $(or ${_MB_PORT_POSIX[${BASE_OS}]},0)

# ifeq (${MB_PORT_POSIX},1)

# # Unless FreeRTOS timers and event queue port is enabled, enable Posix
# # timers, event queue, by default if Posix port is enabled.
# export MB_PORT_POSIX_EVENT_QUEUE ?= $(if ${MB_PORT_FREERTOS_EVENT_QUEUE},0,${MB_PORT_POSIX})
# export MB_PORT_POSIX_TIMERS ?= $(if ${MB_PORT_FREERTOS_TIMERS},0,${MB_PORT_POSIX})

# # Enable Posix serial and sockets by default if Posix port is enabled.
# export MB_PORT_POSIX_SERIAL ?= ${MB_PORT_POSIX}
# export MB_PORT_POSIX_SOCKETS ?= ${MB_PORT_POSIX}

# endif

# Enable FreeMODBUS protocols.
# export MB_ASCII_ENABLED ?= 1
export MB_RTU_ENABLED ?= 1
# export MB_TCP_ENABLED ?= 1

# Create a list of all FreeMODBUS (MB_*) variables.
FREEMODBUS.VARS = $(sort $(filter MB_%,${.VARIABLES}))

# Construct -D flags from enabled feature variables (MB_*).
FREEMODBUS.C_DEFS = $(foreach v,${FREEMODBUS.VARS},-D$v=${$v})

endif # CONFIG/FREEMODBUS.MK
