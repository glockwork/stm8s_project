#ifndef __GPIO_H
#define __GPIO_H

#include "stm8s.h"

#define GPRS_PW_PORT  (GPIOB)
#define GPRS_PW_PIN   (GPIO_PIN_0)      //GPRS��Դʹ��
#define BT_TO_12V_PORT  (GPIOB)
#define BT_TO_12V_PIN   (GPIO_PIN_1)    //���ת12Vʹ��
#define ANION_PORT      (GPIOB)
#define ANION_PIN       (GPIO_PIN_6)    //������ʹ��
#define DC_IN_PORT      (GPIOB)
#define DC_IN_PIN       (GPIO_PIN_7)    //DC������
#define DC_INPUT        1
#define DC_NO_IN        0

#define DS_PW_PORT      (GPIOC)
#define DS_PW_PIN       (GPIO_PIN_6)    //�ҳ���������Դʹ��

#define DS_PORT         (GPIOD)
#define DS_PIN          (GPIO_PIN_4)    //�ҳ�������LEDʹ��

#define LEDS_PORT (GPIOE)
#define LED1_PIN  (GPIO_PIN_4)          //����ָʾ��

#define FAN_PORT  (GPIOF)
#define FAN_PIN   (GPIO_PIN_5)          //���ȵ�Դʹ��


#define CAP_PORT (GPIOD)                
#define CAP_PIN  (GPIO_PIN_0)           //��������

#define RTC_INT_PORT    (GPIOE)
#define RTC_INT_PIN     (GPIO_PIN_3)    //RTC��ʱ�ж����

#define TOUCH_PORT      (GPIOE)
#define TOUCH_PIN       (GPIO_PIN_5)    //��������

void GPIO_Config(void);
void GPIO_Config(void);

#endif /*__GPIO_H */