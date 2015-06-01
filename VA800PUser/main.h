#ifndef __MAIN_H
#define __MAIN_H

#include "uart1.h"
#include "uart3.h"
#include "gpio.h"
#include "tim2.h"
#include "tim3.h"
#include "tim4.h"
#include "alarm_function.h"
#include "net_function.h"
#include "pwm_out.h"
#include "rt8563.h"
#include "key_scan.h"
#include "adc.h"

#define DEVICE_SWITCH_MASK           0x0001          //�豸����״̬

typedef struct
{
    uint32_t    WorkStatus;             // �豸״̬�����ء���Դ�ȵ�״̬��
    uint8_t     RTC[7];                 // RTCʱ�ӵ�ʱ��
    uint8_t     DeviceID[12];           // �豸ID
    uint32_t    SieveRunTime;           // ����ʹ��ʱ��
    uint8_t     FanMode;                 // ��������ģʽ���ߡ��͡��Զ�
    uint8_t     Battery;                // ��ص���
    uint16_t    AQI;                    // ��������
    uint8_t     Time_1_s;               // 1���־λ
}DEVICE_STATUS;

static void CLK_Config(void);
#endif