
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
    /* GPIO configuration ------------------------------------------*/
    GPIO_Config_Init();
    UART1_Config();
    UART3_Config();
    PWM_Config();
    TIM2_Config();
    TIM3_Config();
    TIM4_Config();
    
    I2C_RTC_Init();
    /* Enable general interrupts */  
    enableInterrupts();
                  //【秒, 分, 时, 日, 星期, 月, 年】
    uint8_t time[] = {00, 12, 10, 1, 1, 6, 15};
    //Set_RT8563(time, 2, 7);
    ResetNetMode();
#if 0
    uint8_t Alarm[5][3] = {1, 16, 31, 1, 16, 33, 1, 16, 35};
    uint8_t Time[7] = {0};
    uint8_t AlarmState = 0;
    uint8_t AlarmDelay = 1;
    unsigned int NET_AUTO_SEND = 0;
#endif
    
    while (1)
    {
#if 0
        Delay(1000);
        Send_BAT_Voltage(Get_BAT_Value());
        UART3_SendString("\n", 1);
        AQI2PM25(Get_DS_Value());
#endif      
        if(DeviceStatus.Time_30ms == 1)
        {
            TouchKey_Read();
            DeviceStatus.Time_30ms = 0;
        }
        if(DeviceStatus.Time_100ms == 1)
        {
            if(UART1_GetFlagStatus(UART1_FLAG_IDLE) == SET)
            {
                if(DataSize != 0)
                {
                    DataResolve(RxRecvBuffer, DataSize); 
                    NetProcess();
                    DataSize = 0;
                }
            }
            DeviceStatus.Time_100ms = 0;
        }
        if(DeviceStatus.Time_1_s == 1)
        {
            Get_RT8563(time, 2, 7);
#if 0
            ArrayCopy((uint8_t *)&NetMode.SendData, time, 7);
            UART3_SendString((uint8_t *)&NetMode.SendData, 7);
            if(NetMode.Status & NET_CONNECT)    
            {
                NET_LED_FLASH;
                NET_AUTO_SEND++;
                if(NET_AUTO_SEND == 10)//自动发送数据
                {
                    NetSendDataLength();
                    
                    NET_AUTO_SEND = 0;
                }
            }
            else NET_AUTO_SEND = 0;
#endif
            
            DeviceStatus.Time_1_s = 0;
        }

        if(DeviceStatus.Time_30_s == 1)
        {
            NetModeErrorFix();
            DeviceStatus.Time_30_s = 0;
        }
#if 0
        GetTime(Time);
        if(ArrayCMP(Alarm, Time, 3) == 0 && AlarmState == 0) 
        {
            GPIO_WriteHigh(GPIOF, GPIO_PIN_5);
            FAN_SPEED_HIGH; //高速
            Alarm[2] += AlarmDelay;
            AlarmState = 1;
        }
        else if(ArrayCMP(Alarm, Time, 3) == 0 && AlarmState == 1)
        {
            GPIO_WriteLow(GPIOF, GPIO_PIN_5);
            FAN_SPEED_OFF;
            AlarmState = 0;
            Alarm[2] -= AlarmDelay;
        }
#endif
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
