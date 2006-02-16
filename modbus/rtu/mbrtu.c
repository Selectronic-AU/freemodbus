 /*
  * FreeRTOS Modbus Libary: A Modbus serial implementation for FreeRTOS
  * Copyright (C) 2006 Christian Walter <wolti@sil.at>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  *
  * File: $Id: mbrtu.c,v 1.4 2006/02/25 18:38:03 wolti Exp $
  */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbrtu.h"
#include "mbframe.h"

#include "mbcrc.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_SER_PDU_SIZE_MIN     4       /*!< Minimum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_MAX     256     /*!< Maximum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_CRC     2       /*!< Size of CRC field in PDU. */
#define MB_SER_PDU_ADDR_OFF     0       /*!< Offset of slave address in Ser-PDU. */
#define MB_SER_PDU_PDU_OFF      1       /*!< Offset of Modbus-PDU in Ser-PDU. */

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    STATE_RX_INIT,              /*!< Receiver is in initial state. */
    STATE_RX_INIT_T15,          /*!< Receiver is in init and timer t1.5 expired. */
    STATE_RX_IDLE,              /*!< Receiver is in idle state. */
    STATE_RX_RCV,               /*!< Frame is beeing received. */
    STATE_RX_RCV_T15,           /*!< Frame is beeing received and timer t1.5 expired. */
    STATE_RX_ERROR,             /*!< If the frame is invalid. */
} eMBRcvState;

typedef enum
{
    STATE_TX_IDLE,              /*!< Transmitter is in idle state. */
    STATE_TX_XMIT,              /*!< Transmitter is in transfer state. */
} eMBSndState;

/* ----------------------- Static variables ---------------------------------*/
static volatile eMBSndState eSndState;
static volatile eMBRcvState eRcvState = STATE_RX_INIT;

volatile UCHAR  ucRTUBuf[MB_SER_PDU_SIZE_MAX];

static volatile UCHAR *pucSndBufferCur;
static volatile USHORT usSndBufferCount;

static volatile USHORT usRcvBufferPos;

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode
eMBRTUInit( UCHAR ucSlaveAddress, ULONG ulBaudRate, eMBParity eParity )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          usTimerT15_50us;
    USHORT          usTimerT35_50us;

    ENTER_CRITICAL_SECTION(  );

    /* If baudrate > 19200 then we should use the fixed timer values
     * t15 = 750us and t35 = 1750us. Otherwise t15 must be 1.5 times
     * the character time and similary for t35.
     */
    if( ulBaudRate > 19200 )
    {
        usTimerT15_50us = 15;   /* 750us. */
        usTimerT35_50us = 35;   /* 1800us. */
    }
    else
    {
        /* The timer reload value for a character is given by:
         *
         * ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
         *             = 11 * Ticks_per_1s / Baudrate
         *             = 220000 / Baudrate
         * The reload for t1.5 is 1.5 times this value and similary
         * for t3.5.
         */
        usTimerT15_50us = ( 3UL * 220000UL ) / ( 2UL * ulBaudRate );
        usTimerT35_50us = ( 7UL * 220000UL ) / ( 2UL * ulBaudRate );
    }
    if( xMBPortTimersInit( usTimerT15_50us, usTimerT35_50us ) != TRUE )
    {
        eStatus = MB_EPORTERR;
    }
    /* Modbus RTU uses 8 Databits. */
    if( xMBPortSerialInit( ulBaudRate, 8, eParity ) != TRUE )
    {
        eStatus = MB_EPORTERR;
    }

    EXIT_CRITICAL_SECTION(  );

    return eStatus;
}

eMBErrorCode
eMBRTUStart( void )
{
    ENTER_CRITICAL_SECTION(  );

    /* Initially the receiver is in the state STATE_RX_INIT. we start
     * the timer and if no character is received within t3.5 we change
     * to STATE_RX_IDLE. This makes sure that we delay startup of the
     * modbus protocol stack until the bus is free.
     */
    vMBPortSerialEnable( TRUE, FALSE );
    vMBPortTimersEnable(  );

    EXIT_CRITICAL_SECTION(  );

    return MB_ENOERR;
}

eMBErrorCode
eMBRTUReceive( UCHAR * pucRcvAddress, UCHAR ** pucFrame, USHORT * pusLength )
{
    BOOL            xFrameReceived = FALSE;
    eMBErrorCode    eStatus = MB_ENOERR;

    ENTER_CRITICAL_SECTION(  );
    assert( usRcvBufferPos < MB_SER_PDU_SIZE_MAX );

    /* Length and CRC check */
    if( ( usRcvBufferPos > MB_SER_PDU_SIZE_MIN )
        && ( usMBCRC16( ( UCHAR * ) ucRTUBuf, usRcvBufferPos ) == 0 ) )
    {
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there. 
         */
        *pucRcvAddress = ucRTUBuf[MB_SER_PDU_ADDR_OFF];

        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.
         */
        *pusLength =
            usRcvBufferPos - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_CRC;

        /* Return the start of the Modbus PDU to the caller. */
        *pucFrame = ( UCHAR * ) & ucRTUBuf[MB_SER_PDU_PDU_OFF];
        xFrameReceived = TRUE;
    }
    else
    {
        eStatus = MB_EIO;
    }

    EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

eMBErrorCode
eMBRTUSend( UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          usCRC16;

    ENTER_CRITICAL_SECTION(  );

    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if( eRcvState == STATE_RX_IDLE )
    {
        /* First byte before the Modbus-PDU is the slave address. */
        pucSndBufferCur = ( UCHAR * ) pucFrame - 1;
        usSndBufferCount = 1;

        /* Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU. */
        pucSndBufferCur[MB_SER_PDU_ADDR_OFF] = ucSlaveAddress;
        usSndBufferCount += usLength;

        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        usCRC16 = usMBCRC16( ( UCHAR * ) pucSndBufferCur, usSndBufferCount );
        ucRTUBuf[usSndBufferCount++] = usCRC16 & 0xFF;
        ucRTUBuf[usSndBufferCount++] = usCRC16 >> 8;

        /* Activate the transmitter. */
        eSndState = STATE_TX_XMIT;
        vMBPortSerialEnable( FALSE, TRUE );
    }
    else
    {
        eStatus = MB_EIO;
    }
    EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

BOOL
xMBRTUReceiveFSM( void )
{
    BOOL            xTaskNeedSwitch = FALSE;
    UCHAR           ucByte;

    assert( eSndState == STATE_TX_IDLE );

    switch ( eRcvState )
    {
            /* If we have received a character in the init state we have to
             * wait until the frame is finished.
             */
        case STATE_RX_INIT:
            vMBPortTimersEnable(  );
            break;

            /* Timer t1.5 expired in initial state. */
        case STATE_RX_INIT_T15:
            break;

            /* In the error state we wait until all characters in the 
             * damaged frame are transmitted.
             */
        case STATE_RX_ERROR:
            vMBPortTimersEnable(  );

            /* In the idle state we wait for a new character. If a character
             * is received the t1.5 and t3.5 timers are started and the
             * receiver is in the state STATE_RX_RECEIVCE.
             */
        case STATE_RX_IDLE:
            usRcvBufferPos = 0;
            ( void )xMBPortSerialGetByte( &ucByte );
            ucRTUBuf[usRcvBufferPos++] = ucByte;
            eRcvState = STATE_RX_RCV;

            /* Enable t1.5 and t3.5 timers. */
            vMBPortTimersEnable(  );
            break;

            /* Timer t1.5 expired and we received a character. This is
             * an error.
             */
        case STATE_RX_RCV_T15:
            eRcvState = STATE_RX_ERROR;
            break;

            /* We are currently receiving a frame. Reset the timer after
             * every character received. If more than the maximum possible
             * number of bytes in a modbus frame is received the frame is 
             * ignored.
             */
        case STATE_RX_RCV:
            if( usRcvBufferPos < MB_SER_PDU_SIZE_MAX )
            {
                ( void )xMBPortSerialGetByte( &ucByte );
                ucRTUBuf[usRcvBufferPos++] = ucByte;
            }
            else
            {
                eRcvState = STATE_RX_ERROR;
            }
            vMBPortTimersEnable(  );
            break;

    }
    return xTaskNeedSwitch;
}

BOOL
xMBRTUTransmitFSM( void )
{
    BOOL            xNeedPoll = FALSE;

    assert( eRcvState == STATE_RX_IDLE );

    switch ( eSndState )
    {
            /* We should not get a transmitter event if the transmitter is in
             * idle state.  */
        case STATE_TX_IDLE:
            /* enable receiver/disable transmitter. */
            vMBPortSerialEnable( TRUE, FALSE );
            break;

        case STATE_TX_XMIT:
            /* check if we are finished. */
            if( usSndBufferCount != 0 )
            {
                xMBPortSerialPutByte( *pucSndBufferCur );
                pucSndBufferCur++;      /* next byte in sendbuffer. */
                usSndBufferCount--;
            }
            else
            {
                xNeedPoll = xMBPortEventPost( EV_FRAME_SENT );
                /* Disable transmitter. This prevents another transmit buffer
                 * empty interrupt. */
                vMBPortSerialEnable( TRUE, FALSE );
                eSndState = STATE_TX_IDLE;
            }
            break;
    }

    return xNeedPoll;
}

BOOL
xMBRTUTimerT15Expired( void )
{
    switch ( eRcvState )
    {
        case STATE_RX_INIT:
            eRcvState = STATE_RX_INIT_T15;
            break;
        case STATE_RX_RCV:
            eRcvState = STATE_RX_RCV_T15;
            break;
        default:
            assert( ( eRcvState == STATE_RX_INIT )
                    || ( eRcvState == STATE_RX_RCV ) );
            break;
    }

    /* no context switch required. */
    return FALSE;
}

BOOL
xMBRTUTimerT35Expired( void )
{
    BOOL            xNeedPoll = FALSE;

    switch ( eRcvState )
    {
            /* Timer t35 expired. Startup phase is finished. */
        case STATE_RX_INIT_T15:
            xNeedPoll = xMBPortEventPost( EV_READY );
            break;

            /* A frame was received and t35 expired. Notify the listener that
             * a new frame was received. */
        case STATE_RX_RCV_T15:
            xNeedPoll = xMBPortEventPost( EV_FRAME_RECEIVED );
            break;

            /* An error occured while receiving the frame. */
        case STATE_RX_ERROR:
            break;

            /* Function called in an illegal state. */
        default:
            assert( ( eRcvState == STATE_RX_INIT_T15 ) ||
                    ( eRcvState == STATE_RX_RCV_T15 )
                    || ( eRcvState == STATE_RX_ERROR ) );
    }

    vMBPortTimersDisable(  );
    eRcvState = STATE_RX_IDLE;

    return xNeedPoll;
}