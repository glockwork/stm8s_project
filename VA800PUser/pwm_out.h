#ifndef __PWM_OUT_H
#define __PWM_OUT_H

#include "stm8s.h"

#define LED_8_PCS  0
#define CCR1_Val  ((uint16_t)0)
#define CCR2_Val  ((uint16_t)0)       
#define CCR3_Val  ((uint16_t)0)  
#define SPEED_HIGH      999
#define SPEED_LOW       600
#define SPEED_OFF       0
#define FAN_SPEED_HIGH          TIM1_SetCompare1(SPEED_HIGH)
#define FAN_SPEED_LOW           TIM1_SetCompare1(SPEED_LOW)
#define FAN_SPEED_OFF           TIM1_SetCompare1(SPEED_OFF)


void PWM_Config(void);
void LED_PWM_Show(uint8_t nPort, uint16_t nCCR_Val);

#endif