/*
 * beep.h
 *
 *  Created on: Apr 3, 2021
 *      Author: Francis
 */

#ifndef INC_BEEP_H_
#define INC_BEEP_H_

#include "gpio.h"
#define BEEP_On() HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET)
#define BEEP_Off() HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET)
#define BEEP_Tog() HAL_GPIO_TogglePin(BEEP_GPIO_Port, BEEP_Pin)

#endif /* INC_BEEP_H_ */
