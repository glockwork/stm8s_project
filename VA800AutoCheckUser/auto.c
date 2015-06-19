#include "auto.h"

void AutoCheckFunction(void)
{
  CLK_Config(); 
  GPIO_Config();
//while(1);
//  if ((RESET == GPIO_ReadInputPin(GPIOE, GPIO_PIN_5)) && \
      (RESET == GPIO_ReadInputPin(GPIOE, GPIO_PIN_0)))
//  {
    uint16_t battery = 0;
    uint32_t index = 0, Counter = 0, i;
    uint8_t set_Time_Date[7] = {0x59, 0x59, 0x23, 0x31, 0x02, 0x03, 0x15};
    uint8_t get_Time_Date[7] = {0, 0, 0, 0, 0, 0, 0};
    uint8_t check = 0;
    //uint16_t air_value = 0;
    uint8_t Uart_RX_Buffer[64];
    
    I2C_RTC_Init();
    At24cxx_Init();
    TIM1_Config();
    UART_Config();
    ADC_Config();
    
    enableInterrupts();
    
    //Start
    UART3_SendString("���Կ�ʼ\r\n", sizeof("���Կ�ʼ\r\n")-1);
    GPIO_WriteHigh(GPIOB, GPIO_PIN_1);  //���ת12V
    UART3_SendString("��ع�����...\r\n", sizeof("��ع�����...\r\n")-1);
    
    //���ڴ�ӡ��ص�ѹ
    UART3_SendString("��ص�ѹ:", sizeof("��ص�ѹ:")-1);
    battery = Get_ADC_Value(ADC2_CHANNEL_9);      //����ص�ѹ 
    Send_BAT_Voltage(battery);
   
    //������DC    
    UART3_SendString("\r\n�����DC\r\n", sizeof("\r\n�����DC\r\n")-1);
    while(GPIO_ReadInputPin(GPIOB, GPIO_PIN_7));
    UART3_SendString("DC������...\r\n", sizeof("DC������...\r\n")-1);
    
    

    //����RTC
    if(Set_RT8563(set_Time_Date, 0x02, 7))
      UART3_SendString("RTCͨ�ų���\r\n", sizeof("RTCͨ�ų���\r\n")-1);
    if(Read_RT8563(get_Time_Date, 0x02, 7))
      UART3_SendString("RTCͨ�ų���\r\n", sizeof("RTCͨ�ų���\r\n")-1);
    for (i = 0; i < 7; i++)
    {
      if (set_Time_Date[i] != get_Time_Date[i])
      {
        check = 1;
        break;
      }
    }
    
    //temp=AT24CXX_ReadOneByte(255);
    if (!check)
      UART3_SendString("RTCУ��OK\r\n", sizeof("RTCУ��OK\r\n")-1);
    else
      UART3_SendString("RTCУ�����\r\n", sizeof("RTCУ�����\r\n")-1);
    
    if (AT24CXX_Check())
      UART3_SendString("EEPROMУ�����\r\n", sizeof("EEPROMУ�����\r\n")-1);
    else
      UART3_SendString("EEPROMУ��OK\r\n", sizeof("EEPROMУ��OK\r\n")-1);
    
    //����ָʾ��
    UART3_SendString("LED����,�밴������\r\n", sizeof("LED����,�밴������\r\n")-1);
    while(GPIO_ReadInputPin(GPIOE, GPIO_PIN_5));
    GPIO_WriteHigh(GPIOE, GPIO_PIN_4);
    while(!GPIO_ReadInputPin(GPIOE, GPIO_PIN_5));
    GPIO_WriteLow(GPIOE, GPIO_PIN_4);
    TIM1_SetCompare(1, 999);
    UART3_SendString("���", sizeof("���")-1);
    
    while(GPIO_ReadInputPin(GPIOE, GPIO_PIN_5));
    GPIO_WriteHigh(GPIOE, GPIO_PIN_4);
    while(!GPIO_ReadInputPin(GPIOE, GPIO_PIN_5));
    GPIO_WriteLow(GPIOE, GPIO_PIN_4);
    TIM1_SetCompare(2, 999);
    UART3_SendString("->�̵�", sizeof("->�̵�")-1);
    
    while(GPIO_ReadInputPin(GPIOE, GPIO_PIN_5));
    GPIO_WriteHigh(GPIOE, GPIO_PIN_4);
    while(!GPIO_ReadInputPin(GPIOE, GPIO_PIN_5));
    GPIO_WriteLow(GPIOE, GPIO_PIN_4);
    TIM1_SetCompare(3, 999);
    UART3_SendString("->�Ƶ�", sizeof("->�Ƶ�")-1);
   
    while(GPIO_ReadInputPin(GPIOE, GPIO_PIN_5));
    GPIO_WriteHigh(GPIOE, GPIO_PIN_4);
    while(!GPIO_ReadInputPin(GPIOE, GPIO_PIN_5));
    GPIO_WriteLow(GPIOE, GPIO_PIN_4);
    TIM1_SetCompare(3, 0);
    UART3_SendString("->ָʾ��ȫ��\r\n", sizeof("->ָʾ��ȫ��\r\n")-1);
    
    UART3_SendString("���Ȳ���,����\r\n", sizeof("���Ȳ���,����\r\n")-1);
    GPIO_WriteHigh(GPIOF, GPIO_PIN_5);
    TIM1_SetCompare1(599);
    UART3_SendString("�뽫�����ŵ������������÷��ȸ�������\r\n", \
             sizeof("�뽫�����ŵ������������÷��ȸ�������\r\n")-1);
    while(!GPIO_ReadInputPin(GPIOD, GPIO_PIN_0));
    while(GPIO_ReadInputPin(GPIOD, GPIO_PIN_0));
    TIM1_SetCompare1(999);
    UART3_SendString("���Ȳ���,����\r\n", sizeof("���Ȳ���,����\r\n")-1);
    
    while(GPIO_ReadInputPin(GPIOD, GPIO_PIN_0));
    GPIO_WriteHigh(GPIOE, GPIO_PIN_4);
    UART3_SendString("�ƿ�����������һ������\r\n", sizeof("�ƿ�����������һ������\r\n")-1);
    while(!GPIO_ReadInputPin(GPIOD, GPIO_PIN_0));
    GPIO_WriteLow(GPIOE, GPIO_PIN_4);
    TIM1_SetCompare1(0);
    GPIO_WriteLow(GPIOF, GPIO_PIN_5);
    
    //��ȡGPRS��������򿪸�����
    GPIO_WriteHigh(GPIOB, GPIO_PIN_6);
    //�򿪸�����
    UART3_SendString("�������ѿ���\r\n", sizeof("�������ѿ���\r\n")-1); 

    /*
    //������
    UART3_SendString("��ǰ����������Ϊ:", sizeof("��ǰ����������Ϊ:")-1);
    air_value = Get_DS_Value();
    Send_AIR_Value(air_value);
*/
    //GPRS, ��Ҫ�ȴ�GPRSģ������,���5��8��
    UART1_SendString("ATV1\r\n", sizeof("ATV1\r\n")-1); 
    for(i=0; i<1000000; i++);
    UART1_SendString("ATE1\r\n", sizeof("ATE1\r\n")-1); 
    for(i=0; i<1000000; i++);
    UART1_SendString("AT+CMEE=2\r\n", sizeof("AT+CMEE=2\r\n")-1); 
    for(i=0; i<1000000; i++);
    UART1_SendString("AT+IPR=115200\r\n", sizeof("AT+IPR=115200\r\n")-1); 

    
    
    UART3_SendString("����GPRS�ź�ֵ��ȡ״̬,������������ȡ�ź�ֵ,����Reset������������\r\n", \
             sizeof("����GPRS�ź�ֵ��ȡ״̬,������������ȡ�ź�ֵ,����Reset������������\r\n")-1);
   
    while(GPIO_ReadInputPin(GPIOE, GPIO_PIN_5));
    GPIO_WriteHigh(GPIOF, GPIO_PIN_7);
    while(!GPIO_ReadInputPin(GPIOE, GPIO_PIN_5));
    GPIO_WriteLow(GPIOF, GPIO_PIN_7);

    UART3_SendString("GPRSģ���ͺ�:", sizeof("GPRSģ���ͺ�:")-1);
    UART1_SendString("ATI\r\n", 8);
    while (!UART1_GetFlagStatus(UART1_FLAG_TC))
    for (index = 0; index < 64; index++)
    {
        Counter = 0;
        while ((UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET) && (Counter != 0x4000))
        {
            Counter++;
        }

        if (Counter != 0x4000)
        {
            Uart_RX_Buffer[index] = UART1_ReceiveData8();
        }
    }
    UART3_SendString(Uart_RX_Buffer +47, 13);
    goto gprs;
    while(1)
    {
      if(!GPIO_ReadInputPin(GPIOD, GPIO_PIN_0))
      {
        UART3_SendString("���ƿ�����\r\n", sizeof("���ƿ�����\r\n")-1);
        while(!GPIO_ReadInputPin(GPIOD, GPIO_PIN_0));
        break;
      }
      while(GPIO_ReadInputPin(GPIOE, GPIO_PIN_5))
      {
        if(RESET == GPIO_ReadInputPin(GPIOE, GPIO_PIN_0))
          goto over;
      }
      GPIO_WriteHigh(GPIOF, GPIO_PIN_7);
      while(!GPIO_ReadInputPin(GPIOE, GPIO_PIN_5));
      GPIO_WriteLow(GPIOF, GPIO_PIN_7);
gprs:
      UART3_SendString("GPRS�ź�Ϊ:", sizeof("GPRS�ź�Ϊ:")-1);
      UART1_SendString("AT+CSQ\r\n", 8);
      while (!UART1_GetFlagStatus(UART1_FLAG_TC))
      for (index = 0; index < 27; index++)
      {
          Counter = 0;
          while ((UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET) && (Counter != 0x4000))
          {
              Counter++;
          }

          if (Counter != 0x4000)
          {
              Uart_RX_Buffer[index] = UART1_ReceiveData8();
          }
      }
//      UART3_SendByte(Uart_RX_Buffer[15]);
      UART3_SendByte(Uart_RX_Buffer[16]);
      UART3_SendByte(Uart_RX_Buffer[17]);
      UART3_SendByte(Uart_RX_Buffer[18]);
      UART3_SendByte(Uart_RX_Buffer[19]);
      UART3_SendByte(Uart_RX_Buffer[20]);
      UART3_SendByte(Uart_RX_Buffer[21]);
      //UART3_SendString("\r\n", 2);
      Uart_RX_Buffer[16] = 0;
      Uart_RX_Buffer[17] = 0;
      Uart_RX_Buffer[18] = 0;
      Uart_RX_Buffer[19] = 0;
      Uart_RX_Buffer[20] = 0;
      Uart_RX_Buffer[21] = 0;
    }
over:
    GPIO_WriteLow(GPIOB, GPIO_PIN_6);
    GPIO_WriteLow(GPIOB, GPIO_PIN_1);
    UART3_SendString("���Խ���\r\n************************************************\r\n", \
             sizeof("���Խ���\r\n************************************************\r\n")-1);
//  }
}
void CLK_Config(void)
{
    /* Initialization of the clock */
    /* Clock divider to HSI/1 */
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}
  
void ADC_Config(void)
{
  ADC2_DeInit();
  ADC2_Init(ADC2_CONVERSIONMODE_CONTINUOUS, ADC2_CHANNEL_9, ADC2_PRESSEL_FCPU_D8, \
            ADC2_EXTTRIG_TIM, DISABLE, ADC2_ALIGN_RIGHT, ADC2_SCHMITTTRIG_CHANNEL9,\
            DISABLE);
  ADC2_ITConfig(DISABLE);
  ADC2_Cmd(ENABLE);
  ADC2_StartConversion();
}

void I2C_RTC_Init(void)
{
  I2C_Cmd(DISABLE);
  I2C_DeInit();
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
  GPIO_Init(GPIOE, GPIO_PIN_1, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOE, GPIO_PIN_2, GPIO_MODE_IN_PU_NO_IT);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
  I2C_Cmd( ENABLE);
  I2C_Init(400000, 0xA0, I2C_DUTYCYCLE_2, I2C_ACK_CURR, 
           I2C_ADDMODE_7BIT, 16);
}

uint8_t Read_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToWrite) 
{  
  uint32_t timeout;
  timeout = LONG_TIMEOUT;
  while(I2C->SR3 & 0x02)
  {
    if((timeout--) == 0) return 1;
  }
  I2C->CR2 |= 0x01;  
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x01))
  {
    if((timeout--) == 0) return 1;
  }
  I2C->DR = 0xA2;  
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x02))
  {
    if((timeout--) == 0) return 1;
  }
  I2C->SR1; 
  I2C->SR3; 
  I2C->DR = (uint8_t)(index); 
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x84))
  {
    if((timeout--) == 0) return 1;
  }
  I2C->CR2 |= 0x01;  
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x01))
  {
    if((timeout--) == 0) return 1;
  }
  I2C->DR = 0xA3;
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x02))
  {
    if((timeout--) == 0) return 1;
  }
  I2C->SR1; 
  I2C->SR3; 

  while(NumByteToWrite)  
  {
    if(NumByteToWrite == 1) 
    { 
      I2C->CR2 &= ~0x04;   
      I2C->CR2 |= 0x02;  
    } 
    if(I2C->SR1 & 0x40)
    {    
      *pBuffer=I2C->DR;
      pBuffer++;  
      NumByteToWrite--;     
    }      
  }
  I2C->CR2 |= 0x04;
  return 0;
}

uint8_t Set_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToRead) 
{  
  uint32_t timeout;
  timeout = LONG_TIMEOUT;
  while(I2C->SR3 & 0x02)
  {
    if((timeout--) == 0) return 1;
  }
  I2C->CR2 |= 0x01; 
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x01))
  {
    if((timeout--) == 0) return 1;
  }
  I2C->DR = 0xA2; 
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x02))
  {
    if((timeout--) == 0) return 1;
  }
  I2C->SR1; 
  I2C->SR3; 
  I2C->DR = (uint8_t)(index);
  timeout = FLAG_TIMEOUT;
  while(!(I2C->SR1 & 0x84))
  {
    if((timeout--) == 0) return 1;
  }
  while(NumByteToRead)  
  {    
    I2C->DR = *pBuffer;   
    timeout = LONG_TIMEOUT;
    while(!(I2C->SR1 & 0x04))
    {
    if((timeout--) == 0) return 1;
    }
    pBuffer++;
    NumByteToRead--; 
  }
  I2C->CR2 |= 0x02; 
  (void)I2C->SR1;
  (void)I2C->SR3;  
  return 0;
}

void TIM1_Config(void)
{
  TIM1_DeInit();
  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, 999, 0);
  TIM1_OC1Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE,
               CCR1_Val, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
               TIM1_OCNIDLESTATE_RESET); 

  /*TIM1_Pulse = CCR2_Val Red LED*/
  TIM1_OC2Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, CCR2_Val,
               TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET, 
               TIM1_OCNIDLESTATE_RESET);
#if LED_8_PCS
  /*TIM1_Pulse = CCR3_Val Green LED*/
  TIM1_OC3Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE,
               CCR3_Val, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
               TIM1_OCNIDLESTATE_RESET);
#endif
  /*TIM1_Pulse = CCR4_Val Yellow LED*/
  TIM1_OC4Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, CCR4_Val, TIM1_OCPOLARITY_LOW,
               TIM1_OCIDLESTATE_SET);

  TIM1_Cmd(ENABLE);
  TIM1_CtrlPWMOutputs(ENABLE);
}

uint8_t TIM1_SetCompare(uint8_t nPort, uint32_t nCCR_Val)
{
  if (nPort >0 && nPort < 4)
  {
    switch (nPort)
    {
      case 1: TIM1_SetCompare2(nCCR_Val); 
              TIM1_SetCompare4(0); 
              break;
      case 2: TIM1_SetCompare4(nCCR_Val);
              TIM1_SetCompare2(0); 
              break;
      case 3: TIM1_SetCompare2(nCCR_Val); 
              TIM1_SetCompare4(nCCR_Val); 
              break; 
    }
    return 1;
  }
  else
    return 0;
}

void GPIO_Config(void)
{
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD);
  GPIO_DeInit(GPIOE);
  GPIO_DeInit(GPIOF);

  /* Initialize I/Os in Output Mode */
  GPIO_Init(GPIOE, (GPIO_Pin_TypeDef)GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);    //����ָʾ��
  GPIO_Init(GPIOF, (GPIO_Pin_TypeDef)GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST);    //����ָʾ��
  GPIO_Init(GPIOF, (GPIO_Pin_TypeDef)GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);    //���ȵ�Դ
  GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);    //GPRS��Դ
  GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)GPIO_PIN_1, GPIO_MODE_OUT_PP_LOW_FAST);    //���ת12V
  GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);    //������
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);    //PM2.5 LED
  GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);    //PM2.5��Դ
  GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);        //DC����
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_0, GPIO_MODE_IN_PU_NO_IT);        //��������
  GPIO_Init(GPIOE, (GPIO_Pin_TypeDef)GPIO_PIN_0, GPIO_MODE_IN_PU_NO_IT);        //������λ
  GPIO_Init(GPIOE, (GPIO_Pin_TypeDef)GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT);        //��������
  
  
  /*  Init GPIO for ADC2 */
  GPIO_Init(GPIOE, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);  //battery AIN9
  GPIO_Init(GPIOE, GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);  //pm2.5   AIN8
  
  GPIO_WriteHigh(GPIOB, GPIO_PIN_1);
  GPIO_WriteHigh(GPIOB, GPIO_PIN_0);
  GPIO_WriteHigh(GPIOC, GPIO_PIN_6);

}

uint16_t Get_ADC_Value(ADC2_Channel_TypeDef channel)
{
  uint16_t i, j, Conversion_Value = 0;
  
  ADC2_ConversionConfig(ADC2_CONVERSIONMODE_SINGLE, channel, ADC2_ALIGN_RIGHT);
  
  for (j = 0; j < 0xFFFF; j++);
  
  for (i = 0; i < 10; i++)
  {
    ADC2_StartConversion();
    Conversion_Value += ADC2_GetConversionValue();
    for (j = 0; j < 0xFF; j++);
  }
  
  Conversion_Value /= 10;
  
  return Conversion_Value;
}

/*
uint16_t Get_DS_Value(void)
{
  uint16_t i, j, Conversion_Value = 0;
  
  ADC2_ConversionConfig(ADC2_CONVERSIONMODE_SINGLE, ADC2_CHANNEL_8, ADC2_ALIGN_RIGHT);
  
  for(j = 0; j < 0xFFFF; j++);

  for (i = 0; i < 10; i++)
  {
    GPIO_WriteHigh(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_4);
    //Delay_us(28);
    for(j = 0; j < 900; j++);
    ADC2_StartConversion();
    Conversion_Value += ADC2_GetConversionValue();
    
    //Delay_us(4);
    for(j = 0; j < 10; j++);
    GPIO_WriteLow(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_4);
    //for(j = 0; j < 968; j++);
    //Delay_us(968);
  }
  
  Conversion_Value /= 10;
  
  return Conversion_Value;
}

void Send_AIR_Value(uint16_t Air_Value)
{
//  uint16_t dustdensity;
//  dustdensity = 335.0/1024 * Air_Value * 2 * 0.17 - 0.1;
  
  UART3_SendByte(Air_Value/1000+0x30);
  UART3_SendByte(Air_Value%1000/100+0x30);
  UART3_SendByte(Air_Value%1000%100/10+0x30);
  UART3_SendByte(Air_Value%1000%100%10+0x30);
}

*/

void UART_Config(void)
{
  UART1_DeInit();
  UART3_DeInit();
  UART1_Init((uint32_t)115200, UART1_WORDLENGTH_8D,UART1_STOPBITS_1, UART1_PARITY_NO, 
                   UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

  UART3_Init((uint32_t)9600, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO, 
                   UART3_MODE_TXRX_ENABLE);

  /* Enable UART */
  UART1_Cmd(ENABLE);
  UART3_Cmd(ENABLE);
  
}

void UART1_SendByte(uint8_t Data)
{
  UART1_SendData8((unsigned char)Data);
  /* Loop until the end of transmission */
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
}

void UART1_SendString(uint8_t* Data, uint32_t len)
{
  uint32_t i=0;
  for (; i<len; i++)
    UART1_SendByte(Data[i]);
}

void UART3_SendByte(uint8_t Data)
{
  UART3_SendData8((unsigned char)Data);
  /* Loop until the end of transmission */
  while (UART3_GetFlagStatus(UART3_FLAG_TXE) == RESET);
}

void UART3_SendString(uint8_t* Data, uint32_t len)
{
  uint32_t i=0;
  for (; i<len; i++)
    UART3_SendByte(Data[i]);
}

void Send_BAT_Voltage(uint16_t AD_Value)
{
  uint8_t str[4], i= 0;
  float val;
  val = 335.0/1024 * AD_Value * 2 ;
  
  str[0] = (int)val /100 + 0x30;
  str[1] = '.';
  str[2] = (int)val % 100 / 10 + 0x30;
  str[3] = (int)val % 10 + 0x30;
  
  for (; i < 4; i++)
    UART3_SendByte(str[i]);
  UART3_SendByte('V');
}
