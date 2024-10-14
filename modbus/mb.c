 /*
  * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
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
  * File: $Id: mb.c,v 1.16 2006/08/24 11:19:03 wolti Exp $
  */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbfunc.h"

#include "mbport.h"
#if MB_RTU_ENABLED == 1
#include "mbrtu.h"
#endif
#if MB_ASCII_ENABLED == 1
#include "mbascii.h"
#endif
#if MB_TCP_ENABLED == 1
#include "mbtcp.h"
#endif

#ifndef MB_PORT_HAS_CLOSE
#define MB_PORT_HAS_CLOSE 0
#endif

/* ----------------------- Static variables ---------------------------------*/

static UCHAR    ucMBAddress;
static eMBMode  eMBCurrentMode;

static enum
{
    STATE_ENABLED,
    STATE_DISABLED,
    STATE_NOT_INITIALIZED
} eMBState = STATE_NOT_INITIALIZED;

/* Functions pointer which are initialized in eMBInit( ). Depending on the
 * mode (RTU or ASCII) the are set to the correct implementations.
 */
static peMBFrameSend peMBFrameSendCur;
static pvMBFrameStart pvMBFrameStartCur;
static pvMBFrameStop pvMBFrameStopCur;
static peMBFrameReceive peMBFrameReceiveCur;
static pvMBFrameClose pvMBFrameCloseCur;

/* Callback functions required by the porting layer. They are called when
 * an external event has happend which includes a timeout or the reception
 * or transmission of a character.
 */
BOOL( *pxMBFrameCBByteReceived ) ( void );
BOOL( *pxMBFrameCBTransmitterEmpty ) ( void );
BOOL( *pxMBPortCBTimerExpired ) ( void );

BOOL( *pxMBFrameCBReceiveFSMCur ) ( void );
BOOL( *pxMBFrameCBTransmitFSMCur ) ( void );

/* An array of Modbus functions handlers which associates Modbus function 
 * codes with implementing functions.
 */
static xMBFunctionHandler xFuncHandlers[MB_FUNC_HANDLERS_MAX] = {
#if MB_FUNC_OTHER_REP_SLAVEID_ENABLED > 0
    {MB_FUNC_OTHER_REPORT_SLAVEID, eMBFuncReportSlaveID},
#endif
#if MB_FUNC_READ_INPUT_ENABLED > 0
    {MB_FUNC_READ_INPUT_REGISTER, eMBFuncReadInputRegister},
#endif
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0
    {MB_FUNC_READ_HOLDING_REGISTER, eMBFuncReadHoldingRegister},
#endif
#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_REGISTERS, eMBFuncWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_READ_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_REGISTER, eMBFuncWriteHoldingRegister},
#endif
#if MB_FUNC_READ_COILS_ENABLED > 0
    {MB_FUNC_READ_COILS, eMBFuncReadCoils},
#endif
#if MB_FUNC_WRITE_COIL_ENABLED > 0
    {MB_FUNC_WRITE_SINGLE_COIL, eMBFuncWriteCoil},
#endif
#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_COILS, eMBFuncWriteMultipleCoils},
#endif
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
    {MB_FUNC_READ_DISCRETE_INPUTS, eMBFuncReadDiscreteInputs},
#endif
};

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode
eMBInit( eMBMode eMode, UCHAR ucSlaveAddress, UCHAR ucPort, ULONG ulBaudRate,
         eMBParity eParity )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    /* check preconditions */
    if( ( ucSlaveAddress == MB_ADDRESS_BROADCAST ) ||
        ( ucSlaveAddress < MB_ADDRESS_MIN ) ||
        ( ucSlaveAddress > MB_ADDRESS_MAX ) )
    {
        eStatus = MB_EINVAL;
    }
   ucMBAddress = ucSlaveAddress;

    switch( eMode )
    {
#if MB_RTU_ENABLED > 0
    case MB_RTU:
       pvMBFrameStartCur = eMBRTUStart;
       pvMBFrameStopCur = eMBRTUStop;
       peMBFrameSendCur = eMBRTUSend;
       peMBFrameReceiveCur = eMBRTUReceive;
       pvMBFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBPortClose : NULL;
       pxMBFrameCBByteReceived = xMBRTUReceiveFSM;
       pxMBFrameCBTransmitterEmpty = xMBRTUTransmitFSM;
       pxMBPortCBTimerExpired = xMBRTUTimerT35Expired;

       eStatus = eMBRTUInit( ucMBAddress, ucPort, ulBaudRate, eParity );
       break;
#endif
#if MB_ASCII_ENABLED > 0
    case MB_ASCII:
       pvMBFrameStartCur = eMBASCIIStart;
       pvMBFrameStopCur = eMBASCIIStop;
       peMBFrameSendCur = eMBASCIISend;
       peMBFrameReceiveCur = eMBASCIIReceive;
       pvMBFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBPortClose : NULL;
       pxMBFrameCBByteReceived = xMBASCIIReceiveFSM;
       pxMBFrameCBTransmitterEmpty = xMBASCIITransmitFSM;
       pxMBPortCBTimerExpired = xMBASCIITimerT1SExpired;

       eStatus = eMBASCIIInit( ucMBAddress, ucPort, ulBaudRate, eParity );
       break;
#endif
    default:
        eStatus = MB_EINVAL;
    }

    if( eStatus == MB_ENOERR )
    {
        if( !xMBPortEventInit(  ) )
        {
            /* port dependent event module initalization failed. */
            eStatus = MB_EPORTERR;
        }
        else
        {
            eMBCurrentMode = eMode;
            eMBState = STATE_DISABLED;
        }
    }
    return eStatus;
}

#if MB_TCP_ENABLED > 0
eMBErrorCode
eMBTCPInit( USHORT ucTCPPort )
{
     eMBErrorCode    eStatus = MB_ENOERR;

     if( ( eStatus = eMBTCPDoInit( ucTCPPort ) ) == MB_ENOERR )
     {
         pvMBFrameStartCur = eMBTCPStart;
         pvMBFrameStopCur = eMBTCPStop;
         peMBFrameReceiveCur = eMBTCPReceive;
         peMBFrameSendCur = eMBTCPSend;
         pvMBFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBTCPPortClose : NULL;

         ucMBAddress = MB_TCP_PSEUDO_ADDRESS;
         eMBCurrentMode = MB_TCP;
         eMBState = STATE_DISABLED;
     }
     return eStatus; 
}
#endif

eMBErrorCode
eMBClose( void )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( ( eMBState == STATE_DISABLED ) && ( pvMBFrameCloseCur != NULL ) )
    {
        pvMBFrameCloseCur( );
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

eMBErrorCode
eMBEnable( void )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( eMBState == STATE_DISABLED )
    {
        /* Activate the protocol stack. */
        pvMBFrameStartCur(  );
        eMBState = STATE_ENABLED;
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

eMBErrorCode
eMBDisable( void )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( eMBState == STATE_ENABLED )
    {
        pvMBFrameStopCur(  );
        eMBState = STATE_DISABLED;
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

eMBErrorCode
eMBPoll(  )
{
    static UCHAR   *ucMBFrame;
    static UCHAR    ucRcvAddress;
    static UCHAR    ucFunctionCode;
    static USHORT   usLength;
    static eMBException eException;

    int             i;
    eMBErrorCode    eStatus = MB_ENOERR;
    eMBEventType    eEvent;

    /* Check if the protocol stack is ready. */
    if( eMBState != STATE_ENABLED )
    {
        return MB_EILLSTATE;
    }

    /* Check if there is a event available. If not return control to caller.
     * Otherwise we will handle the event. */
    if( xMBPortEventGet( &eEvent ) == TRUE )
    {
        switch ( eEvent )
        {
            case EV_READY:
                break;

            case EV_FRAME_RECEIVED:
                eStatus = peMBFrameReceiveCur( &ucRcvAddress, &ucMBFrame,
                                               &usLength );
                if( eStatus == MB_ENOERR )
                {
                    /* Check if the frame is for us. If not ignore the frame. */
                    if( ( ucRcvAddress == ucMBAddress ) ||
                        ( ucRcvAddress == MB_ADDRESS_BROADCAST ) )
                    {
                        ( void )xMBPortEventPost( EV_EXECUTE );
                    }
                }
                break;

            case EV_EXECUTE:
                ucFunctionCode = ucMBFrame[MB_PDU_FUNC_OFF];
                eException = MB_EX_ILLEGAL_FUNCTION;
                for( i = 0; i < MB_FUNC_HANDLERS_MAX; i++ )
                {
                    /* No more function handlers registered. Abort. */
                    if( xFuncHandlers[i].ucFunctionCode == 0 )
                    {
                        break;
                    }
                    else if( xFuncHandlers[i].ucFunctionCode == ucFunctionCode )
                    {
                        eException =
                            xFuncHandlers[i].pxHandler( ucMBFrame, &usLength );
                        break;
                    }
                }

                /* If the request was not sent to the broadcast address we
                 * return a reply. */
                if( ucRcvAddress != MB_ADDRESS_BROADCAST )
                {
                    if( eException != MB_EX_NONE )
                    {
                        /* An exception occured. Build an error frame. */
                        usLength = 0;
                        ucMBFrame[usLength++] = ucFunctionCode | MB_FUNC_ERROR;
                        ucMBFrame[usLength++] = eException;
                    }
                    eStatus =
                        peMBFrameSendCur( ucMBAddress, ucMBFrame, usLength );
                }
                break;

            case EV_FRAME_SENT:
                break;
        }
    }
    return MB_ENOERR;
}
