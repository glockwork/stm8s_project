#ifndef __KEY_SCAN_H
#define __KEY_SCAN_H

#include "stm8s.h"
#include "main.h"

#define reset_key_input       GPIO_ReadInputPin(GPIOE, GPIO_PIN_0)    // 按键输入口 
#define touch_key_input       GPIO_ReadInputPin(GPIOE, GPIO_PIN_5)    // 按键输入口 

#define N_key           0             //无键 
#define S_key           1             //单键 
#define D_key           2             //双键 
#define L_key           3             //长键 

#define key_state_0     0 
#define key_state_1     1 
#define key_state_2     2 
#define key_state_3     3 

#define TOUCH_L_TIME    100             //3秒
#define RESET_L_TIME    500             //5秒

unsigned char reset_key_driver(void); 
unsigned char touch_key_driver(void);
void TouchKey_Read(void);
void ResetKey_Read(void);

#endif