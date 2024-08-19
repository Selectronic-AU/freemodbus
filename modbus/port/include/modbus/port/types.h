/**
 * Modbus port types and definitions.
 *
 * Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-Selectronic
 */

#ifndef MODBUS_PORT_TYPES_H
#define MODBUS_PORT_TYPES_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#if defined( USE_FULL_ASSERT ) && __has_include( <stm32_assert.h> )
#include <stm32_assert.h>
#else
#define assert_param( expr ) ( ( void ) 0U )
#endif

#ifndef __unused
#define __unused __attribute__( ( unused ) )
#endif

#ifndef __weak
#define __weak __attribute__( ( weak ) )
#endif

#define INLINE inline

#define PR_BEGIN_EXTERN_C                                                                                              \
    extern "C"                                                                                                         \
    {
#define PR_END_EXTERN_C }

typedef bool           BOOL;
typedef char           CHAR;
typedef long           LONG;
typedef short          SHORT;
typedef unsigned char  UCHAR;
typedef unsigned long  ULONG;
typedef unsigned short USHORT;

#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif

#endif /* MODBUS_PORT_TYPES_H */
