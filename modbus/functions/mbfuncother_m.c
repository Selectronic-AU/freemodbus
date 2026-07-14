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
#include "mb_m.h"
#include "mbframe.h"
#include "mbfunc.h"
#include "mbutils.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_REPORT_MIN            3
#define MB_PDU_REPORT_BYTECNT_OFF    ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_REPORT_SLAVEID_OFF    ( MB_PDU_DATA_OFF + 1 )
#define MB_PDU_REPORT_STATUS_OFF     ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_REPORT_ADDITIONAL_OFF ( MB_PDU_DATA_OFF + 3 )

/* ----------------------- Start implementation -----------------------------*/
#if MB_MASTER_ASCII_ENABLED > 0 || MB_MASTER_RTU_ENABLED > 0
#if MB_FUNC_OTHER_REP_SLAVEID_ENABLED > 0

/**
 * This function will request the slave ID from a Modbus slave device.
 *
 * @param ucSndAddr slave address
 * @param lTimeOut timeout (-1 waits forever)
 *
 * @return error code
 */
eMBMasterReqErrCode
eMBMasterReqReportSlaveID( UCHAR ucSndAddr, LONG lTimeOut )
{
    UCHAR *             ucMBFrame;
    eMBMasterReqErrCode eErrStatus = MB_MRE_NO_ERR;

    if( ( ucSndAddr == MB_ADDRESS_BROADCAST ) || ( ucSndAddr > MB_MASTER_TOTAL_SLAVE_NUM ) )
        eErrStatus = MB_MRE_ILL_ARG;
    else if( xMBMasterRunResTake( lTimeOut ) == FALSE )
        eErrStatus = MB_MRE_MASTER_BUSY;
    else
    {
        vMBMasterGetPDUSndBuf( &ucMBFrame );
        vMBMasterSetDestAddress( ucSndAddr );
        ucMBFrame[MB_PDU_FUNC_OFF] = MB_FUNC_OTHER_REPORT_SLAVEID;
        vMBMasterSetPDUSndLength( MB_PDU_SIZE_MIN );
        ( void ) xMBMasterPortEventPost( EV_MASTER_FRAME_SENT );
        eErrStatus = eMBMasterWaitRequestFinish( );
    }
    return eErrStatus;
}

eMBException
eMBMasterFuncReportSlaveID( UCHAR * pucFrame, USHORT * usLen )
{
    eMBErrorCode eErrorCode = MB_EINVAL;
    USHORT       usPduLen   = ( *usLen >= MB_PDU_DATA_OFF ) ? *usLen - MB_PDU_DATA_OFF : 0;

    /* Must contain at least the slave ID and run-indicator status. */
    if( usPduLen >= MB_PDU_REPORT_MIN )
    {
        USHORT usByteCnt = pucFrame[MB_PDU_REPORT_BYTECNT_OFF];
        if( usByteCnt >= 2 && usByteCnt == ( usPduLen - 1 ) )
        {
            USHORT usSlaveID            = pucFrame[MB_PDU_REPORT_SLAVEID_OFF];
            USHORT usRunIndicatorStatus = pucFrame[MB_PDU_REPORT_STATUS_OFF];

            /* Make callback to report the slave ID value. */
            eErrorCode = eMBMasterReportSlaveIDCB( usSlaveID, usRunIndicatorStatus,
                                                   &pucFrame[MB_PDU_REPORT_ADDITIONAL_OFF], usByteCnt - 2 );

            /* If an error occurred, convert it into a Modbus exception. */
            if( eErrorCode != MB_ENOERR )
            {
                eErrorCode = MB_EINVAL;
            }
        }
    }
    return prveMBError2Exception( eErrorCode );
}

#endif
#endif
