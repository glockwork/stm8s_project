/*
* RTC RT8563 读写, 硬件IIC
* 2015年04月20日 VA800 V1.5 PCB 上调试OK。
* by Hellogz
*/
#include "iic_rt8563.h"

/*
名称: void I2C_RTC_Init(void)
功能: 硬件IIC初始化
形参: 无
返回值：无
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
名称: uint8_t Get_RT8563(uint8_t *pBuffer, 
                          uint8_t index, 
                          uint8_t NumByteToWrite) 
功能: 读取RT8563时间数据
形参: *pData 存储读取数据指针
      index 时间-秒寄存器地址【秒, 分, 时, 日, 星期, 月, 年】
      NumByteToWrite 需要写入字节数
返回值：无
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
名称: void Set_RT8563(uint8_t *pData, 
                      uint8_t index, 
                      uint8_t NumByteToWrite) 
功能: 设置RT8563时间数据
形参: *pData 设置参数数据指针
      index 时间-秒寄存器地址【秒, 分, 时, 日, 星期, 月, 年】
      NumByteToWrite 需要写入字节数
返回值：无
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