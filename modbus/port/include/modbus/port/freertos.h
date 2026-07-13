/**
 * Modbus FreeRTOS port declarations.
 *
 * Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-Selectronic
 */

#ifndef MODBUS_PORT_FREERTOS_H
#define MODBUS_PORT_FREERTOS_H

#if defined( MB_PORT_FREERTOS ) && ( MB_PORT_FREERTOS != 0 )

#include <FreeRTOS.h>
#include <queue.h>  /* <freertos/queue.h> */
#include <task.h>   /* <freertos/task.h> */
#include <timers.h> /* <freertos/timers.h> */

#define ENTER_CRITICAL_SECTION( ) taskENTER_CRITICAL( )
#define EXIT_CRITICAL_SECTION( )  taskEXIT_CRITICAL( )

#endif /* MB_PORT_FREERTOS */

#endif /* MODBUS_PORT_FREERTOS_H */
