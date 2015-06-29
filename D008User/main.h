#ifndef __MAIN_H
#define __MAIN_H

#include "uart1.h"
#include "gpio.h"
#include "uart1.h"
#include "uart3.h"
#include "basic_timer.h"
#include "key_scan.h"
#include "beep.h"
#include "function.h"
#include "net_function.h"




#define DEVICE_SWITCH_MASK           0x0001          //�豸����״̬
#define STATE_AUTO_SET                  16              // �Ե�
#define STATE_HOT_AUTO_SET              15              // Ԥ���Ե�
#define STATE_FOOD_A                    14              // ����
#define STATE_FOOD_B                    13              // ����
#define STATE_FOOD_C                    12              // ����
#define STATE_FOOD_D                    11              // ����
#define STATE_FOOD_E                    10              // ��̢
#define STATE_FOOD_F                    9               // ���
#define STATE_FOOD_G                    8               // ����
#define STATE_FOOD_H                    7               // ����
#define STATE_FOOD_I                    6               // �Ź�
#define STATE_FOOD_J                    5               // ����
#define STATE_FOOD_K                    4               // ����
#define STATE_FOOD_L                    3               // ����

#define HOT_UP                          2               // ���ȹ�
#define HOT_DOWN                        3               // ���ȹ�

#define SET_FUNCTION                    0               // ѡ����
#define SET_TIME                        1               // ����ʱ��
#define SET_TEMP                        2               // �����¶�

#define KNOB_DISABLE                    1               // ������ť

#define SHOW_TEMP                       0               // ��ʾ�¶�
#define SHOW_TIME                       1               // ��ʾʱ��

#define OK                              1               

static uint16_t Temperature[17] = {0, 0, 0, 195, 200, 210, 210, 210, 215, 160, 200, 155, 185, 150, 40, 170, 170};       // Ĭ���¶�ֵ�����϶�
static uint16_t Timing[17] = {0, 0, 0, 28, 60, 20, 28, 18, 40, 18, 20, 27, 16, 12, 50, 30, 30};      // Ĭ�Ϲ���ʱ�䣬����

typedef struct
{
    uint16_t    beep;                   // ����������
    uint8_t     beepSW;                 // ����������
    uint8_t     startWorkBeep;          // ��ʼ������ʾ������
    uint8_t     powerOnBeep;            // ������ʾ��
    uint8_t     hotUpDown;              // ���¹�״̬, 0�ϣ�1��
    uint8_t     up_Temperature;         // �Ϲ��¶�ֵ
    uint8_t     down_Temperature;       // �¹��¶�ֵ
    uint16_t    adc_UP;                 // �ϹܵĴ������¶�ֵ
    uint16_t    adc_DOWN;               // �¹ܵĴ������¶�ֵ 
    uint8_t     temperatureOK;          // �¶����趨���¶�һ�£� 1��һ��         
    uint16_t    workTime;               // ����ʱ��
    uint8_t     timeOut;                // ����ʱ�������־
    uint8_t     preheat;                // Ԥ��
    uint8_t     light;                  // ¯��
    uint8_t     workState;              // ����ѡ��״̬
    uint8_t     enterMode;              // Code ����ģʽ
    uint8_t     setMode;                // ����ģʽ��״̬��ʱ����¶�
    uint8_t     knob;                   // ��ť�Ƿ���ã� 1������
    uint8_t     startWork;              // ��ʼ������־
    uint8_t     flashLight;             // ָʾ��������
    uint8_t     Time_30ms;              // 30ms��־λ
    uint8_t     Time_100ms;             // 100ms��־λ
    uint8_t     Time_1_s;               // 1���־λ
    uint8_t     Time_30_s;              // 30���־λ
}DEVICE_STATUS;



static void CLK_Config(void);
#endif