/**
 * @brief Modbus FreeRTOS event queue.
 *
 * Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-Selectronic
 */

#include <modbus/mb.h>
#include <modbus/port/freertos.h>
#include <modbus/port/types.h>

#if defined( MB_PORT_FREERTOS_EVENT_QUEUE ) && MB_PORT_FREERTOS_EVENT_QUEUE

static const UBaseType_t mb_event_queue_length = 2;
static const UBaseType_t mb_event_queue_size   = sizeof( eMBEventType );
static QueueHandle_t     mb_event_queue_handle;
static StaticQueue_t     mb_event_queue;

// static uint8_t           mb_event_queue_data[mb_event_queue_length *
// mb_event_queue_size];
static uint8_t mb_event_queue_data[2 * sizeof( eMBEventType )];

/// @brief Initialize the event queue for the FreeModbus stack.
BOOL
xMBPortEventInit( void )
{
    mb_event_queue_handle =
        xQueueCreateStatic( mb_event_queue_length, mb_event_queue_size, mb_event_queue_data, &mb_event_queue );
    return mb_event_queue_handle;
}

/// @brief Post an event to the FreeModbus stack.
/// @param eEvent The event to post.
BOOL
xMBPortEventPost( eMBEventType eEvent )
{
    return xQueueSend( mb_event_queue_handle, &eEvent, portMAX_DELAY ) != pdTRUE;
}

/// @brief Get an event from the FreeModbus stack.
/// @param eEvent Pointer to location to store the event.
BOOL
xMBPortEventGet( eMBEventType * eEvent )
{
    return xQueueReceive( mb_event_queue_handle, eEvent, portMAX_DELAY ) != pdTRUE;
}

#endif /* MB_PORT_FREERTOS_EVENT_QUEUE */
