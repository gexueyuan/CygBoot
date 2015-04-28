/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_bsp_usart.h
  * @brief  : bsp usart head file
  * @author : Fred
  * @history:
  *        2015-3-4    Fred    Created file
  *        ...
  ******************************************************************************
  */

#ifndef _BSP_USART_H_
#define _BSP_USART_H_

#include "cv_includes.h"

/*definition for uart pin*/

/*
    USART1 config:
    PA9  -> USART1_TX
    PA10 -> USART1_RX
 */
#define UART1_TX_GPIO_CLK RCC_AHB1Periph_GPIOA
#define UART1_RX_GPIO_CLK RCC_AHB1Periph_GPIOA

#define UART1_CLK RCC_APB2Periph_USART1

#define UART1_TX_GPIO_PORT GPIOA
#define UART1_RX_GPIO_PORT GPIOA

#define UART1_TX_PIN   GPIO_Pin_9
#define UART1_RX_PIN   GPIO_Pin_10

#define UART1_TX_SOURCE   GPIO_PinSource9
#define UART1_RX_SOURCE   GPIO_PinSource10

#define UART1_TX_AF             GPIO_AF_USART1
#define UART1_RX_AF             GPIO_AF_USART1


/*
    USART2 config:
    PA2  -> USART2_TX
    PA3 -> USART2_RX
 */
#define UART2_TX_GPIO_CLK RCC_AHB1Periph_GPIOA
#define UART2_RX_GPIO_CLK RCC_AHB1Periph_GPIOA

#define UART2_CLK RCC_APB1Periph_USART2

#define UART2_TX_GPIO_PORT GPIOA
#define UART2_RX_GPIO_PORT GPIOA

#define UART2_TX_PIN   GPIO_Pin_2
#define UART2_RX_PIN   GPIO_Pin_3

#define UART2_TX_SOURCE   GPIO_PinSource2
#define UART2_RX_SOURCE   GPIO_PinSource3



#define UART2_TX_AF                  GPIO_AF_USART2
#define UART2_RX_AF                  GPIO_AF_USART2



#ifdef _BSP_USART_MODULE_
#define BSP_USART_EXTERN
#else
#define BSP_USART_EXTERN extern
#endif


typedef struct {
    uint8_t buf[UART_REV_BUF_MAX];
    uint8_t head;
    uint8_t tail;
}uart_rev_buf_t;



BSP_USART_EXTERN uart_rev_buf_t  uart2_rev_buf;


void bsp_uart1_init(void);
void bsp_uart2_init(void);
void bsp_uart2_deinit(void);
void bsp_uart1_deinit(void);
uint8_t uart2_read_data(void);
uint8_t uart2_buf_check(void);

uint32_t uart2_buf_read(uint8_t *data);
void uart2_send_char(uint8_t data);
uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum);


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Constants used by Serial Command Line Mode */
#define CMD_STRING_SIZE       128

/* Common routines */
#define IS_AF(c)  ((c >= 'A') && (c <= 'F'))
#define IS_af(c)  ((c >= 'a') && (c <= 'f'))
#define IS_09(c)  ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)  IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)  IS_09(c)
#define CONVERTDEC(c)  (c - '0')

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

#define SerialPutString(x) Serial_PutString((uint8_t*)(x))

/* Exported functions ------------------------------------------------------- */
void Int2Str(uint8_t* str,int32_t intnum);
uint32_t Str2Int(uint8_t *inputstr,int32_t *intnum);
uint32_t GetIntegerInput(int32_t * num);
uint32_t SerialKeyPressed(uint8_t *key);
uint8_t GetKey(void);
void SerialPutChar(uint8_t c);
void Serial_PutString(uint8_t *s);
void GetInputString(uint8_t * buffP);

#endif

