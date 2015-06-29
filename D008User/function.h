#ifndef __FUNCTION_H
#define __FUNCTION_H
#include "main.h"
#if 0
#define A_H	                GPIO_WriteHigh(GPIOC, GPIO_PIN_6)
#define A_L	                GPIO_WriteLow(GPIOC, GPIO_PIN_6)
#define B_H	                GPIO_WriteHigh(GPIOC, GPIO_PIN_7)
#define B_L	                GPIO_WriteLow(GPIOC, GPIO_PIN_7)
#endif
#define READ_STB                GPIO_ReadInputPin(GPIOC, GPIO_PIN_5)    // STB
#define READ_CLK                GPIO_ReadInputPin(GPIOC, GPIO_PIN_4)    // CLK 
#define READ_DIO                GPIO_ReadInputPin(GPIOC, GPIO_PIN_3)    // DIO

#define CodingSwitch_A          GPIO_ReadInputPin(GPIOC, GPIO_PIN_6)
#define CodingSwitch_B          GPIO_ReadInputPin(GPIOC, GPIO_PIN_7)

#define BEEP_H	                GPIO_WriteHigh(GPIOC, GPIO_PIN_1)
#define BEEP_L	                GPIO_WriteLow(GPIOC, GPIO_PIN_1)

#define SYMBOL_DEFAULT          1
#define SYMBOL_UP               2
#define SYMBOL_DOWN             3

#define STB_L	                GPIO_WriteLow(GPIOC, GPIO_PIN_5)
#define STB_H	                GPIO_WriteHigh(GPIOC, GPIO_PIN_5)
#define CLK_L	                GPIO_WriteLow(GPIOC, GPIO_PIN_4)
#define CLK_H	                GPIO_WriteHigh(GPIOC, GPIO_PIN_4)
#define DIO_L	                GPIO_WriteLow(GPIOC, GPIO_PIN_3)
#define DIO_H	                GPIO_WriteHigh(GPIOC, GPIO_PIN_3)

void KeyBeep(void);
void PowerOnBeep(void);
#if 0
void right_handwise(void);
void left_handwise(void);
#endif
void showAll(void);
void clear(void);
void showTemp(uint8_t temp, uint8_t sw);
void showSymbol(uint8_t symbol);
void showOFF(void);
void showFunction(uint8_t function, uint8_t state);
void showLED(uint8_t index, uint8_t number);
void read_key(void);
void key_process(void);
void CodingSwitchPolling(void);
void CodeProcess(int8_t direction);
void showLight(void);
void showPreheat(uint8_t sw);
void showUpDownTemp(void);
void showTime(uint16_t time, uint8_t sw, uint8_t dp);
void DeviceRemind(void);
void SetFunction(uint8_t function);
void LightSwitch(void);
void CancelKey(void);
void SetHotUpOrDown(void);
void SetTimeKey(void);
uint16_t Get_UP_NTC_Value(void);
uint16_t Get_DOWN_NTC_Value(void);
#endif