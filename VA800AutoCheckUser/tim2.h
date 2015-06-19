#ifndef __TIM2_H
#define __TIM2_H

#include "stm8s.h"

void TIM2_Config(void);
void Delay_us(__IO uint32_t nTime);
void TIM2_TimingDelay_Decrement(void);


#endif /* __TIM2_H */