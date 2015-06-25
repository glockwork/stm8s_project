
#include "stm8s.h"
#include "main.h"

extern NET_RECV        NetMode;
extern uint8_t RxRecvBuffer[BUFFERSIZE];
extern uint16_t DataSize;
DEVICE_STATUS           DeviceStatus = {0};

void main(void)
{
    /* Clock configuration -----------------------------------------*/
    CLK_Config();  
    //beep
    Set_Beep_OptionByte();
    Beep_Init(BEEP_FREQUENCY_4KHZ);
    BEEP_LSICalibrationConfig(LSI_128kHz);
    /* GPIO configuration ------------------------------------------*/
    GPIO_Config_Init();
    
    TIM3_Config();
    TIM4_Config();
    UART1_Config();
    UART3_Config();
    
    enableInterrupts();
    DeviceStatus.workState = 16;
    
    showAll();
    PowerOnBeep();
    clear();
    TIM2_Config();
    
    showTemp(Temperature[DeviceStatus.workState], ON);
    showSymbol(SYMBOL_DEFAULT);
    UART3_SendString("Hello", 5);
    while (1)
    {
        if(DeviceStatus.Time_100ms == 1)
        {
            if(UART1_GetFlagStatus(UART1_FLAG_IDLE) == SET)
            {
                if(DataSize != 0)
                {
                    DataResolve(RxRecvBuffer, DataSize); 
                    DataSize = 0;
                }
            }
            DeviceStatus.Time_100ms = 0;
        }
    }
  
}

static void CLK_Config(void)
{
    /* Initialization of the clock */
    /* Clock divider to HSI/1 */
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  
    while (1)
    {
    }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
