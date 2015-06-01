#ifndef __GPIO_H
#define __GPIO_H

#include "stm8s.h"

#define LED_1_ON              GPIO_WriteLow(GPIOB, GPIO_PIN_0)
#define LED_1_OFF             GPIO_WriteHigh(GPIOB, GPIO_PIN_0)
#define LED_1_FLASH           GPIO_WriteReverse(GPIOB, GPIO_PIN_0)
#define LED_2_ON              GPIO_WriteLow(GPIOD, GPIO_PIN_0)
#define LED_2_OFF             GPIO_WriteHigh(GPIOD, GPIO_PIN_0)
#define LED_2_FLASH           GPIO_WriteReverse(GPIOD, GPIO_PIN_0)

#define FLZ_POWER_ON          GPIO_WriteHigh(GPIOF, GPIO_PIN_4)
#define FLZ_POWER_OFF         GPIO_WriteLow(GPIOF, GPIO_PIN_4)

#define ZWD_POWER_ON          GPIO_WriteHigh(GPIOD, GPIO_PIN_2)
#define ZWD_POWER_OFF         GPIO_WriteLow(GPIOD, GPIO_PIN_2)


void GPIO_Config_Init(void);

#endif