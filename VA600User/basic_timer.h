#ifndef __BASEC_TIMER_H
#define __BASEC_TIMER_H

#include "stm8s.h"

#define TIM2_PERIOD       15624
#define TIM3_PERIOD       1249
#define CCR2_Val  ((uint16_t)0)       
#define CCR3_Val  ((uint16_t)0)  
#define SPEED_HIGH      999
#define SPEED_LOW       600
#define SPEED_OFF       0
#define FAN_1_SPEED_HIGH          TIM1_SetCompare3(SPEED_HIGH)
#define FAN_1_SPEED_LOW           TIM1_SetCompare3(SPEED_LOW)
#define FAN_1_SPEED_OFF           TIM1_SetCompare3(SPEED_OFF)

#define FAN_2_SPEED_HIGH          TIM1_SetCompare2(SPEED_HIGH)
#define FAN_2_SPEED_LOW           TIM1_SetCompare2(SPEED_LOW)
#define FAN_2_SPEED_OFF           TIM1_SetCompare2(SPEED_OFF)


void PWM_Config(void);
void BasicTimer_Init(void);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#endif