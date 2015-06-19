#ifndef __UART1_H
#define __UART1_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "m35.h"
#include "kmp.h"
/* Private define ------------------------------------------------------------*/
#define RxBufferSize 255

/* Private variables ---------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/



/* Exported functions ------------------------------------------------------- */
void UART1_SendByte(uint8_t Data);
void UART1_SendString(uint8_t* Data, uint32_t len);
void Send_ADC_Value(u16 AD_Value);
void Send_ADC_Voltage(uint16_t AD_Value);
void UART1_Config(void); 
uint8_t SendCommandList(uint8_t* pCommand[], uint32_t Len, uint8_t pos);
uint8_t SendCommandCMP(uint8_t* pCommand, uint8_t* pStatus);
void CheckConnectStatus(uint8_t* pRxData);
void UART1_SendCommand(uint8_t* Command);
extern void Delay(__IO uint32_t nTime);

#endif /* __UART1_H */