/**
 * @brief Modbus FreeRTOS timers.
 *
 * Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-Selectronic
 */

#include <modbus/mb_m.h>
#include <modbus/port/freertos.h>
#include <modbus/port/types.h>

#if defined( MB_PORT_FREERTOS_TIMERS ) && MB_PORT_FREERTOS_TIMERS

/// @brief The FreeRTOS tick period in microseconds.
#ifndef portTICK_RATE_MICROSECONDS
#define portTICK_RATE_MICROSECONDS ( ( ULONG ) ( portTICK_PERIOD_MS * 1000 ) )
#endif

static USHORT          usT35TimeOut50us;

/// @brief Timer structure for the FreeModbus stack.
static StaticTimer_t mb_port_timer;
static StaticTimer_t mb_master_port_timer;

/// @brief Timer handle for the FreeModbus stack.
static TimerHandle_t mb_port_timer_handle;
static TimerHandle_t mb_master_port_timer_handle;

/// @brief Timer callback for the FreeModbus stack.
/// @param xTimer
static void
xMBPortTimersCallback( __attribute__( ( unused ) ) TimerHandle_t xTimer )
{
    pxMBPortCBTimerExpired( );
}

/// @brief Timer callback for the FreeModbus stack.
/// @param xTimer
static void
xMBMasterPortTimersCallback( __attribute__( ( unused ) ) TimerHandle_t xTimer )
{
    pxMBMasterPortCBTimerExpired( );
}

/// @brief Initialize the timer for the FreeModbus stack.
/// @param usTimeOut50us The timeout in 50us increments.
/// @return TRUE if successful, FALSE otherwise.
BOOL
xMBPortTimersInit( USHORT usTimeOut50us )
{
    TickType_t PeriodInTicks =
        ( TickType_t ) ( ( ( ( ULONG ) usTimeOut50us * 50 ) + ( portTICK_RATE_MICROSECONDS - 1 ) )
                         / portTICK_RATE_MICROSECONDS );

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

BOOL
xMBMasterPortTimersInit( USHORT timeout50us )
{

    TickType_t placeholder = 1;
     /* backup T35 ticks */
    usT35TimeOut50us = timeout50us;
    mb_master_port_timer_handle = xTimerCreateStatic( "modbus", placeholder, pdFALSE, NULL,
                                                      xMBMasterPortTimersCallback, &mb_master_port_timer );
    // Return TRUE only if both handles are valid
    return ( mb_master_port_timer_handle != NULL );
}

/// @brief Enable the timer for the FreeModbus stack.
void
vMBMasterPortTimersT35Enable( void )
{
    TickType_t ticks = (TickType_t)(((ULONG)usT35TimeOut50us * 50 + (portTICK_RATE_MICROSECONDS - 1))
                                     / portTICK_RATE_MICROSECONDS);

    vMBMasterSetCurTimerMode(MB_TMODE_T35);
    xTimerChangePeriod(mb_master_port_timer_handle, ticks, portMAX_DELAY);
    xTimerStart(mb_master_port_timer_handle, portMAX_DELAY);
}

void
vMBMasterPortTimersConvertDelayEnable( void )
{
    TickType_t ticks = pdMS_TO_TICKS(MB_MASTER_DELAY_MS_CONVERT);

    vMBMasterSetCurTimerMode(MB_TMODE_CONVERT_DELAY);

    xTimerChangePeriod(mb_master_port_timer_handle, ticks, portMAX_DELAY);
    xTimerStart(mb_master_port_timer_handle, portMAX_DELAY);
}

void
vMBMasterPortTimersRespondTimeoutEnable( void )
{
    TickType_t ticks = pdMS_TO_TICKS(MB_MASTER_TIMEOUT_MS_RESPOND);

    vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT);

    xTimerChangePeriod(mb_master_port_timer_handle, ticks, portMAX_DELAY);
    xTimerStart(mb_master_port_timer_handle, portMAX_DELAY);
}

void vMBMasterPortTimersDisable(void) {
     ( void ) xTimerStop( mb_master_port_timer_handle, portMAX_DELAY );
}

/// @brief Close the timer for the FreeModbus stack.
void
xMBMasterPortTimersClose( void )
{
    ( void ) xTimerDelete( mb_master_port_timer_handle, portMAX_DELAY );
}

void
prvvTIMERExpiredISR( void )
{
    ( void ) pxMBMasterPortCBTimerExpired( );
}

#endif /* MB_PORT_FREERTOS_TIMERS */
