#include "touch.h"

TouchTypeDef TouchData;         //定义用来存储读取到的数据
static PosTypeDef TouchAdj;     //定义一阵数据用来保存校正因数

void TOUCH_Init(void)
{
  /* 使用EEPROM来存储校正参数，所以注意之前要初始化 */
  /* 检测是否有校正参数 */
	HAL_I2C_Mem_Read(&hi2c1,ADDR_24CXX_READ,TOUCH_ADJ_ADDR,I2C_MEMADD_SIZE_8BIT,&TouchAdj.posState,sizeof(TouchAdj),0xff);
 	if(TouchAdj.posState != TOUCH_ADJ_OK)
  {
		TOUCH_Adjust(); //校正
  }
}

uint8_t WR_Cmd(uint8_t cmd)
{
	uint8_t Tx_DATA = cmd;
	uint8_t Rx_DATA = 0;
	HAL_SPI_TransmitReceive(&hspi1,&Tx_DATA,&Rx_DATA,1,500);
	
	return Rx_DATA;
}

//读取X轴或y轴的ADC值
uint16_t TOUCH_Read_AD(uint8_t cmd)
{
  uint8_t i, j;
	uint16_t NUMH,NUML;
	uint16_t NUM[TOUCH_READ_TIMES] = {0};
	uint16_t temp,value;
	uint32_t totalValue;
	
	/* 读取TOUCH_READ_TIMES次触摸值 */
	for(i=0; i<TOUCH_READ_TIMES; i++)
	{
		TOUCH_CS_LOW();	/* 打开片选 */
		
		WR_Cmd(cmd);
		//HAL_Delay(1);	
		NUMH = WR_Cmd(0XFF);
		NUML = WR_Cmd(0XFF);
		NUM[i] = (NUMH<<8)+ NUML;
		NUM[i] >>= 3;		//高12位有效
		//printf("NUM[%d] = %d\r\n",i,NUM[i]); 
		TOUCH_CS_HIGH();	/*关闭片选 */
	}

    /* 滤波处理 */
    /* 首先从大到小排序 */
	for(i=0; i<(TOUCH_READ_TIMES - 1); i++)
  {
		for(j=i+1; j<TOUCH_READ_TIMES; j++)
    {
			/* 采样值从大到小排序排序 */
      if(NUM[i] < NUM[j])
      {
        temp = NUM[i];
				NUM[i] = NUM[j];
				NUM[j] = temp;
      }   
    }       
	}
   
  /* 去掉最大值，去掉最小值，求平均值 */
  j = TOUCH_READ_TIMES - 1;
  totalValue = 0;
  for(i=1; i<j; i++){     //求y的全部值
    totalValue += NUM[i];
  }
	//printf("totalValue = %d\r\n",totalValue); 
  value = totalValue/(TOUCH_READ_TIMES - 2);
  //printf("value = %d\r\n",value);   
  return value;
}


uint8_t TOUCH_ReadXY(uint16_t *xValue, uint16_t *yValue)
{   
    uint16_t xValue1, yValue1, xValue2, yValue2;

    xValue1 = TOUCH_Read_AD(TOUCH_X_CMD);
    yValue1 = TOUCH_Read_AD(TOUCH_Y_CMD);
    xValue2 = TOUCH_Read_AD(TOUCH_X_CMD);
    yValue2 = TOUCH_Read_AD(TOUCH_Y_CMD);
    
    /* 查看两个点之间的只采样值差距 */
    if(xValue1 > xValue2)
    {
        *xValue = xValue1 - xValue2;
    }
    else
    {
        *xValue = xValue2 - xValue1;
    }

    if(yValue1 > yValue2)
    {
        *yValue = yValue1 - yValue2;
    }
    else
    {
        *yValue = yValue2 - yValue1;
    }
	
    /* 判断采样差值是否在可控范围内 */
	if((*xValue > TOUCH_MAX+0) || (*yValue > TOUCH_MAX+0))  
	{
		return 0xFF;
	}

    /* 求平均值 */
    *xValue = (xValue1 + xValue2) / 2;
    *yValue = (yValue1 + yValue2) / 2;

    /* 判断得到的值，是否在取值范围之内 */
    if((*xValue > TOUCH_X_MAX+0) || (*xValue < TOUCH_X_MIN) 
       || (*yValue > TOUCH_Y_MAX+0) || (*yValue < TOUCH_Y_MIN))
    {                   
        return 0xFF;
    }
 
    return 0; 
}

uint8_t TOUCH_ReadAdjust(uint16_t x, uint16_t y, uint16_t *xValue, uint16_t *yValue)
{
    uint8_t i;
    uint32_t timeCont;

    /* 读取校正点的坐标 */
    LCD_Clear(BACK_COLOR);
    LCD_DrowSign(x, y, RED);
    i = 0;
    while(1)
    {
        if(!TOUCH_ReadXY(xValue, yValue))
        {
            i++;
            if(i > 10)         //延时一下，以读取最佳值
            {
                LCD_DrowSign(x, y, BACK_COLOR);
                return 0;
            }
               
        }
		timeCont++;
        /* 超时退出 */
        if(timeCont > 0xFFFFFFFE)
        {   
                LCD_DrowSign(x, y, BACK_COLOR); 
                return 0xFF;
        } 
    }       
}

void TOUCH_Adjust(void)
{
    uint16_t px[2], py[2], xPot[4], yPot[4];
    float xFactor, yFactor;

    /* 读取第一个点 */
    if(TOUCH_ReadAdjust(LCD_ADJX_MIN, LCD_ADJY_MIN, &xPot[0], &yPot[0]))
    {
        return;
    }   
    HAL_Delay(500);

    /* 读取第二个点 */
    if(TOUCH_ReadAdjust(LCD_ADJX_MIN, LCD_ADJY_MAX, &xPot[1], &yPot[1]))
    {
        return;
    }   
		HAL_Delay(500);

    /* 读取第三个点 */
    if(TOUCH_ReadAdjust(LCD_ADJX_MAX, LCD_ADJY_MIN, &xPot[2], &yPot[2]))
    {
        return;
    }   
    HAL_Delay(500);

    /* 读取第四个点 */
    if(TOUCH_ReadAdjust(LCD_ADJX_MAX, LCD_ADJY_MAX, &xPot[3], &yPot[3]))
    {
        return;
    }   
    HAL_Delay(500);
    
    /* 处理读取到的四个点的数据，整合成对角的两个点 */
    px[0] = (xPot[0] + xPot[1]) / 2;
    py[0] = (yPot[0] + yPot[2]) / 2;
    px[1] = (xPot[3] + xPot[2]) / 2;
    py[1] = (yPot[3] + yPot[1]) / 2;

    /* 求出比例因数 */
    xFactor = (float)LCD_ADJ_X / (px[1] - px[0]);
    yFactor = (float)LCD_ADJ_Y / (py[1] - py[0]);  
    
    /* 求出偏移量 */
    TouchAdj.xOffset = (int16_t)LCD_ADJX_MAX - ((float)px[1] * xFactor);
    TouchAdj.yOffset = (int16_t)LCD_ADJY_MAX - ((float)py[1] * yFactor);

    /* 将比例因数进行数据处理，然后保存 */
    TouchAdj.xFactor = xFactor ;
    TouchAdj.yFactor = yFactor ;
    printf("TouchAdj.xOffset = %d\r\n",TouchAdj.xOffset);
		printf("TouchAdj.yOffset = %d\r\n",TouchAdj.yOffset);
		printf("TouchAdj.xFactor = %f\r\n",TouchAdj.xFactor);
		printf("TouchAdj.yFactor = %f\r\n",TouchAdj.yFactor);
		
    TouchAdj.posState = TOUCH_ADJ_OK;
   	HAL_I2C_Mem_Write(&hi2c1,ADDR_24CXX_WRITE,TOUCH_ADJ_ADDR,I2C_MEMADD_SIZE_8BIT,&TouchAdj.posState,sizeof(TouchAdj),0xff);
}


uint8_t TOUCH_Scan(void)
{
    
    //if(TOUCH_PEN == 0)   //查看是否有触摸
    //{
        if(TOUCH_ReadXY(&TouchData.x, &TouchData.y)) //没有触摸
        {
            return 0xFF;    
        }
        /* 根据物理坐标值，计算出彩屏坐标值 */
        TouchData.lcdx = TouchData.x * TouchAdj.xFactor + TouchAdj.xOffset;
        TouchData.lcdy = TouchData.y * TouchAdj.yFactor + TouchAdj.yOffset;
        //printf("TouchData.lcdx = %d\r\n",TouchData.lcdx);
				//printf("TouchData.lcdy = %d\r\n",TouchData.lcdy);
        /* 查看彩屏坐标值是否超过彩屏大小 */
        if(TouchData.lcdx > lcddev.width)
        {
            TouchData.lcdx = lcddev.width;
        }
        if(TouchData.lcdy > lcddev.height)
        {
            TouchData.lcdy = lcddev.height;
        }
        return 0; 
    //}
		
		//return 0xFF;       
}

