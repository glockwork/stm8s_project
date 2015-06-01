/*
* RTC RT8563 ��д, Ӳ��IIC
* 2015��04��20�� VA800 V1.5 PCB �ϵ���OK��
* by Hellogz
*/
#include "iic_rt8563.h"

/*
����: void I2C_RTC_Init(void)
����: Ӳ��IIC��ʼ��
�β�: ��
����ֵ����
*/ 
void IIC_RTC_Init(void)
{
  I2C_Cmd(DISABLE);
  I2C_DeInit();
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
  GPIO_Init(GPIOE, GPIO_PIN_1, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOE, GPIO_PIN_2, GPIO_MODE_IN_PU_NO_IT);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
  I2C_Cmd( ENABLE);
  I2C_Init(400000, 0xA0, I2C_DUTYCYCLE_2, I2C_ACK_CURR, 
           I2C_ADDMODE_7BIT, 16);
}

/*
����: uint8_t Get_RT8563(uint8_t *pBuffer, 
                          uint8_t index, 
                          uint8_t NumByteToWrite) 
����: ��ȡRT8563ʱ������
�β�: *pData �洢��ȡ����ָ��
      index ʱ��-��Ĵ�����ַ����, ��, ʱ, ��, ����, ��, �꡿
      NumByteToWrite ��Ҫд���ֽ���
����ֵ����
*/ 
uint8_t Get_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToWrite) 
{  
  uint32_t timeout;
  timeout = LONG_TIMEOUT;
  while(I2C->SR3 & 0x02)
  {
    if((timeout--) == 0) return 1;
  }
  I2C->CR2 |= 0x01;  
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x01))
  {
    if((timeout--) == 0) return 1;
  }
  I2C->DR = 0xA2;  
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x02))
  {
    if((timeout--) == 0) return 1;
  }
  I2C->SR1; 
  I2C->SR3; 
  I2C->DR = (uint8_t)(index); 
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x84))
  {
    if((timeout--) == 0) return 1;
  }
  I2C->CR2 |= 0x01;  
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x01))
  {
    if((timeout--) == 0) return 1;
  }
  I2C->DR = 0xA3;
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x02))
  {
    if((timeout--) == 0) return 1;
  }
  I2C->SR1; 
  I2C->SR3; 

  while(NumByteToWrite)  
  {
    if(NumByteToWrite == 1) 
    { 
      I2C->CR2 &= ~0x04;   
      I2C->CR2 |= 0x02;  
    } 
    if(I2C->SR1 & 0x40)
    {    
      *pBuffer=I2C->DR;
      pBuffer++;  
      NumByteToWrite--;     
    }      
  }
  I2C->CR2 |= 0x04;
  return 0;
}

/*
����: void Set_RT8563(uint8_t *pData, 
                      uint8_t index, 
                      uint8_t NumByteToWrite) 
����: ����RT8563ʱ������
�β�: *pData ���ò�������ָ��
      index ʱ��-��Ĵ�����ַ����, ��, ʱ, ��, ����, ��, �꡿
      NumByteToWrite ��Ҫд���ֽ���
����ֵ����
*/ 
uint8_t Set_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToRead) 
{  
  uint32_t timeout;
  timeout = LONG_TIMEOUT;
  while(I2C->SR3 & 0x02)
  {
    if((timeout--) == 0) return 1;
  }
  I2C->CR2 |= 0x01; 
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x01))
  {
    if((timeout--) == 0) return 1;
  }
  I2C->DR = 0xA2; 
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x02))
  {
    if((timeout--) == 0) return 1;
  }
  I2C->SR1; 
  I2C->SR3; 
  I2C->DR = (uint8_t)(index);
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x84))
  {
    if((timeout--) == 0) return 1;
  }
  while(NumByteToRead)  
  {    
    I2C->DR = *pBuffer;   
    timeout = LONG_TIMEOUT;
    while(!(I2C->SR1 & 0x04))
    {
    if((timeout--) == 0) return 1;
    }
    pBuffer++;
    NumByteToRead--; 
  }
  I2C->CR2 |= 0x02; 
  (void)I2C->SR1;
  (void)I2C->SR3;  
  return 0;
}