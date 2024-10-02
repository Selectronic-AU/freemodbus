/**
 * Modbus port declarations.
 *
 * Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-Selectronic
 */

#ifndef MODBUS_PORT_H
#define MODBUS_PORT_H

#include <modbus/port/types.h>

#if defined( __freertos__ )
#include <modbus/port/freertos.h>
#endif

#endif /* MODBUS_PORT_H */
