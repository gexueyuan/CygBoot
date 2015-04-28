/**
  *****************************************************************************
  * Copyright(C) Beijing Carsmart Technology Co., Ltd.
  * All rights reserved.
  *
  * @file   : cv_bsp_usart.c
  * @brief  : bsp usart source file
  * @author : Fred
  * @history:
  *        2015-3-4    Fred    Created file
  *        ...
  ******************************************************************************
  */

#define  _BSP_USART_MODULE_

#include <stdio.h>
#include <string.h>
#include "cv_bsp_usart.h"

#ifdef DEBUG_MODE
#define DEBUG_UART   USART1
#else
#define DEBUG_UART   USART2
#endif


//静止使用半主机模式
//#pragma import(__use_no_semihosting_swi)


struct __FILE
{
	int handle;
	/* Whatever you require here. If the only file you are using is */
	/* standard output using printf() for debugging, no file handling */
	/* is required. */
};
//FILE is typedefed in stdio.h
 //摘自stdio.h注释
/*
* FILE is an object capable of recording all information needed to control
* a stream, such as its file position indicator, a pointer to its
* associated buffer, an error indicator that records whether a read/write
* error has occurred and an end-of-file indicator that records whether the
* end-of-file has been reached.
* Its structure is not made known to library clients.
*/

FILE __stdout;
FILE __stdin;

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(DEBUG_UART, (uint8_t) ch);
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(DEBUG_UART, USART_FLAG_TC) == RESET)
    {}
    return ch;
}

/*****************************************************************************
 @funcname: bsp_uart1_deinit
 @brief   : uart1 deinit
 @param   : void
 @return  :
*****************************************************************************/
void bsp_uart1_deinit(void)
{
    USART_DeInit(USART1);
}

/*****************************************************************************
 @funcname: bsp_uart2_deinit
 @brief   : uart1 deinit
 @param   : void
 @return  :
*****************************************************************************/
void bsp_uart2_deinit(void)
{
    USART_DeInit(USART2);
}
/*****************************************************************************
 @funcname: bsp_uart1_init
 @brief   : uart1 init function
 @param   : void
 @return  :
*****************************************************************************/
void bsp_uart1_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHB1PeriphClockCmd(UART1_TX_GPIO_CLK|UART1_RX_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(UART1_CLK, ENABLE);

    GPIO_PinAFConfig(UART1_TX_GPIO_PORT,UART1_TX_SOURCE,UART1_TX_AF);
    GPIO_PinAFConfig(UART1_RX_GPIO_PORT,UART1_RX_SOURCE,UART1_RX_AF);

    /* Configure USART Tx*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = UART1_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(UART1_TX_GPIO_PORT, &GPIO_InitStructure);

    /* Configure USART Rx*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = UART1_RX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(UART1_RX_GPIO_PORT, &GPIO_InitStructure);

    /* USART resources configuration (Clock, GPIO pins and USART registers) ----*/
    /* USART configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
    */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    USART_GetFlagStatus(USART1, USART_FLAG_TC);
     /* USART configuration */
    USART_Init(USART1,&USART_InitStructure);
    /* Enable USART */
    USART_Cmd(USART1, ENABLE);
}


/*****************************************************************************
 @funcname: bsp_uart2_init
 @brief   : uart2 init function
 @param   : void
 @return  :
*****************************************************************************/
void bsp_uart2_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(UART2_TX_GPIO_CLK|UART2_RX_GPIO_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(UART2_CLK, ENABLE);

    GPIO_PinAFConfig(UART2_TX_GPIO_PORT,UART2_TX_SOURCE,UART2_TX_AF);
    GPIO_PinAFConfig(UART2_RX_GPIO_PORT,UART2_RX_SOURCE,UART2_RX_AF);

    /* Configure USART Tx*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = UART2_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(UART2_TX_GPIO_PORT, &GPIO_InitStructure);

    /* Configure USART Rx*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = UART2_RX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(UART2_RX_GPIO_PORT, &GPIO_InitStructure);

    /* USART resources configuration (Clock, GPIO pins and USART registers) ----*/
    /* USART configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
    */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_GetFlagStatus(USART2, USART_FLAG_TC);

    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    /* USART configuration */
    USART_Init(USART2,&USART_InitStructure);

    /* NVIC configuration */
    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART2_PRE_PRI;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_UART2_SUB_PRI;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /*clear rev buf */
    memset(&uart2_rev_buf,0x00,sizeof(uart2_rev_buf));
    /* Enable USART */
    USART_Cmd(USART2, ENABLE);

}


/*****************************************************************************
 @funcname: uart_buf_check
 @brief   : check uart buf
 @param   : void
 @return  :
*****************************************************************************/
uint8_t uart2_buf_check(void)
{
    if( uart2_rev_buf.head == uart2_rev_buf.tail ) {
        return FALSE;
    }
    else {
        return TRUE;
    }
}


/*****************************************************************************
 @funcname: uart2_read_data
 @brief   : receive a byte from uart2
 @param   : void
 @return  :
*****************************************************************************/
uint8_t uart2_read_data(void)
{
    uint8_t rev_data;
    if ( uart2_rev_buf.head != uart2_rev_buf.tail ) {
        rev_data = uart2_rev_buf.buf[uart2_rev_buf.tail++];
        if(uart2_rev_buf.tail >= UART_REV_BUF_MAX) {
            uart2_rev_buf.tail = 0;
        }
    } else {
        rev_data = 0xff;
    }
    return rev_data;
}


/* for ymodele protocol */
uint32_t uart2_buf_read(uint8_t *data)
{
    if ( uart2_rev_buf.head != uart2_rev_buf.tail ) {
        *data = uart2_rev_buf.buf[uart2_rev_buf.tail++];
        if(uart2_rev_buf.tail >= UART_REV_BUF_MAX) {
            uart2_rev_buf.tail = 0;
        }
        return 1;
    } else {
        return 0;
    }
}

/* for ymodele protocol */
void uart2_send_char(uint8_t data)
{
    USART_SendData(USART2, data);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) {}
}


/**
  * @brief  Convert a string to an integer
  * @param  inputstr: The string to be converted
  * @param  intnum: The integer value
  * @retval 1: Correct
  *         0: Error
  */
uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum)
{
  uint32_t i = 0, res = 0;
  uint32_t val = 0;

  if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
  {
    if (inputstr[2] == '\0')
    {
      return 0;
    }
    for (i = 2; i < 11; i++)
    {
      if (inputstr[i] == '\0')
      {
        *intnum = val;
        /* return 1; */
        res = 1;
        break;
      }
      if (ISVALIDHEX(inputstr[i]))
      {
        val = (val << 4) + CONVERTHEX(inputstr[i]);
      }
      else
      {
        /* Return 0, Invalid input */
        res = 0;
        break;
      }
    }
    /* Over 8 digit hex --invalid */
    if (i >= 11)
    {
      res = 0;
    }
  }
  else /* max 10-digit decimal input */
  {
    for (i = 0;i < 11;i++)
    {
      if (inputstr[i] == '\0')
      {
        *intnum = val;
        /* return 1 */
        res = 1;
        break;
      }
      else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
      {
        val = val << 10;
        *intnum = val;
        res = 1;
        break;
      }
      else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
      {
        val = val << 20;
        *intnum = val;
        res = 1;
        break;
      }
      else if (ISVALIDDEC(inputstr[i]))
      {
        val = val * 10 + CONVERTDEC(inputstr[i]);
      }
      else
      {
        /* return 0, Invalid input */
        res = 0;
        break;
      }
    }
    /* Over 10 digit decimal --invalid */
    if (i >= 11)
    {
      res = 0;
    }
  }

  return res;
}

/**
  * @brief  Convert an Integer to a string
  * @param  str: The string
  * @param  intnum: The integer to be converted
  * @retval None
  */
void Int2Str(uint8_t* str, int32_t intnum)
{
  uint32_t i, Div = 1000000000, j = 0, Status = 0;

  for (i = 0; i < 10; i++)
  {
    str[j++] = (intnum / Div) + 48;

    intnum = intnum % Div;
    Div /= 10;
    if ((str[j-1] == '0') & (Status == 0))
    {
      j = 0;
    }
    else
    {
      Status++;
    }
  }
}

/*****************************************************************************
 @funcname: USART1_IRQHandler
 @brief   : usart1 irq handle
 @param   : void
 @return  :
*****************************************************************************/
void USART1_IRQHandler(void)
{
    uint8_t uart2_rev_data;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
        uart2_rev_data = USART_ReceiveData(USART1);

    }

}

/*****************************************************************************
 @funcname: USART2_IRQHandler
 @brief   : usart2 irq handle
 @param   : void
 @return  :
*****************************************************************************/
void USART2_IRQHandler(void)
{
    uint8_t uart2_rev_data;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
        uart2_rev_data = USART_ReceiveData(USART2);

        uart2_rev_buf.buf[uart2_rev_buf.head++] = uart2_rev_data;
        if(uart2_rev_buf.head >= UART_REV_BUF_MAX) {
            uart2_rev_buf.head = 0;
        }
    }
}

