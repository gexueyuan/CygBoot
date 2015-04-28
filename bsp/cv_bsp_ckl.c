/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_bsp_ckl.c
  * @brief  : bsp clock related source file
  * @author : Fred
  * @history:
  *        2015-3-10    Fred    Created file
  *        ...
  ******************************************************************************
  */

#define _BSP_CLK_MODULE_

#include "cv_bsp_ckl.h"

/*****************************************************************************
 @funcname: bsp_ckl_inf_get
 @brief   : get board clock related information
 @param   : void
 @return  :
*****************************************************************************/
void bsp_clk_inf_get(void)
{
    /* show systerm clk source */
    sys_clk_sorce=RCC_GetSYSCLKSource ();
    RCC_GetClocksFreq(&rcc_clk_freq);
}

/*****************************************************************************
 @funcname: bsp_nvic_config
 @brief   : config nvic
 @param   : void
 @return  :
*****************************************************************************/
void bsp_nvic_config (void)
{
    NVIC_PriorityGroupConfig(SYS_NVIC_PriorityGroup);
}
/*****************************************************************************
 @funcname: sys_tick_init
 @brief   : systick init
 @param   : uint32_t sys_tick_hz: frequency of tick
 @return  :
*****************************************************************************/
void bsp_sys_tick_init(uint32_t sys_tick_hz)
{
    SysTick_Config((rcc_clk_freq.HCLK_Frequency/sys_tick_hz));
}


/*****************************************************************************
 @funcname: sys_tick_deinit
 @brief   : systick deinit
 @param   :
 @return  :
*****************************************************************************/
void bsp_sys_tick_deinit(void)
{
     SysTick->CTRL  = 0x00;
}

/*****************************************************************************
 @funcname: bsp_sys_tick_get
 @brief   : get sys time tick
 @param   : void
 @return  :
*****************************************************************************/
uint32_t  sys_tick_get(void)
{
    return sys_tick_time;
}


/*****************************************************************************
 @funcname: sys_time_delay
 @brief   : delay certain time
 @param   : uint32_t delay_time  (ms)
 @return  :
*****************************************************************************/
void sys_time_delay(uint32_t delay_time)
{
    uint32_t delay_tick_mark;
    uint32_t delay_tick;

    delay_tick = ((delay_time *SYS_TICK_PER_SECOND)/1000);
    delay_tick_mark = sys_tick_get();
    while((sys_tick_get() -delay_tick_mark) < delay_tick){;}
}
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    sys_tick_time++;
}



