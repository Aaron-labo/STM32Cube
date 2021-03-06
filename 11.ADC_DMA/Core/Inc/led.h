/*
 * led.h
 *
 *  Created on: Apr 3, 2021
 *      Author: Francis
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "gpio.h"

//LED宏定义
#define LED0_On() HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
#define LED0_Off() HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
#define LED1_On() HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
#define LED1_Off() HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
#define LED0_Tog() HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
#define LED1_Tog() HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

#endif /* INC_LED_H_ */
