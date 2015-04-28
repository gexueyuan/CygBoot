/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_bsp_gpio.h
  * @brief  : cv_bsp_gpio.c header file
  * @author : Fred
  * @history:
  *        2015-3-9    Fred    Created file
  *        ...
  ******************************************************************************
  */
#ifndef __CV_BSP_GPIO_H__
#define __CV_BSP_GPIO_H__


#include "cv_includes.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#ifdef CV_BSP_GPIO_MODULE_
#define BSP_GPIO_EXTERN
#else
#define BSP_GPIO_EXTERN extern
#endif


#ifdef TEST_BOARD
/* PC9->key */
#define   KEY_CLK_SRC   RCC_AHB1Periph_GPIOC
#define   KEY_PORT    GPIOC
#define   KEY_PIN       GPIO_Pin_9

/* PC13->led */
#define LED_CLK_SRC   RCC_AHB1Periph_GPIOC
#define LED_PORT   GPIOC
#define LED_PIN      GPIO_Pin_13

#define bsp_led_on()    (GPIO_ResetBits(LED_PORT,LED_PIN))
#define bsp_led_off()   (GPIO_SetBits(LED_PORT,LED_PIN))


#else
/* PC7->key */
#define   KEY_CLK_SRC   RCC_AHB1Periph_GPIOC
#define   KEY_PORT    GPIOC
#define   KEY_PIN       GPIO_Pin_7

/* PC9->led */
#define LED_CLK_SRC   RCC_AHB1Periph_GPIOC
#define LED_PORT   GPIOC
#define LED_PIN      GPIO_Pin_9

#define bsp_led_on()    (GPIO_SetBits(LED_PORT,LED_PIN))
#define bsp_led_off()   (GPIO_ResetBits(LED_PORT,LED_PIN))
#endif

#define KEY_PRESS_ON      (0)
#define KEY_PRESS_OFF     (1)

void bsp_key_init(void);
uint8_t bsp_key_state(void);
void led_slow_blink(void);



void bsp_led_init(void);


#define LED_BLINK_PERIOD     10
#define LED_BLINK_FIRST         0
#define LED_BLINK_SECOND     2
#define LED_BLINK_THIRD         4
#define LED_BLINK_FOURTH     6


typedef enum {

    LED_BLINK_QUICK = 1,
    LED_BLINK_SLOW,
    LED_BLINK_ONE,
    LED_BLINK_TWO,
    LED_BLINK_THREE,
    LED_BLINK_FOUR
}led_blink_mode_e;

void led_blink(led_blink_mode_e blink_mode);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CV_BSP_GPIO_H__ */
