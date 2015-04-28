/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_bsp_gpio.c
  * @brief  : bsp goip source file
  * @author : Fred
  * @history:
  *        2015-3-9    Fred    Created file
  *        ...
  ******************************************************************************
  */
#define CV_BSP_GPIO_MODULE_

#include "cv_bsp_gpio.h"


uint8_t led_conut;

/*****************************************************************************
 @funcname: bsp_key_init
 @brief   : key io init
 @param   : void
 @return  :
*****************************************************************************/
void bsp_key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(KEY_CLK_SRC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = KEY_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}

uint8_t bsp_key_state(void)
{
    return GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN);
}

/*****************************************************************************
 @funcname: bsp_led_init
 @brief   : led init
 @param   : void
 @return  :
*****************************************************************************/
void bsp_led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_WriteBit(LED_PORT,LED_PIN,Bit_RESET);
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
}

void led_blink(led_blink_mode_e blink_mode)
{
    static uint8_t led_count;
    uint8_t led_period;
    uint8_t led_state;

    led_count++;
    led_period = (led_count % LED_BLINK_PERIOD);
    switch(blink_mode) {
        case LED_BLINK_QUICK : {
            if((led_count % 2) == 0) {
                led_state = FALSE;
            }
            else {
                led_state = TRUE;
            }
        }
        break;

        case LED_BLINK_SLOW : {

            if(led_period < (LED_BLINK_PERIOD/2)) {
                led_state = TRUE;
            }
            else {
                led_state = FALSE;
            }
        }
        break;

        case LED_BLINK_ONE : {
            if(led_period == LED_BLINK_FIRST) {
                led_state = TRUE;
            }
            else  {
                led_state = FALSE;
            }
        }
        break;

        case LED_BLINK_TWO :{
            if((led_period == LED_BLINK_FIRST)||(led_period == LED_BLINK_SECOND)) {
                led_state = TRUE;
            }
            else {
                led_state = FALSE;
            }
        }
        break;

        case LED_BLINK_THREE : {
            if((led_period == LED_BLINK_FIRST)||(led_period == LED_BLINK_SECOND)
               ||(led_period == LED_BLINK_THIRD)) {
                led_state = TRUE;
            }
            else {
                led_state = FALSE;
            }
        }
        break;

        case LED_BLINK_FOUR : {
            if((led_period == LED_BLINK_FIRST)||(led_period == LED_BLINK_SECOND)
                ||(led_period == LED_BLINK_THIRD) ||(led_period == LED_BLINK_FOURTH)) {
                led_state = TRUE;
            }
            else {
                led_state = FALSE;
            }
        }
        break;

        default:
        break;

    }

    if(led_state == TRUE) {
        bsp_led_on();
    }
    else {
        bsp_led_off();
    }
}

