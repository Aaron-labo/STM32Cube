/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
rb_t rb_tmp;

/************************************源文件前置定义****************************************************/
//串口中断服务程序
uint8_t aRxBuffer[RXBUFFERSIZE];       //HAL库使用的串口接收缓冲
cb_DataParse data_parse_callback;      // 数据帧处理回调函数指针

/// 注意，以下几个定义不应当在usart.c文件中，作为协议部分，
/// 该函数应当置于独立的协议处理文件中，并暴露访问接口

//注意,读取USARTx->SR能避免莫名其妙的错误
uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，    接收完成标志
//bit14，    接收到0x0d
//bit13~0，  接收到的有效字节数目
uint16_t USART_RX_STA = 0;       //接收状态标记

/**************************************************************************************************/
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle) {

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	if (uartHandle->Instance == USART1) {
		/* USER CODE BEGIN USART1_MspInit 0 */

		/* USER CODE END USART1_MspInit 0 */
		/* USART1 clock enable */
		__HAL_RCC_USART1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**USART1 GPIO Configuration
		 PA9     ------> USART1_TX
		 PA10     ------> USART1_RX
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* USART1 interrupt Init */
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		/* USER CODE BEGIN USART1_MspInit 1 */

		/* USER CODE END USART1_MspInit 1 */
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle) {

	if (uartHandle->Instance == USART1) {
		/* USER CODE BEGIN USART1_MspDeInit 0 */

		/* USER CODE END USART1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_USART1_CLK_DISABLE();

		/**USART1 GPIO Configuration
		 PA9     ------> USART1_TX
		 PA10     ------> USART1_RX
		 */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

		/* USART1 interrupt Deinit */
		HAL_NVIC_DisableIRQ(USART1_IRQn);
		/* USER CODE BEGIN USART1_MspDeInit 1 */

		/* USER CODE END USART1_MspDeInit 1 */
	}
}

/* USER CODE BEGIN 1 */

// 接收中断服务函数
/**
 * @brief 每次接收完成之后，需要重新开中断
 * @param huart : 串口句柄
 * @param buf   : 串口接收缓冲区地址
 * @param bufSize:  串口接收缓冲区大小
 * @retval None
 */
void enable_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *buf,
		uint16_t bufSize) {
	uint32_t timeout = 0;

	while (HAL_UART_GetState(huart) != HAL_UART_STATE_READY) {    //等待就绪
		timeout++;                                                  //超时处理
		if (timeout > HAL_MAX_DELAY)
			break;
	}
	timeout = 0;

	while (HAL_UART_Receive_IT(huart, buf, bufSize) != HAL_OK) { //一次处理完成之后，重新开启中断并设置RxXferCount为1
		timeout++; //超时处理
		if (timeout > HAL_MAX_DELAY)
			break;
	}
}

/**********************************************************************
 * @brief 正点原子例程中的帧检测函数
 * @param dat: 串口终端中接收到的数据
 * @retval None
 * @desc: 正点原子的串口接收例程里，需要检测'\r\n'来判断是否接收完成，
 *                   如果直接使用HAL_UART_Transmit进行发送，必须在发送完成后
 *                  再发送一个'\r\n'，否则会出现没有接收完成的问题。
 **********************************************************************/
void ATEK_frmCheck(uint8_t dat) {
	UNUSED(dat);

	if ((USART_RX_STA & 0x8000) == 0) {     //接收未完成
		if (USART_RX_STA & 0x4000) {          //接收到了0x0d
			if (aRxBuffer[0] != 0x0a) {         //接收错误,重新开始
				USART_RX_STA = 0;
			} else {                            //接收完成了
				USART_RX_STA |= 0x8000;
			}
		} else {                              //还没收到0X0D
			if (aRxBuffer[0] == 0x0d)
				USART_RX_STA |= 0x4000;
			else {
				//rb_writebyte(&rb_tmp, aRxBuffer[0]);
				USART_RX_BUF[USART_RX_STA & 0X3FFF] = aRxBuffer[0];
				USART_RX_STA++;
				if (USART_RX_STA > (USART_REC_LEN - 1))
					USART_RX_STA = 0; //接收数据错误,重新开始接收
			}
		}
	}
}
/**********************************************************************
 * @brief 注册数据处理回调函数
 * @param usart_id: 串口号, 1: USART1, 2: USART2 ...
 * @param callback: 需要注册的回调函数
 * @desc: cb_DataParse 为回调函数指针类型，定义为 void callback(uint8_t d)
 **********************************************************************/
void usart_register_data_callback(uint8_t usart_id, cb_DataParse callback) {
	switch (usart_id) {
	case 1:                               // usart1
		data_parse_callback = callback;
		break;
	default:
		data_parse_callback = NULL;
		break;
	}

}

/**
 * @brief Rx Transfer completed callbacks
 * @param huart: uart handle
 * @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {          //串口1

		// data_parse_callback是数据处理函数指针
		if (data_parse_callback != NULL) {
			data_parse_callback(aRxBuffer[0]);
		} else {
			print("未注册数据处理回调函数");
		}
		// 开启接收中断
		enable_UART_Receive_IT(huart, (uint8_t*) aRxBuffer, RXBUFFERSIZE);
	}
}

// 须在头文件中添加 #include <stdio.h>

//////////////////////////////////////////////////////////////////
// 加入以下代码,支持printf函数,而不需要选择use MicroLIB
// MDK中同样定义了 __GNUC__，使用__ARMCC_VERSION 或  __CC_ARM 区分MDK和STM32CubeIDE
// MDK预定义宏，详见 ：https://www.keil.com/support/man/docs/armcc/armcc_chr1359125007083.htm

#ifdef __CC_ARM // 或 #ifdef __ARMCC_VERSION
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#else
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#endif
#if 1

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x) {
	x = x;
}
//重定义fputc函数
PUTCHAR_PROTOTYPE {
	// 具体哪个串口可以更改huart2为其它串口
	// HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1 , 0xffff);
	while ((USART1->SR & 0X40) == 0) {
	}; //循环发送,直到发送完毕
	USART1->DR = (uint8_t) ch;

//  while ((USART2->SR & 0X40) == 0) {
//  }; //循环发送,直到发送完毕
//  USART2->DR = (uint8_t) ch;

	return ch;
}
#endif

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
