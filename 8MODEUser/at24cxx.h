#ifndef __AT24CXX_H
#define __AT24CXX_H

#include "stm8s.h"
#include "gpio2iic.h"

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	        8191
#define AT24C128	16383
#define AT24C256	32767  
 
#define EE_TYPE AT24C02

uint8_t AT24Cxx_ReadOneByte(GPIO_TypeDef* IIC_PORT, 
			GPIO_Pin_TypeDef SDA_PIN, 
			GPIO_Pin_TypeDef SCL_PIN, 
			uint16_t ReadAddr);
void AT24Cxx_WriteOneByte(GPIO_TypeDef* IIC_PORT,  
			GPIO_Pin_TypeDef SDA_PIN, 
			GPIO_Pin_TypeDef SCL_PIN, 
			uint16_t WriteAddr, 
			uint8_t DataToWrite);
void AT24Cxx_Read(GPIO_TypeDef* IIC_PORT, 
			GPIO_Pin_TypeDef SDA_PIN, 
			GPIO_Pin_TypeDef SCL_PIN, 
			uint16_t ReadAddr, 
			uint8_t *pBuffer, 
			uint16_t NumToRead);
void AT24Cxx_Write(GPIO_TypeDef* IIC_PORT, 
			GPIO_Pin_TypeDef SDA_PIN, 
			GPIO_Pin_TypeDef SCL_PIN, 
			uint16_t WriteAddr, 
			uint8_t *pBuffer, 
			uint16_t NumToWrite);
uint8_t AT24Cxx_Check(GPIO_TypeDef* IIC_PORT,
                   GPIO_Pin_TypeDef SDA_PIN,
                   GPIO_Pin_TypeDef SCL_PIN);

#endif