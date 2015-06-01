#ifndef __ADC_H
#define __ADC_H

#include "stm8s.h"
#include "main.h"

#define DELAY_1 280
#define DELAY_2 40
#define DELAY_3 9600

#define BAT_0   496     //3.2V
#define BAT_L   527     //3.4V
#define BAT_M   590     //3.8V
#define BAT_H   621     //4.0v

#define PM_0    1
#define PM_1    2
#define PM_2    3
#define PM_3    4


//¿ÅÁ£Îï´«¸ÐÆ÷ LED GPIO H: 1, L: 0
#define GPCTRL_PORT     GPIOD
#define GPCTRL          GPIO_PIN_4

uint16_t MathData(uint16_t *Data);
uint16_t Get_BAT_Value(void);
uint16_t Get_DS_Value(void);

#endif /* __ADC_H */