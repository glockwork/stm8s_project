#include "gpio.h"
#include "main.h"


/*
����: void GPIO_Init(void)
����: GPIO��ʼ��
�β�: ��
����ֵ����
*/ 
void GPIO_Config_Init(void)
{
  GPIO_Init(GPIOB, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);      //Green
  GPIO_Init(GPIOB, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);      //Red
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);      //Blue
  GPIO_Init(GPIOG, GPIO_PIN_0, GPIO_MODE_OUT_PP_HIGH_FAST);     //������
  GPIO_Init(GPIOG, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);      //��Դ����
  
  GPIO_Init(GPIOG, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);          //DC_input check
  GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);          //Key1
  GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);          //Key2
  GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);          //Key3
}

/*
����: void LED_Backlit(uint8_t color)
����: LED������ʾ��ɫ
�β�: color ������ɫ
����ֵ����
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