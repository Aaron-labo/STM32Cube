/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "RingBuffer.h"
/* USER CODE END Includes */


//定义rb结构图和读写缓存
extern rb_t rb_tmp;

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
/************************************头文件定义****************************************************/

 #define RXBUFFERSIZE   1                        //接收缓存大小
 extern uint8_t aRxBuffer[RXBUFFERSIZE];         //USART接收Buffer

 typedef void(*cb_DataParse)(uint8_t dat);
 void usart_register_data_callback(uint8_t usart_id, cb_DataParse callback);           // 注册数据解析回调函数

 // 注意，以下几个定义不应当在usart.c文件中，作为协议部分，
 // 该函数应当置于独立的协议处理文件中，并暴露访问接口
 // 此处将该函数暴露出来，为的是测试函数里可以统一注册该处理函数
 void ATEK_frmCheck(uint8_t dat);                // 正点原子解析函数，
 #define USART_REC_LEN           200             //定义最大接收字节数
 extern uint8_t  USART_RX_BUF[USART_REC_LEN];    //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
 extern uint16_t USART_RX_STA;                   //接收状态标记

 /**************************************************************************************************/
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
