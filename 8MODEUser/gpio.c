#include "gpio.h"
#include "main.h"


/*
名称: void GPIO_Init(void)
功能: GPIO初始化
形参: 无
返回值：无
*/ 
void GPIO_Config_Init(void)
{
  GPIO_Init(GPIOB, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);      //Green
  GPIO_Init(GPIOB, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);      //Red
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);      //Blue
  GPIO_Init(GPIOG, GPIO_PIN_0, GPIO_MODE_OUT_PP_HIGH_FAST);     //蜂鸣器
  GPIO_Init(GPIOG, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);      //电源控制
  
  GPIO_Init(GPIOG, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);          //DC_input check
  GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);          //Key1
  GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);          //Key2
  GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);          //Key3
}

/*
名称: void LED_Backlit(uint8_t color)
功能: LED背光显示颜色
形参: color 背光颜色
返回值：无
*/ 
void LED_Backlit(uint8_t color)
{
  if(color == BLUE)
  {
    LED_BLUE_ON;
    LED_RED_OFF;
    LED_GREEN_OFF;
  }
  else if(color == GREEN)
  {
    LED_BLUE_OFF;
    LED_RED_OFF;
    LED_GREEN_ON;
  }
  else if(color == RED)
  {
    LED_BLUE_OFF;
    LED_RED_ON;
    LED_GREEN_OFF;
  }
  else
  {
    LED_BLUE_OFF;
    LED_RED_OFF;
    LED_GREEN_OFF;
  }
}