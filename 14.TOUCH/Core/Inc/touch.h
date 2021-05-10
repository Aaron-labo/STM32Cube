#ifndef _TOUCH_H_
#define _TOUCH_H_
#include <stdio.h>
#include "lcd.h"
#include "main.h"
#include "24cxx.h"
#include "i2c.h"
#include "spi.h"

/* 定义数据类型 */
typedef struct 
{
	uint16_t x;
	uint16_t y;
  uint16_t lcdx;
  uint16_t lcdy;
}TouchTypeDef;

extern TouchTypeDef TouchData;

typedef struct{
  uint8_t posState;   
  int16_t xOffset;
  int16_t yOffset; 
  float xFactor;
  float yFactor;
}PosTypeDef;


#define TOUCH_ADJ_OK          'Y'              //表示触摸校正参数准备好
#define TOUCH_ADJ_ADDR        200          //校正参数在24C02中的首地址200
/* 触摸校正因数设置 */
#define LCD_ADJX_MIN (10)                      //读取四个点的最小X值
#define LCD_ADJX_MAX (lcddev.width - LCD_ADJX_MIN) //读取四个点的最大X值
#define LCD_ADJY_MIN (10)                      //读取四个点的最小Y值
#define LCD_ADJY_MAX (lcddev.height - LCD_ADJY_MIN) //读取四个点的最大Y值

#define LCD_ADJ_X (LCD_ADJX_MAX - LCD_ADJY_MIN)//读取方框的宽度
#define LCD_ADJ_Y (LCD_ADJY_MAX - LCD_ADJY_MIN)//读取方框的高度

#define TOUCH_READ_TIMES 40     //一次读取触摸值的次数

#define TOUCH_X_CMD      0xD0  //读取X轴命令
#define TOUCH_Y_CMD      0x90  //读取Y轴命令
#define TOUCH_MAX        20    //预期差值
#define TOUCH_X_MAX      4000  //X轴最大值
#define TOUCH_X_MIN      100   //X轴最小值
#define TOUCH_Y_MAX      4000  //Y轴最大值
#define TOUCH_Y_MIN      100   //Y轴最小值


//电阻屏芯片连接引脚
#define	TOUCH_PEN						HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7)
#define	TOUCH_CS_HIGH()			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET)
#define	TOUCH_CS_LOW() 			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_RESET)

void TOUCH_Init(void);
uint8_t WR_Cmd(uint8_t cmd);
uint16_t TOUCH_Read_AD(uint8_t cmd);
uint8_t TOUCH_ReadXY(uint16_t *xValue, uint16_t *yValue);
uint8_t TOUCH_ReadAdjust(uint16_t x, uint16_t y, uint16_t *xValue, uint16_t *yValue);
void TOUCH_Adjust(void);
uint8_t TOUCH_Scan(void);


#endif


