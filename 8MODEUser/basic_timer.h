#ifndef __BASEC_TIMER_H
#define __BASEC_TIMER_H

#include "stm8s.h"

void BasicTimer_Init(void);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#endif