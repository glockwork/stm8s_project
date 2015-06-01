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

#define DEVICE_SWITCH_MASK           0x0001          //设备开关状态

typedef struct
{
    uint32_t    WorkStatus;             // 设备状态（开关、电源等的状态）
    uint8_t     RTC[7];                 // RTC时钟的时间
    uint8_t     DeviceID[12];           // 设备ID
    uint32_t    SieveRunTime;           // 滤网使用时间
    uint8_t     FanMode;                 // 风扇运行模式，高、低、自动
    uint8_t     Battery;                // 电池电量
    uint16_t    AQI;                    // 空气质量
    uint8_t     Time_1_s;               // 1秒标志位
}DEVICE_STATUS;

static void CLK_Config(void);
#endif