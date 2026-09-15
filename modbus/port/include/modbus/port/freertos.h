/**
 * Modbus FreeRTOS port declarations.
 *
 * Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-Selectronic
 */

#ifndef MODBUS_PORT_FREERTOS_H
#define MODBUS_PORT_FREERTOS_H

#if MB_PORT_FREERTOS

#include <FreeRTOS.h>
#include <queue.h>  /* <freertos/queue.h> */
#include <task.h>   /* <freertos/task.h> */
#include <timers.h> /* <freertos/timers.h> */

#define ENTER_CRITICAL_SECTION( ) taskENTER_CRITICAL( )
#define EXIT_CRITICAL_SECTION( )  taskEXIT_CRITICAL( )

#if defined(__linux__) || defined(_WIN32)
#define IS_ISR_CONTEXT( ) ( 0 )
#else
#define IS_ISR_CONTEXT( ) xPortIsInsideInterrupt( )
#endif

#endif /* MB_PORT_FREERTOS */

#endif /* MODBUS_PORT_FREERTOS_H */
