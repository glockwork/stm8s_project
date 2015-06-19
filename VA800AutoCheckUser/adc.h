#ifndef __ADC_H
#define __ADC_H

#include "stm8s.h"
#include "tim4.h"
#include "tim2.h"
#include "gpio.h"

#define CONVERSIONMODE   0
#define CONVERSIONMODE_SINGLE  0
#define CONVERSIONMODE_CONTINUOUS  1

void ADC_Config(void);
uint16_t Start_Channel_Converion(ADC2_Channel_TypeDef channel);
uint16_t Get_ADC_Value(ADC2_Channel_TypeDef channel);
uint16_t Get_DS_Value(void);

#endif /* __ADC_H */