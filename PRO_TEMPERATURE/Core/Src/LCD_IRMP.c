/*
 * LCD_IRMP.c
 *
 *  Created on: May 10, 2021
 *      Author: Francis
 */

#include "lcd_irmp.h"

void show_Data(float voltage, float temperature, float target_temp1,
		float target_temp2, float pwmVal_fan, float pwmVal_res) {
	static uint8_t Show_Str[64];
	sprintf((char*) Show_Str, "TEMPERATURE CONTROL");
	LCD_ShowString(10, 30, 240, 24, 24, Show_Str);
	sprintf((char*) Show_Str, "Current voltage   : %.3f", voltage);
	LCD_ShowString(20, 80, 220, 16, 16, Show_Str);
	sprintf((char*) Show_Str, "Current temperature: %.2f", temperature);
	LCD_ShowString(20, 110, 220, 16, 16, Show_Str);
	sprintf((char*) Show_Str, "Initial temperature: %.1f", target_temp1);
	LCD_ShowString(20, 140, 220, 16, 16, Show_Str);
	sprintf((char*) Show_Str, "Target  temperature: %.1f", target_temp2);
	LCD_ShowString(20, 170, 220, 16, 16, Show_Str);
	sprintf((char*) Show_Str, "PWM_Fan: %.2f", pwmVal_fan / 1000);
	LCD_ShowString(20, 200, 220, 16, 16, Show_Str);
	sprintf((char*) Show_Str, "PWM_Res: %.2f", pwmVal_res / 1000);
	LCD_ShowString(20, 230, 220, 16, 16, Show_Str);
}

void Beep(void) {
	BEEP_On();
	HAL_Delay(100);
	BEEP_Off();
}

//红外遥控扫描
void IRMP_Scan(uint8_t mode, uint8_t *isStart) {
	static uint8_t isPress = 1;
	static IRMP_DATA irmp_data;
	if (irmp_get_data(&irmp_data)) {
		HAL_Delay(50);
		printf("按键被按下了......");
		if (mode == 1) {
			isPress = 1;
		}

		if (isPress && irmp_get_data(&irmp_data)) {
			HAL_Delay(10);
			isPress = 0;
			switch (irmp_data.command) {
			case POWER:
				Beep();
				break;

			case UP:
				Beep();
				break;

			case DOWN:
				Beep();
				break;

			case RIGHT:
				Beep();
				break;

			case LEFT:
				Beep();
				break;

			case CENTER:
				Beep();
				break;

			case VOL_ADD:
				Beep();
				break;

			case VOL_DEC:
				Beep();
				break;

			case ONE:
				Beep();
				break;

			case TWO:
				Beep();
				break;

			case THREE:
				Beep();
				break;

			case FOUR:
				Beep();
				break;

			case FIVE:
				Beep();
				break;

			case SIX:
				Beep();
				break;

			case SEVEN:
				Beep();
				break;

			case EIGHT:
				Beep();
				break;

			case NINE:
				Beep();
				break;

			case ZERO:
				Beep();
				break;

			case BACK:
				Beep();
				break;
			}
		} else {
			isPress = 1;
		}
	}
}
