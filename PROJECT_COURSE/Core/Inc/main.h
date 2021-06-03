/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY1_Pin GPIO_PIN_3
#define KEY1_GPIO_Port GPIOE
#define KEY0_Pin GPIO_PIN_4
#define KEY0_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_5
#define LED1_GPIO_Port GPIOE
#define KY_UP_Pin GPIO_PIN_0
#define KY_UP_GPIO_Port GPIOA
#define HC_SR501_Pin GPIO_PIN_5
#define HC_SR501_GPIO_Port GPIOA
#define Trig_Pin GPIO_PIN_6
#define Trig_GPIO_Port GPIOA
#define Echo_Pin GPIO_PIN_7
#define Echo_GPIO_Port GPIOA
#define NRF_IRQ_Pin GPIO_PIN_6
#define NRF_IRQ_GPIO_Port GPIOG
#define NRF_CS_Pin GPIO_PIN_7
#define NRF_CS_GPIO_Port GPIOG
#define NRF_CE_Pin GPIO_PIN_8
#define NRF_CE_GPIO_Port GPIOG
#define LED0_Pin GPIO_PIN_5
#define LED0_GPIO_Port GPIOB
#define BEEP_Pin GPIO_PIN_8
#define BEEP_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#ifdef DEBUG
//#define print(format, ...) printf( format "\r\n",##__VA_ARGS__)
#define print(format, ...) printf("[%04d: %s ][ %10s ]:" format "\r\n",__LINE__,__FILE__,__func__, ##__VA_ARGS__)
#define debug(format, ...) printf("[%04d: %s ][ %10s ]:" format "\r\n",__LINE__,__FILE__,__func__, ##__VA_ARGS__)
#else
//#define print(format, ...)
#define print(format, ...) printf( format "\r\n",##__VA_ARGS__)
#define debug(format, ...)
#endif
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
