/*
* GPIOģ��IIC
* 2015��04��20�� ��VA800 V1.5���ϵ���OK
* by Hellogz
*/
#include "gpio2iic.h" 

/*
������void DelayUs(uint16_t time) 
���ܣ�΢�뼶�ӳٺ���
������time �ӳ�ʱ��
����ֵ����
*/
void DelayUs(uint16_t time) 
{ 
  uint16_t i, j;
  for(i = 0; i < time; i++)
    for(j = 0; j < 4; j++);
}

/*
������void IIC_Init(GPIO_TypeDef* IIC_PORT, 
				GPIO_Pin_TypeDef SDA_PIN,
				GPIO_Pin_TypeDef SCL_PIN)
���ܣ�IIC ��ʼ��
������IIC_PORT IIC�˿�
	  SDA_PIN SDA�˿ں�
	  SCL_PIN SCL�˿ں�
����ֵ����
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
������void IIC_Start(GPIO_TypeDef* IIC_PORT, 
				GPIO_Pin_TypeDef SDA_PIN,
				GPIO_Pin_TypeDef SCL_PIN)
���ܣ�IIC ���߲�����ʼ�ź�
������IIC_PORT IIC�˿�
	  SDA_PIN SDA�˿ں�
	  SCL_PIN SCL�˿ں�
����ֵ����
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
������void IIC_Stop(GPIO_TypeDef* IIC_PORT, 
				GPIO_Pin_TypeDef SDA_PIN,
				GPIO_Pin_TypeDef SCL_PIN)
���ܣ�IIC ���߲���ֹͣ�ź�
������IIC_PORT IIC�˿�
	  SDA_PIN SDA�˿ں�
	  SCL_PIN SCL�˿ں�
����ֵ����
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
������void IIC_Respons(GPIO_TypeDef* IIC_PORT, 
				GPIO_Pin_TypeDef SDA_PIN,
				GPIO_Pin_TypeDef SCL_PIN)
���ܣ�IIC ���ߵȴ�ACK�ź�
������IIC_PORT IIC�˿�
	  SDA_PIN SDA�˿ں�
	  SCL_PIN SCL�˿ں�
����ֵ����
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
������void IIC_SendACK(GPIO_TypeDef* IIC_PORT, 
				GPIO_Pin_TypeDef SDA_PIN,
				GPIO_Pin_TypeDef SCL_PIN,
				uint8_t ACK)
���ܣ�IIC ���߷���ACK�ź�
������IIC_PORT IIC�˿�
	  SDA_PIN SDA�˿ں�
	  SCL_PIN SCL�˿ں�
����ֵ����
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
������void IIC_Send_Byte(GPIO_TypeDef* IIC_PORT, 
				GPIO_Pin_TypeDef SDA_PIN,
				GPIO_Pin_TypeDef SCL_PIN,
			    uint8_t TXdate)
���ܣ�IIC ��������һ���ֽ�����
������IIC_PORT IIC�˿�
	  SDA_PIN SDA�˿ں�
	  SCL_PIN SCL�˿ں�
	  TXdate ���͵�����
����ֵ����
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
������uint8_t IIC_Read_Byte(GPIO_TypeDef* IIC_PORT, 
				GPIO_Pin_TypeDef SDA_PIN,
				GPIO_Pin_TypeDef SCL_PIN,
				uint8_t Ack)
���ܣ�IIC ��������һ���ֽ�����
������IIC_PORT IIC�˿�
	  SDA_PIN SDA�˿ں�
	  SCL_PIN SCL�˿ں�
	  Ack Ӧ���־, 1Ӧ��, 0��Ӧ��
����ֵ�����յ�������
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