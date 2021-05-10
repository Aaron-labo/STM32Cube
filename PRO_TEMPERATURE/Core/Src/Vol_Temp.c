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
	temperature = (voltage - 0.4)/0.04;
	return temperature;
}

float conversion2(float voltage)
{
	float temperature = 0;
	temperature = (voltage - 0.4)/0.0427;
	return temperature;
}
