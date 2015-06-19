#include "gpio.h"

void GPIO_Config_Init(void)
{
    /* Initialize I/Os in Output Mode */

    GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT);        // ÐýÅ¥Êä³öA
    GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT);        // ÐýÅ¥Êä³öB
 
    GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);   // DIO
    GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);   // CLK
    GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);   // STB

    GPIO_Init(GPIOC, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);   // Code Key
    GPIO_Init(GPIOC, GPIO_PIN_1, GPIO_MODE_OUT_PP_LOW_FAST);    // Beep GPIO
    
    GPIO_Init(GPIOF, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);    // ¼ÌµçÆ÷ 1
    GPIO_Init(GPIOF, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);    // ¼ÌµçÆ÷ 2
    GPIO_Init(GPIOF, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);    // ¼ÌµçÆ÷ 3
    GPIO_Init(GPIOF, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);    // ¼ÌµçÆ÷ 4
    GPIO_Init(GPIOF, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST);    // ¼ÌµçÆ÷ 5
}

