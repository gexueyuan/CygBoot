/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_sys_main.c
  * @brief  : this is sys main entry file
  * @author : Fred
  * @history:
  *        2015-3-4    Fred    Created file
  *        ...
  ******************************************************************************
  */


#define _CV_SYS_MAIN_MODULE_

#include <stdio.h>
#include "cv_sys_main.h"
#include "cv_bsp_usart.h"
#include "cv_bsp_gpio.h"
#include "cv_bsp_timer.h"
#include "cv_app_upgrade.h"
#include "cv_app_iap.h"
#include "cv_drives_sd_spi.h"
#include "diskio.h"



/*****************************************************************************
 @funcname: bsp_clk_inf_show
 @brief   : show sys clk related inf
 @param   : void
 @return  :
*****************************************************************************/
static void clk_inf_show(void)
{
    if (sys_clk_sorce == CLK_HSI) {
        printf("\r\nCLK: %d [HSI]\r\n",rcc_clk_freq.SYSCLK_Frequency);
    }
    else if (sys_clk_sorce == CLK_HSE) {
        printf("\r\nCLK: %d [HSE]\r\n",rcc_clk_freq.SYSCLK_Frequency);
    }
    else  if (sys_clk_sorce == CLK_PLL){
         printf("\r\nCLK: %d [PLL]\r\n",rcc_clk_freq.SYSCLK_Frequency);
    }

    /* show systerm clk freq*/
    //printf("\r\nS: %d\r\n",rcc_clk_freq.SYSCLK_Frequency);
    //printf("HCLK: %d\r\n",rcc_clk_freq.HCLK_Frequency);
    //printf("PCLK1(APB1): %d\r\n",rcc_clk_freq.PCLK1_Frequency);
    //printf("PCLK2(APB2): %d\r\n",rcc_clk_freq.PCLK2_Frequency);
    /*The system frequency computed by this function is not the real
    frequency in the chip. It is calculated based on the predefined
    constant and the selected clock source*/
}

/*****************************************************************************
 @funcname: app_inf_show
 @brief   : sys start show
 @param   : void
 @return  :
*****************************************************************************/
static void app_start_show(void)
{
    printf("\r\n\r\nCygnoides boot start...\r\nVER: V0.3 [%s,%s]",__TIME__,__DATE__);
    clk_inf_show();
}
/*****************************************************************************
 @funcname: board_init
 @brief   : board init
 @param   : void
 @return  :
*****************************************************************************/
static void board_init(void)
{
    bsp_clk_inf_get();
    bsp_nvic_config();
    bsp_uart2_init();
    bsp_uart1_init();
    bsp_key_init();
    bsp_led_init();
    bsp_tim3_init();
    bsp_tim2_init();
    bsp_sys_tick_init(SYS_TICK_PER_SECOND);
    disk_low_level_init();
}
static void app_sys_init(void)
{
    update_process_fsm = UPDATE_REQ;
    update_led_set(LED_DEFAULT);
    tim3_hook_func =upgrade_led_display;
}
/*****************************************************************************
 @funcname: main
 @brief   : main entry
 @param   : void
 @return  :
*****************************************************************************/
int main(void)
{
    board_init();
    app_start_show();
    app_sys_init();
    while (1) {
        app_upgrade_process();
    }
}

