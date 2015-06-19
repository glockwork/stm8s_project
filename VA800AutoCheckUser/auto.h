#ifndef __AUTO_H
#define __AUTO_H

#include "stm8s.h"
#include "24Lc02.h"

#define FLAG_TIMEOUT         ((uint32_t)0x1000)
#define LONG_TIMEOUT         ((uint32_t)(10 * FLAG_TIMEOUT))
#define AT_CSQ_GET	     "AT+CSQ\r\n"
#define LED_8_PCS            0

#define CCR1_Val  ((uint16_t)0)
#define CCR2_Val  ((uint16_t)0)       //LED_Red
#if LED_8_PCS
  #define CCR3_Val  ((uint16_t)0)     //LED_Green
#endif
#define CCR4_Val  ((uint16_t)0)       //LED_Yellow

void CLK_Config(void);
void CLK_Config(void);
void I2C_RTC_Init(void);
void UART_Config(void);
void GPIO_Config(void);
void ADC_Config(void);
void AutoCheckFunction(void);
uint16_t Get_ADC_Value(ADC2_Channel_TypeDef channel);
//uint16_t Get_DS_Value(void);
void UART1_SendByte(uint8_t Data);
void UART1_SendString(uint8_t* Data, uint32_t len);
void UART3_SendByte(uint8_t Data);
void UART3_SendString(uint8_t* Data, uint32_t len);
//void Send_AIR_Value(uint16_t Air_Value);
void Send_BAT_Voltage(uint16_t AD_Value);
void TIM1_Config(void);
uint8_t TIM1_SetCompare(uint8_t nPort, uint32_t nCCR_Val);
uint8_t Set_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToRead);
uint8_t Read_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToWrite);

#endif