/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_sys_main.h
  * @brief  : cv_sys_main.c header file
  * @author : Fred
  * @history:
  *        2015-3-4    Fred    Created file
  *        ...
  ******************************************************************************
  */
#ifndef __CV_SYS_MAIN_H__
#define __CV_SYS_MAIN_H__


#include "cv_includes.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */



/* upgrade func return  */
typedef uint8_t func_result;

#define  FUNC_FINSH       0x01

/* upgrade step identify */
#define UPDATE_PORT_INIT      0x00
#define UPDATE_FM_TRANS       0x01
#define UPDATE_EXE_STEP        0x02

typedef func_result (*update_func_t) (void);


update_func_t updata_func[UPDATE_EXE_STEP];





#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CV_SYS_MAIN_H__ */
