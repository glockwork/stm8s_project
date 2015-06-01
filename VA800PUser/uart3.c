#include "uart3.h"

void UART3_Config(void)
{
    UART3_DeInit();

    UART3_Init((uint32_t)115200, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO, 
                   UART3_MODE_TXRX_ENABLE);

    UART3_ITConfig(UART3_IT_RXNE_OR, ENABLE);

    /* Enable UART */
    UART3_Cmd(ENABLE);
}

void UART3_SendByte(uint8_t Data)
{
    UART3_SendData8(Data);
    /* Loop until the end of transmission */
    while (UART3_GetFlagStatus(UART3_FLAG_TXE) == RESET);
}

void UART3_SendString(uint8_t* Data, uint32_t len)
{
    uint32_t i=0;
    for (; i<len; i++) UART3_SendByte(Data[i]);
}

void AQI2PM25(uint16_t voltage)
{
    static uint16_t PM_V0 = 80;
    
    float aqi = 0;
    Send_AIR_Value(voltage);
    aqi = (3.347/1023 *2*600)*(voltage - PM_V0);

    UART3_SendByte((uint16_t)aqi /100 %10 + 0x30);
    UART3_SendByte((uint16_t)aqi /10 %10 + 0x30);
    UART3_SendByte((uint16_t)aqi %10 + 0x30);
}

void Send_AIR_Value(uint16_t Air_Value)
{
    UART3_SendByte(Air_Value /1000 % 10 + 0x30);
    UART3_SendByte(Air_Value /100 % 10 + 0x30);
    UART3_SendByte(Air_Value /10 % 10 + 0x30);
    UART3_SendByte(Air_Value %10 + 0x30);
}

void Send_BAT_Voltage(uint16_t AD_Value)
{
    uint8_t str[4], i= 0;
    float val;
    val = 334.7/1023 * AD_Value ;

    str[0] = (int)val /100 % 10 + 0x30;
    str[1] = '.';
    str[2] = (int)val / 10 % 10 + 0x30;
    str[3] = (int)val % 10 + 0x30;

    for (; i < 4; i++) UART3_SendByte(str[i]);
    UART3_SendByte('V');
}