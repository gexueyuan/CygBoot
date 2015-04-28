/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_bsp_ckl.h
  * @brief  : cv_bsp_ckl.c header file
  * @author : Fred
  * @history:
  *        2015-3-10    Fred    Created file
  *        ...
  ******************************************************************************
  */
#ifndef __CV_BSP_CKL_H__
#define __CV_BSP_CKL_H__

#include "cv_includes.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define SYS_TICK_PER_SECOND   100

/* Internal high-speed clock, After a system reset, the HSI oscillator is selected as the system clock*/
#define CLK_HSI    0x00
/* External high-speed clock */
#define CLK_HSE    0x04
/* Phase-lock Loop clock */
#define CLK_PLL    0x08





#ifdef   _BSP_CLK_MODULE_
#define BSP_CLK_EXTERN
#else
#define BSP_CLK_EXTERN   extern
#endif




BSP_CLK_EXTERN RCC_ClocksTypeDef rcc_clk_freq;
BSP_CLK_EXTERN  uint8_t sys_clk_sorce;
BSP_CLK_EXTERN uint32_t sys_tick_time;



void bsp_clk_inf_get(void);
void bsp_sys_tick_init(uint32_t sys_tick_hz);
void bsp_sys_tick_deinit(void);
void bsp_clk_inf_show(void);
void bsp_nvic_config (void);
uint32_t  sys_tick_get(void);
void sys_time_delay(uint32_t delay_time);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CV_BSP_CKL_H__ */
