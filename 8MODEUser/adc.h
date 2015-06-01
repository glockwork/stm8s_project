#ifndef __ADC_H
#define __ADC_H

#include "stm8s.h"

#define DELAY_1 280
#define DELAY_2 40
#define DELAY_3 9600

//¿ÅÁ£Îï´«¸ÐÆ÷ LED GPIO H: 1, L: 0

#define GPCTRL_PORT     GPIOC
#define GPCTRL          GPIO_PIN_2

uint16_t MathData(uint16_t *Data);
uint16_t Get_BAT_Value(void);
uint16_t Get_TVOC_Value(void);
uint16_t Get_DS_Value(void);

#endif /* __ADC_H */