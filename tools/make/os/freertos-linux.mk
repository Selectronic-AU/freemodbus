#
# FREERTOS-LINUX.MK	--Macros and definitions for FreeRTOS on Linux.
#

include os/linux.mk
include os/freertos.mk

OS.C_DEFS += ${FREERTOS.C_DEFS}
OS.C_FLAGS += ${FREERTOS.C_FLAGS}
OS.C_WARN_FLAGS += ${FREERTOS.C_WARN_FLAGS}
OS.C_CPPFLAGS += ${FREERTOS.C_CPPFLAGS}
OS.C_SHARED_FLAGS += ${FREERTOS.C_SHARED_FLAGS}
