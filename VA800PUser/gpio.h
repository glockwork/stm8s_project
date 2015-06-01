#ifndef __GPIO_H
#define __GPIO_H

#include "stm8s.h"
#include "main.h"

#define NET_POWER_ON	        GPIO_WriteHigh(GPIOB, GPIO_PIN_0)
#define NET_POWER_OFF	        GPIO_WriteLow(GPIOB, GPIO_PIN_0)
#define ANION_POWER_ON	        GPIO_WriteHigh(GPIOB, GPIO_PIN_6)
#define ANION_POWER_OFF	        GPIO_WriteLow(GPIOB, GPIO_PIN_6)
#define PM_POWER_ON	        GPIO_WriteHigh(GPIOC, GPIO_PIN_6)
#define PM_POWER_OFF	        GPIO_WriteLow(GPIOC, GPIO_PIN_6)
#define FAN_POWER_ON	        GPIO_WriteHigh(GPIOF, GPIO_PIN_5)
#define FAN_POWER_OFF	        GPIO_WriteLow(GPIOF, GPIO_PIN_5)
#define D12_POWER_ON	        GPIO_WriteHigh(GPIOB, GPIO_PIN_1)
#define D12_POWER_OFF	        GPIO_WriteLow(GPIOB, GPIO_PIN_1)

#define	NET_LED_ON		GPIO_WriteHigh(GPIOF, GPIO_PIN_7)
#define NET_LED_OFF		GPIO_WriteLow(GPIOF, GPIO_PIN_7)
#define NET_LED_FLASH           GPIO_WriteReverse(GPIOF, GPIO_PIN_7)
#define	FILTER_LED_ON		GPIO_WriteHigh(GPIOE, GPIO_PIN_4)
#define FILTER_LED_OFF		GPIO_WriteLow(GPIOE, GPIO_PIN_4)
#define FILTER_LET_FLASH        GPIO_WriteReverse(GPIOE, GPIO_PIN_4)

/*
����: void GPIO_Init(void)
����: GPIO��ʼ��
�β�: ��
����ֵ����
*/ 
void GPIO_Config_Init(void);

/*
����: void DeviceWork(void)
����: ���豸��ģ���Դ
�β�: ��
����ֵ����
*/ 
void DeviceWork(void);

/*
����: void DeviceWait(void)
����: �ر��豸��ģ���Դ
�β�: ��
����ֵ����
*/ 
void DeviceWait(void);
#endif