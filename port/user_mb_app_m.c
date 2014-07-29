/*
 * FreeModbus Library: user callback functions and buffer define in master mode
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
 * File: $Id: user_mb_app_m.c,v 1.60 2013/11/23 11:49:05 Armink $
 */
#include "user_mb_app.h"

/*-----------------------Master mode use these variables----------------------*/
#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
//Master mode:DiscreteInputs variables
USHORT          usMDiscInStart = M_DISCRETE_INPUT_START;

#if      M_DISCRETE_INPUT_NDISCRETES%8
UCHAR           ucMDiscInBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_DISCRETE_INPUT_NDISCRETES / 8 + 1];
#else
UCHAR           ucMDiscInBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_DISCRETE_INPUT_NDISCRETES / 8];
#endif
//Master mode:Coils variables
USHORT          usMCoilStart = M_COIL_START;

#if      M_COIL_NCOILS%8
UCHAR           ucMCoilBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_COIL_NCOILS / 8 + 1];
#else
UCHAR           ucMCoilBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_COIL_NCOILS / 8];
#endif
//Master mode:InputRegister variables
USHORT          usMRegInStart = M_REG_INPUT_START;
USHORT          usMRegInBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_INPUT_NREGS];

//Master mode:HoldingRegister variables
USHORT          usMRegHoldStart = M_REG_HOLDING_START;
USHORT          usMRegHoldBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];

//******************************怀敵湔隙覃滲杅**********************************
//滲杅隅砱: eMBErrorCode eMBMasterRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
//鏡    扴ㄩ怀敵湔眈壽腔髡夔ㄗ黍﹜蟀哿黍ㄘ
//諳統杅ㄩpucRegBuffer : 隙覃滲杅蔚Modbus敵湔腔絞硉迡腔遣喳
//          usAddress    : 敵湔腔宎華硊ㄛ怀敵湔腔華硊毓峓岆1-65535﹝
//          usNRegs      : 敵湔杅講
//堤諳統杅ㄩeMBErrorCode : 涴跺滲杅蔚殿隙腔渣昫鎢
//掘    蛁ㄩEditorㄩArmink 2013-11-25    Company: BXXJS
//**********************************************************************************
eMBErrorCode
eMBMasterRegInputCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex;
    USHORT         *pusRegInputBuf;
    USHORT          REG_INPUT_START;
    USHORT          REG_INPUT_NREGS;
    USHORT          usRegInStart;

    pusRegInputBuf = usMRegInBuf[ucMBMasterGetDestAddress(  ) - 1];
    REG_INPUT_START = M_REG_INPUT_START;
    REG_INPUT_NREGS = M_REG_INPUT_NREGS;
    usRegInStart = usMRegInStart;

    usAddress--;                //FreeModbus髡夔滲杅笢眒冪樓1ㄛ峈悵痐迵遣喳忑華硊珨祡ㄛ嘟熬1
    if( ( usAddress >= REG_INPUT_START ) && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = usAddress - usRegInStart;
        while( usNRegs > 0 )
        {
            pusRegInputBuf[iRegIndex] = *pucRegBuffer++ << 8;
            pusRegInputBuf[iRegIndex] |= *pucRegBuffer++;
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

//******************************悵厥敵湔隙覃滲杅**********************************
//滲杅隅砱: eMBErrorCode eMBMasterRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
//鏡    扴ㄩ悵厥敵湔眈壽腔髡夔ㄗ黍﹜蟀哿黍﹜迡﹜蟀哿迡ㄘ
//諳統杅ㄩpucRegBuffer : 彆剒猁載陔蚚誧敵湔杅硉ㄛ涴跺遣喳斛剕硌砃陔腔敵湔杅硉﹝
//                         彆衪祜梬砑眭耋絞腔杅硉ㄛ隙覃滲杅斛剕蔚絞硉迡涴跺遣喳
//          usAddress    : 敵湔腔宎華硊﹝
//          usNRegs      : 敵湔杅講
//          eMode        : 彆蜆統杅峈eMBRegisterMode::MB_REG_WRITEㄛ蚚誧腔茼蚚杅硉蔚植pucRegBuffer笢腕善載陔﹝
//                         彆蜆統杅峈eMBRegisterMode::MB_REG_READㄛ蚚誧剒猁蔚絞腔茼蚚杅擂湔揣婓pucRegBuffer笢
//堤諳統杅ㄩeMBErrorCode : 涴跺滲杅蔚殿隙腔渣昫鎢
//掘    蛁ㄩEditorㄩArmink 2013-11-25    Company: BXXJS
//**********************************************************************************
eMBErrorCode
eMBMasterRegHoldingCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex;
    USHORT         *pusRegHoldingBuf;
    USHORT          REG_HOLDING_START;
    USHORT          REG_HOLDING_NREGS;
    USHORT          usRegHoldStart;

    pusRegHoldingBuf = usMRegHoldBuf[ucMBMasterGetDestAddress(  ) - 1];
    REG_HOLDING_START = M_REG_HOLDING_START;
    REG_HOLDING_NREGS = M_REG_HOLDING_NREGS;
    usRegHoldStart = usMRegHoldStart;
    //If mode is read,the master will wirte the received date to bufffer.
    eMode = MB_REG_WRITE;

    usAddress--;                //FreeModbus髡夔滲杅笢眒冪樓1ㄛ峈悵痐迵遣喳忑華硊珨祡ㄛ嘟熬1
    if( ( usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = usAddress - usRegHoldStart;
        switch ( eMode )
        {
            /* Pass current register values to the protocol stack. */
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
                *pucRegBuffer++ = ( unsigned char )( pusRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( unsigned char )( pusRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

            /* Update current register values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
                pusRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                pusRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
            break;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

//****************************盄袨怓敵湔隙覃滲杅********************************
//滲杅隅砱: eMBErrorCode eMBMasterRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
//鏡    扴ㄩ盄袨怓敵湔眈壽腔髡夔ㄗ黍﹜蟀哿黍﹜迡﹜蟀哿迡ㄘ
//諳統杅ㄩpucRegBuffer : 弇郪傖珨跺趼誹ㄛ宎敵湔勤茼腔弇揭衾蜆趼誹pucRegBuffer腔郔腴弇LSB﹝
//                         彆隙覃滲杅猁迡涴跺遣喳ㄛ羶衄蚚善腔盄ㄗ瞰祥岆8跺珨郪腔盄袨怓ㄘ勤茼腔弇腔杅硉斛剕扢离弇0﹝
//          usAddress    : 菴珨跺盄華硊﹝
//          usNCoils     : 腔盄跺杅
//          eMode        ˙彆蜆統杅峈eMBRegisterMode::MB_REG_WRITEㄛ蚚誧腔茼蚚杅硉蔚植pucRegBuffer笢腕善載陔﹝
//                         彆蜆統杅峈eMBRegisterMode::MB_REG_READㄛ蚚誧剒猁蔚絞腔茼蚚杅擂湔揣婓pucRegBuffer笢
//堤諳統杅ㄩeMBErrorCode : 涴跺滲杅蔚殿隙腔渣昫鎢
//掘    蛁ㄩEditorㄩArmink 2013-11-25    Company: BXXJS
//**********************************************************************************
eMBErrorCode
eMBMasterRegCoilsCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex, iRegBitIndex, iNReg;
    UCHAR          *pucCoilBuf;
    USHORT          COIL_START;
    USHORT          COIL_NCOILS;
    USHORT          usCoilStart;

    iNReg = usNCoils / 8 + 1;   //梩蚚敵湔杅講

    pucCoilBuf = ucMCoilBuf[ucMBMasterGetDestAddress(  ) - 1];
    COIL_START = M_COIL_START;
    COIL_NCOILS = M_COIL_NCOILS;
    usCoilStart = usMCoilStart;
    //If mode is read,the master will wirte the received date to bufffer.
    eMode = MB_REG_WRITE;

    usAddress--;                //FreeModbus髡夔滲杅笢眒冪樓1ㄛ峈悵痐迵遣喳忑華硊珨祡ㄛ嘟熬1
    if( ( usAddress >= COIL_START ) && ( usAddress + usNCoils <= COIL_START + COIL_NCOILS ) )
    {
        iRegIndex = ( int )( usAddress - usCoilStart ) / 8;     //藩跺敵湔湔8跺
        iRegBitIndex = ( int )( usAddress - usCoilStart ) % 8;  //眈勤衾敵湔囀窒腔弇華硊
        switch ( eMode )
        {
            /* Pass current coil values to the protocol stack. */
        case MB_REG_READ:
            while( iNReg > 0 )
            {
                *pucRegBuffer++ = xMBUtilGetBits( &pucCoilBuf[iRegIndex++], iRegBitIndex, 8 );
                iNReg--;
            }
            pucRegBuffer--;
            usNCoils = usNCoils % 8;    //豻狟腔盄杅
            *pucRegBuffer = *pucRegBuffer << ( 8 - usNCoils );  //詢弇硃錨
            *pucRegBuffer = *pucRegBuffer >> ( 8 - usNCoils );
            break;

            /* Update current coil values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
            while( iNReg > 1 )  //郔綴醱豻狟懂腔杅等黃呾
            {
                xMBUtilSetBits( &pucCoilBuf[iRegIndex++], iRegBitIndex, 8, *pucRegBuffer++ );
                iNReg--;
            }
            usNCoils = usNCoils % 8;    //豻狟腔盄杅
            if( usNCoils != 0 ) //xMBUtilSetBits源楊 婓紱釬弇杅講峈0奀湔婓bug
            {
                xMBUtilSetBits( &pucCoilBuf[iRegIndex++], iRegBitIndex, usNCoils, *pucRegBuffer++ );
            }
            break;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

//****************************燭汃怀敵湔隙覃滲杅********************************
//滲杅隅砱: eMBErrorCode eMBMasterRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
//鏡    扴ㄩ燭汃怀敵湔眈壽腔髡夔ㄗ黍﹜蟀哿黍ㄘ
//諳統杅ㄩpucRegBuffer : 蚚絞腔盄杅擂載陔涴跺敵湔ㄛ宎敵湔勤茼腔弇揭衾蜆趼誹pucRegBuffer腔郔腴弇LSB﹝
//                         彆隙覃滲杅猁迡涴跺遣喳ㄛ羶衄蚚善腔盄ㄗ瞰祥岆8跺珨郪腔盄袨怓ㄘ勤茼腔弇腔杅硉斛剕扢离峈0﹝
//          usAddress    : 燭汃怀腔宎華硊
//          usNDiscrete  : 燭汃怀萸杅講
//堤諳統杅ㄩeMBErrorCode : 涴跺滲杅蔚殿隙腔渣昫鎢
//掘    蛁ㄩEditorㄩArmink 2013-11-25    Company: BXXJS
//**********************************************************************************
eMBErrorCode
eMBMasterRegDiscreteCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex, iRegBitIndex, iNReg;
    UCHAR          *pucDiscreteInputBuf;
    USHORT          DISCRETE_INPUT_START;
    USHORT          DISCRETE_INPUT_NDISCRETES;
    USHORT          usDiscreteInputStart;

    iNReg = usNDiscrete / 8 + 1;        //梩蚚敵湔杅講

    pucDiscreteInputBuf = ucMDiscInBuf[ucMBMasterGetDestAddress(  ) - 1];
    DISCRETE_INPUT_START = M_DISCRETE_INPUT_START;
    DISCRETE_INPUT_NDISCRETES = M_DISCRETE_INPUT_NDISCRETES;
    usDiscreteInputStart = usMDiscInStart;

    usAddress--;                //FreeModbus髡夔滲杅笢眒冪樓1ㄛ峈悵痐迵遣喳忑華硊珨祡ㄛ嘟熬1
    if( ( usAddress >= DISCRETE_INPUT_START )
        && ( usAddress + usNDiscrete <= DISCRETE_INPUT_START + DISCRETE_INPUT_NDISCRETES ) )
    {
        iRegIndex = ( int )( usAddress - usDiscreteInputStart ) / 8;    //藩跺敵湔湔8跺
        iRegBitIndex = ( int )( usAddress - usDiscreteInputStart ) % 8; //眈勤衾敵湔囀窒腔弇華硊

        /* Update current coil values with new values from the
         * protocol stack. */
        while( iNReg > 1 )      //郔綴醱豻狟懂腔杅等黃呾
        {
            xMBUtilSetBits( &pucDiscreteInputBuf[iRegIndex++], iRegBitIndex, 8, *pucRegBuffer++ );
            iNReg--;
        }
        usNDiscrete = usNDiscrete % 8;  //豻狟腔盄杅
        if( usNDiscrete != 0 )  //xMBUtilSetBits源楊 婓紱釬弇杅講峈0奀湔婓bug
        {
            xMBUtilSetBits( &pucDiscreteInputBuf[iRegIndex++], iRegBitIndex, usNDiscrete, *pucRegBuffer++ );
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}
#endif
