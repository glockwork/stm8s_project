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

#define ENTER_CHOICE_FUNCTION           0       // Code���� Ĭ��״̬-ѡ����
#define ENTER_SELECTED_FUNCTION         1       // Code���� ����ȷ��
#define ENTER_START_WORK                2       // Code���� ��ʼ����״̬-��ʱ����
#define ENTER_PAUSE_WORK                3       // Code���� ��ͣ����״̬
#define ENTER_PREHEAT                   4       // Code���� Ԥ�ȹ���״̬

#define ON	        1               // ��
#define	OFF	        0               // ��
#define END             2               // ������־
#define PREHEAT_RING    3               // Ԥ�������ʾ��
unsigned char code_key_driver(void); 
void code_key_read(void);
void SelectedFunction(void);

#endif