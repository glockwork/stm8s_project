#ifndef __KEY_SCAN_H
#define __KEY_SCAN_H

#include "stm8s.h"

#define code_key_input	        GPIO_ReadInputPin(GPIOC, GPIO_PIN_2)


#define N_key           0             //无键 
#define S_key           1             //单键 
#define D_key           2             //双键 
#define L_key           3             //长键 

#define key_state_0     0 
#define key_state_1     1 
#define key_state_2     2 
#define key_state_3     3 

#define KEY_LONG_TIME   200             //2秒

#define ENTER_CHOICE_FUNCTION           0       // Code按键 默认状态-选择功能
#define ENTER_SELECTED_FUNCTION         1       // Code按键 功能确认
#define ENTER_START_WORK                2       // Code按键 开始工作状态-计时加热
#define ENTER_PAUSE_WORK                3       // Code按键 暂停工作状态
#define ENTER_PREHEAT                   4       // Code按键 预热工作状态

#define ON	        1               //开
#define	OFF	        0               //关

unsigned char code_key_driver(void); 
void code_key_read(void);
void SelectedFunction(void);

#endif