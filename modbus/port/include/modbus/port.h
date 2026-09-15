/**
 * Modbus port declarations.
 *
 * Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-Selectronic
 */

#ifndef MODBUS_PORT_H
#define MODBUS_PORT_H

#include <modbus/port/config.h>

#if defined( MB_PORT_FREERTOS ) && ( MB_PORT_FREERTOS != 0 )
#include <modbus/port/freertos.h>
#endif /* MB_PORT_FREERTOS */

#include <modbus/port/types.h>
#include <modbus/port/defaults.h>

#endif /* MODBUS_PORT_H */
