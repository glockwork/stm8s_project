#include "gpio.h"

void GPIO_Config_Init(void)
{
    /* Initialize I/Os in Output Mode */

    GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT);        // 旋钮输出A
    GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT);        // 旋钮输出B
 
    GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);   // TM1628_DIO
    GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);   // TM1628_CLK
    GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);   // TM1628_STB

    GPIO_Init(GPIOC, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);   // 旋钮按键
    GPIO_Init(GPIOC, GPIO_PIN_1, GPIO_MODE_OUT_PP_LOW_FAST);    // 蜂鸣器 Power GPIO, STM8207的BEEP功能脚接蜂鸣器控制脚
    
    GPIO_Init(GPIOF, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);    // 继电器 1 炉灯
    GPIO_Init(GPIOF, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);    // 继电器 2 发酵下管
    GPIO_Init(GPIOF, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);    // 继电器 3 电机
    GPIO_Init(GPIOF, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);    // 继电器 4 下管
    GPIO_Init(GPIOF, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST);    // 继电器 5 上管
}

