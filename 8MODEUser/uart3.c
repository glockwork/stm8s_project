#include "uart3.h"

void UART3_Config(void)
{
  UART3_DeInit();

  UART3_Init((uint32_t)115200, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO, 
                   UART3_MODE_TXRX_ENABLE);

  /* Enable UART */
  UART3_Cmd(ENABLE);
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

void AQI2PM25(uint16_t voltage)
{
  float pm25 = 0;
  float aqi = 0;
//  aqi = ((3.35/1023)*2*voltage)*0.17 - 0.1;
  aqi = (3.347/1024 *2*voltage)*0.172 - 0.115;
  if(aqi < 0)
    aqi = 0;
  aqi *= 1000;
  
  UART3_SendByte((uint8_t)aqi/1000+0x30);
  UART3_SendByte((uint8_t)aqi%1000/100+0x30);
  UART3_SendByte((uint8_t)aqi%1000%100/10+0x30);
  UART3_SendByte((uint8_t)aqi%1000%100%10+0x30);
  
  UART3_SendByte(' ');
  
  if (aqi < 12.0)
    pm25 = 50/12.0 *(aqi);
  else if (12.0 < aqi && aqi < 35.4)
    pm25 = (100-51)/(35.4-12.1) *(aqi-12.1)+51;
  else if (35.4 < aqi && aqi < 55.4)
    pm25 = (150-101)/(55.4-35.5) *(aqi-35.5)+101;
  else if (55.4 < aqi && aqi < 150.4)
    pm25 = (200-151)/(150.4-55.5) *(aqi-55.5)+151;
  else if (150.4 < aqi && aqi < 250.4)
    pm25 = (300-201)/(250.4-150.5) *(aqi-150.5)+201;
  else if (250.4 < aqi && aqi < 350.4)
    pm25 = (400-301)/(350.4-250.5) *(aqi-250.5)+301;
  else if (350.4 < aqi && aqi < 500.4)
    pm25 = (500-401)/(500.4-350.5) *(aqi-350.5)+401;
  else if (aqi > 500.4)
    pm25 = 500;
  
   if(pm25 < 100)
  {
    UART3_SendByte((uint8_t)pm25%1000/100+0x30);
    UART3_SendByte((uint8_t)pm25%1000%100/10+0x30);
    UART3_SendByte((uint8_t)pm25%1000%100%10+0x30);
  }
  else
  {
    UART3_SendByte((uint8_t)pm25%1000/100+0x30);
    UART3_SendByte((uint8_t)pm25%1000%100/10+0x30);
    UART3_SendByte((uint8_t)pm25%1000%100%10+0x30);
  }
}

void Send_AIR_Value(uint16_t Air_Value)
{
  UART3_SendByte(Air_Value/1000+0x30);
  UART3_SendByte(Air_Value%1000/100+0x30);
  UART3_SendByte(Air_Value%1000%100/10+0x30);
  UART3_SendByte(Air_Value%1000%100%10+0x30);
}

void Send_BAT_Voltage(uint16_t AD_Value)
{
  uint8_t str[4], i= 0;
  float val;
  val = 334.7/1024 * AD_Value ;
  
  str[0] = (int)val /100 + 0x30;
  str[1] = '.';
  str[2] = (int)val % 100 / 10 + 0x30;
  str[3] = (int)val % 10 + 0x30;
  
  for (; i < 4; i++)
    UART3_SendByte(str[i]);
  UART3_SendByte('V');
}