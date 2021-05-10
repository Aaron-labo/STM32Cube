/*
* u8g2_support.h
*
*  Created on: 2020Äê12ÔÂ25ÈÕ
*      Author: Administrator
*/

#ifndef U8G2_SUPPORT_H_
#define U8G2_SUPPORT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "u8g2.h"

#include <stdio.h>
//#include "board.h"
//#include "peripherals.h"
//#include "pin_mux.h"
//#include "clock_config.h"
//#include "LPC55S69_cm33_core0.h"
///* TODO: insert other include files here. */
//#include "delay.h"
//
//#include "ss1306_oled.h"

#define DATA_BUFFER_SIZE 1024         //the size of buffer depends on how many pages are transfered at once e.g. one page are 128byte  1024
#define I2C_TIMEOUT 1000
#define DEVICE_ADDRESS 0x78         //device address is added

extern u8g2_t u8g2; // a structure which will contain all the data for one display


uint8_t u8x8_byte_stm32_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_gpio_and_delay_stm32(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void u8g2_init(void);

#ifdef __cplusplus
  }
#endif

#endif /* U8G2_SUPPORT_H_ */
