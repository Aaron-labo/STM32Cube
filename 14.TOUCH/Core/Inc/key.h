#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"

//操作HAL库函数读取IO口状态
#define KEY_RIGHT	HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)
#define KEY_DOWN    HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)
#define KEY_LEFT    HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)
#define KEY_UP      HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)
//定义按键按下时的键值
#define KEY_RIGHT_PRES 	1
#define KEY_DOWN_PRES	2
#define KEY_LEFT_PRES	3
#define KEY_UP_PRES   	4

uint8_t KEY_Scan(uint8_t mode);

#endif
