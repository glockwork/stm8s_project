#ifndef __GPIO_H
#define __GPIO_H

#include "stm8s.h"
#include "main.h"

#define RELAY_1_H	                GPIO_WriteHigh(GPIOF, GPIO_PIN_3)
#define RELAY_1_L	                GPIO_WriteLow(GPIOF, GPIO_PIN_3)

#define RELAY_2_H	                GPIO_WriteHigh(GPIOF, GPIO_PIN_4)
#define RELAY_2_L	                GPIO_WriteLow(GPIOF, GPIO_PIN_4)

#define RELAY_3_H	                GPIO_WriteHigh(GPIOF, GPIO_PIN_5)
#define RELAY_3_L	                GPIO_WriteLow(GPIOF, GPIO_PIN_5)

#define RELAY_4_H	                GPIO_WriteHigh(GPIOF, GPIO_PIN_6)
#define RELAY_4_L	                GPIO_WriteLow(GPIOF, GPIO_PIN_6)

#define RELAY_5_H	                GPIO_WriteHigh(GPIOF, GPIO_PIN_7)
#define RELAY_6_L	                GPIO_WriteLow(GPIOF, GPIO_PIN_7)

/*
名称: void GPIO_Init(void)
功能: GPIO初始化
形参: 无
返回值：无
*/ 
void GPIO_Config_Init(void);


#endif