#ifndef __RT8563_H
#define __RT8563_H

#include "stm8s.h"
#include "gpio2iic.h"

#define TYPE    1
#define VA800   0
#define MODE_8  1


//RTC IIC GPIO模拟
#if (TYPE == VA800)
  #define       RTC_ROPT	GPIOE
  #define	RTC_SDA_PIN	GPIO_PIN_2		//GPIOE
  #define	RTC_SCL_PIN	GPIO_PIN_1		//GPIOE
#endif
#if (TYPE == MODE_8)
  #define       RTC_ROPT	GPIOC
  #define	RTC_SDA_PIN	GPIO_PIN_6		//GPIOC
  #define	RTC_SCL_PIN	GPIO_PIN_7		//GPIOC
#endif

static unsigned char TimeData[7] = {0, 0, 0, 1, 4, 1, 15};//【0秒, 0分, 0时, 1日, 星期4, 1月, 15年】

unsigned char BCD2Dec(unsigned char bcd);
unsigned char Dec2BCD(unsigned char dec);

void Set_RT8563(GPIO_TypeDef* IIC_PORT,
                GPIO_Pin_TypeDef SDA_PIN,
                GPIO_Pin_TypeDef SCL_PIN,
                uint8_t *pData,
                uint8_t index,
                uint8_t NumByteToWrite);
void Get_RT8563(GPIO_TypeDef* IIC_PORT,
                GPIO_Pin_TypeDef SDA_PIN,
                GPIO_Pin_TypeDef SCL_PIN,
                uint8_t *pData,
                uint8_t index,
                uint8_t NumByteToRead);

#endif