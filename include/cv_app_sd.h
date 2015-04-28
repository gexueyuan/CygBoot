/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_app_sd.h
  * @brief  : cv_app_sd.c header file
  * @author : Fred
  * @history:
  *        2015-3-11    Fred    Created file
  *        ...
  ******************************************************************************
  */
#ifndef __CV_APP_SD_H__
#define __CV_APP_SD_H__


#include "cv_includes.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

update_req_check_e app_sd_request_check(void);
uint8_t app_sd_trans_process(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CV_APP_SD_H__ */
