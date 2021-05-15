/*
 * key.h
 *
 *  Created on: Apr 3, 2021
 *      Author: Francis
 */

#ifndef INC_KEY_H_
#define INC_KEY_H_

#include "gpio.h"

//按键宏定义
#define KEY0 HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin)
#define KEY1 HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)
#define KEY_UP HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin)
enum KEY{
	KEY0_PRES = 1,
	KEY1_PRES,
	KEY_UP_PRES
};

uint8_t KeyScan(uint8_t mode);

#endif /* INC_KEY_H_ */
