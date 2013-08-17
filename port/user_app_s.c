#include "user_app.h"

/* ----------------------- Variables ---------------------------------*/
USHORT          usDiscreteInputStart = DISCRETE_INPUT_START;
UCHAR           usDiscreteInputBuf[DISCRETE_INPUT_NDISCRETES / 8];
USHORT          usCoilStart = COIL_START;
UCHAR           usCoilBuf[COIL_NCOILS / 8];
USHORT          usRegInputStart = REG_INPUT_START;
USHORT          usRegInputBuf[REG_INPUT_NREGS];
USHORT          usRegHoldingStart = REG_HOLDING_START;
USHORT          usRegHoldingBuf[REG_HOLDING_NREGS];

//******************************怀敵湔隙覃滲杅**********************************
//滲杅隅砱: eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
//鏡    扴ㄩ怀敵湔眈壽腔髡夔ㄗ黍﹜蟀哿黍ㄘ
//諳統杅ㄩpucRegBuffer : 隙覃滲杅蔚Modbus敵湔腔絞硉迡腔遣喳
//          usAddress    : 敵湔腔宎華硊ㄛ怀敵湔腔華硊毓峓岆1-65535﹝
//          usNRegs      : 敵湔杅講
//堤諳統杅ㄩeMBErrorCode : 涴跺滲杅蔚殿隙腔渣昫鎢
//掘    蛁ㄩEditorㄩArmink 2010-10-31    Company: BXXJS
//**********************************************************************************
eMBErrorCode
eMBRegInputCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START ) && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
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
//滲杅隅砱: eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
//鏡    扴ㄩ悵厥敵湔眈壽腔髡夔ㄗ黍﹜蟀哿黍﹜迡﹜蟀哿迡ㄘ
//諳統杅ㄩpucRegBuffer : 彆剒猁載陔蚚誧敵湔杅硉ㄛ涴跺遣喳斛剕硌砃陔腔敵湔杅硉﹝
//                         彆衪祜梬砑眭耋絞腔杅硉ㄛ隙覃滲杅斛剕蔚絞硉迡涴跺遣喳
//          usAddress    : 敵湔腔宎華硊﹝
//          usNRegs      : 敵湔杅講
//          eMode        : 彆蜆統杅峈eMBRegisterMode::MB_REG_WRITEㄛ蚚誧腔茼蚚杅硉蔚植pucRegBuffer笢腕善載陔﹝
//                         彆蜆統杅峈eMBRegisterMode::MB_REG_READㄛ蚚誧剒猁蔚絞腔茼蚚杅擂湔揣婓pucRegBuffer笢
//堤諳統杅ㄩeMBErrorCode : 涴跺滲杅蔚殿隙腔渣昫鎢
//掘    蛁ㄩEditorㄩArmink 2010-10-31    Company: BXXJS
//**********************************************************************************
eMBErrorCode
eMBRegHoldingCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        switch ( eMode )
        {
            /* Pass current register values to the protocol stack. */
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
                *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

            /* Update current register values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
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
//滲杅隅砱: eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
//鏡    扴ㄩ盄袨怓敵湔眈壽腔髡夔ㄗ黍﹜蟀哿黍﹜迡﹜蟀哿迡ㄘ
//諳統杅ㄩpucRegBuffer : 弇郪傖珨跺趼誹ㄛ宎敵湔勤茼腔弇揭衾蜆趼誹pucRegBuffer腔郔腴弇LSB﹝
//                         彆隙覃滲杅猁迡涴跺遣喳ㄛ羶衄蚚善腔盄ㄗ瞰祥岆8跺珨郪腔盄袨怓ㄘ勤茼腔弇腔杅硉斛剕扢离弇0﹝
//          usAddress    : 菴珨跺盄華硊﹝
//          usNCoils     : 腔盄跺杅
//          eMode        ˙彆蜆統杅峈eMBRegisterMode::MB_REG_WRITEㄛ蚚誧腔茼蚚杅硉蔚植pucRegBuffer笢腕善載陔﹝
//                         彆蜆統杅峈eMBRegisterMode::MB_REG_READㄛ蚚誧剒猁蔚絞腔茼蚚杅擂湔揣婓pucRegBuffer笢
//堤諳統杅ㄩeMBErrorCode : 涴跺滲杅蔚殿隙腔渣昫鎢
//掘    蛁ㄩEditorㄩArmink 2010-10-31    Company: BXXJS
//**********************************************************************************
eMBErrorCode
eMBRegCoilsCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex, iRegBitIndex, iNReg;

    iNReg = usNCoils / 8 + 1;   //梩蚚敵湔杅講
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
                *pucRegBuffer++ = xMBUtilGetBits( &usCoilBuf[iRegIndex++], iRegBitIndex, 8 );
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
                xMBUtilSetBits( &usCoilBuf[iRegIndex++], iRegBitIndex, 8, *pucRegBuffer++ );
                iNReg--;
            }
            usNCoils = usNCoils % 8;    //豻狟腔盄杅
            xMBUtilSetBits( &usCoilBuf[iRegIndex++], iRegBitIndex, usNCoils, *pucRegBuffer++ );
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
//滲杅隅砱: eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
//鏡    扴ㄩ燭汃怀敵湔眈壽腔髡夔ㄗ黍﹜蟀哿黍ㄘ
//諳統杅ㄩpucRegBuffer : 蚚絞腔盄杅擂載陔涴跺敵湔ㄛ宎敵湔勤茼腔弇揭衾蜆趼誹pucRegBuffer腔郔腴弇LSB﹝
//                         彆隙覃滲杅猁迡涴跺遣喳ㄛ羶衄蚚善腔盄ㄗ瞰祥岆8跺珨郪腔盄袨怓ㄘ勤茼腔弇腔杅硉斛剕扢离峈0﹝
//          usAddress    : 燭汃怀腔宎華硊
//          usNDiscrete  : 燭汃怀萸杅講
//堤諳統杅ㄩeMBErrorCode : 涴跺滲杅蔚殿隙腔渣昫鎢
//掘    蛁ㄩEditorㄩArmink 2010-10-31    Company: BXXJS
//**********************************************************************************
eMBErrorCode
eMBRegDiscreteCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex, iRegBitIndex, iNReg;

    iNReg = usNDiscrete / 8 + 1;        //梩蚚敵湔杅講
    if( ( usAddress >= DISCRETE_INPUT_START )
        && ( usAddress + usNDiscrete <= DISCRETE_INPUT_START + DISCRETE_INPUT_NDISCRETES ) )
    {
        iRegIndex = ( int )( usAddress - usDiscreteInputStart ) / 8;    //藩跺敵湔湔8跺
        iRegBitIndex = ( int )( usAddress - usDiscreteInputStart ) % 8; //眈勤衾敵湔囀窒腔弇華硊
        while( iNReg > 0 )
        {
            *pucRegBuffer++ = xMBUtilGetBits( &usDiscreteInputBuf[iRegIndex++], iRegBitIndex, 8 );
            iNReg--;
        }
        pucRegBuffer--;
        usNDiscrete = usNDiscrete % 8;  //豻狟腔盄杅
        *pucRegBuffer = *pucRegBuffer << ( 8 - usNDiscrete );   //詢弇硃錨
        *pucRegBuffer = *pucRegBuffer >> ( 8 - usNDiscrete );
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}
