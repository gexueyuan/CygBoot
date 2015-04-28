/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_app_upgrade.h
  * @brief  : cv_app_upgrade.c header file
  * @author : Fred
  * @history:
  *        2015-3-10    Fred    Created file
  *        ...
  ******************************************************************************
  */
#ifndef __CV_APP_UPGRADE_H__
#define __CV_APP_UPGRADE_H__


#include "cv_includes.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */



/* update process fsm def */
typedef enum {
    UPDATE_REQ = 0,
    UPDATE_TRANS,
    UPDATE_CHECK,
    UPDATE_FINSH,
    UPDATE_FAIL
}update_process_e;



/* upgrade approach def*/
typedef enum {
    UPDATE_NULL = 0,
    UPDATE_FROM_UART,
    UPDATE_FROM_SD
}update_way_e;


typedef enum {
    LED_DEFAULT =1,
    LED_UPDATE_ING,
    LED_UPDATE_FAIL,
    LED_JUMP_FILE
}led_state_e;

typedef uint8_t (*upgrade_trans_hook) (void);

#ifdef  _APP_UPGRADE_MODULE_
#define UPGRADE_MODULE_EXTERN
#else
#define UPGRADE_MODULE_EXTERN extern
#endif


UPGRADE_MODULE_EXTERN update_process_e update_process_fsm;

UPGRADE_MODULE_EXTERN led_state_e updare_led_state;

#define update_led_set(u)         (updare_led_state = u)


void app_upgrade_process(void);
update_way_e upgrade_request_check(void);
void upgrade_led_display(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CV_APP_UPGRADE_H__ */
