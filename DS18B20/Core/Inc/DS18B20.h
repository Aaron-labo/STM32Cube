/*
 * BS18B20.h
 *
 *  Created on: Apr 26, 2021
 *      Author: Francis
 */


#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_
#include "main.h"
#include "stdio.h"

#define DS18B20_UP() HAL_GPIO_WritePin(DS18B20_Pin, DS18B20_GPIO_Port, GPIO_PIN_SET)
#define DS18B20_DOWM() HAL_GPIO_WritePin(DS18B20_Pin, DS18B20_GPIO_Port, GPIO_PIN_RESET)
#define DS18B20_IN HAL_GPIO_ReadPin(DS18B20_Pin, DS18B20_GPIO_Port)

void delay_us(uint32_t duration);
void DS18B20_IO_IN(void);
void DS18B20_IO_OUT(void);
void DS18B20_Rst(void);
uint8_t DS18B20_Check(void);
uint8_t DS18B20_Reset();
uint8_t DS18B20_Read_Bit(void) ;
uint8_t DS18B20_Read_Byte(void);
void DS18B20_Write_Byte(uint8_t dat);

#endif /* INC_DS18B20_H_ */
