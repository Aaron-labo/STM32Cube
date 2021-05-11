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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "vol_temp.h"
#include "PID.h"
#include "malloc.h"
#include "LCD_IRMP.h"
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

uint16_t adc_buffer[20] = { 0 }; //ADC采集数据缓冲区
uint16_t temp_vol = 0;
float voltage = 0; //存放采样的电压值
float temperature;
float errTemp;
float show_voltage;
int ChaVoltage;
uint8_t i = 0;
uint8_t Voltage_Str[] = "Voltage value is:";
uint8_t Temperature_Str[] = "Temperature is:";
uint8_t Show_Str[64];

float pwmVal_fan = 0; //调节输出PWM波的占空比，0~1000,当设为500时占空比为50%
float pwmVal_res = 10; //调节输出PWM波的占空比，0~1000,当设为500时占空比为50%
float ChaPWM;  //PWM占空比的变换值

float target_temp = 40;

uint8_t mode = 0;
float LastTemp = 0;
static uint8_t isUp = 1;

/*定义风扇和水泥电阻调节PID结构体
 * 需要注意的是：定义结构体指针时需要为其申请内存空间，否则程序会跑飞
 * （将会在main函数中为其申请空间）
 */
PID *pid_res;

uint8_t Rx_Date; //接受串口输入

//DMA回调函数
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc1.Instance == ADC1) {  //判断是否为ADC1的模拟输入
		//软件低通滤波(但是效果并不理想)
		for (i = 0; i < 19; i++) {
			temp_vol = adc_buffer[i];
			if (temp_vol > adc_buffer[i + 1]) {
				adc_buffer[i] = adc_buffer[i + 1];
				adc_buffer[i + 1] = temp_vol;
			}
		}
		voltage = 0;
		for (i = 8; i < 14; i++) {
			voltage += ((float) adc_buffer[i]) * 3.3 / 4096;
		}
		voltage /= 6;

		//根据电压——温度转化函数得到对应温度值
		temperature = conversion1(voltage);
		//通过PID调节水泥电阻占空比
		if (mode == 1) {
			if (LastTemp != 0
					&& (temperature - LastTemp >= 1
							|| LastTemp - temperature >= 1)) {
				if (isUp == 0) {
					isUp = 1;
				} else {
					isUp = 0;
				}
			}
			if (isUp == 1) {
				temperature = conversion1(voltage);
				//printf("采用上升温度。。。。\r\n");
			} else if (isUp == 0) {
				temperature = conversion2(voltage);
				//printf("采用下降温度。。。。\r\n");
			}

			if (temperature <= pid_res->SetTemp - 10) {
				pwmVal_res = 700;
				__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, pwmVal_res);
			} else {
				pwmVal_res = PID_Calc(pid_res, temperature);
				__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, pwmVal_res);
//				printf("\r\n水泥电阻电压PWM的当前占空比为%.2f\r\n", pwmVal_res / 1000);
			}

			pwmVal_fan = voltage * 272.7 - 263.56; //电压为1.7V是占空比为0.20;电压为2.8V时占空比为0.50
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, pwmVal_fan);
		} else if (mode == 2) {
			temperature = conversion2(voltage);
			if (temperature >= pid_res->SetTemp-2) {
				pwmVal_fan = 1000;
				__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, pwmVal_fan);
				pwmVal_res = 10;
				__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, pwmVal_res);
			} else {
				mode = 1;
			}
		} else {
			pwmVal_fan = voltage * 272.7 - 263.56; //电压为1.7V是占空比为0.20;电压为2.8V时占空比为0.50
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, pwmVal_fan);
		}

		//LCD输出相关信息
		show_Data(voltage, temperature, target_temp, pwmVal_fan, pwmVal_res);

		//当温度达到60±1℃时，LED1亮；达到40±1℃时，LED0亮；其他时候灭
		if (temperature >= 59 && temperature <= 61) {
			LED0_On();
		} else if (temperature >= 39 && temperature <= 41) {
			LED1_On();
		} else {
			LED0_Off();
			LED1_Off();
		}
	}
}

//USART接收回调函数
//在调试阶段，使用串口调节占空比
/* 将目的温度值调整到任意位置  0x**  (“**”为目的温度值的十六进制形式)
 *   开始升温  0xa1    开始降温 0xa2    停止调温  0xa3
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		if (Rx_Date <= 0xa0) {
			target_temp = Rx_Date;
			ChaSetTemp(pid_res, target_temp);
			HAL_UART_Receive_IT(&huart1, &Rx_Date, 1);
			printf("当前温度目标值为 %.2f\r\n", target_temp);
		} else {
			if (Rx_Date == 0xa1) {
				mode = 1;
				PID_inte_Init(pid_res);
				printf("\r\n开始升温！！！温度升至%.2f\r\n", target_temp);
				HAL_UART_Receive_IT(&huart1, &Rx_Date, 1);
			} else if (Rx_Date == 0xa2) {
				mode = 2;
				PID_inte_Init(pid_res);
				printf("\r\n开始降温！！！温度降至%.2f\r\n", target_temp);
				HAL_UART_Receive_IT(&huart1, &Rx_Date, 1);
			} else if (Rx_Date == 0xa3) {
				mode = 0;
				printf("停止调温！！！");
				HAL_UART_Receive_IT(&huart1, &Rx_Date, 1);
			}
//			else if (Rx_Date == 0xb1) {
//				pwmVal_res += 10;
//				__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, pwmVal_res);
//				printf("水泥电阻的占空比增加了0.01, 当前水泥电阻占空比为%.2f", pwmVal_fan / 1000);
//				HAL_UART_Receive_IT(&huart1, &Rx_Date, 1);
//			} else if (Rx_Date == 0xb2) {
//				pwmVal_res -= 10;
//				__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, pwmVal_res);
//				printf("水泥电阻的占空比减小了0.01, 当前水泥电阻占空比为%.2f", pwmVal_fan / 1000);
//				HAL_UART_Receive_IT(&huart1, &Rx_Date, 1);
//			}
		}
		show_Data(voltage, temperature, target_temp, pwmVal_fan, pwmVal_res);
	}
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
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_TIM3_Init();
	MX_USART1_UART_Init();
	MX_TIM2_Init();
	MX_FSMC_Init();
	MX_TIM4_Init();
	/* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim2); // 开启计时器
	HAL_TIM_Base_Start_IT(&htim3); // 开启计时器
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //开启PWM波输出
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); //开启PWM波输出2
	HAL_UART_Receive_IT(&huart1, &Rx_Date, 1); //开启串口接受中断
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) &adc_buffer, 20); //DMA方式开启ADC

	//分别为风扇和水泥电阻的结构体申请内存空间
	pid_res = (PID*) malloc(sizeof(PID));

	PID_Init(pid_res); //初始化PID结构体
	LCD_Init(); //LCD显示屏初始化
	irmp_init(); //初始化红外遥控

	printf("初始化完成。。。\r\n");
	//LCD显示各项数据
	show_Data(voltage, temperature, target_temp, pwmVal_fan, pwmVal_res);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
//		IRMP_Scan(0, &mode);
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
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

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
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
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
