/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_bsp_timer.c
  * @brief  : bsp timer source file
  * @author : Fred
  * @history:
  *        2015-3-10    Fred    Created file
  *        ...
  ******************************************************************************
  */


#define _BSP_TIMER_MODULE_

#include "cv_includes.h"
#include "cv_bsp_timer.h"

uint32_t  TIM3_Period;
uint32_t  TIM3_Prescaler;

uint32_t  TIM2_Period;
uint32_t  TIM2_Prescaler;

void bsp_tim_deinit(void)
{
    TIM_DeInit(TIM2);
    TIM_DeInit(TIM3);
}
/*****************************************************************************
 @funcname: bsp_tim2_init
 @brief   : bsp timer 3 init  32 bit timer
 @param   : void
 @return  :
*****************************************************************************/
void bsp_tim2_init(void)
{
    uint32_t tim_clk_src_freq;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* Enable the TIM3 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_TIM2_IRQ_PRE_PRI;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_TIM2_IRQ_SUB_PRI;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /*calulate tim clock source freq*/
    if(rcc_clk_freq.PCLK1_Frequency == 0) {
        return;
    }

    if(rcc_clk_freq.HCLK_Frequency == rcc_clk_freq.PCLK1_Frequency) {
        tim_clk_src_freq = rcc_clk_freq.PCLK1_Frequency;
    }
    else {
        tim_clk_src_freq = (rcc_clk_freq.PCLK1_Frequency * 2);
    }

    TIM2_Prescaler = 0;
    TIM2_Period = ((tim_clk_src_freq/(TIM2_Prescaler+1))/TIM2_FREQ);


    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = TIM2_Period;
    /*TIM Freq=Sorce freq/(TIM_Perscaler+1)*/
    TIM_TimeBaseStructure.TIM_Prescaler = TIM2_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* TIM Interrupts enable */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    tim2_hook_func = NULL;

    /* TIM3 enable counter */
    TIM_Cmd(TIM2, ENABLE);

}

/*****************************************************************************
 @funcname: bsp_tim3_init
 @brief   : bsp timer 3 init  16bit timer
 @param   : void
 @return  :
*****************************************************************************/
void bsp_tim3_init(void)
{
    uint32_t tim_clk_src_freq;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* Enable the TIM3 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_TIM3_IRQ_PRE_PRI;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_TIM3_IRQ_SUB_PRI;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /*calulate tim clock source freq*/
    if(rcc_clk_freq.PCLK1_Frequency == 0) {
        return;
    }

    if(rcc_clk_freq.HCLK_Frequency == rcc_clk_freq.PCLK1_Frequency) {
        tim_clk_src_freq = rcc_clk_freq.PCLK1_Frequency;
    }
    else {
        tim_clk_src_freq = (rcc_clk_freq.PCLK1_Frequency * 2);
    }

    TIM3_Prescaler = 1000;
    TIM3_Period = ((tim_clk_src_freq/(TIM3_Prescaler+1))/TIM3_FREQ);


    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = TIM3_Period;
    /*TIM Freq=Sorce freq/(TIM_Perscaler+1)*/
    TIM_TimeBaseStructure.TIM_Prescaler = TIM3_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* TIM Interrupts enable */
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    tim3_hook_func = NULL;

    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);

}


/*****************************************************************************
 @funcname: TIM3_IRQHandler
 @brief   : This function handles TIM3 global interrupt request.
 @param   : void
 @return  :
*****************************************************************************/
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

        if(tim3_hook_func != NULL) {

            (*tim3_hook_func)();
        }
    }
}

/*****************************************************************************
 @funcname: TIM2_IRQHandler
 @brief   : This function handles TIM3 global interrupt request.
 @param   : void
 @return  :
*****************************************************************************/
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

        if(tim2_hook_func != NULL) {

            (*tim2_hook_func)();
        }
    }
}



