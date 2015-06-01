#include "uart1.h"

/* Includes ------------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

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
    UART1_SendData8((uint8_t)Data);
    /* Loop until the end of transmission */
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
}



/**
  * @brief  Send Data.
  * @param  *Data: Data address.
  * @param  len: Data length.
  * @retval None
  */
void UART1_SendString(uint8_t* Data, uint32_t len)
{
    int32_t i=0;
    for (; i<len; i++) UART1_SendByte(Data[i]);
}
