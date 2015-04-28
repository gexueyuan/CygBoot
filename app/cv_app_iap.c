/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_app_iap.c
  * @brief  : uart iap upgrade related source file
  * @author : Fred
  * @history:
  *        2015-3-11    Fred    Created file
  *        ...
  ******************************************************************************
  */

#define _CV_APP_IAP_MODULE_

#include "cv_app_iap.h"
#include "cv_bsp_usart.h"
#include "ymodem.h "
#include "flash_if.h"


uint8_t file_buf[FILE_BUF_LEN] = {0};
uint8_t file_name[FILE_NAME_LENGTH];
/*****************************************************************************
 @funcname: app_iap_request_check
 @brief   : check uart iap upgrade request
 @param   : void
 @return  :
*****************************************************************************/
update_req_check_e app_iap_request_check(void)
{
    static uint8_t key_press_count = 0;
    update_req_check_e iap_req_check = UPDATE_REQ_FALSE;

    if(uart2_buf_check() == TRUE) {
        if(uart2_read_data() == IAP_LOAD_KEY) {
            key_press_count ++;
        }
    }

    if(key_press_count >= IAP_LOAD_KEY_TIME) {
        iap_req_check =UPDATE_REQ_TRUE;
        key_press_count =0;
    }
    return iap_req_check;
}



/*****************************************************************************
 @funcname: app_iap_trans_process
 @brief   : trans file through iap process
 @param   : void
 @return  :
*****************************************************************************/
uint8_t app_iap_trans_process(void)
{

    int32_t size =0;
    char * string;
    uint8_t Number[10] ={0};
    printf("please send file through ymodem protocol,wait...\n\r");
    size = Ymodem_Receive(file_buf);
    if (size > 0)
    {
        string =(char *) &file_name[0];
        printf("\r\nprogramming successfully!");
        printf("\r\nfilename:%s",string);
        Int2Str(Number, size);
        printf("\r\nfilesize: %s byte\r\n",Number);
        return TRUE;
    }
    else if (size == -1)
    {
        printf("\n\n\rThe image size is higher than the allowed space memory!\n\r");
    }
    else if (size == -2)
    {
        printf("\n\n\rVerification failed!\n\r");
    }
    else if (size == -3)
    {
        printf("\r\n\nAborted by user.\n\r");
    }
    else
    {
        printf("\n\rFailed to receive the file!\n\r");
    }
    return FALSE;

}


