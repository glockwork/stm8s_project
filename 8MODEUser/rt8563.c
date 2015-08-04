/*
*  RTC RT8563 ��д
* 2015��04��20�� VA800 V1.5 PCB �ϵ���OK��
* by Hellogz
*/
#include "rt8563.h"

#define RT8563TEST 0

#if RT8563TEST
//����, ��, ʱ, ��, ����, ��, �꡿
  uint8_t send[7] = {0x00, 0x46, 0x08, 0x21, 0x02, 0x04, 0x15};
  uint8_t recv[7], i;

  IIC_Init(RTC_ROPT, RTC_SDA_PIN, RTC_SCL_PIN);
  
  Set_RT8563(RTC_ROPT, RTC_SDA_PIN, RTC_SCL_PIN, send, 2, 7);
  Get_RT8563(RTC_ROPT, RTC_SDA_PIN, RTC_SCL_PIN, recv, 2, 7);
  
  for(i = 0; i < 7; i++)
  {
    UART3_SendByte(recv[i]);
    send[i] = recv[i];
  }
  while (1)
  {
    Get_RT8563(RTC_ROPT, RTC_SDA_PIN, RTC_SCL_PIN, recv, 2, 7);
    delay(500);
    Get_RT8563(RTC_ROPT, RTC_SDA_PIN, RTC_SCL_PIN, send, 2, 7);
    if(send[1] != recv[1])      //ÿһ���Ӹ���һ��
    {
      for(i = 0; i < 7; i++)
        UART3_SendByte(send[i]);       
    }
  }

#endif
  
/*
����: unsigned char BCD2Dec(unsigned char bcd)
����: BCD��תΪ10����
�β�: bcd BCD��
����: 10������
*/  
unsigned char BCD2Dec(unsigned char bcd)
{
  return ((bcd / 16) * 10 + bcd % 16);
}

/*
����: unsigned char Dec2BCD(unsigned char dec)
����: 10����תΪBCD��
�β�: dec 10������
����: BCD��
*/ 
unsigned char Dec2BCD(unsigned char dec)
{
  return (dec / 10 * 16 + dec % 10);
}
  
/*
����: void Set_RT8563(GPIO_TypeDef* IIC_PORT, 
			GPIO_Pin_TypeDef SDA_PIN,
			GPIO_Pin_TypeDef SCL_PIN,
			uint8_t *pData, 
			uint8_t index, 
			uint8_t NumByteToWrite) 
����: ����RT8563ʱ������
�β�: IIC_PORT IIC�˿�
	  SDA_PIN SDA�˿ں�
	  SCL_PIN SCL�˿ں�
	  *pData ���ò�������ָ��
      index ʱ��-��Ĵ�����ַ����, ��, ʱ, ��, ����, ��, �꡿
	  NumByteToWrite ��Ҫд���ֽ���
����ֵ����
*/ 
void Set_RT8563(GPIO_TypeDef* IIC_PORT,
                GPIO_Pin_TypeDef SDA_PIN,
                GPIO_Pin_TypeDef SCL_PIN,
                uint8_t *pData, 
                uint8_t index, 
                uint8_t NumByteToWrite) 
{
  IIC_Start(IIC_PORT, SDA_PIN, SCL_PIN);
  IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, 0xA2);
  IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);
  IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, (uint8_t)(index));
  IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);

  while(NumByteToWrite)  
  {    
    IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, Dec2BCD(*pData));
    IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);
    pData++;
    NumByteToWrite--; 
  }
  IIC_Stop(IIC_PORT, SDA_PIN, SCL_PIN);
}

/*
����: void Get_RT8563(GPIO_TypeDef* IIC_PORT, 
			GPIO_Pin_TypeDef SDA_PIN,
			GPIO_Pin_TypeDef SCL_PIN,
			uint8_t *pData, 
			uint8_t index, 
			uint8_t NumByteToRead)
����: ��ȡRT8563ʱ������
�β�: IIC_PORT IIC�˿�
	  SDA_PIN SDA�˿ں�
	  SCL_PIN SCL�˿ں�
	  *pData �洢��ȡ����ָ��
      index ʱ��-��Ĵ�����ַ����, ��, ʱ, ��, ����, ��, �꡿
	  NumByteToWrite ��Ҫд���ֽ���
����ֵ����
*/ 
void Get_RT8563(GPIO_TypeDef* IIC_PORT, 
                GPIO_Pin_TypeDef SDA_PIN,
                GPIO_Pin_TypeDef SCL_PIN,
                uint8_t *pData, 
                uint8_t index, 
                uint8_t NumByteToRead) 
{
  IIC_Start(IIC_PORT, SDA_PIN, SCL_PIN);
  IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, 0xA2);
  IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);
  IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, (uint8_t)(index));
  IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);
  
  IIC_Start(IIC_PORT, SDA_PIN, SCL_PIN);
  IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, 0xA3);
  IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);
  
  pData[0] = BCD2Dec(IIC_Read_Byte(IIC_PORT, SDA_PIN, SCL_PIN, ACK) & 0x7F); // second, remove VL error bit
  pData[1] = BCD2Dec(IIC_Read_Byte(IIC_PORT, SDA_PIN, SCL_PIN, ACK) & 0x7F); // minute, remove unwanted bits from MSB
  pData[2] = BCD2Dec(IIC_Read_Byte(IIC_PORT, SDA_PIN, SCL_PIN, ACK) & 0x3F); // hour
  pData[3] = BCD2Dec(IIC_Read_Byte(IIC_PORT, SDA_PIN, SCL_PIN, ACK) & 0x3F); // dayOfMonth
  pData[4] = BCD2Dec(IIC_Read_Byte(IIC_PORT, SDA_PIN, SCL_PIN, ACK) & 0x07); // dayOfWeek
  pData[5] = BCD2Dec(IIC_Read_Byte(IIC_PORT, SDA_PIN, SCL_PIN, ACK) & 0x1F); // month, remove century bit, 1999 is over
  
  pData[6] = BCD2Dec(IIC_Read_Byte(IIC_PORT, SDA_PIN, SCL_PIN, NACK)); 

  IIC_Stop(IIC_PORT, SDA_PIN, SCL_PIN);
}	