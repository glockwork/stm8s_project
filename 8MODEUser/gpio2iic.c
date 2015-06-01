/*
* GPIO模拟IIC
* 2015年04月20日 在VA800 V1.5版上调试OK
* by Hellogz
*/
#include "gpio2iic.h" 

/*
函数：void DelayUs(uint16_t time) 
功能：微秒级延迟函数
参数：time 延迟时间
返回值：无
*/
void DelayUs(uint16_t time) 
{ 
  uint16_t i, j;
  for(i = 0; i < time; i++)
    for(j = 0; j < 4; j++);
}

/*
函数：void IIC_Init(GPIO_TypeDef* IIC_PORT, 
				GPIO_Pin_TypeDef SDA_PIN,
				GPIO_Pin_TypeDef SCL_PIN)
功能：IIC 初始化
参数：IIC_PORT IIC端口
	  SDA_PIN SDA端口号
	  SCL_PIN SCL端口号
返回值：无
*/
void IIC_Init(GPIO_TypeDef* IIC_PORT, 
               GPIO_Pin_TypeDef SDA_PIN,
               GPIO_Pin_TypeDef SCL_PIN)
{
  SDA_OUT(IIC_PORT, SDA_PIN);
  SCL_OUT(IIC_PORT, SCL_PIN);
  SDA_1(IIC_PORT, SDA_PIN);
  DelayUs(DELAY_US);
  SCL_1(IIC_PORT, SCL_PIN);
  DelayUs(DELAY_US);
}
 
/*
函数：void IIC_Start(GPIO_TypeDef* IIC_PORT, 
				GPIO_Pin_TypeDef SDA_PIN,
				GPIO_Pin_TypeDef SCL_PIN)
功能：IIC 总线产生开始信号
参数：IIC_PORT IIC端口
	  SDA_PIN SDA端口号
	  SCL_PIN SCL端口号
返回值：无
*/
void IIC_Start(GPIO_TypeDef* IIC_PORT, 
                GPIO_Pin_TypeDef SDA_PIN,
                GPIO_Pin_TypeDef SCL_PIN)
{      
  SDA_OUT(IIC_PORT, SDA_PIN);
  
  SDA_1(IIC_PORT, SDA_PIN);	  
  DelayUs(DELAY_US);
  SCL_1(IIC_PORT, SCL_PIN);
  DelayUs(DELAY_US);
  SDA_0(IIC_PORT, SDA_PIN);
  DelayUs(DELAY_US); 
}	  

/*
函数：void IIC_Stop(GPIO_TypeDef* IIC_PORT, 
				GPIO_Pin_TypeDef SDA_PIN,
				GPIO_Pin_TypeDef SCL_PIN)
功能：IIC 总线产生停止信号
参数：IIC_PORT IIC端口
	  SDA_PIN SDA端口号
	  SCL_PIN SCL端口号
返回值：无
*/
void IIC_Stop(GPIO_TypeDef* IIC_PORT, 
              GPIO_Pin_TypeDef SDA_PIN,
              GPIO_Pin_TypeDef SCL_PIN)
{ 
  SDA_OUT(IIC_PORT, SDA_PIN);

  SDA_0(IIC_PORT, SDA_PIN);
  DelayUs(DELAY_US); 
  SCL_1(IIC_PORT, SCL_PIN);
  DelayUs(DELAY_US);  	
  SDA_1(IIC_PORT, SDA_PIN);	  	  
  DelayUs(DELAY_US);						   	
}

/*
函数：void IIC_Respons(GPIO_TypeDef* IIC_PORT, 
				GPIO_Pin_TypeDef SDA_PIN,
				GPIO_Pin_TypeDef SCL_PIN)
功能：IIC 总线等待ACK信号
参数：IIC_PORT IIC端口
	  SDA_PIN SDA端口号
	  SCL_PIN SCL端口号
返回值：无
*/
void IIC_Respons(GPIO_TypeDef* IIC_PORT,
                 GPIO_Pin_TypeDef SDA_PIN,
                 GPIO_Pin_TypeDef SCL_PIN)
{
  uint8_t wait = 0;

  SDA_IN(IIC_PORT, SDA_PIN);
  SCL_1(IIC_PORT, SCL_PIN);	  	
  DelayUs(DELAY_US);	 	
  while(GPIO_ReadInputPin(IIC_PORT,SDA_PIN) && (wait < 250))
    wait++;
  SCL_0(IIC_PORT, SCL_PIN); 	 
  DelayUs(DELAY_US);
} 

/*
函数：void IIC_SendACK(GPIO_TypeDef* IIC_PORT, 
				GPIO_Pin_TypeDef SDA_PIN,
				GPIO_Pin_TypeDef SCL_PIN,
				uint8_t ACK)
功能：IIC 总线发送ACK信号
参数：IIC_PORT IIC端口
	  SDA_PIN SDA端口号
	  SCL_PIN SCL端口号
返回值：无
*/
void IIC_SendACK(GPIO_TypeDef* IIC_PORT,
                 GPIO_Pin_TypeDef SDA_PIN,
                 GPIO_Pin_TypeDef SCL_PIN,
                 uint8_t Ack)
{
  SDA_OUT(IIC_PORT, SDA_PIN);

  if (Ack == 1)
    SDA_0(IIC_PORT, SDA_PIN);
  else
    SDA_1(IIC_PORT, SDA_PIN);

  SCL_1(IIC_PORT, SCL_PIN);
  DelayUs(DELAY_US);
  SCL_0(IIC_PORT, SCL_PIN);
  DelayUs(DELAY_US);
}
		 				     
	
/*
函数：void IIC_Send_Byte(GPIO_TypeDef* IIC_PORT, 
				GPIO_Pin_TypeDef SDA_PIN,
				GPIO_Pin_TypeDef SCL_PIN,
			    uint8_t TXdate)
功能：IIC 主机发送一个字节数据
参数：IIC_PORT IIC端口
	  SDA_PIN SDA端口号
	  SCL_PIN SCL端口号
	  TXdate 发送的数据
返回值：无
*/  	
void IIC_Send_Byte(GPIO_TypeDef* IIC_PORT,
                   GPIO_Pin_TypeDef SDA_PIN,
                   GPIO_Pin_TypeDef SCL_PIN,
                   uint8_t TXdate)
{                        
  uint8_t i, temp;   
  temp = TXdate;

  SDA_OUT(IIC_PORT, SDA_PIN);

  for(i = 0; i < 8; i++)
  {         
    SCL_0(IIC_PORT, SCL_PIN);
    DelayUs(DELAY_US); 
    if((temp << i) & 0x80)
    {
      SDA_1(IIC_PORT, SDA_PIN);
    }
    else
    {
      SDA_0(IIC_PORT, SDA_PIN);
    }	  
    DelayUs(DELAY_US);  
    SCL_1(IIC_PORT, SCL_PIN);
    DelayUs(DELAY_US); 
  }
  SCL_0(IIC_PORT, SCL_PIN);
  DelayUs(DELAY_US); 
  SDA_1(IIC_PORT, SDA_PIN);
  DelayUs(DELAY_US); 
} 	    

/*
函数：uint8_t IIC_Read_Byte(GPIO_TypeDef* IIC_PORT, 
				GPIO_Pin_TypeDef SDA_PIN,
				GPIO_Pin_TypeDef SCL_PIN,
				uint8_t Ack)
功能：IIC 主机接收一个字节数据
参数：IIC_PORT IIC端口
	  SDA_PIN SDA端口号
	  SCL_PIN SCL端口号
	  Ack 应答标志, 1应答, 0不应答
返回值：接收到的数据
*/ 
uint8_t IIC_Read_Byte(GPIO_TypeDef* IIC_PORT,
                      GPIO_Pin_TypeDef SDA_PIN,
                      GPIO_Pin_TypeDef SCL_PIN,
                      uint8_t Ack)
{
  uint8_t i, receive = 0;

  SDA_OUT(IIC_PORT, SDA_PIN);
  
  SCL_0(IIC_PORT, SCL_PIN);
  DelayUs(DELAY_US);
  SDA_1(IIC_PORT, SDA_PIN);
  DelayUs(DELAY_US);

  SDA_IN(IIC_PORT, SDA_PIN);
  for(i = 0; i < 8; i++)
  {
    SCL_1(IIC_PORT, SCL_PIN);
    DelayUs(DELAY_US);
    receive <<= 1;
    if(GPIO_ReadInputPin(IIC_PORT, SDA_PIN))
      receive++;
    SCL_0(IIC_PORT, SCL_PIN);
    DelayUs(DELAY_US); 
  }					 
  IIC_SendACK(IIC_PORT, SDA_PIN, SCL_PIN, Ack);
  return receive;
} 