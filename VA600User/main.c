#include "main.h"

DEVICE_STATUS           DeviceStatus = {0};

void main(void)
{  

    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    GPIO_Config_Init();
    
    BasicTimer_Init();

    IWDG_Config();

    enableInterrupts(); 

    while(1)
    {   
        if(DeviceStatus.T_30ms_Out == 1)
        {
            openings_check();                   // ¿ª¸Ç¼ì²â
            feng_key_read();                    // ·çÉÈ¿ØÖÆ¼ü¼ì²â
            disinfect_key_read();               // Ïû¶¾¼ü¼ì²â
            DeviceStatus.T_30ms_Out = 0;
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

