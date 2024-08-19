#
# FREERTOS.MK	--Macros and definitions for FreeRTOS.
#

FREERTOS_ROOT ?= $(error define FREERTOS_ROOT to path to FreeRTOS sources)
FREERTOS_CONFIG_H ?= $(error define FREERTOS_CONFIG_H to path to FreeRTOSConfig.h)

export FREERTOS_ROOT := $(realpath ${FREERTOS_ROOT})
export FREERTOS_CONFIG_H := $(realpath ${FREERTOS_CONFIG_H})

BASE_OS = $(patsubst freertos-%,%,${OS})

FREERTOS.PORT[linux] = portable/ThirdParty/GCC/Posix
FREERTOS.PORT[freertos] = FREERTOS.PORT[${ARCH}]

FREERTOS.PORT[arm-cm0] = portable/GCC/ARM_CM0
FREERTOS.PORT[arm-cm3] = portable/GCC/ARM_CM3
FREERTOS.PORT[arm-cm4f] = portable/GCC/ARM_CM4F
FREERTOS.PORT[arm-cm7] = portable/GCC/ARM_CM7

FREERTOS.PORT[${BASE_OS}] ?= $(error "FreeRTOS port not implemented for ${BASE_OS}")

FREERTOS.INCLUDE_DIRS = \
	${FREERTOS_ROOT}/include \
	${FREERTOS_ROOT}/${FREERTOS.PORT[${BASE_OS}]} \
	$(dir ${FREERTOS_CONFIG_H})

FREERTOS.DEFINES = \
	__freertos__

FREERTOS.C_DEFS = ${FREERTOS.DEFINES:%=-D%}
FREERTOS.C_CPPFLAGS = ${FREERTOS.INCLUDE_DIRS:%=-I%}
