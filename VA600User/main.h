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
    uint8_t     T_30ms_Out;     // 30秒超时标志
    uint16_t    T_6Min_Count;   // 6分钟计数
//    uint8_t     T_6Min_Out;     // 6分钟超时标志
    uint8_t     Openings;       // 开盖状态标志 1：合盖
    uint8_t     Disinfect;      // 消毒工作状态标志
    uint8_t     Feng;           // 风扇工作状态标志
} DEVICE_STATUS;

#endif