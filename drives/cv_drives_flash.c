/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_drives_flash.c
  * @brief  : update process flash operation
  * @author : Fred
  * @history:
  *        2015-3-12    Fred    Created file
  *        ...
  ******************************************************************************
  */
#define _CV_DRIVES_FLASH_MODULE_

#include <string.h>
#include "cv_drives_md5.h"
#include "cv_drives_flash.h"
#include "flash_if.h"
#include "cv_bsp_usart.h"
#include "cv_bsp_timer.h"



md5_ctx context;
uint8_t md5_cal[16];
uint32_t JumpAddress;
typedef  void (*app_function)(void);
app_function Jump_To_Application;
fm_header_t fm_header;

update_flash_manage_t  update_flash_manage;

fm_header_check_e  firmware_head_analysis(uint8_t *data_ptr)
{
    /* analyze fm heard */
    memcpy(&fm_header,data_ptr,sizeof(fm_header_t));
    if(strcmp(fm_header.signature,STM32_PLATFORM_FM_SGN) == 0) {
        /* stm32 platform */
        //printf("fm head match \r\n");
        return FM_MATCH;
    }
    else if( strcmp (fm_header.signature,NL6621_PLATFORM_FM_SGN) == 0 ) {

        /* nl6621 platform*/
        if((strcmp(fm_header.type,LOAD_FILE_RS) ==0) ||(strcmp(fm_header.type,LOAD_FILE_PA) == 0)) {
            //printf("fm head not match,but is source\r\n");
            return FM_BOTH_OK;
        }
        //printf("fm head not match\r\n");
        return FM_NO_MATCH;
    }
    else {
        /* fm head in null */
        memset((void *)&fm_header.signature,0x00,sizeof(fm_header_t));
        //printf("fm head null \r\n");
        return FM_NULL;
    }
}


/*****************************************************************************
 @funcname: erase_occupy_flash
 @brief   : erase need occupy flash
 @param   : void
 @return  :
*****************************************************************************/
uint8_t erase_occupy_flash(void)
{
    uint32_t fm_head_flash;
    /*Unlocks Flash for write access*/
    FLASH_If_Init();

    if(update_flash_manage.write_tyte == FLASH_WRITE_FM) {

        FLASH_If_Erase(FLASH_FM_HEAD_LOAD_ADDR,update_flash_manage.erase_size);

        fm_head_flash = FLASH_FM_HEAD_LOAD_ADDR;
        FLASH_If_Write(&fm_head_flash,(uint32_t *)&fm_header,sizeof(fm_header_t));
    }
    else if (update_flash_manage.write_tyte == FLASH_WRITE_SOURCE) {

        FLASH_If_Erase(update_flash_manage.write_ptr,update_flash_manage.erase_size);
    }
    else  {
        FLASH_If_Erase(FLASH_FM_DEFAULT_LOAD_ADDR,FLASH_FM_DEFAULT_SIZE);
    }

    return TRUE;
}


uint8_t md5_check(void)
{
    #ifdef DEBUG_MODE
    uint8_t cir_count;
    #endif

    wb_md5_final(md5_cal, &context);

    #ifdef DEBUG_MODE
    printf("--cal md5 check:");
    for(cir_count = 0; cir_count<16;cir_count++)
    {
        printf("%x ",md5_cal[cir_count]);
    }

    printf("\r\n--read md5 check:");
    for(cir_count = 0; cir_count<16;cir_count++)
    {
        printf("%x ",fm_header.check_md5[cir_count]);
    }
    #endif


    if(0 == memcmp(md5_cal,fm_header.check_md5,sizeof(md5_cal))) {
        return TRUE;
    }
    else  {
        return FALSE;
    }
}
/*****************************************************************************
 @funcname: drives_flash_packet_write
 @brief   : write packet on flash
 @param   : void
 @return  :
*****************************************************************************/
uint8_t drives_flash_packet_write_t(uint8_t *data_ptr,uint32_t data_len,uint32_t pack_seq)
{
     fm_header_check_e  fm_header_check_res;
     uint8_t *md5_data;
     uint32_t md5_len;
#ifdef DEBUG_MODE
    uint8_t cir_count;
#endif


    /* first package */
    if (pack_seq == 0x01) {

        /* clear flash manage struct */
        memset(&update_flash_manage,0x00,sizeof(update_flash_manage_t));
        fm_header_check_res = firmware_head_analysis(data_ptr);

        #ifdef DEBUG_MODE

        printf("****fm head info:****\r\n--fm sgn:%s\r\n--fm type:%s\r\n--fm base:%x\r\n--fm len:%d \r\n--fm load:%x\r\n--fm exec:%x\r\n",
        fm_header.signature,fm_header.type,fm_header.base,fm_header.length,fm_header.load_address,fm_header.exec_address);
        printf("read md5:");
        for(cir_count = 0 ;cir_count<16;cir_count++) {
            printf("%x",fm_header.check_md5[cir_count]);
        }
        printf("\r\n");
        #endif

        if( (fm_header_check_res ==FM_MATCH) ||(fm_header_check_res == FM_BOTH_OK) ) {

            update_flash_manage.write_ptr = fm_header.load_address;

            if( strcmp (fm_header.type,LOAD_FILE_FW) == 0) {
                update_flash_manage.write_tyte = FLASH_WRITE_FM;
                update_flash_manage.erase_size  = fm_header.length + fm_header.base;
            }
            else {
                update_flash_manage.write_tyte = FLASH_WRITE_SOURCE;
                update_flash_manage.erase_size = fm_header.length;
            }
            update_flash_manage.write_data =  (uint32_t *)(data_ptr+fm_header.base);
            update_flash_manage.write_len = (data_len-fm_header.base);
            update_flash_manage.file_len = fm_header.length;
        }

        /* no file head handle */
        else if(fm_header_check_res ==FM_NULL) {
            #if (0)
            /* default load addr */
            update_flash_manage.write_ptr = FLASH_FM_DEFAULT_LOAD_ADDR;
            update_flash_manage.erase_size = FLASH_FM_DEFAULT_SIZE;
            update_flash_manage.write_data =  (uint32_t *)data_ptr;
            update_flash_manage.write_len = data_len;
            #endif
            return 1;
        }

        else {
             /* file no match platfrom */
            return 1;
        }


        //printf("\r\n****flash manage info****\r\n--data type:%d\r\n--write ptr:%x\r\n--write data:%x\r\n--data len:%x\r\n--erase size:%d\r\n",update_flash_manage.write_tyte,
        //update_flash_manage.write_ptr,*update_flash_manage.write_data,update_flash_manage.write_len,update_flash_manage.erase_size);
        /* erase flash */
        wb_md5_init(&context);
        erase_occupy_flash();
    }
    else {
        update_flash_manage.write_data =  (uint32_t *)data_ptr;
        update_flash_manage.write_len = data_len;
    }
    /* erase user application area */


    /* write data to flash */
    #ifdef DEBUG_MODE
    //printf("--write flash ptr:%x\r\n--write len:%d\r\n--write data:%x",update_flash_manage.write_ptr,update_flash_manage.write_len,update_flash_manage.write_data);
    #endif

    /* ymodel file 1a adjust */
    if (update_flash_manage.file_len >= update_flash_manage.write_len) {
        update_flash_manage.file_len -=  update_flash_manage.write_len;
    }
    else {
        update_flash_manage.write_len = update_flash_manage.file_len;
        update_flash_manage.file_len = 0;
    }

     md5_data = (uint8_t *)update_flash_manage.write_data;
     md5_len = update_flash_manage.write_len;
     wb_md5_update(&context,(unsigned char *)md5_data, md5_len);
     if (FLASH_If_Write((&update_flash_manage.write_ptr),\
         (uint32_t *)update_flash_manage.write_data,(update_flash_manage.write_len/4)) == 0 ) {

        if(update_flash_manage.file_len <= 0) {
            if(FALSE== md5_check()) {
                return 1;
            }
            else {
                printf("MD5 check OK\r\n");
            }
        }
        return 0;
     }
     else {
        return 1;
     }
}


/*****************************************************************************
 @funcname: board_deinit
 @brief   : deinit board peripherals  interrupt
 @param   : void
 @return  :
*****************************************************************************/
void board_deinit(void)
{
    bsp_uart1_deinit();
    bsp_uart2_deinit();
    bsp_tim_deinit();
    bsp_sys_tick_deinit();
}



void jump_to_address(uint32_t run_address)
{
     /* jump to address */
        if (((*(__IO uint32_t*)run_address) & 0x2FFE0000 ) == 0x20000000) {

            /* close interrupt */
            board_deinit();
            /* jump to application */
            JumpAddress = *(__IO uint32_t*) (run_address+ 4);
            Jump_To_Application = (app_function) JumpAddress;
            /* Initialize user application's Stack Pointer */
            __set_MSP(*(__IO uint32_t*)run_address);
           // __set_FAULTMASK(1);
           /* CONTROL register:Stack Point + Access level in Thread mode +floating point unit or not*/
           __set_CONTROL(0x00);
            Jump_To_Application();
        }
}
/*****************************************************************************
 @funcname: application_jump
 @brief   : boot to application to run
 @param   : void
 @return  :
*****************************************************************************/
void application_jump(void)
{
    fm_header_t fm_head_jump;

    memcpy(&fm_head_jump,(void *)FLASH_FM_HEAD_LOAD_ADDR,sizeof(fm_header_t));
    if( strcmp(fm_head_jump.signature,STM32_PLATFORM_FM_SGN) == 0) {
        /* check fiemware */
        printf("fm head check ok,app address:%x\r\n",fm_head_jump.load_address);

        /* jump to address */
        jump_to_address(fm_head_jump.load_address);
    }
    else {
        printf("fm head check error,app address:%x\r\n",FLASH_FM_DEFAULT_LOAD_ADDR);
        /* jump to address */
        jump_to_address(FLASH_FM_DEFAULT_LOAD_ADDR);
    }
}


void sys_soft_reset(void)
{
        printf("error!reset systerm!\r\n");
	__set_FAULTMASK(1);
	NVIC_SystemReset();
}






