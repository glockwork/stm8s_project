/*
*  RTC RT8563 读写
* 2015年04月20日 VA800 V1.5 PCB 上调试OK。
* by Hellogz
*/
#include "rt8563.h"

#define RT8563TEST 0

#if RT8563TEST
//【秒, 分, 时, 日, 星期, 月, 年】
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
    if(send[1] != recv[1])      //每一分钟更新一次
    {
      for(i = 0; i < 7; i++)
        UART3_SendByte(send[i]);       
    }
  }

#endif
  
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
名称: void Set_RT8563(GPIO_TypeDef* IIC_PORT, 
			GPIO_Pin_TypeDef SDA_PIN,
			GPIO_Pin_TypeDef SCL_PIN,
			uint8_t *pData, 
			uint8_t index, 
			uint8_t NumByteToWrite) 
功能: 设置RT8563时间数据
形参: IIC_PORT IIC端口
	  SDA_PIN SDA端口号
	  SCL_PIN SCL端口号
	  *pData 设置参数数据指针
      index 时间-秒寄存器地址【秒, 分, 时, 日, 星期, 月, 年】
	  NumByteToWrite 需要写入字节数
返回值：无
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
名称: void Get_RT8563(GPIO_TypeDef* IIC_PORT, 
			GPIO_Pin_TypeDef SDA_PIN,
			GPIO_Pin_TypeDef SCL_PIN,
			uint8_t *pData, 
			uint8_t index, 
			uint8_t NumByteToRead)
功能: 读取RT8563时间数据
形参: IIC_PORT IIC端口
	  SDA_PIN SDA端口号
	  SCL_PIN SCL端口号
	  *pData 存储读取数据指针
      index 时间-秒寄存器地址【秒, 分, 时, 日, 星期, 月, 年】
	  NumByteToWrite 需要写入字节数
返回值：无
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