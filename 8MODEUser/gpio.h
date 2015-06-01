#ifndef __GPIO_H
#define __GPIO_H

#include "stm8s.h"

//HT1621B Start 均带上拉
#define	SDA2		        GPIO_PIN_5		//GPIOB, DATA
#define	SCL2		        GPIO_PIN_4		//GPIOB, WR
#define	CS			GPIO_PIN_3		//GPIOF
#define	RD			GPIO_PIN_6		//GPIOF
#define	IQR			GPIO_PIN_4		//GPIOF
//HT1621B End

//DC_IN to 5V NC?
#define	ST1			GPIO_PIN_0		//GPIOF

//TVOC传感器 VRL 1/2Voc
#define	VO1			GPIO_PIN_6		//GPIOB

//温湿度传感器 IIC 硬件
#define	SDA			GPIO_PIN_2		//GPIOE
#define	SCL			GPIO_PIN_1		//GPIOE

//RTC IIC GPIO模拟
#define	SDA3		        GPIO_PIN_6		//GPIOC
#define	SCL3		        GPIO_PIN_7		//GPIOC
//RTC INT GPIO	H: interrupt L: no interrupt
#define	PO			GPIO_PIN_7		//GPIOD

//显示屏背光 GPIO 带下拉, H: 1, L: 0
#define	LED_RED	                GPIO_PIN_2		//GPIOB
#define	LED_GREEN	        GPIO_PIN_3		//GPIOB
#define	LED_BLUE		GPIO_PIN_3		//GPIOC

#define RED                     3
#define BLUE                    2
#define GREEN                   1
#define BACKLIT_OFF             0

#define LED_RED_ON              GPIO_WriteHigh(GPIOB, GPIO_PIN_2)
#define LED_RED_OFF             GPIO_WriteLow(GPIOB, GPIO_PIN_2)
#define LED_BLUE_ON             GPIO_WriteHigh(GPIOC, GPIO_PIN_3)
#define LED_BLUE_OFF            GPIO_WriteLow(GPIOC, GPIO_PIN_3)
#define LED_GREEN_ON            GPIO_WriteHigh(GPIOB, GPIO_PIN_3)
#define LED_GREEN_OFF           GPIO_WriteLow(GPIOB, GPIO_PIN_3)
#define POWER_SWITCH_ON         GPIO_WriteHigh(GPIOG, GPIO_PIN_4)
#define POWER_SWITCH_OFF        GPIO_WriteLow(GPIOG, GPIO_PIN_4)

//颗粒物传感器 VO	1/2 Voc
#define	VO			GPIO_PIN_7		//GPIOE

//电池检测	1/2 Voc
#define	VBAT_ADC	        GPIO_PIN_6		//GPIOE

//按键x3	带上拉。
#define	K_OUT1		        GPIO_PIN_5		//GPIOE
#define	K_OUT2		        GPIO_PIN_5		//GPIOC
#define	K_OUT3		        GPIO_PIN_4		//GPIOC

//功能未知
#define	HFD23		        GPIO_PIN_1		//GPIOC

//蜂鸣器 GPIO	H: 0, L:1
#define	BEEP_OUT	        GPIO_PIN_0		//GPIOG
#define BEEP_WORK               GPIO_WriteReverse(GPIOG, GPIO_PIN_0)
#define BEEP_OFF                GPIO_WriteHigh(GPIOG, GPIO_PIN_0)

//LED BLUE？ H: 1, L:0
#define	LED			GPIO_PIN_2		//GPIOD

#define POWER_SWITCH            GPIO_PIN_4              //GPIOG 电源控制IO
#define DC_INPUT_CHECK          GPIO_PIN_3              //GPIOG DC输入检测

void GPIO_Config_Init(void);
void LED_Backlit(uint8_t color);

#endif