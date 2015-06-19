#ifndef __GPIO_H
#define __GPIO_H

#include "stm8s.h"

#define GPRS_PW_PORT  (GPIOB)
#define GPRS_PW_PIN   (GPIO_PIN_0)      //GPRS电源使能
#define BT_TO_12V_PORT  (GPIOB)
#define BT_TO_12V_PIN   (GPIO_PIN_1)    //电池转12V使能
#define ANION_PORT      (GPIOB)
#define ANION_PIN       (GPIO_PIN_6)    //负离子使能
#define DC_IN_PORT      (GPIOB)
#define DC_IN_PIN       (GPIO_PIN_7)    //DC输入检测
#define DC_INPUT        1
#define DC_NO_IN        0

#define DS_PW_PORT      (GPIOC)
#define DS_PW_PIN       (GPIO_PIN_6)    //灰尘传感器电源使能

#define DS_PORT         (GPIOD)
#define DS_PIN          (GPIO_PIN_4)    //灰尘传感器LED使能

#define LEDS_PORT (GPIOE)
#define LED1_PIN  (GPIO_PIN_4)          //滤网指示灯

#define FAN_PORT  (GPIOF)
#define FAN_PIN   (GPIO_PIN_5)          //风扇电源使能


#define CAP_PORT (GPIOD)                
#define CAP_PIN  (GPIO_PIN_0)           //霍尔开关

#define RTC_INT_PORT    (GPIOE)
#define RTC_INT_PIN     (GPIO_PIN_3)    //RTC定时中断输出

#define TOUCH_PORT      (GPIOE)
#define TOUCH_PIN       (GPIO_PIN_5)    //触摸开关

void GPIO_Config(void);
void GPIO_Config(void);

#endif /*__GPIO_H */