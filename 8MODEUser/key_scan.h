#ifndef __KEY_SCAN_H
#define __KEY_SCAN_H

#include "stm8s.h"

#define power_key_input	        GPIO_ReadInputPin(GPIOE, GPIO_PIN_5)
#define backlit_key_input	GPIO_ReadInputPin(GPIOC, GPIO_PIN_5)
#define beep_key_input	        GPIO_ReadInputPin(GPIOC, GPIO_PIN_4)
#define dc_in_input	        GPIO_ReadInputPin(GPIOG, GPIO_PIN_3)

#define N_key           0             //�޼� 
#define S_key           1             //���� 
#define D_key           2             //˫�� 
#define L_key           3             //���� 

#define key_state_0     0 
#define key_state_1     1 
#define key_state_2     2 
#define key_state_3     3 

#define KEY_LONG_TIME   200             //2��

#define SETMODE		1               //���ü�ģʽ
#define POWERMODE	0               //��Դ��ģʽ

#define SET_MINUTE	1               //���÷���
#define SET_HOUR	2               //����Сʱ
#define SET_DAY         3               //������(��)
#define SET_WEEK        4               //��������
#define SET_MONTH       5               //�����·�
#define SET_YEAR        6               //������

#define ON	        1               //��
#define	OFF	        0               //��

unsigned char power_key_driver(void); 
unsigned char backlit_key_driver(void);
unsigned char beep_key_driver(void);
void DC_IN_CHECK_driver(void);
void power_key_read(void);
void backlit_key_read(void);
void beep_key_read(void);

#endif