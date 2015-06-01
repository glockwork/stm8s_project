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
  GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_OUT_PP_HIGH_FAST);      //指示灯_1
  GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_IN_FL_NO_IT);          //消毒按键
  GPIO_Init(GPIOB, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);          //风扇,负离子按键
  GPIO_Init(GPIOB, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);          //开盖检测
  GPIO_Init(GPIOF, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);      //负离子电源
  GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_HIGH_FAST);      //指示灯_2
  GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);      //紫外灯电源
}
