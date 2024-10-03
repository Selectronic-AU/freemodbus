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

#ifndef _MB_FRAME_H
#define _MB_FRAME_H

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"

#ifdef __cplusplus
/* *INDENT-OFF* */
PR_BEGIN_EXTERN_C
/* *INDENT-ON* */
#endif

/*!
 * Constants which defines the format of a modbus frame. The example is
 * shown for a Modbus RTU/ASCII frame. Note that the Modbus PDU is not
 * dependent on the underlying transport.
 *
 * <code>
 * <--------------------- MODBUS SERIAL LINE ADU (1) ---------------------->
 *              <------------- MODBUS PDU (1') -------------->
 *  +-----------+---------------+----------------------------+-------------+
 *  | Address   | Function Code | Data                       | CRC/LRC     |
 *  +-----------+---------------+----------------------------+-------------+
 *  |           |               |                                   |
 * (2)        (3/2')           (3')                                (4)
 *
 * (1)  ... MB_SER_ADU_SIZE_MAX = 256
 * (2)  ... MB_SER_ADU_ADDR_OFF = 0
 * (3)  ... MB_SER_ADU_PDU_OFF  = 1
 * (4)  ... MB_SER_ADU_SIZE_CRC = 2
 *
 * (1') ... MB_PDU_SIZE_MAX     = 253
 * (2') ... MB_PDU_FUNC_OFF     = 0
 * (3') ... MB_PDU_DATA_OFF     = 1
 *
 *  <------------------------ MODBUS TCP ADU (1") ------------------------->
 *                            <------------- MODBUS PDU (1') -------------->
 *  +-------------------------+---------------+----------------------------+
 *  | MBAP Header             | Function Code | Data                       |
 *  +-------------------------+---------------+----------------------------+
 *  |                         |               |
 * (2")                    (3"/2')           (3')
 *
 * (1") ... MB_TCP_ADU_SIZE_MAX = 260
 * (2") ... MB_TCP_ADU_MBAP_OFF = 0
 * (3") ... MB_TCP_ADU_PDU_OFF  = 7
 *
 * </code>
 */

/* ----------------------- Defines ------------------------------------------*/

#define MB_SER_ADU_SIZE_MAX 256 /*!< Maximum size of a serial ADU. */
#define MB_SER_ADU_ADDR_OFF 0   /*!< Offset of address in serial ADU. */
#define MB_SER_ADU_PDU_OFF  1   /*!< Offset of PDU in serial ADU. */
#define MB_SER_ADU_SIZE_CRC 2   /*!< Size of CRC in serial ADU. */

#define MB_PDU_SIZE_MAX     253 /*!< Maximum size of a PDU. */
#define MB_PDU_SIZE_MIN     1   /*!< Minimum size of a PDU (e.g. function code). */
#define MB_PDU_FUNC_OFF     0   /*!< Offset of function code in PDU. */
#define MB_PDU_DATA_OFF     1   /*!< Offset for response data in PDU. */

#define MB_TCP_ADU_SIZE_MAX 260 /*!< Maximum size of a TCP ADU. */
#define MB_TCP_ADU_MBAP_OFF 0   /*!< Offset of MBAP header in TCP ADU. */
#define MB_TCP_ADU_PDU_OFF  1   /*!< Offset of PDU in TCP ADU. */

#define MB_TCP_UDP_PORT     502 /*!< MODBUS port. */

/* ----------------------- Prototypes  0-------------------------------------*/
typedef void ( *pvMBFrameStart )( void );

typedef void ( *pvMBFrameStop )( void );

typedef eMBErrorCode ( *peMBFrameReceive )( UCHAR * pucRcvAddress, UCHAR ** pucFrame, USHORT * pusLength );

typedef eMBErrorCode ( *peMBFrameSend )( UCHAR slaveAddress, const UCHAR * pucFrame, USHORT usLength );

typedef void ( *pvMBFrameClose )( void );

#ifdef __cplusplus
/* *INDENT-OFF* */
PR_END_EXTERN_C
/* *INDENT-ON* */
#endif

#endif
