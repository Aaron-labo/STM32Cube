/*
 * key.c
 *
 *  Created on: Apr 3, 2021
 *      Author: Francis
 */

#include "key.h"

//按键扫描函数
//mode==1时，按键按下亮，松开灭
//mode==0时，按键按下亮，再次按下灭
uint8_t KeyScan(uint8_t mode)
{
	static uint8_t key_up = 1;
	if(mode == 1){
		key_up = 1;
	}

	if(key_up && (KEY0 == GPIO_PIN_RESET || KEY1 == GPIO_PIN_RESET || KEY_UP == GPIO_PIN_SET)){
		HAL_Delay(10);
		key_up = 0;
		if(KEY0 == GPIO_PIN_RESET){
			return KEY0_PRES;
		}else if(KEY1 == GPIO_PIN_RESET){
			return KEY1_PRES;
		}else if(KEY_UP == GPIO_PIN_SET){
			return KEY_UP_PRES;
		}
	}else if(KEY0 == GPIO_PIN_SET && KEY0 == GPIO_PIN_SET && KEY_UP == GPIO_PIN_RESET){
		key_up = 1;
	}
	return 0;
}
