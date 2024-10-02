/**
 * @brief Modbus FreeRTOS timers.
 *
 * Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-Selectronic
 */

#include <modbus/mb.h>
#include <modbus/port/freertos.h>
#include <modbus/port/types.h>

#if defined( MB_PORT_FREERTOS_TIMERS ) && MB_PORT_FREERTOS_TIMERS

/// @brief Timer structure for the FreeModbus stack.
static StaticTimer_t mb_port_timer;

/// @brief Timer handle for the FreeModbus stack.
static TimerHandle_t mb_port_timer_handle;

/// @brief Timer callback for the FreeModbus stack.
/// @param xTimer
static void
xMBPortTimersCallback( __attribute__( ( unused ) ) TimerHandle_t xTimer )
{
    pxMBPortCBTimerExpired( );
}

/// @brief Initialize the timer for the FreeModbus stack.
/// @param usTimeOut50us The timeout in 50us increments.
/// @return TRUE if successful, FALSE otherwise.
BOOL
xMBPortTimersInit( USHORT usTimeOut50us )
{
    TickType_t PeriodInTicks = ( ( TickType_t ) usTimeOut50us * 50 ) / portTICK_RATE_MICROSECONDS;

    mb_port_timer_handle =
        xTimerCreateStatic( "modbus", PeriodInTicks, pdFALSE, NULL, xMBPortTimersCallback, &mb_port_timer );

    return mb_port_timer_handle;
}

/// @brief Close the timer for the FreeModbus stack.
void
vMBPortTimersClose( void )
{
    ( void ) xTimerDelete( mb_port_timer_handle, portMAX_DELAY );
}

/// @brief Enable the timer for the FreeModbus stack.
void
vMBPortTimersEnable( void )
{
    ( void ) xTimerStart( mb_port_timer_handle, portMAX_DELAY );
}

/// @brief Disable the timer for the FreeModbus stack.
void
vMBPortTimersDisable( void )
{
    ( void ) xTimerStop( mb_port_timer_handle, portMAX_DELAY );
}

/// @brief Delay for a specified time in milliseconds.
/// @param usTimeOutMS The time to delay in milliseconds.
void
vMBPortTimersDelay( USHORT usTimeOutMS )
{
    ( void ) vTaskDelay( usTimeOutMS / portTICK_PERIOD_MS );
}

#endif /* MB_PORT_FREERTOS_TIMERS */
