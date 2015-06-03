/*
* RTC RT8563 读写, 硬件IIC
* 2015年04月20日 VA800 V1.5 PCB 上调试OK。
* by Hellogz
*/
#include "rt8563.h"

/*
名称: unsigned char BCD2Dec(unsigned char bcd)
功能: BCD码转为10进制
形参: bcd BCD码
返回: 10进制码
*/  
unsigned char BCD2Dec(unsigned char bcd)
{
    unsigned char dec = 0;
    dec = (bcd>>4)*10 + (bcd&0x0F);

    return dec;
}

/*
名称: unsigned char Dec2BCD(unsigned char dec)
功能: 10进制转为BCD码
形参: dec 10进制码
返回: BCD码
*/ 
unsigned char Dec2BCD(unsigned char dec)
{
    unsigned char bcd = 0;
    bcd = (dec/10)<<4 | (dec%10);

    return bcd;
}

/*
名称: uint8_t I2Ct0StandbyState(void)
功能: 让IIC恢复空闲状态
形参: 无
返回: 1
*/
uint8_t I2Ct0StandbyState(void)
{
    I2C->SR1; 
    I2C->SR3;
    I2C->CR2 |= 0x02; 		//STOP
    return 1;
}

/*
名称: void I2C_RTC_Init(void)
功能: 硬件IIC初始化
形参: 无
返回值：无
*/ 
void I2C_RTC_Init(void)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
    I2C_Cmd( ENABLE);
    I2C_Init(400000, 0xA0, I2C_DUTYCYCLE_2, I2C_ACK_CURR, 
           I2C_ADDMODE_7BIT, 16);
}

/*
名称: uint8_t Get_RT8563(uint8_t *pBuffer, 
                          uint8_t index, 
                          uint8_t NumByteToRead) 
功能: 读取RT8563时间数据
形参: *pData 存储读取数据指针
      index 时间-秒寄存器地址【秒, 分, 时, 日, 星期, 月, 年】
      NumByteToRead 需要读取的字节数
返回值：无
*/ 
uint8_t Get_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToRead) 
{  
    uint32_t timeout;
    timeout = LONG_TIMEOUT;
    while(I2C->SR3 & 0x02)
    {
        if((timeout--) == 0) return I2Ct0StandbyState();
    }
    I2C->CR2 |= 0x01;  
    timeout = FLAG_TIMEOUT;
    while(!(I2C->SR1 & 0x01))
    {
        if((timeout--) == 0) return I2Ct0StandbyState();
    }
    I2C->DR = 0xA2;  
    timeout = FLAG_TIMEOUT;
    while(!(I2C->SR1 & 0x02))
    {
        if((timeout--) == 0) return I2Ct0StandbyState();
    }
    I2C->SR1; 
    I2C->SR3; 
    I2C->DR = (uint8_t)(index); 
    timeout = FLAG_TIMEOUT;
    while(!(I2C->SR1 & 0x84))
    {
        if((timeout--) == 0) return I2Ct0StandbyState();
    }
    I2C->CR2 |= 0x01;  
    timeout = FLAG_TIMEOUT;
    while(!(I2C->SR1 & 0x01))
    {
        if((timeout--) == 0) return I2Ct0StandbyState();
    }
    I2C->DR = 0xA3;
    timeout = FLAG_TIMEOUT;
    while(!(I2C->SR1 & 0x02))
    {
        if((timeout--) == 0) return I2Ct0StandbyState();
    }
    I2C->SR1; 
    I2C->SR3; 

    while(NumByteToRead)  
    {
        if(NumByteToRead == 1) 
        { 
            I2C->CR2 &= ~0x04;   
            I2C->CR2 |= 0x02;  
        } 
        if(I2C->SR1 & 0x40)
        {    
            *pBuffer = I2C->DR;
            pBuffer++;  
            NumByteToRead--;     
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
uint8_t Set_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToWrite) 
{  
    uint32_t timeout;
    timeout = LONG_TIMEOUT;
    while(I2C->SR3 & 0x02)
    {
        if((timeout--) == 0) return I2Ct0StandbyState();
    }
    I2C->CR2 |= 0x01; 
    timeout = FLAG_TIMEOUT;
    while(!(I2C->SR1 & 0x01))
    {
        if((timeout--) == 0) return I2Ct0StandbyState();
    }
    I2C->DR = 0xA2; 
    timeout = FLAG_TIMEOUT;
    while(!(I2C->SR1 & 0x02))
    {
        if((timeout--) == 0) return I2Ct0StandbyState();
    }
    I2C->SR1; 
    I2C->SR3; 
    I2C->DR = (uint8_t)(index);
    timeout = FLAG_TIMEOUT;
    while(!(I2C->SR1 & 0x84))
    {
        if((timeout--) == 0) return I2Ct0StandbyState();
    }
    while(NumByteToWrite)  
    {    
        I2C->DR = Dec2BCD(*pBuffer);   
        timeout = LONG_TIMEOUT;
        while(!(I2C->SR1 & 0x04))
        {
            if((timeout--) == 0) return I2Ct0StandbyState();
        }
        pBuffer++;
        NumByteToWrite--; 
    }
    I2C->CR2 |= 0x02; 
    (void)I2C->SR1;
    (void)I2C->SR3;  
    return 0;
}