#ifndef __UART1_H
#define __UART1_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"


void UART1_SendByte(uint8_t Data);
void UART1_SendString(uint8_t* Data, uint32_t len);
void UART1_Config(void); 


#endif /* __UART1_H */