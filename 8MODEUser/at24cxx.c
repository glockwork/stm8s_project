/*
* 读写AT24Cxx系列EEPROM
* 2015年04月20日 VA800 V1.5 PCB 上调试OK。
* by Hellogz
*/
#include "at24cxx.h"

#define AT24CXXTEST 0

#if AT24CXXTEST

  IIC_Init(EEP_IIC_PORT, EEP_SDA_PIN, EEP_SCL_PIN);
  DelayUs(10000);
  uint8_t send[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ";
  uint8_t recv[26*3];
  uint16_t i, data = 0;
  
  AT24Cxx_Write(EEP_IIC_PORT, EEP_SDA_PIN, EEP_SCL_PIN, 0, send, 26*3);
  AT24Cxx_Read(EEP_IIC_PORT, EEP_SDA_PIN, EEP_SCL_PIN, 0, recv, 26*3);
  
  for(i = 0; i < 26*3; i++)
    UART3_SendByte(recv[i]);

#if 1
  AT24Cxx_WriteOneByte(EEP_IIC_PORT, EEP_SDA_PIN, EEP_SCL_PIN, 0, '$');
  data = AT24Cxx_ReadOneByte(EEP_IIC_PORT, EEP_SDA_PIN, EEP_SCL_PIN, 0);
  UART3_SendByte(data);
  
#endif

  if(AT24Cxx_Check(EEP_IIC_PORT, EEP_SDA_PIN, EEP_SCL_PIN) == 0)
    UART3_SendString("OK\r\n", sizeof("OK\r\n")-1);
  else
    UART3_SendString("NO\r\n", sizeof("NO\r\n")-1);

#endif




/*
名称: uint8_t AT24Cxx_ReadOneByte(GPIO_TypeDef* IIC_PORT, 
			GPIO_Pin_TypeDef SDA_PIN,
			GPIO_Pin_TypeDef SCL_PIN,
			uint16_t ReadAddr)
功能: 读取AT24Cxx系列的EEPROM一个字节的数据
形参: IIC_PORT IIC端口
	  SDA_PIN SDA端口号
	  SCL_PIN SCL端口号
	  ReadAddr EEPROM的数据地址
返回值：读取到的数据
*/ 	
uint8_t AT24Cxx_ReadOneByte(GPIO_TypeDef* IIC_PORT, 
                            GPIO_Pin_TypeDef SDA_PIN,
                            GPIO_Pin_TypeDef SCL_PIN,
                            uint16_t ReadAddr)
{				  
  uint8_t temp = 0;		  	    																 
  IIC_Start(IIC_PORT, SDA_PIN, SCL_PIN);  
  IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, 0xA0); 
  IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);
  if(EE_TYPE > AT24C16)
  {
    IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, (uint8_t)(ReadAddr >> 8));
    IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);	 
  }

  IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, (uint8_t)(ReadAddr));  
  IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);	
       
  IIC_Start(IIC_PORT, SDA_PIN, SCL_PIN);  	 	   
  IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, (0xA1));           		   
  IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);	 
  temp = IIC_Read_Byte(IIC_PORT, SDA_PIN, SCL_PIN, NACK);		   
  IIC_Stop(IIC_PORT, SDA_PIN, SCL_PIN);    
  return temp;
}

/*
名称: void AT24Cxx_WriteOneByte(GPIO_TypeDef* IIC_PORT, 
			GPIO_Pin_TypeDef SDA_PIN,
			GPIO_Pin_TypeDef SCL_PIN,
			uint16_t WriteAddr,
			uint8_t DataToWrite)
功能: 写入AT24Cxx系列的EEPROM一个字节的数据
形参: IIC_PORT IIC端口
	  SDA_PIN SDA端口号
	  SCL_PIN SCL端口号
	  WriteAddr EEPROM的数据地址
	  DataToWrite 要写入的数据
返回值：无
*/
void AT24Cxx_WriteOneByte(GPIO_TypeDef* IIC_PORT,
                          GPIO_Pin_TypeDef SDA_PIN,
                          GPIO_Pin_TypeDef SCL_PIN,
                          uint16_t WriteAddr,
                          uint8_t DataToWrite)
{				   	  	    																 
  IIC_Start(IIC_PORT, SDA_PIN, SCL_PIN);  
  IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, 0xA0);
  IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);
  if(EE_TYPE > AT24C16)
  {
    IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, (uint8_t)(WriteAddr >> 8));
    IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);		 
  }

  IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, (uint8_t)(WriteAddr));
  IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);
  IIC_Send_Byte(IIC_PORT, SDA_PIN, SCL_PIN, DataToWrite);
  IIC_Respons(IIC_PORT, SDA_PIN, SCL_PIN);
  IIC_Stop(IIC_PORT, SDA_PIN, SCL_PIN);
  DelayUs(750);
}

/*
名称: void AT24Cxx_Read(GPIO_TypeDef* IIC_PORT, 
			GPIO_Pin_TypeDef SDA_PIN,
			GPIO_Pin_TypeDef SCL_PIN,
			uint16_t ReadAddr,
			uint8_t *pBuffer,
			uint16_t NumToRead)
功能: 读取AT24Cxx系列的EEPROM多个字节的数据
形参: IIC_PORT IIC端口
	  SDA_PIN SDA端口号
	  SCL_PIN SCL端口号
	  ReadAddr EEPROM的数据地址
	  *pBuffer 存储读取数据指针
	  NumToRead 读取数据的个数
返回值：无
*/
void AT24Cxx_Read(GPIO_TypeDef* IIC_PORT,
                  GPIO_Pin_TypeDef SDA_PIN,
                  GPIO_Pin_TypeDef SCL_PIN,
                  uint16_t ReadAddr,
                  uint8_t *pBuffer,
                  uint16_t NumToRead)
{
  uint8_t i;
  
  for(i = 0; i < NumToRead; i++)
  {
    *pBuffer = AT24Cxx_ReadOneByte(IIC_PORT, SDA_PIN, SCL_PIN, ReadAddr+i);	
    pBuffer++;
	DelayUs(750);
  }
}  

/*
名称: void AT24Cxx_Write(GPIO_TypeDef* IIC_PORT, 
			GPIO_Pin_TypeDef SDA_PIN,
			GPIO_Pin_TypeDef SCL_PIN,
			uint16_t WriteAddr,
			uint8_t *pBuffer,
			uint16_t NumToWrite)
功能: 写入AT24Cxx系列的EEPROM多个字节的数据
形参: IIC_PORT IIC端口
	  SDA_PIN SDA端口号
	  SCL_PIN SCL端口号
	  WriteAddr EEPROM的数据地址
	  *pBuffer 要写入数据的指针
	  NumToWrite 写入数据的个数
返回值：无
*/
void AT24Cxx_Write(GPIO_TypeDef* IIC_PORT,
                   GPIO_Pin_TypeDef SDA_PIN,
                   GPIO_Pin_TypeDef SCL_PIN,
                   uint16_t WriteAddr,
                   uint8_t *pBuffer,
                   uint16_t NumToWrite)
{
  uint8_t i;
  
  for(i = 0; i < NumToWrite; i++)
  {
    AT24Cxx_WriteOneByte(IIC_PORT, SDA_PIN, SCL_PIN, WriteAddr+i,*pBuffer);
    pBuffer++;
	DelayUs(750);
  }
}

/*
名称: uint8_t AT24Cxx_Check(void)
功能: 检查AT24Cxx系列的EEPROM通信是否正常
形参: IIC_PORT IIC端口
      SDA_PIN SDA端口号
      SCL_PIN SCL端口号
返回值：通行正常返回0, 通信错误返回1
*/
uint8_t AT24Cxx_Check(GPIO_TypeDef* IIC_PORT,
                   GPIO_Pin_TypeDef SDA_PIN,
                   GPIO_Pin_TypeDef SCL_PIN)
{
  AT24Cxx_WriteOneByte(IIC_PORT, SDA_PIN, SCL_PIN, 0, '$');

  if('$' == AT24Cxx_ReadOneByte(IIC_PORT, SDA_PIN, SCL_PIN, 0))
    return 0;
  return 1;											  
}