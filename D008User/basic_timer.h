#ifndef __BASEC_TIMER_H
#define __BASEC_TIMER_H

#include "stm8s.h"
#include "main.h"

void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void TIM4_Config(void);
void TIM3_Config(void);
void TIM2_Config(void);

#endif