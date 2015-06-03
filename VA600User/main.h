#ifndef __MAIN_H
#define __MAIN_H

#include "stm8s.h"
#include "basic_timer.h"
#include "iwdg.h"
#include "gpio.h"
#include "key_scan.h"

#define DEBUG   1

typedef struct
{
    uint8_t     T_30ms_Out;     // 30�볬ʱ��־
    uint16_t    T_6Min_Count;   // 6���Ӽ���
//    uint8_t     T_6Min_Out;     // 6���ӳ�ʱ��־
    uint8_t     Openings;       // ����״̬��־ 1���ϸ�
    uint8_t     Disinfect;      // ��������״̬��־
    uint8_t     Feng;           // ���ȹ���״̬��־
} DEVICE_STATUS;

#endif