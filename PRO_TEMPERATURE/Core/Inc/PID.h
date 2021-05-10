#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include "main.h"
#include "vol_temp.h"

typedef struct {
	float SetTemp;  //定义设定值
	float ActualTemp;  //定义实际值
	float err;  //定义偏差值
	float err_last;   //定义上一个偏差值
	float Kp, Ki, Kd;  //定义比例、积分、微分系数
	float voltage;  //定义电压值
	float integral;  //定义积分值
} PID;

void PID_Init_Fan(PID* pid);
void PID_Init_Res(PID* pid);
float PID_Calc_Fan(PID* pid, float temp);
float PID_Calc_Res(PID* pid, float temp);
void ChaSetTemp(PID* pid, float settemp);

#endif
