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
  GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_OUT_PP_HIGH_FAST);      //ָʾ��_1
  GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_IN_FL_NO_IT);          //��������
  GPIO_Init(GPIOB, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);          //����,�����Ӱ���
  GPIO_Init(GPIOB, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);          //���Ǽ��
  GPIO_Init(GPIOF, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);      //�����ӵ�Դ
  GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_HIGH_FAST);      //ָʾ��_2
  GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);      //����Ƶ�Դ
}
