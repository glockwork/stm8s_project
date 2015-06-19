#ifndef __KEY_SCAN_H
#define __KEY_SCAN_H

#include "stm8s.h"

#define code_key_input	        GPIO_ReadInputPin(GPIOC, GPIO_PIN_2)


#define N_key           0             //�޼� 
#define S_key           1             //���� 
#define D_key           2             //˫�� 
#define L_key           3             //���� 

#define key_state_0     0 
#define key_state_1     1 
#define key_state_2     2 
#define key_state_3     3 

#define KEY_LONG_TIME   200             //2��
#define ENTER_DEFINE                    0       // Code����Ĭ��״̬
#define ENTER_SET_UP_DOWN_TEMP          1       // Code�����������¹��¶�״̬
#define ENTER_START_WORK                2       // Code������ʼ����״̬
#define ENTER_SET_TIME                  3       // Code�������ù���ʱ��״̬


#define ON	        1               //��
#define	OFF	        0               //��

unsigned char code_key_driver(void); 
void code_key_read(void);

#endif