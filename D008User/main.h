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




#define DEVICE_SWITCH_MASK           0x0001          //设备开关状态
#define STATE_AUTO_SET                  16              // 自调
#define STATE_HOT_AUTO_SET              15              // 预热自调
#define STATE_FOOD_A                    14              // 发酵
#define STATE_FOOD_B                    13              // 饼干
#define STATE_FOOD_C                    12              // 烤饼
#define STATE_FOOD_D                    11              // 蛋糕
#define STATE_FOOD_E                    10              // 蛋挞
#define STATE_FOOD_F                    9               // 面包
#define STATE_FOOD_G                    8               // 烤鸡
#define STATE_FOOD_H                    7               // 烤翅
#define STATE_FOOD_I                    6               // 排骨
#define STATE_FOOD_J                    5               // 烤鱼
#define STATE_FOOD_K                    4               // 红薯
#define STATE_FOOD_L                    3               // 玉米

#define HOT_UP                          2               // 上热管
#define HOT_DOWN                        3               // 下热管

#define SET_FUNCTION                    0               // 选择功能
#define SET_TIME                        1               // 设置时间
#define SET_TEMP                        2               // 设置温度

#define KNOB_DISABLE                    1               // 禁用旋钮

#define SHOW_TEMP                       0               // 显示温度
#define SHOW_TIME                       1               // 显示时间

#define OK                              1    
#define NO                              0

static uint16_t Temperature[17] = {0, 0, 0, 195, 200, 210, 210, 210, 215, 160, 200, 155, 185, 150, 40, 170, 170};       // 默认温度值，摄氏度
static uint16_t Timing[17] = {0, 0, 0, 28, 60, 20, 28, 18, 40, 18, 20, 27, 16, 12, 50, 30, 30};      // 默认工作时间，分钟

typedef struct
{
    uint32_t    beep;                   // 蜂鸣器计数
    uint8_t     beepSW;                 // 蜂鸣器开关
    uint8_t     startWorkBeep;          // 开始工作提示音开关
    uint8_t     powerOnBeep;            // 开机提示音
    uint8_t     hotUpDown;              // 上下管状态, 0上，1下
    uint8_t     up_Temperature;         // 上管温度值
    uint8_t     down_Temperature;       // 下管温度值
    uint16_t    adc_UP;                 // 上管的传感器温度值
    uint16_t    adc_DOWN;               // 下管的传感器温度值 
    uint8_t     temperatureOK;          // 温度与设定的温度一致， 1：一致         
    uint16_t    workTime;               // 工作时间
    uint8_t     timeOut;                // 工作时间结束标志
    uint8_t     preheat;                // 预热
    uint8_t     light;                  // 炉灯
    uint8_t     workState;              // 功能选择状态
    uint8_t     enterMode;              // Code 开关模式
    uint8_t     setMode;                // 设置模式的状态：时间或温度
    uint8_t     knob;                   // 旋钮是否禁用， 1：禁用
    uint8_t     startWork;              // 开始工作标志
    uint8_t     flashLight;             // 指示灯闪计数
    uint8_t     Time_30ms;              // 30ms标志位
    uint8_t     Time_100ms;             // 100ms标志位
    uint8_t     Time_1_s;               // 1秒标志位
    uint8_t     Time_30_s;              // 30秒标志位
}DEVICE_STATUS;



static void CLK_Config(void);
#endif