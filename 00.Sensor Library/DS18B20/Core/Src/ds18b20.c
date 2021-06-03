#include "ds18b20.h"
#include "tim.h"


void DS18B20_IO_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pin = GPIO_PIN_11;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOG,&GPIO_InitStructure);
}

void DS18B20_IO_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_11;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOG,&GPIO_InitStructure);
}

void DS18B20_Rst(void)
{
	DS18B20_IO_OUT();
	DS18B20_DQ_OUT_LOW;
	delay_us(750);
	DS18B20_DQ_OUT_HIGH;
	delay_us(15);
}

uint8_t DS18B20_Check(void)
{
	uint8_t retry = 0;
	DS18B20_IO_IN();
	while(DS18B20_DQ_IN && retry < 200)
	{
		retry++;
		delay_us(1);
	}

	if(retry >= 200)
		return 1;
	else
		retry = 0;

	while(!DS18B20_DQ_IN && retry < 240)
	{
		retry++;
		delay_us(1);
	}

	if(retry >= 240)
		return 1;

	return 0;
}

uint8_t DS18B20_Read_Bit(void)
{
	uint8_t data;
	DS18B20_IO_OUT();
	DS18B20_DQ_OUT_LOW;
	delay_us(2);
	DS18B20_DQ_OUT_HIGH;
	DS18B20_IO_IN();
	delay_us(12);

	if(DS18B20_DQ_IN)
		data = 1;
	else
		data = 0;

	delay_us(50);
	return data;
}

uint8_t DS18B20_Read_Byte(void)
{
	uint8_t i,j,data;
	data = 0;
	for(i=1;i<=8;i++)
	{
		j = DS18B20_Read_Bit();
		data = (j<<7)|(data>>1);
	}
	return data;
}

void DS18B20_Write_Byte(uint8_t data)
{
	uint8_t j;
	uint8_t testb;
	DS18B20_IO_OUT();
	for(j=1;j<=8;j++)
	{
		testb=data&0x01;
		data=data>>1;
		if(testb)
		{
			DS18B20_DQ_OUT_LOW;
			delay_us(2);
			DS18B20_DQ_OUT_HIGH;
			delay_us(60);
		}
		else
		{
			DS18B20_DQ_OUT_LOW;
			delay_us(60);
			DS18B20_DQ_OUT_HIGH;
			delay_us(2);
		}
	}
}

void DS18B20_Start(void)
{
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);
	DS18B20_Write_Byte(0x44);
}

uint8_t DS18B20_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_11;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOG,&GPIO_InitStructure);

	DS18B20_Rst();
	return DS18B20_Check();
}

float DS18B20_Get_Temperature(void)
{
	uint8_t temp_L,temp_H;
	uint16_t temp;
	float temperature = 0;

	DS18B20_Start();
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);
	DS18B20_Write_Byte(0xbe);
	temp_L = DS18B20_Read_Byte();
	temp_H = DS18B20_Read_Byte();

	if(temp_H>7)
	{
		temp_H = ~temp_H;
		temp_L = ~temp_L;
	}

	temp = temp_L | (temp_H << 8);
	temperature += (float)(temp >> 4);
	temperature += (float)(temp &= 0x0f)/100;

	return temperature;
}

