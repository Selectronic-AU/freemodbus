/**
 * Modbus FreeRTOS port declarations.
 *
 * Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-Selectronic
 */

#ifndef MODBUS_PORT_FREERTOS_H
#define MODBUS_PORT_FREERTOS_H

#include <modbus/port/config.h>

#if MB_PORT_FREERTOS

#include <FreeRTOS.h>
#include <queue.h>  /* <freertos/queue.h> */
#include <task.h>   /* <freertos/task.h> */
#include <timers.h> /* <freertos/timers.h> */

#define ENTER_CRITICAL_SECTION( ) taskENTER_CRITICAL( )
#define EXIT_CRITICAL_SECTION( )  taskEXIT_CRITICAL( )

/* Neither the Posix nor the MSVC-Mingw ports have real interrupts, and
 * both required special handling to safely simulate interrupts.  As at
 * present we are not endeavouring to simulate real interrupts we shall
 * define IS_ISR_CONTEXT( ) to always return 0 on these platforms.  For
 * most (we'll assume all) Cortex-M ports will implement the function
 * xPortIsInsideInterrupt which returns true if we are inside an ISR.
 *
 * Test:
 * | Macro                | Platform or Architecture                        |
 * |:---------------------|:------------------------------------------------|
 * | __linux__            | Linux                                           |
 * | __CYGWIN__           | Cygwin                                          |
 * | _WIN32               | Windows/MinGW                                   |
 * | __ARM_ARCH_6M__      | Cortex-M0                                       |
 * | __ARM_ARCH_7M__      | Cortex-M3                                       |
 * | __ARM_ARCH_7EM__     | Cortex-M4, Cortex-M4F, Cortex-M7                |
 * | __ARM_ARCH_8M_BASE__ | Cortex-M23                                      |
 * | __ARM_ARCH_8M_MAIN__ | Cortex-M33, Cortex-M35P, Cortex-M55, Cortex-M85 |
 *
 */

#if defined( __linux__ ) || defined( __CYGWIN__ ) || defined( _WIN32 )
#define IS_ISR_CONTEXT( ) ( 0 )
#elif defined( __ARM_ARCH_6M__ ) || defined( __ARM_ARCH_7M__ ) || defined( __ARM_ARCH_7EM__ )                          \
    || defined( __ARM_ARCH_8M_BASE__ ) || defined( __ARM_ARCH_8M_MAIN__ )
#define IS_ISR_CONTEXT( ) xPortIsInsideInterrupt( )
#else
#error The FreeRTOS port for this architecture is not supported.
#endif

#endif /* MB_PORT_FREERTOS */

#endif /* MODBUS_PORT_FREERTOS_H */
