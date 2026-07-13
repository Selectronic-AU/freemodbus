/**
 * Modbus default port declarations.
 *
 * Provides default implementations for port interfaces.  Essentially defines
 * empty macros and no-op implementations for non-RTOS environments.
 *
 * Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-Selectronic
 */

#ifndef MODBUS_PORT_DEFAULTS_H
#define MODBUS_PORT_DEFAULTS_H

#ifndef ENTER_CRITICAL_SECTION
#define ENTER_CRITICAL_SECTION( )
#endif

#ifndef EXIT_CRITICAL_SECTION
#define EXIT_CRITICAL_SECTION( )
#endif

#endif /* MODBUS_PORT_DEFAULTS_H */
