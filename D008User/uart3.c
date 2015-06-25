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
