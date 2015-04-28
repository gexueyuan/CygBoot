/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_drives_flash.h
  * @brief  : cv_drives_flash.c header file
  * @author : Fred
  * @history:
  *        2015-3-12    Fred    Created file
  *        ...
  ******************************************************************************
  */
#ifndef __CV_DRIVES_FLASH_H__
#define __CV_DRIVES_FLASH_H__

#include "cv_includes.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define STM32_PLATFORM_FM_SGN    ("CV1")
#define NL6621_PLATFORM_FM_SGN    ("CV2")

#define LOAD_FILE_FW                     ("FW")
#define LOAD_FILE_RS                      ("RS")
#define LOAD_FILE_PA                      ("PA")

#define FLASH_WRITE_FM                1
#define FLASH_WRITE_SOURCE       2




typedef enum {
    FM_MATCH,
    FM_NO_MATCH,
    FM_BOTH_OK,
    FM_NULL
}fm_header_check_e;


typedef struct {
    uint32_t write_tyte;
    uint32_t write_ptr;
    uint32_t *write_data;
    uint32_t file_len;
    uint32_t write_len;
    uint32_t erase_size;
}update_flash_manage_t;


uint8_t drives_flash_packet_write(uint8_t *data_ptr,uint8_t data_len,uint32_t pack_seq);
uint8_t drives_flash_packet_write_t(uint8_t *data_ptr,uint32_t data_len,uint32_t pack_seq);
uint8_t drives_flash_packet_write_tt(uint8_t *data_ptr,uint32_t data_len,uint32_t pack_seq);
void application_jump(void);
void sys_soft_reset(void);
uint8_t md5_check(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CV_DRIVES_FLASH_H__ */
