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
 * File: $Id: portserial.c,v 1.60 2013/08/13 15:07:05 Armink $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void     prvvUARTTxReadyISR( void );
static void     prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/

void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    if( xRxEnable )
    {
        SLAVE_RS485_RECEIVE_MODE;
        USART_ITConfig( USART1, USART_IT_RXNE, ENABLE );
    }
    else
    {
        SLAVE_RS485_SEND_MODE;
        USART_ITConfig( USART1, USART_IT_RXNE, DISABLE );
    }
    if( xTxEnable )
    {
        USART_ITConfig( USART1, USART_IT_TXE, ENABLE );
    }
    else
    {
        USART_ITConfig( USART1, USART_IT_TXE, DISABLE );
    }
}

void
vMBPortClose( void )
{
    USART_ITConfig( USART1, USART_IT_TXE | USART_IT_RXNE, DISABLE );
    USART_Cmd( USART1, DISABLE );
}

//蘇珨跺植儂 揹諳1 疏杻薹褫扢离  髒潰桄褫扢离
BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //======================奀笘場宎趙=======================================
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_USART1, ENABLE );
    //======================IO場宎趙=======================================
    //USART1_TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    //USART1_RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    //饜离485楷冞睿諉彶耀宒
//    TODO   婃奀珂迡B13 脹眳綴郪厙聆彸奀婬党蜊
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init( GPIOB, &GPIO_InitStructure );
    //======================揹諳場宎趙=======================================
    USART_InitStructure.USART_BaudRate = ulBaudRate;
    //扢离苺桄耀宒
    switch ( eParity )
    {
    case MB_PAR_NONE:          //拸苺桄
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        break;
    case MB_PAR_ODD:           //苺桄
        USART_InitStructure.USART_Parity = USART_Parity_Odd;
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        break;
    case MB_PAR_EVEN:          //髒苺桄
        USART_InitStructure.USART_Parity = USART_Parity_Even;
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        break;
    default:
        return FALSE;
    }

    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    if( ucPORT != 1 )
        return FALSE;

    ENTER_CRITICAL_SECTION(  ); //壽擁笢剿

    USART_Init( USART1, &USART_InitStructure );
    USART_ITConfig( USART1, USART_IT_RXNE, ENABLE );
    USART_Cmd( USART1, ENABLE );

    //=====================笢剿場宎趙======================================
    //扢离NVIC蚥珂撰煦郪峈Group2ㄩ0-3梩宒蚥珂撰ㄛ0-3腔砒茼宒蚥珂撰
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    EXIT_CRITICAL_SECTION(  );  //羲擁笢剿

    return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    USART_SendData( USART1, ucByte );
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR *pucByte )
{
    *pucByte = USART_ReceiveData( USART1 );
    return TRUE;
}

/*
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call
 * xMBPortSerialPutByte( ) to send the character.
 */
void
prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/*
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void
prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}

/*******************************************************************************
 * Function Name  : USART1_IRQHandler
 * Description    : This function handles USART1 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void
USART1_IRQHandler( void )
{
    rt_interrupt_enter(  );
    //諉彶笢剿
    if( USART_GetITStatus( USART1, USART_IT_RXNE ) == SET )
    {
        USART_ClearITPendingBit( USART1, USART_IT_RXNE );
        prvvUARTRxISR(  );
    }
    //楷冞笢剿
    if( USART_GetITStatus( USART1, USART_IT_TXE ) == SET )
    {
        prvvUARTTxReadyISR(  );
    }
    rt_interrupt_leave(  );
}
