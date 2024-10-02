/**
 * Modbus FreeRTOS port declarations.
 *
 * Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-Selectronic
 */

#ifndef MODBUS_PORT_FREERTOS_H
#define MODBUS_PORT_FREERTOS_H

#if !defined( __freertos__ )
#error "This file should only be included in a FreeRTOS build"
#endif

#include <FreeRTOS.h>
#include <queue.h>  /* <freertos/queue.h> */
#include <task.h>   /* <freertos/task.h> */
#include <timers.h> /* <freertos/timers.h> */

#define ENTER_CRITICAL_SECTION( ) taskENTER_CRITICAL( )
#define EXIT_CRITICAL_SECTION( )  taskEXIT_CRITICAL( )

#endif /* MODBUS_PORT_FREERTOS_H */
