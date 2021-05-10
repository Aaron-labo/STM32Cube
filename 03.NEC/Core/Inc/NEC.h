/*
 * NEC.h
 *
 *  Created on: Apr 28, 2021
 *      Author: Francis
 */

#ifndef INC_NEC_H_
#define INC_NEC_H_

#include "main.h"

void delay_us(uint32_t duration);
uint8_t IR_HighLevelPeriod(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);


#endif /* INC_NEC_H_ */
