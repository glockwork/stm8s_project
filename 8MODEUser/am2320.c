/*
*AM2320读取
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
名称: void I2C_AM2320_Init(void)
功能: I2C初始化, 50Kb/s
形参: 无
返回: 无
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
名称: uint8_t Get_AM2320(uint8_t *pBuffer) 
功能: 读温湿度传感器
形参: *pBuffer 缓冲区指针, 4byte
返回: 读取成功返回0, 读取失败返回1
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

//激活AM2320  
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
  
//发送获取数据指令
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
    I2C->DR = TX_Buffer[i]; //功能码
    timeout = FLAG_TIMEOUT;
    while(!(I2C->SR1 & 0x84))	//wait for Send Data OK
    {
      if((timeout--) == 0) return I2Ct0StandbyState();
    }
  }
  I2C->CR2 |= 0x02; 		//STOP
  I2C->SR1; 
  I2C->SR3;
  
  Delay(2);		//等待最少1.5ms
  
  timeout = LONG_TIMEOUT; 
  while(I2C->SR3 & 0x02)	//wait for Bus No busy
  {
    if((timeout--) == 0) return I2Ct0StandbyState();
  }
//开始读寄存器
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
    if(i == 1) //实验证明在最后一个字节前后都一样
    { 
      I2C->CR2 &= ~0x04; //ack使能     
      I2C->CR2 |= 0x02;  //停止位产生stop
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
名称: uint16_t CRC16(uint8_t *pBuffer, uint8_t len)
功能: 计算CRC16
形参: *pBuffer 数据指针
      len 数据长度
返回: CRC16值
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
名称: uint8_t CheckCRC(uint8_t *pBuffer,uint8_t len)
功能: 检测CRC是否正确
形参: *pBuffer 数据指针
      len 数据长度
返回: 校验成功返回1, 校验失败返回0
*/
uint8_t CheckCRC(uint8_t *pBuffer,uint8_t len)
{
  uint16_t crc;
  crc = (uint16_t)CRC16(pBuffer, len - 2);
  if(pBuffer[len-1] == (crc >> 8) && pBuffer[len-2] == (crc & 0x00ff)) return 1;
  else return 0;
}

/*
名称: void Clear_Buffer(uint8_t *Data)
功能: 缓冲区清零
形参: *Data 缓冲区数据指针
返回: 无
*/
void Clear_Buffer(uint8_t *Data)
{
  uint8_t i;
  for(i = 0; i < 8; i++)
    Data[i] = 0;
}