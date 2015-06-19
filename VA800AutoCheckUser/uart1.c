#include "uart1.h"

/* Includes ------------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 RxBuffer[RxBufferSize];
u8 UART_RX_NUM = 1;
uint8_t ConnectFail[] = " CONNECT FAIL";
uint8_t ConnectOK[] = " CONNECT OK";
uint8_t AlreadyConnect[] = " ALREADY CONNECT";
uint8_t ClosedConnect[] = " CLOSED";
uint8_t CloseOK[] = " CLOSE OK";
uint8_t OK[] = " OK";
uint8_t connecting = 0;
uint8_t Uart1_Len = 0;      //0数据长度
uint8_t Uart1_Sta = 0;      //标志位
/* Exported types ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/


/**
  * @brief  Configure UART1 for the communication with HyperTerminal
  * @param  None
  * @retval None
  */
void UART1_Config(void)
{
  /* EVAL COM (UART) configuration -----------------------------------------*/
  /* USART configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - Odd parity
        - Receive and transmit enabled
        - UART Clock disabled
  */
  UART1_Init((uint32_t)115200, UART1_WORDLENGTH_8D,UART1_STOPBITS_1, UART1_PARITY_NO,
                   UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

  /* Enable the UART Receive interrupt: this interrupt is generated when the UART
    receive data register is not empty */
  //UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
  
  /* Enable the UART Transmit complete interrupt: this interrupt is generated 
     when the UART transmit Shift Register is empty */
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);

  /* Enable UART */
  UART1_Cmd(ENABLE);
  
  /* Enable general interrupts */
//  enableInterrupts(); 
}

/**
  * @brief  Send Data.
  * @param  Data: Data.
  * @retval None
  */
void UART1_SendByte(uint8_t Data)
{
  UART1_SendData8((unsigned char)Data);
  /* Loop until the end of transmission */
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
}

/**
  * @brief  Send ADC Value.
  * @param  AD_Value: AD Data.
  * @retval None
  */
void Send_ADC_Value(uint16_t AD_Value)
{
  UART1_SendByte(AD_Value/1000+0x30);
  UART1_SendByte(AD_Value%1000/100+0x30);
  UART1_SendByte(AD_Value%1000%100/10+0x30);
  UART1_SendByte(AD_Value%1000%100%10+0x30);
}

/**
  * @brief  Send ADC Voltage.
  * @param  AD Value: AD Data.
  * @retval None
  */
void Send_ADC_Voltage(uint16_t AD_Value)
{
  uint8_t str[4], i= 0;
  uint16_t val;
  val = 335.0/1024 * AD_Value ;
  
  str[0] = val /100 + 0x30;
  str[1] = '.';
  str[2] = val % 100 / 10 + 0x30;
  str[3] = val % 10 + 0x30;
  
  for (; i < 4; i++)
    UART1_SendByte(str[i]);
  UART1_SendByte('V');
}

/**
  * @brief  Send Data.
  * @param  *Data: Data address.
  * @param  len: Data length.
  * @retval None
  */
void UART1_SendString(uint8_t* Data, uint32_t len)
{
  uint32_t i=0;
  for (; i<len; i++)
    UART1_SendByte(Data[i]);
}

uint32_t strlen(uint8_t *str)
{
  uint32_t i = 0;
  while((*str++) != '\0') i++;
  return i;
}

/**
  * @brief  Send Comand.
  * @param  *Data: Data address.
  * @retval None
  */
void UART1_SendCommand(uint8_t* Command)
{
  uint32_t i, len = strlen(Command);
  for (i = 0; i < len; i++)
  {
    UART1_SendData8((unsigned char)Command[i]);
    /* Loop until the end of transmission */
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
  }
}

/**
  * @brief  Send Comand list.
  * @param  *pCommand: Comand list address.
  * @param  pos: Command list Start Position.
  * @retval OK return 0, Fail return Position.
  */
uint8_t SendCommandList(uint8_t* pCommand[], uint32_t Len, uint8_t pos)
{
  OK[0] = sizeof(OK)/sizeof(uint8_t) -2;
  uint8_t state = 0;
  uint8_t flag = 1;
  uint32_t i = pos, j=0;
  for (; i < Len; i++)
  {
    for (j = 0; j < 3; j++)
    {
      UART1_SendCommand(pCommand[i]);
      Delay(500);//300ms
      if (Uart1_Sta)
      {
        RxBuffer[0] = Uart1_Len-2;/*得到此次接收到的数据长度*/
        state = Index_KMP(RxBuffer, OK, 1);
        if (state)
        {
          Uart1_Sta = 0;
          flag = 0;
          UART_RX_NUM = 0;
          break;
        }
        UART_RX_NUM = 0;
      }
      else
        flag = 1;
    }
    if (flag)
      return i;
  }
  return 0;
}

/**
  * @brief  Send Comand list.
  * @param  *pCommand: Comand list address.
  * @param  pos: Command list Start Position.
  * @retval OK return 0, Fail return Position.
  */
void CheckConnectStatus(uint8_t* pRxData)
{
  ConnectOK[0] = sizeof(ConnectOK)/sizeof(uint8_t) -2;
  ConnectFail[0] = sizeof(ConnectFail)/sizeof(uint8_t) -2;
  AlreadyConnect[0] = sizeof(AlreadyConnect)/sizeof(uint8_t) -2;
  ClosedConnect[0] = sizeof(ClosedConnect)/sizeof(uint8_t) -2;
  CloseOK[0] = sizeof(CloseOK)/sizeof(uint8_t) -2;
  
  uint8_t *Status[] = {ConnectOK, ConnectFail, \
                       AlreadyConnect, ClosedConnect, \
                       CloseOK};
  uint8_t state = 0;
  uint8_t i=0;
  
  for (i = 0; i < 5; i++)
  {
    state = Index_KMP(pRxData, Status[i], 1);
    if (state)
    {
      switch(i)
      {
      case 0://Connect OK
        //标记连接OK
        connecting = 1;
        break;
      case 1://ConnectFail
        connecting = 0;
        SendCommandCMP(AT_QICLOSE, CloseOK);
        break;
      case 2://AlreadyConnect
        connecting = 0;
        SendCommandCMP(AT_QICLOSE, CloseOK);
        break;
      case 3://ClosendConnect
        connecting = 0;
        SendCommandCMP(AT_QIOPEN(MODE,IP,PORT), ConnectOK);
        break;
      case 4://Close OK
        connecting = 0;
        SendCommandCMP(AT_QIOPEN(MODE,IP,PORT), ConnectOK);
        break;
      }
      break;
    }
  }
}
//发送命令，并对比返回值是否符合预期字符串。
//uint8_t* pCommand :要发送的命令。
//uint8_t* pStatus :需要对比的预期字符串。
//返回值：符合预期返回1， 不符合预期返回0.
uint8_t SendCommandCMP(uint8_t* pCommand, uint8_t* pStatus)
{
  pStatus[0] = sizeof(pStatus)/sizeof(uint8_t) -2;//需要对比的正确返回字符串
  uint8_t state, j;
  for (j = 0; j < 3; j++)
  {
    UART1_SendCommand(pCommand);
    Delay(500);//300ms
    if (Uart1_Sta)
    {
      RxBuffer[0] = Uart1_Len-2;/*得到此次接收到的数据长度*/
      state = Index_KMP(RxBuffer, pStatus, 1);
      if (state)
      {
        UART_RX_NUM = 0;
        Uart1_Sta = 0;
        return 1;
      }
      UART_RX_NUM = 0;
    }
  }
  return 0;
}

/**
  * @brief Compares two buffers.
  * @param[in] pBuffer1 First buffer to be compared.
  * @param[in] pBuffer2 Second buffer to be compared.
  * @param[in] BufferLength Buffer's length
  * @retval TestStatus Status of buffer comparison
  * - PASSED: pBuffer1 identical to pBuffer2
  * - FAILED: pBuffer1 differs from pBuffer2
  * @par Required preconditions:
  * None
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}