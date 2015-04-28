/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_app_upgrade.c
  * @brief  : app upgrade related source file
  * @author : Fred
  * @history:
  *        2015-3-10    Fred    Created file
  *        ...
  ******************************************************************************
  */


#define _APP_UPGRADE_MODULE_


#include "cv_app_upgrade.h"
#include <stdio.h>
#include "cv_bsp_gpio.h"
#include "cv_app_sd.h"
#include "cv_app_iap.h"
#include "cv_drives_flash.h"


//(ms)
#define INTERFACE_CLASS__CHECK_TIME      (200)
#define INTERFACE_CLASS__CHECK_TICK      ((INTERFACE_CLASS__CHECK_TIME*SYS_TICK_PER_SECOND)/1000)

upgrade_trans_hook  upgrade_trans_func = NULL;



/*****************************************************************************
 @funcname: upgrade_led_display
 @brief   : disaplay boot state
 @param   : void
 @return  :
*****************************************************************************/
void upgrade_led_display(void)
{
    led_blink_mode_e led_blink_mode;

    switch(updare_led_state) {

        case LED_DEFAULT: {
            led_blink_mode = LED_BLINK_SLOW;
        }
        break;

        case LED_UPDATE_ING: {
            led_blink_mode = LED_BLINK_TWO;
        }
        break;

        case LED_UPDATE_FAIL: {
            led_blink_mode = LED_BLINK_QUICK;
        }
        break;

        case LED_JUMP_FILE : {
            led_blink_mode = LED_BLINK_FOUR;
        }
        break;

        default: {
             led_blink_mode = LED_BLINK_SLOW;
        }
        break;
    }
    led_blink(led_blink_mode);
}
/*****************************************************************************
 @funcname: upgrade_req_check
 @brief   : check upgrade request
 @param   : void
 @return  :
*****************************************************************************/
update_way_e upgrade_req_check(void)
{
    uint32_t sys_tick_mark;
    update_way_e update_port = UPDATE_NULL;

    /* notice print */
    printf("--(tips) upgrade method:\r\nsd card:press button on device\r\nuart: press \"l\"(load) on keyboard\r\n\r\n");

    /* key press trigger */
    if (app_sd_request_check() == UPDATE_REQ_TRUE) {
        update_port =UPDATE_FROM_SD;
        return update_port;
    }

    /* interface class check */
    sys_tick_mark = sys_tick_get();
    while((sys_tick_get()-sys_tick_mark) < INTERFACE_CLASS__CHECK_TICK) {

        /* uart iap load requset  */
        if(app_iap_request_check() == UPDATE_REQ_TRUE) {
            update_port =UPDATE_FROM_UART;
            return update_port;
        }

        /* add other laod way check here */
    }
    return update_port;
}
/*****************************************************************************
 @funcname: app_upgrade_process
 @brief   : upgrade operation
 @param   : void
 @return  :
*****************************************************************************/
void app_upgrade_process(void)
{
    update_way_e update_port;
    uint8_t trans_result;

    switch (update_process_fsm) {

        case UPDATE_REQ : {
            update_port = upgrade_req_check();
            /* for  test */
            //update_port = UPDATE_FROM_SD;

            if( update_port == UPDATE_FROM_SD) {
                printf("sd update request trigger!\r\n");
                upgrade_trans_func = app_sd_trans_process;
                update_process_fsm =UPDATE_TRANS;
            }
            else if(update_port == UPDATE_FROM_UART) {
                printf("uart update request trigger!\r\n");
                upgrade_trans_func = app_iap_trans_process;
                update_process_fsm =UPDATE_TRANS;
            }
            else if(update_port == UPDATE_NULL) {
               printf("no need update\r\n");
               upgrade_trans_func = NULL;
               update_process_fsm =UPDATE_FINSH;
            }

            if(update_port != UPDATE_NULL) {
                update_led_set(LED_UPDATE_ING);
            }
        }
        break;

        case UPDATE_TRANS : {
            if(upgrade_trans_func != NULL) {
                trans_result=(*upgrade_trans_func)();

                if(trans_result == TRUE) {
                    /* trans file success */
                    update_process_fsm =UPDATE_FINSH;
                } else  {
                    /* trans file fail  */
                    update_process_fsm =UPDATE_FAIL;
                }
            } else {
                printf("error:trans func is null\r\n");
            }
        }
        break;

        #if (0)
        case UPDATE_CHECK : {
            printf("trans success,start md5 check\r\n");
            if (TRUE == md5_check() ) {
                printf("md5 check OK\r\n");
                update_process_fsm =UPDATE_FINSH;
            }
            else {
                 printf("md5 check error\r\n");
                update_process_fsm =UPDATE_FAIL;
            }
        }
        break;
        #endif

        case UPDATE_FINSH : {
            printf("update success or no need update..\r\n");
            application_jump();
            printf("jump to app fail..\r\n");
            update_led_set(LED_JUMP_FILE);
            while(1) {
                /* error  */
                //sys_soft_reset();
            }
        }
        //break;

        case UPDATE_FAIL : {
            printf("update fail..\r\n");
            update_led_set(LED_UPDATE_FAIL);
            while(1);
        }
        //break;
   }
}



