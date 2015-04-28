/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_drives_md5.h
  * @brief  : cv_drives_md5.c header file
  * @author : Fred
  * @history:
  *        2015-4-2    Fred    Created file
  *        ...
  ******************************************************************************
  */
#ifndef __CV_DRIVES_MD5_H__
#define __CV_DRIVES_MD5_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef char            int8_t;
typedef unsigned char   uint8_t;
typedef short           int16_t;
typedef unsigned short  uint16_t;
typedef int             int32_t;
typedef unsigned int    uint32_t;


typedef char            int8;
typedef unsigned char   uint8;
typedef short           int16;
typedef unsigned short  uint16;
typedef int             int32;
typedef unsigned int    uint32;


struct _md5_ctx
{
    uint32_t state[4]; /* state (ABCD) */
    uint32_t count[2]; /* number of bits, modulo 2^64 (lsb first) */
    uint8_t  buffer[64]; /* input buffer */
};
typedef struct _md5_ctx md5_ctx;









void wb_md5_init(md5_ctx* context);
void wb_md5_update(md5_ctx* context, uint8_t* input, uint32_t inputLen);
void wb_md5_final(uint8_t digest[16], md5_ctx* context);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CV_DRIVES_MD5_H__ */
