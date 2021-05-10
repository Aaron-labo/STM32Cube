/*
 * PID.h
 *
 *  Created on: Apr 27, 2021
 *      Author: Francis
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "main.h"

typedef struct PID
{
	float SetPoint; //设定目标温度
	float P; //比例系数
	float I; //积分系数
	float D; //微分系数
	int LastError; //上次误差
	int SumError; //历史累计误差
} PID;

#endif /* INC_PID_H_ */
