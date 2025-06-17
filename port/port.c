/*
 * FreeModbus Library: STM32 Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
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
 * File: $Id: portevent.c,v 1.60 2013/12/28 9:18:05 Armink $
 */

/* ----------------------- System includes --------------------------------*/

/* ----------------------- Modbus includes ----------------------------------*/
#include "port.h"

/* ----------------------- Variables ----------------------------------------*/

/* ----------------------- Start implementation -----------------------------*/
void
EnterCriticalSection( void )
{
    //壽敕擁笢剿
    __disable_irq(  );
}

void
ExitCriticalSection( void )
{
    //羲擁笢剿
    __enable_irq(  );
}

void
vMBDelay( ULONG nCount )
{
    for( ; nCount > 0; nCount-- );
}

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0

/**
 *
 * Just use it for modbus master.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 */
void
vMBMasterErrorCBRespondTimeout( UCHAR ucDestAddress, const UCHAR *pucPDUData, USHORT ucPDULength )
{

}

void
vMBMasterErrorCBReceiveData( UCHAR ucDestAddress, const UCHAR *pucPDUData, USHORT ucPDULength )
{

}

void
vMBMasterErrorCBExecuteFunction( UCHAR ucDestAddress, const UCHAR *pucPDUData, USHORT ucPDULength )
{

}
#endif
