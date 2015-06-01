#ifndef __UART3_H
#define __UART3_H
#include "stm8s.h"

void UART3_Config(void);
void UART3_SendByte(uint8_t Data);
void UART3_SendString(uint8_t* Data, uint32_t len);
void Send_AIR_Value(uint16_t Air_Value);
void Send_BAT_Voltage(uint16_t AD_Value);
void AQI2PM25(uint16_t voltage);

#endif