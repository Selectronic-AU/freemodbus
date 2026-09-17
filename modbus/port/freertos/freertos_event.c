/**
 * @brief Modbus FreeRTOS event queue.
 *
 * Copyright (c) 2024 Selectronic Australia Pty Ltd. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-Selectronic
 */

#include <modbus/mb.h>
#include <modbus/mb_m.h>

#include <modbus/port/freertos.h>
#include <modbus/port/types.h>

#include <stdalign.h>

#if defined( MB_PORT_FREERTOS_EVENT_QUEUE ) && MB_PORT_FREERTOS_EVENT_QUEUE

#if !defined( configSUPPORT_STATIC_ALLOCATION ) || configSUPPORT_STATIC_ALLOCATION != 1
#error "Static allocation support must be enabled for the FreeRTOS event queue"
#endif

#if MB_SLAVE_ASCII_ENABLED > 0 || MB_SLAVE_RTU_ENABLED > 0 || MB_SLAVE_TCP_ENABLED > 0

#ifndef MB_SLAVE_FREERTOS_EVENT_QUEUE_LENGTH
#define MB_SLAVE_FREERTOS_EVENT_QUEUE_LENGTH ( 2 )
#endif

static const UBaseType_t mb_event_queue_length = MB_SLAVE_FREERTOS_EVENT_QUEUE_LENGTH;
static const UBaseType_t mb_event_queue_size   = sizeof( eMBEventType );
static QueueHandle_t     mb_event_queue_handle;
static StaticQueue_t     mb_event_queue;

// Queue storage must stay word-aligned for the FreeRTOS port.
static alignas( portBYTE_ALIGNMENT ) uint8_t
    mb_event_queue_data[MB_SLAVE_FREERTOS_EVENT_QUEUE_LENGTH * sizeof( eMBEventType )];

/// @brief Initialize the event queue for the FreeModbus stack.
BOOL
xMBPortEventInit( void )
{
    mb_event_queue_handle =
        xQueueCreateStatic( mb_event_queue_length, mb_event_queue_size, mb_event_queue_data, &mb_event_queue );
    return mb_event_queue_handle != NULL;
}

/// @brief Post an event to the FreeModbus stack.
/// @param eEvent The event to post.
BOOL
xMBPortEventPost( eMBEventType eEvent )
{
    if( IS_ISR_CONTEXT( ) )
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        const BOOL xEventPosted =
            xQueueSendFromISR( mb_event_queue_handle, &eEvent, &xHigherPriorityTaskWoken ) == pdPASS;

        if( xEventPosted && xHigherPriorityTaskWoken != pdFALSE )
        {
            portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        }
        return xEventPosted;
    }
    return xQueueSend( mb_event_queue_handle, &eEvent, portMAX_DELAY ) == pdPASS;
}

/// @brief Get an event from the FreeModbus stack.
/// @param eEvent Pointer to location to store the event.
BOOL
xMBPortEventGet( eMBEventType * eEvent )
{
    return xQueueReceive( mb_event_queue_handle, eEvent, portMAX_DELAY ) == pdPASS;
}

#endif /* MB_SLAVE_ASCII_ENABLED > 0 || MB_SLAVE_RTU_ENABLED > 0 || MB_SLAVE_TCP_ENABLED > 0 */

#if MB_MASTER_ASCII_ENABLED > 0 || MB_MASTER_RTU_ENABLED > 0

#ifndef MB_MASTER_FREERTOS_EVENT_QUEUE_LENGTH
#define MB_MASTER_FREERTOS_EVENT_QUEUE_LENGTH ( 2 )
#endif

static const UBaseType_t mb_master_event_queue_length = MB_MASTER_FREERTOS_EVENT_QUEUE_LENGTH;
static const UBaseType_t mb_master_event_queue_size   = sizeof( eMBMasterEventType );
static QueueHandle_t     mb_master_event_queue_handle;
static StaticQueue_t     mb_master_event_queue;

// Queue storage must stay word-aligned for the FreeRTOS port.
static alignas( portBYTE_ALIGNMENT ) uint8_t
    mb_master_event_queue_data[MB_MASTER_FREERTOS_EVENT_QUEUE_LENGTH * sizeof( eMBMasterEventType )];

/// @brief Initialize the event queue for the FreeModbus master stack.
/// @param  None.
/// @return TRUE if the event queue was successfully initialized, FALSE otherwise.
BOOL
xMBMasterPortEventInit( void )
{
    mb_master_event_queue_handle = xQueueCreateStatic( mb_master_event_queue_length, mb_master_event_queue_size,
                                                       mb_master_event_queue_data, &mb_master_event_queue );
    return mb_master_event_queue_handle != NULL;
}

/// @brief Post an event to the FreeModbus master stack.
/// @param eEvent The event to post.
/// @return TRUE if the event was successfully posted, FALSE otherwise.
BOOL
xMBMasterPortEventPost( eMBMasterEventType eEvent )
{
    if( IS_ISR_CONTEXT( ) )
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        const BOOL xEventPosted =
            xQueueSendFromISR( mb_master_event_queue_handle, &eEvent, &xHigherPriorityTaskWoken ) == pdPASS;

        if( xEventPosted && xHigherPriorityTaskWoken != pdFALSE )
        {
            portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        }
        return xEventPosted;
    }
    return xQueueSend( mb_master_event_queue_handle, &eEvent, portMAX_DELAY ) == pdPASS;
}

/// @brief Get an event from the FreeModbus master stack.
/// @param eEvent Pointer to location to store the event.
/// @return TRUE if an event was successfully retrieved, FALSE otherwise.
BOOL
xMBMasterPortEventGet( eMBMasterEventType * eEvent )
{
    return xQueueReceive( mb_master_event_queue_handle, eEvent, portMAX_DELAY ) == pdPASS;
}

#endif /* MB_MASTER_ASCII_ENABLED > 0 || MB_MASTER_RTU_ENABLED > 0 */

#endif /* MB_PORT_FREERTOS_EVENT_QUEUE */
