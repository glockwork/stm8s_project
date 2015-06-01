#ifndef __IIC_RT8563_H
#define __IIC_RT8563_H

#include "stm8s.h"

#define FLAG_TIMEOUT         ((uint32_t)0x1000)
#define LONG_TIMEOUT         ((uint32_t)(10 * FLAG_TIMEOUT))

unsigned char BCD2Dec(unsigned char bcd);
unsigned char Dec2BCD(unsigned char dec);
void I2C_RTC_Init(void);
uint8_t I2Ct0StandbyState(void);
uint8_t Get_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToRead);
uint8_t Set_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToWrite);

#endif