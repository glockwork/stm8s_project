#ifndef __TIM4_H
#define __TIM4_H

#include "stm8s.h"

void TIM4_Config(void);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#endif /* __TIM4_H */