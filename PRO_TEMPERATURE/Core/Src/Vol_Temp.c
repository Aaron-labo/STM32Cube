/*
 * Vol_Temp.c
 *
 *  Created on: 2021年5月7日
 *      Author: Francis
 */

#include "vol_temp.h"


/*电压与温度的函数为:
 * v = 0.0156*t+0.4164(v为电压值，t为温度值）
 * t = (v-0.4164)/0.0156
 */
float conversion1(float voltage)
{
	float temperature = 0;
	if(voltage >= 1.56)
	{
		temperature = (voltage - 0.21)/0.039;
	}else if(voltage >= 1.513)
	{
		temperature = (voltage - 0.358)/0.035;
	}else if(voltage >= 1.479)
	{
		temperature = (voltage - 0.952)/0.017;
	}else if(voltage >= 1.478)
	{
		temperature = (voltage - 1.4635)/0.0005;
	}
	return temperature;
}

float conversion2(float voltage)
{
	float temperature = 0;
	if(voltage >= 1.898)
	{
		temperature = (voltage - 0.18)/0.0395;
	}else{
		temperature = (voltage - 0.37)/0.0354;
	}

	return temperature;
}
