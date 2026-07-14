/*
 * FreeModbus Library: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006-2018 Christian Walter <cwalter@embedded-solutions.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/* ----------------------- System includes ----------------------------------*/
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbframe.h"
#include "mbfunc.h"

#if MB_FUNC_OTHER_REP_SLAVEID_ENABLED > 0

/* ----------------------- Defines ------------------------------------------*/

#define MB_REPORT_MIN            3
#define MB_REPORT_BYTECNT_OFF    ( 0 )
#define MB_REPORT_SLAVEID_OFF    ( 1 )
#define MB_REPORT_STATUS_OFF     ( 2 )
#define MB_REPORT_ADDITIONAL_OFF ( 3 )

#if MB_FUNC_OTHER_REP_SLAVEID_BUF < MB_REPORT_MIN
#error "Define at least 3 bytes for MB_FUNC_OTHER_REP_SLAVEID_BUF"
#endif

/* ----------------------- Static variables ---------------------------------*/

static UCHAR  ucMBSlaveID[MB_FUNC_OTHER_REP_SLAVEID_BUF];
static USHORT usMBSlaveIDLen;

/* ----------------------- Start implementation -----------------------------*/

eMBErrorCode
eMBSetSlaveID( UCHAR ucSlaveID, BOOL xIsRunning, UCHAR const * pucAdditional, USHORT usAdditionalLen )
{
    eMBErrorCode eStatus = MB_ENOERR;

    /* The first three bytes are reserved for the Modbus response layout:
     * byte count, slave ID, and run indicator status. The remaining bytes
     * are available for additional identification data. */

    if( usAdditionalLen <= ( MB_FUNC_OTHER_REP_SLAVEID_BUF - MB_REPORT_MIN )
        && usAdditionalLen <= ( MB_PDU_SIZE_MAX - MB_PDU_DATA_OFF - MB_REPORT_MIN ) )
    {
        ucMBSlaveID[MB_REPORT_BYTECNT_OFF] = 2;
        ucMBSlaveID[MB_REPORT_SLAVEID_OFF] = ucSlaveID;
        ucMBSlaveID[MB_REPORT_STATUS_OFF]  = ( UCHAR ) ( xIsRunning ? 0xFF : 0x00 );
        if( usAdditionalLen > 0 )
        {
            memcpy( &ucMBSlaveID[MB_REPORT_ADDITIONAL_OFF], pucAdditional, ( size_t ) usAdditionalLen );
            ucMBSlaveID[MB_REPORT_BYTECNT_OFF] += ( UCHAR ) usAdditionalLen;
        }
        usMBSlaveIDLen = MB_REPORT_MIN + usAdditionalLen;
    }
    else
    {
        eStatus = MB_ENORES;
    }
    return eStatus;
}

eMBException
eMBFuncReportSlaveID( UCHAR * pucFrame, USHORT * usLen )
{
    memcpy( &pucFrame[MB_PDU_DATA_OFF], &ucMBSlaveID[0], ( size_t ) usMBSlaveIDLen );
    *usLen = ( USHORT ) ( MB_PDU_DATA_OFF + usMBSlaveIDLen );
    return MB_EX_NONE;
}

#endif
