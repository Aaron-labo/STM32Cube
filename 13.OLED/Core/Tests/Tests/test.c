//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103RCT6,正点原子MiniSTM32开发板,主频72MHZ，晶振12MHZ
// QDtech-OLED液晶驱动 for STM32
// xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
// wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567
//手机:15989313508（冯工）
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/8/27
//版本：V1.0
//版权所有，盗版必究。
// Copyright(C) 深圳市全动电子技术有限公司 2018-2028
// All rights reserved
/****************************************************************************************************
 //=========================================电源接线================================================//
 // OLED模块               STM32单片机
 //   VCC         接       DC 5V/3.3V      //OLED屏电源正
 //   GND         接          GND          //OLED屏电源地
 //=======================================液晶屏数据线接线==========================================//
 //本模块默认数据总线类型为4线制SPI
 // OLED模块               STM32单片机
 //   D1          接          PB15        //OLED屏SPI写信号
 //=======================================液晶屏控制线接线==========================================//
 // OLED模块               STM32单片机
 //   CS          接          PB11        //OLED屏片选控制信号
 //   RES         接          PB12        //OLED屏复位控制信号
 //   DC          接          PB10        //OLED屏数据/命令选择控制信号
 //   D0          接          PB13        //OLED屏SPI时钟信号
 //=========================================触摸屏接线=========================================//
 //本模块不带触摸功能，所以不需要触摸屏接线
 ****************************************************************************************************/
/***************************************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 ****************************************************************************************************/

#include "stdio.h"
#include "stdlib.h"
//#include "delay.h"
#include "main.h"
#include "u8g2.h"
#include "bsp_u8g2.h"
//#include "test.h"

uint8_t show_delay_max = 0; //等待最长时间
uint8_t show_delay_cnt = 0;     //等待延时标志
uint8_t show_picture_index = 0;   //对应图片选择

void demo_u8g2_draw(u8g2_t *u8g2) {
  u8g2_SetFontMode(u8g2, 1);        // Transparent
  u8g2_SetFontDirection(u8g2, 0);
  u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
  u8g2_DrawStr(u8g2, 0, 30, "U");

  u8g2_SetFontDirection(u8g2, 1);
  u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
  u8g2_DrawStr(u8g2, 21, 8, "8");

  u8g2_SetFontDirection(u8g2, 0);
  u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
  u8g2_DrawStr(u8g2, 51, 30, "g");
  u8g2_DrawStr(u8g2, 67, 30, "\xb2");

  u8g2_DrawHLine(u8g2, 2, 35, 47);
  u8g2_DrawHLine(u8g2, 3, 36, 47);
  u8g2_DrawVLine(u8g2, 45, 32, 12);
  u8g2_DrawVLine(u8g2, 46, 33, 12);

  u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
  u8g2_DrawStr(u8g2, 1, 54, "github.com/olikraus/u8g2");
}

//////demo2
#define hare1_width 25
#define hare1_height 28
static unsigned char hare1_bits[] = { 0x00, 0x08, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00, 0x80, 0x80, 0x01, 0x00, 0x00,
    0x01, 0x02, 0x00, 0x00, 0x0e, 0x0c, 0x00, 0x00, 0x01, 0x30, 0x00, 0x80, 0x00, 0x40, 0x00, 0x80, 0x00, 0x88, 0x00,
    0x40, 0x1c, 0x1c, 0x01, 0x80, 0x23, 0x1c, 0x01, 0x00, 0x20, 0x08, 0x01, 0x00, 0x1c, 0x80, 0x00, 0x00, 0x03, 0x40,
    0x00, 0x80, 0x00, 0x20, 0x00, 0x60, 0x00, 0x20, 0x00, 0x10, 0x00, 0x20, 0x00, 0x08, 0x00, 0x40, 0x00, 0x04, 0x00,
    0x40, 0x00, 0x02, 0x80, 0x87, 0x00, 0x01, 0x40, 0x88, 0x00, 0x01, 0x20, 0x48, 0x00, 0x03, 0x20, 0x30, 0x00, 0x1c,
    0xc0, 0x00, 0x00, 0x20, 0x00, 0x01, 0x00, 0x40, 0x00, 0x01, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x67, 0x00, 0x00,
    0x00, 0x18, 0x00, 0x00 };

#define hare2_width 30
#define hare2_height 28
static unsigned char hare2_bits[] = { 0x00, 0xc0, 0x03, 0x00, 0x00, 0x20, 0x04, 0x00, 0x00, 0x20, 0x08, 0x00, 0x00,
    0x40, 0x10, 0x00, 0x00, 0x80, 0x20, 0x00, 0x00, 0x70, 0x40, 0x00, 0x00, 0x08, 0x80, 0x01, 0x00, 0x04, 0x00, 0x02,
    0x00, 0xf8, 0x00, 0x04, 0x00, 0x00, 0x81, 0x08, 0x00, 0x00, 0xc1, 0x11, 0x00, 0xfc, 0xc0, 0x11, 0x00, 0x03, 0x80,
    0x10, 0x80, 0x00, 0x00, 0x08, 0x70, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x03, 0x06, 0x00, 0xc0, 0x00, 0x01, 0x00,
    0x20, 0x00, 0x01, 0x00, 0x10, 0x00, 0x01, 0x00, 0x10, 0x00, 0x06, 0x00, 0x20, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08,
    0xf0, 0x41, 0x00, 0x08, 0x08, 0x46, 0x00, 0x08, 0x04, 0x48, 0x00, 0x08, 0x08, 0x30, 0x00, 0x30, 0x10, 0x00, 0x00,
    0xc0, 0x0f, 0x00, 0x00 };

void u8g2_prepare() {
  u8g2_SetFont(&u8g2, u8g2_font_courB10_tr);
  u8g2_SetFontRefHeightExtendedText(&u8g2);
  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_SetFontPosTop(&u8g2);
  u8g2_SetFontDirection(&u8g2, 0);
}

void u8g2_drawLogo(void) {
  u8g2_SetFontMode(&u8g2, 1);        // Transparent
  u8g2_SetDrawColor(&u8g2, 1);

  u8g2_SetFontDirection(&u8g2, 0);
  u8g2_SetFont(&u8g2, u8g2_font_inb24_mf);
  u8g2_DrawStr(&u8g2, 0, 5, "U");

  u8g2_SetFontDirection(&u8g2, 1);
  u8g2_SetFont(&u8g2, u8g2_font_inb30_mn);
  u8g2_DrawStr(&u8g2, 51, 8, "8");

  u8g2_SetFontDirection(&u8g2, 0);
  u8g2_SetFont(&u8g2, u8g2_font_inb24_mf);
  u8g2_DrawStr(&u8g2, 51, 5, "g");
  u8g2_DrawStr(&u8g2, 67, 5, "\xb2");

  u8g2_DrawHLine(&u8g2, 2, 35, 47);
  u8g2_DrawHLine(&u8g2, 3, 36, 47);
  u8g2_DrawVLine(&u8g2, 45, 32, 12);
  u8g2_DrawVLine(&u8g2, 46, 33, 12);

  u8g2_SetFont(&u8g2, u8g2_font_5x8_tr);

  u8g2_DrawStr(&u8g2, 1, 54, "github.com/olikraus/u8g2");
}

void u8g2_box_frame(uint8_t a) {
  u8g2_DrawStr(&u8g2, 0, 0, "DrawBox");
  u8g2_DrawBox(&u8g2, 0, 15, 20, 10);
  u8g2_DrawBox(&u8g2, 0 + a, 15, 30, 7);
  u8g2_DrawStr(&u8g2, 0, 32, "DrawFrame");
  u8g2_DrawFrame(&u8g2, 0, 15 + 32, 20, 10);
  u8g2_DrawFrame(&u8g2, 0 + a, 15 + 32, 30, 7);
}

void u8g2_disc_circle(uint8_t a) {
  u8g2_DrawStr(&u8g2, 0, 0, "DrawDisc");
  u8g2_DrawDisc(&u8g2, 10, 22, 7, U8G2_DRAW_ALL);
  u8g2_DrawDisc(&u8g2, 24 + a, 20, 5, U8G2_DRAW_ALL);
  u8g2_DrawStr(&u8g2, 0, 32, "DrawCircle");
  u8g2_DrawCircle(&u8g2, 10, 22 + 32, 7, U8G2_DRAW_ALL);
  u8g2_DrawCircle(&u8g2, 24 + a, 20 + 32, 5, U8G2_DRAW_ALL);
}

void u8g2_string(uint8_t a) {
  u8g2_SetFontDirection(&u8g2, 0);
  u8g2_DrawStr(&u8g2, 70 + a, 31, " 0");
  u8g2_SetFontDirection(&u8g2, 1);
  u8g2_DrawStr(&u8g2, 68, 0 + a, " 90");
  u8g2_SetFontDirection(&u8g2, 2);
  u8g2_DrawStr(&u8g2, 70 - a, 31, " 180");
  u8g2_SetFontDirection(&u8g2, 3);
  u8g2_DrawStr(&u8g2, 72, 64 - a, " 270");
}

void u8g2_line(uint8_t a) {
  u8g2_DrawStr(&u8g2, 0, 0, "DrawLine");
  u8g2_DrawLine(&u8g2, 7 + a, 15, 60 - a, 60);
  u8g2_DrawLine(&u8g2, 7 + a * 2, 15, 80 - a, 60);
  u8g2_DrawLine(&u8g2, 7 + a * 3, 15, 100 - a, 60);
  u8g2_DrawLine(&u8g2, 7 + a * 4, 15, 120 - a, 60);
}

void u8g2_hare(uint8_t a) {
  u8g2_DrawStr(&u8g2, 0, 0, "DrawXBM");
  if ((a & 1) == 0) {
    u8g2_DrawXBM(&u8g2, 0 + a, 30, hare1_width, hare1_height, hare1_bits);
  } else {
    u8g2_DrawXBM(&u8g2, 0 + a, 30, hare2_width, hare2_height, hare2_bits);
  }
}

void u8g2_CN_Text(uint8_t a){
  //u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, const char *str
  u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
//  u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_gb2312);
  u8g2_DrawUTF8(&u8g2,0 + a, 30,"FM收音机");
}

void demo_u8g2_draw2(void) {
  print("%s called: %d ", __func__, show_picture_index);
  u8g2_prepare();
  switch (show_picture_index) {
  case 0:
    //show_delay_max = 12;
    u8g2_drawLogo();
    break;
  case 1:
    show_delay_max = 150;
    u8g2_box_frame(show_delay_cnt);
    break;
  case 2:
    show_delay_max = 150;
    u8g2_disc_circle(show_delay_cnt);
    break;
  case 3:
    show_delay_max = 60;
    u8g2_string(show_delay_cnt);
    break;
  case 4:
    show_delay_max = 50;
    u8g2_line(show_delay_cnt);
    break;
  case 5:
    show_delay_max = 120;
    u8g2_hare(show_delay_cnt);
    break;
  case 6:
    show_delay_max = 120;
    u8g2_CN_Text(show_delay_cnt);
    break;
  }
}

//初始化
void demo_u8g2_init(void) {
  u8g2_init();
}
//循环
void demo_u8g2_task(void) {
  print("%s 调用", __func__);
//  u8g2_FirstPage(&u8g2);
//  do {                //demo_u8g2_draw(&u8g2);
//    demo_u8g2_draw2();
//  } while (u8g2_NextPage(&u8g2));
//
//  HAL_Delay(100);
//
//  if (show_delay_cnt <= show_delay_max)
//    show_delay_cnt += 3;
//  else {
//    show_delay_cnt = 0;
//    if (++show_picture_index > 6) show_picture_index = 0;
//  }

//  static uint8_t radius = 30;
//  u8g2_DrawCircle(&u8g2, 64, 32, radius, U8G2_DRAW_ALL);
//  u8g2_SendBuffer(&u8g2);
//  HAL_Delay(1000);
//  radius = (radius == 30)?20:30;
//  u8g2_ClearDisplay(&u8g2);


  u8g2_prepare();
//  demo_u8g2_draw(&u8g2);
////  u8g2_ClearDisplay(&u8g2);
////  u8g2_CN_Text(100);
////  u8g2_SendBuffer(&u8g2);

  u8g2_SetFontMode(&u8g2, 1);        // Transparent
  u8g2_SetDrawColor(&u8g2, 1);

  u8g2_SetFontDirection(&u8g2, 0);
  u8g2_SetFont(&u8g2, u8g2_font_inb24_mf);
  u8g2_DrawStr(&u8g2, 0, 5, "U");

  u8g2_CN_Text(50);
  u8g2_SendBuffer(&u8g2);
  HAL_Delay(1000);
}

