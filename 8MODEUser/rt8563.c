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
  uint8_t i;
  IIC_Start(IIC_PORT, SDA_PIN, SCL_PIN);
  IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, 0xA2);
  IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);
  IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, (uint8_t)(index));
  IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);
  
  IIC_Start(IIC_PORT, SDA_PIN, SCL_PIN);
  IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, 0xA3);
  IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);

  for(i = 0; i < NumByteToRead -1; i++)  
    pData[i] = BCD2Dec(IIC_Read_Byte(IIC_PORT, SDA_PIN, SCL_PIN, ACK));       
  pData[i] = BCD2Dec(IIC_Read_Byte(IIC_PORT, SDA_PIN, SCL_PIN, NACK)); 

  IIC_Stop(IIC_PORT, SDA_PIN, SCL_PIN);
}	