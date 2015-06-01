/*
*AM2320��ȡ
*/
#include "am2320.h"

void I2C_AM2320_DeInit(void)
{
  /* sEE_I2C Peripheral Disable */
  I2C_Cmd(DISABLE);

  /* sEE_I2C DeInit */
  I2C_DeInit();

  /*!< sEE_I2C Peripheral clock disable */
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);

  /*!< GPIO configuration */
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_Init(GPIOE, GPIO_PIN_1, GPIO_MODE_IN_PU_NO_IT);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_Init(GPIOE, GPIO_PIN_2, GPIO_MODE_IN_PU_NO_IT);
}

/*
����: void I2C_AM2320_Init(void)
����: I2C��ʼ��, 50Kb/s
�β�: ��
����: ��
*/
void I2C_AM2320_Init(void)
{  
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
  uint8_t Input_Clock = 0x0;
  /* Get system clock frequency */
  Input_Clock = CLK_GetClockFreq()/1000000;	
  /* I2C Peripheral Enable */
  I2C_Cmd( ENABLE);
  /* Apply I2C configuration after enabling it */
  I2C_Init(I2C_SPEED, I2C_SLAVE_ADDRESS7, I2C_DUTYCYCLE_2, I2C_ACK_CURR, 
           I2C_ADDMODE_7BIT, Input_Clock);
}

/*
����: uint8_t Get_AM2320(uint8_t *pBuffer) 
����: ����ʪ�ȴ�����
�β�: *pBuffer ������ָ��, 4byte
����: ��ȡ�ɹ�����0, ��ȡʧ�ܷ���1
*/
uint8_t Get_AM2320(uint8_t *pBuffer) 
{  
  uint8_t i, TX_Buffer[3] = {0x03, 0x00, 0x04};
  uint32_t timeout;
  
  timeout = LONG_TIMEOUT; 
  while(I2C->SR3 & 0x02)	//wait for Bus No busy
  {
    if((timeout--) == 0) return I2Ct0StandbyState();
  }

//����AM2320  
  I2C->CR2 |= 0x01;  		//Start
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x01))	//wait for Start Send OK
  {
    if((timeout--) == 0) return I2Ct0StandbyState();
  }
  I2C->DR = 0xB8;  			//Addr+W
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x02))	//wait for Send Addr OK
  {
    if((timeout--) == 0) break;
  }
  I2C->SR1; 
  I2C->SR3;
  Delay(2);
  I2C->CR2 |= 0x02; 		//STOP
  
//���ͻ�ȡ����ָ��
  timeout = LONG_TIMEOUT; 
  while(I2C->SR3 & 0x02)	//wait for Bus No busy
  {
    if((timeout--) == 0) return I2Ct0StandbyState();
  }
  
  I2C->CR2 |= 0x01;  	//S
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x01))	//wait S OK
  {
    if((timeout--) == 0) return I2Ct0StandbyState();
  }
  I2C->DR = 0xB8;  			//Addr+W
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x02)) //wait for Send Addr OK
  {
    if((timeout--) == 0) return I2Ct0StandbyState();
  }
  I2C->SR1; 
  I2C->SR3; 
  
  for(i = 0; i < 3; i++)
  {
    I2C->DR = TX_Buffer[i]; //������
    timeout = FLAG_TIMEOUT;
    while(!(I2C->SR1 & 0x84))	//wait for Send Data OK
    {
      if((timeout--) == 0) return I2Ct0StandbyState();
    }
  }
  I2C->CR2 |= 0x02; 		//STOP
  I2C->SR1; 
  I2C->SR3;
  
  Delay(2);		//�ȴ�����1.5ms
  
  timeout = LONG_TIMEOUT; 
  while(I2C->SR3 & 0x02)	//wait for Bus No busy
  {
    if((timeout--) == 0) return I2Ct0StandbyState();
  }
//��ʼ���Ĵ���
  I2C->CR2 |= 0x01;  		//S
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x01)) //wait S OK
  {
    if((timeout--) == 0) return I2Ct0StandbyState();
  }
  I2C->DR = 0xB9;		//Addr+R
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x02)) //wait for Send Addr OK
  {
    if((timeout--) == 0) return I2Ct0StandbyState();
  }
  I2C->SR1; 
  I2C->SR3; 
  
  Delay(1);
  
  i = 8;
  while(i)  
  {
    if(i == 1) //ʵ��֤�������һ���ֽ�ǰ��һ��
    { 
      I2C->CR2 &= ~0x04; //ackʹ��     
      I2C->CR2 |= 0x02;  //ֹͣλ����stop
    } 
    
    if(I2C->SR1 & 0x40)
    {    
      *pBuffer = I2C->DR;
      pBuffer++;  
      i--;     
    }      
  }

  I2C->CR2 |= 0x04;
  return 0;
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
����: uint16_t CRC16(uint8_t *pBuffer, uint8_t len)
����: ����CRC16
�β�: *pBuffer ����ָ��
      len ���ݳ���
����: CRC16ֵ
*/	
uint16_t CRC16(uint8_t *pBuffer, uint8_t len)
{
  uint16_t crc = 0xffff;
  uint8_t i;
  while(len--)
  {
    crc ^= *pBuffer++;
    for(i = 0; i < 8; i++)
    {
      if(crc & 0x1)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else crc >>= 1;
    }
  }
  return crc;
}

/*
����: uint8_t CheckCRC(uint8_t *pBuffer,uint8_t len)
����: ���CRC�Ƿ���ȷ
�β�: *pBuffer ����ָ��
      len ���ݳ���
����: У��ɹ�����1, У��ʧ�ܷ���0
*/
uint8_t CheckCRC(uint8_t *pBuffer,uint8_t len)
{
  uint16_t crc;
  crc = (uint16_t)CRC16(pBuffer, len - 2);
  if(pBuffer[len-1] == (crc >> 8) && pBuffer[len-2] == (crc & 0x00ff)) return 1;
  else return 0;
}

/*
����: void Clear_Buffer(uint8_t *Data)
����: ����������
�β�: *Data ����������ָ��
����: ��
*/
void Clear_Buffer(uint8_t *Data)
{
  uint8_t i;
  for(i = 0; i < 8; i++)
    Data[i] = 0;
}