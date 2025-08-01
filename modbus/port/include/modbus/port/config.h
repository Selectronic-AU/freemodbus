/**
 * Modbus port configuration defaults.
 *
 * Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-Selectronic
 */

#ifndef MODBUS_PORT_CONFIG_H
#define MODBUS_PORT_CONFIG_H

#ifndef MB_PORT_POSIX_TIMERS
#define MB_PORT_POSIX_TIMERS ( 0 )
#endif

#ifndef MB_PORT_POSIX_EVENT_QUEUE
#define MB_PORT_POSIX_EVENT_QUEUE ( 0 )
#endif

#ifndef MB_PORT_POSIX_SERIAL
#define MB_PORT_POSIX_SERIAL ( 0 )
#endif

#ifndef MB_PORT_POSIX_SOCKETS
#define MB_PORT_POSIX_SOCKETS ( 0 )
#endif

#ifndef MB_PORT_FREERTOS_TIMERS
#define MB_PORT_FREERTOS_TIMERS ( 0 )
#endif

#ifndef MB_PORT_FREERTOS_EVENT_QUEUE
#define MB_PORT_FREERTOS_EVENT_QUEUE ( 0 )
#endif

#endif /* MODBUS_PORT_CONFIG_H */
