/*
 * u8g2_support.h
 *
 *  Created on: 2020��12��25��
 *      Author: Administrator
 */
#include "main.h"
#include "i2c.h"
#include "bsp_u8g2.h"
#include "u8g2.h"
#include "u8x8.h"
#include <stdio.h>

//// I2C �豸��ַ
//#define I2C_WRITE_ADDR  0x78
//#define I2C_READ_ADDR  0x79
//
//// ʹ�� I2C ʱ��ֱ�ӶԵ�ַ 0x00 ����Ϊд���ֱ�Ӷ� 0x40����Ϊд����
//#define OLED_CMD     0x00  //д����
//#define OLED_DATA    0x40  //д����

u8g2_t u8g2; // a structure which will contain all the data for one display

uint8_t u8x8_gpio_and_delay_stm32(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  switch (msg) {
  case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
    __NOP();
    break;
  case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
    HAL_Delay(arg_int * 10);
    break;
  case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
    HAL_Delay(arg_int);
    break;
  case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
    delay_us(1);
    break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us


  default:
    u8x8_SetGPIOResult(u8x8, 1); // default return value
    break;
  }

  return 1;
}

uint8_t u8x8_byte_stm32_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  {
    static uint8_t buffer[32];        /* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
    static uint8_t buf_idx;
    uint8_t *data;

    switch(msg)
    {
      case U8X8_MSG_BYTE_SEND:
        data = (uint8_t *)arg_ptr;
        while( arg_int > 0 )
        {
      buffer[buf_idx++] = *data;
      data++;
      arg_int--;
        }
        break;
      case U8X8_MSG_BYTE_INIT:
        /* add your custom code to init i2c subsystem */
        break;
      case U8X8_MSG_BYTE_SET_DC:
        /* ignored for i2c */
        break;
      case U8X8_MSG_BYTE_START_TRANSFER:
        buf_idx = 0;
        break;
      case U8X8_MSG_BYTE_END_TRANSFER:
//        print("0x%02X",u8x8_GetI2CAddress(u8x8));
        HAL_I2C_Master_Transmit(&hi2c1, u8x8_GetI2CAddress(u8x8), buffer, buf_idx, 10);
        break;
      default:
        return 0;
    }
    return 1;
  }
}

//��ʼ��
void u8g2_init(void) {
  print("%s called", __func__);
  // ��ʼ�� u8g2 �ṹ��
  u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_stm32_hw_i2c, u8x8_gpio_and_delay_stm32);
  // ������ѡ��оƬ���г�ʼ����������ʼ����ɺ���ʾ�����ڹر�״̬
  u8g2_InitDisplay(&u8g2);
  u8g2_ClearDisplay(&u8g2);
  // ����ʾ��
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_ClearBuffer(&u8g2);

  print("%s called", __func__);
}

