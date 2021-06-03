/*
 * hc_sr501.c
 *
 *  Created on: May 24, 2021
 *      Author: Francis
 */

#include "hc_sr501.h"


void PIR_Init(void)
{
	//HAL_Delay(PIR_STARTUP_DELAY);
	return;
}

PIRMotionSensor get_PIR_status(void)
{
	GPIO_PinState state = HAL_GPIO_ReadPin(PIRmotion_GPIO_Port, PIRmotion_Pin);
	PIRMotionSensor pir;
	if (state == GPIO_PIN_SET)
	{
		pir.status = 1;
	} else {
		pir.status = 0;
	}
	return (pir);
}
