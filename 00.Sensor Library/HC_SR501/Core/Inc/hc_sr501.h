/*
 * hc-sr501.h
 *
 *  Created on: May 24, 2021
 *      Author: Francis
 */

#ifndef INC_HC_SR501_H_
#define INC_HC_SR501_H_

#include "main.h"

#define PIRmotion HAL_GPIO_ReadPin(PIRmotion_GPIO_Port, PIRmotion_Pin)

typedef struct {
 uint8_t status;
} PIRMotionSensor;

void PIR_Init(void);
PIRMotionSensor get_PIR_status(void);


#endif /* INC_HC_SR501_H_ */
