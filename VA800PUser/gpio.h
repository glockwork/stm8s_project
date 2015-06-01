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
名称: void GPIO_Init(void)
功能: GPIO初始化
形参: 无
返回值：无
*/ 
void GPIO_Config_Init(void);

/*
名称: void DeviceWork(void)
功能: 打开设备的模块电源
形参: 无
返回值：无
*/ 
void DeviceWork(void);

/*
名称: void DeviceWait(void)
功能: 关闭设备的模块电源
形参: 无
返回值：无
*/ 
void DeviceWait(void);
#endif