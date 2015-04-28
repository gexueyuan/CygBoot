/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_includes.h
  * @brief  : sygnoides global head file
  * @author : Fred
  * @history:
  *        2015-3-11    Fred    Created file
  *        ...
  ******************************************************************************
  */

#ifndef _CV_INCLUDES_H_
#define _CV_INCLUDES_H_

#include "stm32f4xx.h"
#include "cv_sys_conf.h"
#include "cv_bsp_ckl.h"
#include <stdio.h>

#ifndef TRUE
#define TRUE  (1)
#endif

#ifndef FALSE
#define FALSE  (0)
#endif

#ifndef NULL
#define NULL (0)
#endif

/* upgrade request port check rerurn */
typedef enum {
    UPDATE_REQ_WAIT = 0,
    UPDATE_REQ_TRUE,
    UPDATE_REQ_FALSE
}update_req_check_e;


/*typedef struct {
    uint32_t head_sgn;
    uint32_t type;
    uint32_t file_base;
    uint32_t file_len;
    uint32_t load_addr;
    uint32_t exec_addr;
    uint32_t version;
    uint32_t image_md5[5];
}fm_header_t;*/

typedef struct
{
      char signature[4];
      char type[4];
      unsigned int base;
      unsigned int length;
      unsigned int load_address;
      unsigned int exec_address;
      char name[20];
      unsigned char check_md5[16];
} fm_header_t;



#endif
