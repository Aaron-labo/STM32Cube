///*
// * Vol_Temp.c
// *
// *  Created on: 2021年5月7日
// *      Author: Francis
// */
//
//#include "vol_temp.h"
//
//const float UpTemp[38] = { 1.476, 1.478, 1.479, 1.484, 1.501, 1.528, 1.563,
//		1.603, 1.642, 1.682, 1.718, 1.755, 1.792, 1.831, 1.871, 1.910, 1.946,
//		1.984, 2.024, 2.065, 2.098, 2.139, 2.177, 2.215, 2.254, 2.294, 2.332,
//		2.370, 2.413, 2.453, 2.493, 2.534, 2.574, 2.615, 2.653, 2.691, 2.731,
//		2.77 };
//
//const float DownTemp[38] = { 1.487, 1.497, 1.514, 1.535, 1.558, 1.583, 1.616,
//		1.645, 1.681, 1.718, 1.754, 1.789, 1.818, 1.852, 1.881, 1.925, 1.965,
//		2.002, 2.04, 2.081, 2.122, 2.154, 2.19, 2.226, 2.272, 2.308, 2.344,
//		2.389, 2.423, 2.464, 2.499, 2.536, 2.572, 2.615, 2.656, 2.701, 2.743,
//		2.780 };
//
//const float UpTempFast[38] =
//		{ 1.479, 1.479, 1.479, 1.48, 1.481, 1.483, 1.494, 1.508, 1.542, 1.563,
//				1.609, 1.636, 1.674, 1.717, 1.77, 1.812, 1.837, 1.895, 1.935,
//				1.977, 2.017, 2.056, 2.107, 2.146, 2.182, 2.23, 2.275, 2.321,
//				2.361, 2.403, 2.453, 2.49, 2.53, 2.583, 2.63, 2.668, 2.71, 2.756 };
//
//const float DownTempFast[38] = { 1.03, 1.519, 1.531, 1.547, 1.566, 1.584, 1.604,
//		1.63, 1.661, 1.688, 1.721, 1.747, 1.781, 1.816, 1.851, 1.88, 1.926,
//		1.961, 1.997, 2.038, 2.07, 2.111, 2.144, 2.18, 2.225, 2.261, 2.299,
//		2.346, 2.384, 2.423, 2.476, 2.512, 2.556, 2.606, 2.648, 2.687, 2.717,
//		2.771 };
//
///*电压与温度的函数为:
// * v = 0.0156*t+0.4164(v为电压值，t为温度值）
// * t = (v-0.4164)/0.0156
// */
//float conversion1(float voltage) {
//	float temperature = 0;
//	uint8_t i;
//
//	for (i = 0; i < 37; i++) {
//		if (voltage >= DownTemp[i] && voltage < DownTemp[i + 1]) {
//			temperature = 29 + i
//					+ (voltage - DownTemp[i]) / (DownTemp[i + 1] - DownTemp[i]);
//			break;
//		}
//	}
//
//	return temperature;
//}
//
//float conversion2(float voltage) {
//	float temperature = 0;
//	uint8_t i;
//
//	for (i = 0; i < 37; i++) {
//		if (voltage >= UpTemp[i] && voltage < UpTemp[i + 1]) {
//			temperature = 29 - 0.5 + i
//					+ (voltage - UpTemp[i]) / (UpTemp[i + 1] - UpTemp[i]);
//			break;
//		}
//	}
//
//	return temperature;
//}
//
//float conversion3(float voltage) {
//	float temperature = 0;
//	uint8_t i;
//
//	for (i = 0; i < 37; i++) {
//		if (voltage >= DownTempFast[i] && voltage < DownTempFast[i + 1]) {
//			temperature = 29 + i
//					+ (voltage - DownTempFast[i])
//							/ (DownTempFast[i + 1] - DownTempFast[i]);
//			break;
//		}
//	}
//
//	return temperature;
//}
//
//float conversion4(float voltage) {
//	float temperature = 0;
//	uint8_t i;
//
//	for (i = 0; i < 37; i++) {
//		if (voltage >= UpTempFast[i] && voltage < UpTempFast[i + 1]) {
//			temperature = 29 + i
//					+ (voltage - UpTempFast[i])
//							/ (UpTempFast[i + 1] - UpTempFast[i]);
//			break;
//		}
//	}
//
//	return temperature;
//}
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
	temperature -= 1;
	return temperature;
}
