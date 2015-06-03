/*
* RTC RT8563 ��д, Ӳ��IIC
* 2015��04��20�� VA800 V1.5 PCB �ϵ���OK��
* by Hellogz
*/
#include "rt8563.h"

/*
����: unsigned char BCD2Dec(unsigned char bcd)
����: BCD��תΪ10����
�β�: bcd BCD��
����: 10������
*/  
unsigned char BCD2Dec(unsigned char bcd)
{
    unsigned char dec = 0;
    dec = (bcd>>4)*10 + (bcd&0x0F);

    return dec;
}

/*
����: unsigned char Dec2BCD(unsigned char dec)
����: 10����תΪBCD��
�β�: dec 10������
����: BCD��
*/ 
unsigned char Dec2BCD(unsigned char dec)
{
    unsigned char bcd = 0;
    bcd = (dec/10)<<4 | (dec%10);

    return bcd;
}

/*
����: uint8_t I2Ct0StandbyState(void)
����: ��IIC�ָ�����״̬
�β�: ��
����: 1
*/
uint8_t I2Ct0StandbyState(void)
{
    I2C->SR1; 
    I2C->SR3;
    I2C->CR2 |= 0x02; 		//STOP
    return 1;
}

/*
����: void I2C_RTC_Init(void)
����: Ӳ��IIC��ʼ��
�β�: ��
����ֵ����
*/ 
void I2C_RTC_Init(void)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
    I2C_Cmd( ENABLE);
    I2C_Init(400000, 0xA0, I2C_DUTYCYCLE_2, I2C_ACK_CURR, 
           I2C_ADDMODE_7BIT, 16);
}

/*
����: uint8_t Get_RT8563(uint8_t *pBuffer, 
                          uint8_t index, 
                          uint8_t NumByteToRead) 
����: ��ȡRT8563ʱ������
�β�: *pData �洢��ȡ����ָ��
      index ʱ��-��Ĵ�����ַ����, ��, ʱ, ��, ����, ��, �꡿
      NumByteToRead ��Ҫ��ȡ���ֽ���
����ֵ����
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
����: void Set_RT8563(uint8_t *pData, 
                      uint8_t index, 
                      uint8_t NumByteToWrite) 
����: ����RT8563ʱ������
�β�: *pData ���ò�������ָ��
      index ʱ��-��Ĵ�����ַ����, ��, ʱ, ��, ����, ��, �꡿
      NumByteToWrite ��Ҫд���ֽ���
����ֵ����
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