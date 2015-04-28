/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_sys_conf.h
  * @brief  : sys configure head file
  * @author : Fred
  * @history:
  *        2015-3-10    Fred    Created file
  *        ...
  ******************************************************************************
  */

#ifndef  CV_SYS_CONF_H_
#define CV_SYS_CONF_H_


//#define  TEST_BOARD
//#define DEBUG_MODE

/*NVIC config*/
#define SYS_NVIC_PriorityGroup    NVIC_PriorityGroup_1

#define NVIC_UART2_PRE_PRI        (0x01)      /*0 -1*/
#define NVIC_UART2_SUB_PRI        (0x01)       /*0 -7 */

#define  NVIC_TIM3_IRQ_PRE_PRI       (0x01)
#define  NVIC_TIM3_IRQ_SUB_PRI       (0x05)

#define  NVIC_TIM2_IRQ_PRE_PRI       (0x01)
#define  NVIC_TIM2_IRQ_SUB_PRI       (0x03)


/* timer freq (hz) */
#define TIM3_FREQ         8

#define TIM2_FREQ         1


#define UART_REV_BUF_MAX      20

/* IAP load request key "l" 0x6c  108 */
#define IAP_LOAD_KEY         (0x6c)
#define IAP_LOAD_KEY_TIME         (0x2)


/* flash related config */
#define FLASH_FM_HEAD_LOAD_ADDR       (0x08020000)
#define FLASH_FM_DEFAULT_LOAD_ADDR     (0x08020200)

#define FLASH_FM_DEFAULT_SIZE              (1024*128*3)

#define APP_FILE_NAME       ("app.bin")
#define FILE_BUF_LEN          (1024)



#endif
