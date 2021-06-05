/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdlib.h"
#include "lcd.h"
#include "hc_sr04.h"
#include "ds18b20.h"
#include "nrf24l01.h"
#include "key.h"
#include "beep.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
hc_sr04_device_t hc_sr04;
uint8_t manCome, LastCome = 2;
float temperature;
uint8_t Rx_Buf[32];
uint8_t Tx_Buf[32];
uint8_t mode, key;
uint32_t i = 0, time = 0;

void LCD_Show() {
	uint8_t ShowBuffer[64]; //LCD输出缓冲区

	//LCD输出一个logo
	POINT_COLOR = BLACK; //将画笔颜色设置为黑色
	LCD_ShowString(25, 30, 240, 16, 16, (uint8_t*) ".------..------..------.");
	LCD_ShowString(25, 45, 240, 16, 16, (uint8_t*) "|W.--. ||X.--. ||Y.--. |");
	LCD_ShowString(25, 60, 240, 16, 16,
			(uint8_t*) "| :/\\: || :/\\: || (\\/) |");
	LCD_ShowString(25, 75, 240, 16, 16,
			(uint8_t*) "| :\\/: || (__) || :\\/: |");
	LCD_ShowString(25, 90, 240, 16, 16, (uint8_t*) "| '--'W|| '--'X|| '--'Y|");
	LCD_ShowString(25, 105, 240, 16, 16, (uint8_t*) "`------'`------'`------'");

	//LCD输出测距结果
	POINT_COLOR = BLACK; //将画笔颜色设置为黑色
	sprintf((char*) ShowBuffer, "DIST:%7.2f", hc_sr04.distance);
	LCD_ShowString(30, 140, 240, 24, 24, ShowBuffer);

	//LCD输出测温结果
	sprintf((char*) ShowBuffer, "TEMP:%7.2f", temperature);
	LCD_ShowString(30, 170, 240, 24, 24, ShowBuffer);

	//根据按键选择的工作模式的不同，LCD上显示不同的提示信息
	if (mode == KEY0_PRES) {
		LCD_ShowString(25, 280, 200, 24, 24, (uint8_t*) "NRF24L01 RX_MODE");
	} else if (mode == KEY1_PRES) {
		LCD_ShowString(25, 280, 200, 24, 24, (uint8_t*) "NRF24L01 TX_MODE");
	}

	//LCD输出红外结果
	//为避免不同字符混杂的情况，当LCD显示的字符不同时，先执行一次清屏操作
	if (LastCome != manCome) {
		LCD_Fill(20, 200, 220, 280, WHITE);
	}
	if (manCome == 1) {
		POINT_COLOR = RED;
		LCD_ShowString(30, 210, 240, 24, 24, (uint8_t*) "SOMEBODY");
		LCD_ShowString(30, 240, 240, 24, 24, (uint8_t*) "APPROCHING!!!!");
	} else {
		POINT_COLOR = BLACK;
		LCD_ShowString(40, 220, 240, 24, 24, (uint8_t*) "NOBODY......");
	}
	LastCome = manCome;
}

//选择nRF24L01的工作模式
void nRF24L01_Start(void) {
	//首先判断是否正常连接上了nRF24L01
	//若未正常连接，LCD显示错误提醒
	while (NRF24L01_Check()) {
		POINT_COLOR = RED;
		LCD_ShowString(30, 70, 200, 24, 24, (uint8_t*) "NRF24L01 Error...");
		HAL_Delay(700);
		LCD_Fill(30, 70, 230, 94, WHITE);
		HAL_Delay(700);
	}
	//若正常连接nRF24L01则显示相关提示
	LCD_Fill(30, 70, 230, 94, WHITE);
	POINT_COLOR = BLACK;
	LCD_ShowString(30, 70, 200, 24, 24, (uint8_t*) "NRF24L01 OK");
	HAL_Delay(300);
	LCD_ShowString(30, 70, 200, 24, 24, (uint8_t*) "NRF24L01 OK.");
	HAL_Delay(500);
	LCD_ShowString(30, 70, 200, 24, 24, (uint8_t*) "NRF24L01 OK..");
	HAL_Delay(500);
	LCD_ShowString(30, 70, 200, 24, 24, (uint8_t*) "NRF24L01 OK...");
	HAL_Delay(500);

	//使用按键选择单片机的工作模式
	//Key0----接收模式
	//Key1----发送模式
	while (1) {
		i++;
		mode = KeyScan(0);
		POINT_COLOR = RED;
		if (i == 80000) {
			LCD_ShowString(40, 130, 200, 24, 24, (uint8_t*) "RECEIVE(KEY0)");
			LCD_ShowString(100, 170, 200, 24, 24, (uint8_t*) "OR");
			LCD_ShowString(60, 210, 200, 24, 24, (uint8_t*) "SENT(KEY1)");
		} else if (i == 160000) {
			LCD_Fill(30, 130, 230, 234, WHITE);
			i = 0;
		}

		POINT_COLOR = BLACK;
		switch (mode) {
		case KEY0_PRES:
			NRF24L01_RX_Mode();
			break;
		case KEY1_PRES:
			NRF24L01_TX_Mode();
			break;
		default:
			continue;
		}
		break;
	}
	//结束后全屏清屏，方便后续处理
	LCD_Fill(0, 0, 320, 240, WHITE);
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_FSMC_Init();
	MX_USART1_UART_Init();
	MX_TIM2_Init();
	MX_SPI2_Init();
	MX_TIM7_Init();
	/* USER CODE BEGIN 2 */
	HC_SR04_Init(&hc_sr04); //HC_SR04初始化
	DS18B20_Init(); //DS18B20初始化
	NRF24L01_Init(); //nRF24L01初始化

	LCD_Init(); //LCD初始化
	LCD_Clear(WHITE); //清屏为白色
	POINT_COLOR = BLACK; //将画笔颜色设置为黑色

	nRF24L01_Start(); //nRF24L01
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	//判断当前应该进入发送模式还是接收模式
	if (mode == KEY1_PRES) {
		while (1) {
			/*
			 * 发送模式中发送的字符串由两部分组成
			 * 第一个字符用于接收机判断当前接收的数据是哪个类型的数据：
			 * 		D-----距离
			 * 		T-----温度
			 * 		I-----红外
			 * 从第二个字符开始即为实际发送数据
			 */

			//使用HC_SR04测距
			HC_SR04_Measure(&hc_sr04);
			LCD_Show();
			sprintf((char*) Tx_Buf, "D%.2f", hc_sr04.distance);
			while (NRF24L01_TxPacket(Tx_Buf) != TX_OK)
				;

			//使用18b20测温
			temperature = DS18B20_Get_Temperature();
			LCD_Show();
			sprintf((char*) Tx_Buf, "T%.2f", temperature);
			while (NRF24L01_TxPacket(Tx_Buf) != TX_OK)
				;

			//当红外传感器检测到人时，让LED0亮
			manCome = HC_SR501;
			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, !HC_SR501);
			LCD_Show();
			sprintf((char*) Tx_Buf, "I%d", manCome);
			while (NRF24L01_TxPacket(Tx_Buf) != TX_OK)
				;

			HAL_Delay(500);
		}
	} else if (mode == KEY0_PRES) {
		while (1) {
			/*
			 * 接收模式中，根据发送的格式对接收到的数据进行处理
			 * 首先提取第一个字符，用于判断当前数据是哪个类型的数据
			 * 然后从数据的第二位开始，即为传感器的检测数据
			 */
			if (NRF24L01_RxPacket(Rx_Buf) == 0) {
				//数组的最后加上'\0'，表示字符串结束
				Rx_Buf[31] = 0;
				//提取数据字符串，并将字符串转换成float或double或int
				if (Rx_Buf[0] == 'D') {
					hc_sr04.distance = atof((char*) (&Rx_Buf[1]));
				} else if (Rx_Buf[0] == 'T') {
					temperature = atoff((char*) (&Rx_Buf[1]));
				} else if (Rx_Buf[0] == 'I') {
					manCome = atoi((char*) (&Rx_Buf[1]));
					HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, !manCome);
					//若检测到有人，则开始计时
					if (manCome == 1) {
						time++;
					}
				}
			}
			LCD_Show();

			//若检测到有人、距离较近且长时间逗留，则报警(蜂鸣器响)
			if (manCome == 1 && time >= 25 && hc_sr04.distance <= 100) {
				BEEP_On();
			}
			//若此时人走了，则关闭蜂鸣器
			if (manCome == 0) {
				BEEP_Off();
				time = 0;
			}
			//按照要求，串口输出相关信息
			if (manCome == 0) {
				printf("附近没人。。。。\r\n");
			} else {
				printf("注意！有人靠近！！！！\r\n");
			}
			printf("当前测距结果为：%.2f cm\r\n", hc_sr04.distance);
			printf("当前温度为：%.2f ℃\r\n", temperature);
		}
	}

	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */

	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
// 须在头文件中添加 #include <stdio.h>
//////////////////////////////////////////////////////////////////
// 加入以下代码,支持printf函数,而不需要选择use MicroLIB
// MDK中同样定义了 __GNUC__，使用__ARMCC_VERSION 或  __CC_ARM 区分MDK和STM32CubeIDE
// MDK预定义宏，详见 ：https://www.keil.com/support/man/docs/armcc/armcc_chr1359125007083.htm
#ifdef __CC_ARM // 或 #ifdef __ARMCC_VERSION
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#else
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#endif
#if 1

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x) {
	x = x;
}
//重定义fputc函数
PUTCHAR_PROTOTYPE {
	// 具体哪个串口可以更改huart2为其它串口
	// HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1 , 0xffff);
	while ((USART1->SR & 0X40) == 0) {
	}; //循环发送,直到发送完毕
	USART1->DR = (uint8_t) ch;

//  while ((USART2->SR & 0X40) == 0) {
//  }; //循环发送,直到发送完毕
//  USART2->DR = (uint8_t) ch;

	return ch;
}
#endif

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
