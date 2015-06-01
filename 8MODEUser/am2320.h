#ifndef __AM2320_H
#define __AM2320_H

#include "stm8s.h"
#include "basic_timer.h"

#define FLAG_TIMEOUT            ((uint32_t)0x1000)
#define LONG_TIMEOUT            ((uint32_t)(10 * FLAG_TIMEOUT))
#define I2C_SPEED               50000
#define I2C_SLAVE_ADDRESS7      0xA0

void I2C_AM2320_DeInit(void);
void I2C_AM2320_Init(void);
uint8_t Get_AM2320(uint8_t *pBuffer);
uint8_t I2Ct0StandbyState(void);
uint16_t CRC16(uint8_t *pBuffer, uint8_t len);
uint8_t CheckCRC(uint8_t *pBuffer,uint8_t len);
void Clear_Buffer(uint8_t *Data);

#endif