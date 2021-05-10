/*
 * NEC.c
 *
 *  Created on: Apr 28, 2021
 *      Author: Francis
 */

#include "NEC.h"

/**
 * @brief 微秒级延时，参考F103的库实现
 * @retval None
 */
void delay_us(uint32_t duration) {
  //见stm32f1xx_hal_rcc.c -- static void RCC_Delay(uint32_t mdelay)
  __IO uint32_t Delay = duration * (SystemCoreClock / 8U / 1000000U);
  do {
	__NOP();
  } while (Delay--);
}

uint8_t IR_HighLevelPeriod(void){	//高电平持续时间函数
	uint8_t t=0;
	while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)==1){  //高电平
		t++;
		delay_us(20);
		if(t>=250) return t;	//超时溢出
	}
	return t;
}

//在中断回调函数中完成红外解码
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){		//红外遥控外部中断回调函数
	uint8_t Tim=0,Ok=0,Data,Num=0;
	while(1){
	   	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)==1){
			 Tim = IR_HighLevelPeriod();	//获得此次高电平时间

			 if(Tim>=250)
			 	break;		//无用的信号
			 if(Tim>=200 && Tim<250)
			 	Ok=1;		//收到起始信号
			 else if(Tim>=60 && Tim<90)
			 	Data=1;		//收到数据1
			 else if(Tim>=10 && Tim<50)
			 	Data=0;		//收到数据0

			 if(Ok==1){
			 	receive_Code <<= 1;
				receive_Code += Data;
				if(Num>=32){
					receive_Flag=1;
				    break;
				}
			 }
			 Num++;
		}
	}
}

