#include "key.h"

/*******************************************
函数原型：uint8_t KEY_Scan(uint8_t mode)
传 入 值：mode = 0 单次按下按键
					mode = 1 连续按下按键
返 回 值：0 没有按键按下
					1	KEY_RIGHT按下
					2	KEY_DOWN按下
					3	KEY_LEFT按下
					4	KEY_UP按下
*******************************************/
uint8_t KEY_Scan(uint8_t mode)
{
	static uint8_t key = 1;		//按键松开标志
	if(mode == 1){						//支持连按
		key = 1;
	}
	
	if(key&&(KEY_UP==1||KEY_DOWN==0||KEY_LEFT==0||KEY_RIGHT==0)){
		HAL_Delay(10);					//按键消抖
		key = 0;
		if(KEY_UP==1)						return KEY_UP_PRES;
		else if(KEY_DOWN==0)		return KEY_DOWN_PRES; 
		else if(KEY_LEFT==0)		return KEY_LEFT_PRES; 
		else if(KEY_RIGHT==0)		return KEY_RIGHT_PRES; 
	}
	else if(KEY_UP==0&&KEY_DOWN==1&&KEY_LEFT==1&&KEY_RIGHT==1){
		key = 1;
	}
	
	return 0;
}

