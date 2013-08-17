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
 * File: $Id: porttimer.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void     prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{

    uint16_t        PrescalerValue = 0;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //====================================奀笘場宎趙===========================
    //妏夔隅奀3奀笘
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );
    //====================================隅奀場宎趙===========================
    //隅奀奀潔價饜离佽隴
    //HCLK峈72MHzㄛAPB1冪徹2煦峈36MHz
    //TIM3腔奀笘捷綴峈72MHzㄗ茞璃赻雄捷,湛善郔湮ㄘ
    //TIM3腔煦炵杅峈3599ㄛ奀潔價薹峈72 / (1 + Prescaler) = 20KHz,價袧峈50us
    //TIM郔湮數杅硉峈usTim1Timerout50u

    PrescalerValue = ( uint16_t ) ( SystemCoreClock / 20000 ) - 1;
    //隅奀1場宎趙
    TIM_TimeBaseStructure.TIM_Period = ( uint16_t ) usTim1Timerout50us;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure );
    //啎蚾婥妏夔
    TIM_ARRPreloadConfig( TIM3, ENABLE );
    //====================================笢剿場宎趙===========================
    //扢离NVIC蚥珂撰煦郪峈Group2ㄩ0-3梩宒蚥珂撰ㄛ0-3腔砒茼宒蚥珂撰
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );
    //壺祛堤笢剿梓祩弇
    TIM_ClearITPendingBit( TIM3, TIM_IT_Update );
    //隅奀3祛堤笢剿壽敕
    TIM_ITConfig( TIM3, TIM_IT_Update, DISABLE );
    //隅奀3輦夔
    TIM_Cmd( TIM3, DISABLE );
    return TRUE;
}

void
vMBPortTimersEnable(  )
{
    TIM_ClearITPendingBit( TIM3, TIM_IT_Update );
    TIM_ITConfig( TIM3, TIM_IT_Update, ENABLE );
    TIM_SetCounter( TIM3, 0 );
    TIM_Cmd( TIM3, ENABLE );
}

void
vMBPortTimersDisable(  )
{
    TIM_ClearITPendingBit( TIM3, TIM_IT_Update );
    TIM_ITConfig( TIM3, TIM_IT_Update, DISABLE );
    TIM_SetCounter( TIM3, 0 );
    TIM_Cmd( TIM3, DISABLE );
}

void
prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}

void
TIM3_IRQHandler( void )
{
    rt_interrupt_enter(  );
    if( TIM_GetITStatus( TIM3, TIM_IT_Update ) != RESET )
    {

        TIM_ClearFlag( TIM3, TIM_FLAG_Update ); //笢剿梓暮
        TIM_ClearITPendingBit( TIM3, TIM_IT_Update );   //壺隅奀T3祛堤笢剿梓祩弇
        prvvTIMERExpiredISR(  );
    }
    rt_interrupt_leave(  );
}
