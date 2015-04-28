/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_app_sd.c
  * @brief  : sd file upgrade source file
  * @author : Fred
  * @history:
  *        2015-3-11    Fred    Created file
  *        ...
  ******************************************************************************
  */

#define _CV_APP_SD_MODULE

#include <string.h>
#include "cv_app_sd.h"
#include "cv_bsp_gpio.h"
#include "cv_drives_flash.h"
#include "diskio.h"
#include "ff.h"


#define SD_PRINTF_TIME    100
#define SD_PRINTF_TICK   (SD_PRINTF_TIME*SYS_TICK_PER_SECOND/1000)
extern uint8_t file_buf[FILE_BUF_LEN];
/*****************************************************************************
 @funcname: app_sd_request_check
 @brief   : sd upgrade check
 @param   : void
 @return  :
*****************************************************************************/
update_req_check_e app_sd_request_check(void)
{
    update_req_check_e sd_check =UPDATE_REQ_FALSE;
    if(bsp_key_state() == KEY_PRESS_ON) {
        sd_check = UPDATE_REQ_TRUE;
    }
    return sd_check;
}
/*****************************************************************************
 @funcname: file_sys_init
 @brief   : file systerm init
 @param   : void
 @return  :
*****************************************************************************/
uint8_t file_sys_init(void)
{
    FATFS file_sys;
    FRESULT res;
    FIL  fdes;
    uint8_t test_buf[]="fred";
    uint8_t read_buf[10];
    uint32_t w_len;
    uint32_t r_len;

    /* mount file systerm */
    res = f_mount(&file_sys,"0:",1);
    if (res != FR_OK) {
        return FALSE;
    }

    //#if (0)
    res = f_open(&fdes,"test.txt",FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
    if (res != FR_OK) {
        return FALSE;
    }

    res = f_write(&fdes,test_buf,sizeof(test_buf),&w_len);
    if ((res != FR_OK) || (w_len == 0)) {
        return FALSE;
    }

    f_close(&fdes);

    if(res != FR_OK) {
        return FALSE;
    }

    res = f_open(&fdes, "test.txt", FA_OPEN_EXISTING | FA_READ);
    res = f_read(&fdes,read_buf,sizeof(test_buf),&r_len);
    if ((res != FR_OK) || (r_len == 0)) {
        return FALSE;
    }

    if (memcmp(read_buf,test_buf,sizeof(test_buf))) {
        return FALSE;
    }

    f_close(&fdes);

    return TRUE;

    //#endif

}

uint8_t sd_file_trans(uint8_t *file_buf,FIL *file_ptr)
{
    uint32_t pack_seq,file_read_len,file_size,remain_size,old_tick;
   // FRESULT file_sult;

    #ifdef DEBUG_MODE
     printf("****\r\n--file id:%d;--file flag:%d;--file ptr:%d,file size:%x;--file clust:%x;--file sclust:%x;--file sect:%x; ",
            file_ptr->id,file_ptr->flag,file_ptr->fptr,file_ptr->fsize,file_ptr->clust,file_ptr->sclust,file_ptr->dsect);
    #endif
    memset(file_buf,0x00,FILE_BUF_LEN);
    file_size = file_ptr->fsize;
    old_tick = sys_tick_get();
    for(pack_seq = 1,remain_size = file_ptr->fsize,file_read_len = 0;;) {

        if (f_read(file_ptr,file_buf,FILE_BUF_LEN,&file_read_len) == FR_OK) {
            if(file_read_len >0) {
                 if (0 == drives_flash_packet_write_t(file_buf,file_read_len,pack_seq) ) {
                    remain_size = remain_size - file_read_len;
                    pack_seq ++;

                    if((sys_tick_get()-old_tick) >=SD_PRINTF_TIME) {
                        printf("complete: %d/%d\r\n",(file_size-remain_size),file_size);
                        old_tick = sys_tick_get();
                    }
                    /* trans complete */
                    if(remain_size <= 0) {
                        break;
                    }
                    if(file_read_len < FILE_BUF_LEN) {
                        break;
                    }

                 }
                 else {
                    printf("write flash error!\r\n");
                    return FALSE;
                 }
            }
            else {
                printf("file read len 0 error!\r\n");
                return FALSE;
            }
        }
        else {
            printf("read file error!\r\n");
            return FALSE;
        }

    }

    return TRUE;
}
/*****************************************************************************
 @funcname: app_sd_trans_process
 @brief   : trans update file from sd file
 @param   : void
 @return  :
*****************************************************************************/
uint8_t app_sd_trans_process(void)
{
    uint8_t sd_return = FALSE;
    FRESULT res;
    FIL file_des;
    uint8_t fine_file = 0;

    sd_return = disk_card_check();
    if(sd_return ==  FALSE) {
        printf("sd card not insert!\r\n");
        return FALSE;
    }

    sd_return = disk_initialize(0);
    if(sd_return == 0) {
        printf("sd  init OK!\r\n");
    }
    else {
        printf("sd  init error!\r\n");
        return FALSE;
    }

    sd_return = file_sys_init();
    if (sd_return == TRUE) {
        printf("file systerm init OK!\r\n");
    }
    else {
        printf("file systerm init error!\r\n");
        return FALSE;
    }


     /* find  app file*/
    do {
        res = f_open(&file_des,"app.image",FA_READ|FA_OPEN_EXISTING);
        if(res == FR_OK) {
            fine_file++;
            printf("---find app.image\r\n");
            if (sd_file_trans(file_buf,&file_des) != TRUE) {
                printf("updata app.image error!\r\n");
                f_close(&file_des);
                return FALSE;
            }
        }
        else if (res ==FR_NO_FILE ) {
            printf("can not find app file!\r\n");
            break;
        }
        else {
             printf("open app file error!\r\n");
             return FALSE;
        }
        printf("--updata app.image success!\r\n");
        f_close(&file_des);
    }
    while(0);


    /* find  source file*/
    do {
       res = f_open(&file_des,"para.image",FA_READ|FA_OPEN_EXISTING);
       if(res == FR_OK) {
            fine_file++;
            printf("---find para.image\r\n");
            if (sd_file_trans(file_buf,&file_des) != TRUE) {
                printf("updata app.image error!\r\n");
                f_close(&file_des);
                return FALSE;
            }
       }
       else if (res ==FR_NO_FILE ) {
            printf("can not find parameter file!\r\n");

            break;
       }
       else {
            printf("open parameter file error!\r\n");
            return FALSE;
       }
       printf("--updata para.image success!\r\n");
       f_close(&file_des);
    }
    while(0);

    if(fine_file <= 0) {
        printf("find 0 vaild file\r\n");
        return FALSE;
    }

    return TRUE;
}

