#
# FREERTOS.MK	--Macros and definitions for FreeRTOS.
#

# Get the underlying OS name from the OS variable.
BASE_OS = $(patsubst freertos-%,%,${OS})

# Check that FREERTOS_ROOT and FREERTOS_CONFIG_H are defined.
FREERTOS_ROOT ?= $(error define FREERTOS_ROOT to path to FreeRTOS sources)
FREERTOS_CONFIG_H ?= $(error define FREERTOS_CONFIG_H to path to FreeRTOSConfig.h)

# Force FREERTOS_ROOT and FREERTOS_CONFIG_H to be absolute paths.
export FREERTOS_ROOT := $(realpath ${FREERTOS_ROOT})
export FREERTOS_CONFIG_H := $(realpath ${FREERTOS_CONFIG_H})

# FreeRTOS port source paths, based on the possible OS values.
FREERTOS.PORT[cygwin_nt] = portable/ThirdParty/GCC/Posix
FREERTOS.PORT[linux] = portable/ThirdParty/GCC/Posix
FREERTOS.PORT[mingw64_nt] = portable/MSVC-MingW
FREERTOS.PORT[Windows_NT] = portable/MSVC-MingW

# FreeRTOS port source paths, when OS is "none", try to look up using the
# value given to the -mcpu option in ARCH.CFLAGS.
FREERTOS.MCPU = $(patsubst -mcpu=%,%,$(filter -mcpu=%,${ARCH.CFLAGS}))
FREERTOS.PORT[none] = ${FREERTOS.PORT[${FREERTOS.MCPU}]}
FREERTOS.PORT[cortex-m0] = portable/GCC/ARM_CM0
FREERTOS.PORT[cortex-m3] = portable/GCC/ARM_CM3
FREERTOS.PORT[cortex-m4] = portable/GCC/ARM_CM4F
FREERTOS.PORT[cortex-m7] = portable/GCC/ARM_CM7/r0p1

# Select the FreeRTOS port source path based on the OS value.
FREERTOS.PORT = ${FREERTOS.PORT[${BASE_OS}]}
FREERTOS.PORT ?= $(error no FreeRTOS port for ${BASE_OS})

# FreeRTOS kernel include paths.
FREERTOS.INCLUDE_DIRS = \
	${FREERTOS_ROOT}/include ${FREERTOS_ROOT}/${FREERTOS.PORT} \
	$(dir ${FREERTOS_CONFIG_H})

# FreeRTOS kernel definitions.
FREERTOS.DEFINES[none] = ${FREERTOS.DEFINES[${FREERTOS.MCPU}]}
FREERTOS.DEFINES[cygwin_nt] = ${FREERTOS.DEFINES[posix]}
FREERTOS.DEFINES[linux] = ${FREERTOS.DEFINES[posix]}
FREERTOS.DEFINES[posix] = \
	_XOPEN_SOURCE=700

FREERTOS.DEFINES = \
	${FREERTOS.DEFINES[${BASE_OS}]} \
	__freertos__

# FreeRTOS flags and options.
FREERTOS.C_DEFS = ${FREERTOS.DEFINES:%=-D%}
FREERTOS.C_FLAGS =
FREERTOS.C_WARN_FLAGS =
FREERTOS.C_CPPFLAGS = ${FREERTOS.INCLUDE_DIRS:%=-I%}
FREERTOS.C_SHARED_FLAGS =