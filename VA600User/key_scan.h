#ifndef __KEY_SCAN_H
#define __KEY_SCAN_H

#include "stm8s.h"

#define openings_check_input	        GPIO_ReadInputPin(GPIOB, GPIO_PIN_3)
#define feng_key_input	                GPIO_ReadInputPin(GPIOB, GPIO_PIN_2)
#define disinfect_key_input	        GPIO_ReadInputPin(GPIOB, GPIO_PIN_1)


#define N_key           0             //�޼� 
#define S_key           1             //���� 
#define D_key           2             //˫�� 
#define L_key           3             //���� 

#define key_state_0     0 
#define key_state_1     1 
#define key_state_2     2 
#define key_state_3     3 

#define KEY_LONG_TIME   200             //2��


#define ON	        1               //��
#define	OFF	        0               //��

void openings_check(void); 
unsigned char feng_key_driver(void);
unsigned char disinfect_key_driver(void);

void feng_key_read(void);
void disinfect_key_read(void);

#endif