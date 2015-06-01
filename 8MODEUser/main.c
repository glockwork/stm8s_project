#include "main.h"

DATA_STATUS     DataStatus;
TIME_COUNT      TimeCount;
unsigned char PM_V0 = 100, old_PM_V0 = 100;     //0.65V
float    Rmax = 4.8*10000/(3.3/1023*224) - 10000, old_Rmax = 4.8*10000/(3.3/1023*224) - 10000;


//��ʾ����ʾ��������
void LCD_ShowAll(void)
{
  if(DataStatus.status & LED_ON)
  {
    if(DataStatus.TVOC_Level < 4)
    {
        if(DataStatus.PM_Level == 1)
          LED_Backlit(GREEN);  //��
        else if(DataStatus.PM_Level == 2)
          LED_Backlit(BLUE);  //һ��
        else if(DataStatus.PM_Level == 3)
          LED_Backlit(RED);  //��  
    }
    else LED_Backlit(RED);  //�� 
  }
  
//  if(DataStatus.status & TEMP_GET)
//  {
    LCD_ShowWenDu(DataStatus.temp_data);
    LCD_ShowShiDu(DataStatus.rh_data);
//    DataStatus.status &= ~TEMP_GET; 
//  }

  LCD_ShowPM(DataStatus.pm_data);
//  if(DataStatus.KeyMode == POWERMODE)
#if(MODE == TVOC)
  LCD_ShowTVOC(DataStatus.tvoc_data);
#else
  if(DataStatus.KeyMode != SETMODE)
  {
    LCD_ShowTime_Hour(TimeData[2]);
    LCD_ShowTime_Minute(TimeData[1]);
  }
#endif
  LCD_ShowAQI(DataStatus.pm_data);
  if((DataStatus.status & DC_INPUT) && (DataStatus.battery_data < 652))    //DC���� �� ��ص�ѹ С��4.2V
  {
    DataStatus.battery_count++;
    if(DataStatus.battery_count == 4)
      DataStatus.battery_count = 0;
    LCD_ShowBatteryCharge(DataStatus.battery_count);
  }
  else
    LCD_ShowBattery(DataStatus.battery_data); 
  LCD_ShowBeep(DataStatus.status & BEEP_ON);
  //LCD_ShowON(DataStatus.status & STATUS_ON);
}

//��ȡ����������
void Get_Sensor(void)
{
#if(MODE == TIMER)
  Get_RT8563(RTC_ROPT, RTC_SDA_PIN, RTC_SCL_PIN, TimeData, 2, 7);
#endif
  DataStatus.pm_data = Get_DS_Value();
  if(DataStatus.pm_data < PM_V0 && DataStatus.pm_data > 62)     // 0.4V
  {
    old_PM_V0 = DataStatus.pm_data;
    DataStatus.pm_data = PM_V0;
  }
  else if (DataStatus.pm_data <= 62) DataStatus.pm_data = PM_V0;
  
  DataStatus.tvoc_data = Get_TVOC_Value();
  DataStatus.battery_data = Get_BAT_Value();  
  
  if((3.3/1023*2)*(DataStatus.pm_data - PM_V0)*600 < AQI_LEVEL_3)
    DataStatus.PM_Level = 1;  //��
  else if((3.3/1023*2)*(DataStatus.pm_data - PM_V0)*600 < AQI_LEVEL_4)
    DataStatus.PM_Level = 2;  //һ��
  else
    DataStatus.PM_Level = 3;  //��  
}

//��ȡ��ʪ��
void Get_Temp(void)
{
  uint8_t recv[8];
  
  if(!Get_AM2320(recv)) //��ȡ���ݳɹ���У����ȷ��ʾ����ֵ��У��ʧ����ʾ�㡣
  {
    if(CheckCRC(recv, 8))
    {
//      DataStatus.status |= TEMP_GET;
      DataStatus.rh_data = recv[2] * 256 + recv[3];
      DataStatus.temp_data = (recv[4] & 0x03) * 256 + recv[5];
    }
    //else DataStatus.status &= ~TEMP_GET;
    else 
    {
      DataStatus.rh_data = 0;
      DataStatus.temp_data = 0;
    }
  }
#if 1
  else//��ȡʧ�ܣ���ʪ����ʾ0
  {
    I2C_AM2320_DeInit();
    Delay(100);
    I2C_AM2320_Init();
    DataStatus.status |= TEMP_GET;
    DataStatus.rh_data = 0;
    DataStatus.temp_data = 0;
  }
#endif
}



void main(void)
{  
  uint8_t temp;
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  
  I2C_AM2320_Init();
  
  DataStatus.SetTime = SET_MINUTE; 

  
#if(MODE == TIMER)
  IIC_Init(RTC_ROPT, RTC_SDA_PIN, RTC_SCL_PIN);
#if DEBUG   
  Set_RT8563(RTC_ROPT, RTC_SDA_PIN, RTC_SCL_PIN, TimeData, 2, 7); //��ʼ��RTCʱ��       
#endif
  Get_RT8563(RTC_ROPT, RTC_SDA_PIN, RTC_SCL_PIN, TimeData, 2, 7); //�ָ�RTCʱ��  
#endif
  GPIO_Config_Init();
  
  BasicTimer_Init();
  
  IWDG_Config();
  
  enableInterrupts();
   
 //�ָ�V0ֵ 
//  clear_flash_data(FLASH_ADDRESS, 1);
  get_data(FLASH_ADDRESS, &temp, 1);
  if(temp == 0) 
  {
    temp = Get_DS_Value();
    if(temp > 0 && temp <= PM_V0) PM_V0 = temp;
    DataStatus.pm_data = PM_V0+1;
  }
  else PM_V0 = temp;
  DataStatus.pm_data = PM_V0 +1;
  
  LCD_Init();
  
  LCD_Disable();
  
  DataStatus.status |= LED_ON; 			//���⿪��
  
  Delay(1000);
  Get_Temp();

  while(1)
  {   

    if(TimeCount.w1sFlags)
    {
      if(DataStatus.status & POWER_ON) 
      {
        Get_Sensor();   
        LCD_ShowAll();
      }
      
      TimeCount.w1sFlags = 0;
    }

    if(TimeCount.w2sFlags)
    {
      if(DataStatus.status & POWER_ON) Get_Temp();
      TimeCount.w2sFlags = 0;
    }
    
    if(TimeCount.w10minFlags)
    {
      if(DataStatus.TVOC_Level == 0)
      {
        Rmax = old_Rmax;
        old_Rmax = 0;
      }
      
      if(PM_V0 > old_PM_V0)     // 0.4V
      {
          PM_V0 = old_PM_V0;
          save_data(FLASH_ADDRESS, (uint8_t *)&PM_V0, 1);
      }
      
      TimeCount.w10minFlags = 0;
    }
    
    if(TimeCount.w30minFlags)
    {
    
      TimeCount.w30minFlags = 0;
    }
  }
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

