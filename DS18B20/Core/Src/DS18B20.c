/*
 * DS18B20.c
 *
 *  Created on: Apr 26, 2021
 *      Author: Francis
 */

#include "DS18B20.h"

//us延时函数
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

//使DS18B20引脚变为输入模式
void DS18B20_IO_IN(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStruct.Pin = DS18B20_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);
}

//使DS18B20引脚变为推挽输出模式
void DS18B20_IO_OUT(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStruct.Pin = DS18B20_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DS18B20_Rst(void) {
	DS18B20_IO_OUT(); //引脚输出模式

	//拉低总线并延时750us
	DS18B20_DOWM();
	delay_us(750);

	//释放总线为高电平并延时等待15~60us
	DS18B20_UP();
	delay_us(15);
}

/***************************************************************************
 函数名：DS18B20_Check
 功  能：检测DS18B20返回的存在脉冲
 输  入: 无
 输  出：无
 返回值：0:成功  1：失败   2:释放总线失败
 备  注：
 ***************************************************************************/
uint8_t DS18B20_Check(void) {
	//定义一个脉冲持续时间
	uint8_t retry = 0;
	//引脚设为输入模式
	DS18B20_IO_IN();
	while (DS18B20_IN && retry < 200) {
		retry++;
		delay_us(1);
	}

	if (retry >= 200)
		return 1;
	else
		retry = 0;

	//判断DS18B20是否释放总线
	while (!DS18B20_IN && retry < 240) {
		retry++;
		delay_us(1);
	}

	if (retry >= 240)
		return 2;

	return 0;
}

//DS18B20复位函数
uint8_t DS18B20_Reset() {
	bit ackset;
	DS18B20_DOWM();
	delay_us(60);
	DS18B20_UP();
	delay_us(6);
	ackset = DS18B20_IN;
	while (!DS18B20_IN)
		;
	return ackset;
}

//读取一个位
uint8_t DS18B20_Read_Bit(void)              // read one bit

{
	uint8_t data;
	DS18B20_IO_OUT();              //设置为输出
	DS18B20_DOWM(); //输出低电平2us
	delay_us(2);
	DS18B20_UP(); //拉高释放总线
	DS18B20_IO_IN(); //设置为输入
	delay_us(12); //延时12us
	if (DS18B20_IN)
		data = 1; //读取总线数据
	else
		data = 0;
	delay_us(50);  //延时50us
	return data;
}

uint8_t DS18B20_Read_Byte(void)    // read one byte
{
	uint8_t i, j, dat;
	dat = 0;
	for (i = 1; i <= 8; i++) {
		j = DS18B20_Read_Bit();
		dat = (j << 7) | (dat >> 1);
	}
	return dat;
}

void DS18B20_Write_Byte(uint8_t dat) {
	uint8_t j;
	uint8_t testb;
	DS18B20_IO_OUT();    //设置PA0为输出
	for (j = 1; j <= 8; j++) {
		testb = dat & 0x01;
		dat = dat >> 1;
		if (testb) //输出高
		{
			DS18B20_DOWM(); // 主机输出低电平
			delay_us(2);                  //延时2us
			DS18B20_UP();                  //释放总线
			delay_us(60); //延时60us
		} else //输出低
		{
			DS18B20_DOWM(); //主机输出低电平
			delay_us(60);               //延时60us
			DS18B20_UP();               //释放总线
			delay_us(2);                  //延时2us
		}
	}
}

