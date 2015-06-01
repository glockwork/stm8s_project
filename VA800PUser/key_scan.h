#ifndef __KEY_SCAN_H
#define __KEY_SCAN_H

#include "stm8s.h"
#include "main.h"

#define reset_key_input       GPIO_ReadInputPin(GPIOE, GPIO_PIN_0)    // ��������� 
#define touch_key_input       GPIO_ReadInputPin(GPIOE, GPIO_PIN_5)    // ��������� 

#define N_key           0             //�޼� 
#define S_key           1             //���� 
#define D_key           2             //˫�� 
#define L_key           3             //���� 

#define key_state_0     0 
#define key_state_1     1 
#define key_state_2     2 
#define key_state_3     3 

#define TOUCH_L_TIME    100             //3��
#define RESET_L_TIME    500             //5��

unsigned char reset_key_driver(void); 
unsigned char touch_key_driver(void);
void TouchKey_Read(void);
void ResetKey_Read(void);

#endif