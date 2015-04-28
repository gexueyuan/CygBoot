/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_bsp_timer.h
  * @brief  : cv_bsp_timer.c header file
  * @author : Fred
  * @history:
  *        2015-3-10    Fred    Created file
  *        ...
  ******************************************************************************
  */
#ifndef __CV_BSP_TIMER_H__
#define __CV_BSP_TIMER_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#ifndef NULL
#define NULL (0)
#endif





#ifdef _BSP_TIMER_MODULE_
#define  BSP_TIMER_EXTERN
#else
#define BSP_TIMER_EXTERN extern
#endif


typedef void (*tim_hook_t) (void);

BSP_TIMER_EXTERN tim_hook_t tim3_hook_func;
BSP_TIMER_EXTERN tim_hook_t tim2_hook_func;

void bsp_tim3_init(void);
void bsp_tim2_init(void);
void bsp_tim_deinit(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CV_BSP_TIMER_H__ */
