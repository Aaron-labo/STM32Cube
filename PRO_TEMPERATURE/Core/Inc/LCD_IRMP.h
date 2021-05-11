/*
 * LCD_IRMP.h
 *
 *  Created on: May 10, 2021
 *      Author: Francis
 */

#ifndef INC_LCD_IRMP_H_
#define INC_LCD_IRMP_H_

#include "main.h"

#define POWER	0x45
#define UP    	0x46
#define DOWN  	0x15
#define RIGHT 	0x43
#define LEFT  	0x44
#define CENTER	0x40
#define VOL_ADD	0x09
#define VOL_DEC 0x07
#define ONE		0x16
#define TWO		0x19
#define THREE	0x0D
#define FOUR	0x0C
#define FIVE	0x18
#define SIX		0x5E
#define SEVEN	0x08
#define EIGHT	0x1C
#define NINE	0x5A
#define ZERO	0x42
#define BACK	0x4A

void show_Data(float voltage, float temperature, float target_temp, float pwmVal_fan, float pwmVal_res);
void Beep(void);
void IRMP_Scan(uint8_t mode, uint8_t* isStart);

#endif /* INC_LCD_IRMP_H_ */
