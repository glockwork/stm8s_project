#ifndef __GPIO2IIC_H
#define __GPIO2IIC_H

#include "stm8s.h"
#include "gpio2iic.h"

#define EEP_IIC_PORT   GPIOG
#define EEP_SDA_PIN    GPIO_PIN_3
#define EEP_SCL_PIN    GPIO_PIN_4

#define SDA_OUT(IIC_PORT, SDA_PIN)	\
	GPIO_Init(IIC_PORT, SDA_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW)
#define SCL_OUT(IIC_PORT, SCL_PIN)	\
	GPIO_Init(IIC_PORT, SCL_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW)
#define SDA_IN(IIC_PORT, SDA_PIN)	\
	GPIO_Init(IIC_PORT, SDA_PIN, GPIO_MODE_IN_PU_NO_IT)
#define SDA_1(IIC_PORT, SDA_PIN)	GPIO_WriteHigh(IIC_PORT, SDA_PIN)
#define SDA_0(IIC_PORT, SDA_PIN)	GPIO_WriteLow(IIC_PORT, SDA_PIN)	
#define SCL_1(IIC_PORT, SCL_PIN)	GPIO_WriteHigh(IIC_PORT, SCL_PIN)	
#define SCL_0(IIC_PORT, SCL_PIN)	GPIO_WriteLow(IIC_PORT, SCL_PIN)	

#define ACK	        1
#define NACK		0
#define DELAY_US        30
          
void DelayUs(uint16_t time);
void IIC_Init(GPIO_TypeDef* IIC_PORT, 
              GPIO_Pin_TypeDef SDA_PIN, 
              GPIO_Pin_TypeDef SCL_PIN);
void IIC_Start(GPIO_TypeDef* IIC_PORT, 
               GPIO_Pin_TypeDef SDA_PIN, 
               GPIO_Pin_TypeDef SCL_PIN);
void IIC_Stop(GPIO_TypeDef* IIC_PORT, 
              GPIO_Pin_TypeDef SDA_PIN, 
              GPIO_Pin_TypeDef SCL_PIN);
void IIC_Respons(GPIO_TypeDef* IIC_PORT, 
                 GPIO_Pin_TypeDef SDA_PIN, 
                 GPIO_Pin_TypeDef SCL_PIN);
void IIC_SendACK(GPIO_TypeDef* IIC_PORT, 
                 GPIO_Pin_TypeDef SDA_PIN, 
                 GPIO_Pin_TypeDef SCL_PIN, 
                 uint8_t Ack);
void IIC_Send_Byte(GPIO_TypeDef* IIC_PORT, 
                   GPIO_Pin_TypeDef SDA_PIN, 
                   GPIO_Pin_TypeDef SCL_PIN, 
                   uint8_t TXdate);
uint8_t IIC_Read_Byte(GPIO_TypeDef* IIC_PORT, 
                      GPIO_Pin_TypeDef SDA_PIN, 
                      GPIO_Pin_TypeDef SCL_PIN, 
                      uint8_t Ack);
#endif