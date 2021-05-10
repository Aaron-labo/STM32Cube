#include "PID.h"

//风扇PID初始化
void PID_Init_Fan(PID *pid) {
	printf("PID Init begin\r\n");
	pid->SetTemp = 70;
	pid->ActualTemp = 0;
	pid->err = 0;
	pid->err_last = 0;
	pid->voltage = 0;
	pid->integral = 0;
	pid->Kp = 10.0;
	pid->Ki = 0.0;
	pid->Kd = 0.0;
}

//水泥电阻PID初始化
void PID_Init_Res(PID *pid) {
	printf("PID Init begin\r\n");
	pid->SetTemp = 40;
	pid->ActualTemp = 0;
	pid->err = 0;
	pid->err_last = 0;
	pid->voltage = 0;
	pid->integral = 0;
	pid->Kp = 32.5;
	pid->Ki = 0.5;
	pid->Kd = 20.0;
}

//风扇PID调节函数
float PID_Calc_Fan(PID *pid, float temp) {
	pid->ActualTemp = temp;
	pid->err = pid->SetTemp - pid->ActualTemp;

	if (pid->err >= 20) {
		printf("比例%.2f\r\n", pid->err);
		return 800;
	}

	pid->integral += pid->err;
	//PID算法核心函数
	pid->voltage = pid->Kp * pid->err + pid->Ki * pid->integral
			+ pid->Kd * (pid->err - pid->err_last);
	pid->ActualTemp = pid->voltage * 1.0;
	printf("比例%.2f\r\n积分%.2f\r\n微分%.2f\r\n输出为%.2f", pid->err, pid->integral,
			(pid->err - pid->err_last), pid->ActualTemp);
	pid->err_last = pid->err;

	//设定其最大值和最小值
	if (pid->ActualTemp >= 1000) {
		pid->ActualTemp = 1000;
	} else if (pid->ActualTemp <= 0) {
		pid->ActualTemp = 0;
	}

	return pid->ActualTemp;
}

//水泥电阻PID调节函数
float PID_Calc_Res(PID *pid, float temp) {

	pid->ActualTemp = temp;
	pid->err = pid->SetTemp - pid->ActualTemp;

	if (pid->err >= 20) {
		printf("比例%.2f\r\n", pid->err);
		return 800;
	}

	pid->integral += pid->err;
	//PID算法核心函数
	pid->voltage = pid->Kp * pid->err + pid->Ki * pid->integral
			+ pid->Kd * (pid->err - pid->err_last);
	pid->ActualTemp = pid->voltage * 1.0;
	printf("比例%.2f\r\n积分%.2f\r\n微分%.2f\r\n输出为%.2f", pid->err, pid->integral,
			(pid->err - pid->err_last), pid->ActualTemp);
	pid->err_last = pid->err;

	//设定其最大值和最小值
	if (pid->ActualTemp >= 700) {
		pid->ActualTemp = 700;
	} else if (pid->ActualTemp <= 10) {
		pid->ActualTemp = 10;
	}

	return pid->ActualTemp;
}

//改变设定值
void ChaSetTemp(PID* pid, float settemp)
{
	pid->SetTemp = settemp;
}