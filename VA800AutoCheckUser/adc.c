#include "adc.h"


uint16_t Conversion_Value = 0;

/**
  * @brief  Configure ADC2 Continuous Conversion with End Of Conversion interrupt 
  *         enabled .
  * @param  None
  * @retval None
  */
void ADC_Config()
{
/*NAME          7       6       5       4       3       2       1       0
  ADC_CSR:      EOC     AWD     EOCIE   AWDIE   CH[3:0]
  ADC_CR1       NC      SPSEL[2:0]              NC      NC      CONT    ADON
  ADC_CR2       NC      EXITRIG EXSEL[1:0]      ALIGN   NC      SCAN    NC
  ADC_CR3       DBUF    OVR     NC      NC      NC      NC      NC      NC
  ADC_DRH       Data Register High
  ADC_DRL       Data Register Low
  
  EOC:转换结束标志位, 0转换未结束,1转换结束
  EOCIE:转换结束EOC的中断使能, 0:禁止, 1:使能转换结束中断
  CH:选择转换通道(0~15)
  
  SPSEL:预分频选择位:2,3,4,6,8,10,12,18
  CONT:连续转换,0:单次,1:连续
  ADON:AD转换开关,1:使能ADC并开始转换
  
  EXTTRIG:外触发使能位,1:使能
  EXTSEL:外部事件选择位, 00:内部定时器1 TRG事件, 01:ADC_ETR引脚的外部中断
  ALIGN:数据排列, 0:数据左对齐, 1:数据右对齐
*/ 
 
  /* De-Init ADC peripheral*/
  ADC2_DeInit();

  /* Init ADC2 peripheral */
#if CONVERSIONMODE ==CONVERSIONMODE_SINGLE
  ADC2_Init(ADC2_CONVERSIONMODE_SINGLE, ADC2_CHANNEL_9, ADC2_PRESSEL_FCPU_D8, \
            ADC2_EXTTRIG_TIM, DISABLE, ADC2_ALIGN_RIGHT, ADC2_SCHMITTTRIG_CHANNEL9,\
            DISABLE);

  /* Enable EOC interrupt */
//  ADC2_ITConfig(ENABLE);
  ADC2_ITConfig(DISABLE);
#elif CONVERSIONMODE ==CONVERSIONMODE_CONTINUOUS
  ADC2_Init(ADC2_CONVERSIONMODE_CONTINUOUS, ADC2_CHANNEL_9, ADC2_PRESSEL_FCPU_D8, \
            ADC2_EXTTRIG_TIM, DISABLE, ADC2_ALIGN_RIGHT, ADC2_SCHMITTTRIG_CHANNEL9,\
            DISABLE);

  /* Enable EOC interrupt */
  ADC2_ITConfig(DISABLE);
#endif
  
  ADC2_Cmd(ENABLE);
  
  /*Start Conversion */
  ADC2_StartConversion();
}

uint16_t Get_ADC_Value(ADC2_Channel_TypeDef channel)
{
  uint16_t i, j, Conversion_Value = 0;
  
  ADC2_ConversionConfig(ADC2_CONVERSIONMODE_SINGLE, channel, ADC2_ALIGN_RIGHT);
  j = 0xFFFF;
  while(j > 0)
  {
    j--;
  }
  
  for (i = 0; i < 10; i++)
  {
    ADC2_StartConversion();
  
    Conversion_Value += ADC2_GetConversionValue();
    j = 0xFF;
    while(j > 0)
    {
      j--;
    }
  }
  
  Conversion_Value /= 10;
  
  return Conversion_Value;
}

uint16_t Get_DS_Value(void)
{
  uint16_t i, j, Conversion_Value = 0;
  
  ADC2_ConversionConfig(ADC2_CONVERSIONMODE_SINGLE, ADC2_CHANNEL_8, ADC2_ALIGN_RIGHT);
  
  j = 0xFFFF;
  while(j > 0)
  {
    j--;
  }
  
  for (i = 0; i < 50; i++)
  {
    GPIO_WriteHigh(DS_PORT, (GPIO_Pin_TypeDef)DS_PIN);
    Delay_us(28);
    
    ADC2_StartConversion();
  
    Conversion_Value += ADC2_GetConversionValue();
    
    Delay_us(4);
    GPIO_WriteLow(DS_PORT, (GPIO_Pin_TypeDef)DS_PIN);
    Delay_us(968);
  }
  
  Conversion_Value /= 50;
  
  return Conversion_Value;
}

//    Start_Channel_Converion();
    
/*
uint16_t Get_ADC_Value(ADC2_Channel_TypeDef channel)
{
  uint16_t i, j, k = 0;//, temp[10];
  
  ADC2_ConversionConfig(ADC2_CONVERSIONMODE_SINGLE, channel, ADC2_ALIGN_RIGHT);
  
  Delay(100);

  for (i = 0; i < 10; i++)
  {
//    temp[i] = ADC2_GetConversionValue();
    ADC2_StartConversion();
    
//    while(ADC2_GetFlagStatus() != SET);
  
//    temp[i] = ADC2_GetConversionValue();
    k += ADC2_GetConversionValue();
    j = 0xFF;
    while(j > 0)
    {
      j--;
    }
  }
  
//  for (i = 0; i < 10; i++)
//  {
//    for (j = i; j < 10; j++)
//    {
//      if (temp[i] < temp[j])
//      {
//        k = temp[i];
//        temp[i] = temp[j];
//        temp[j] = k;
//      }
//    }
//  }
//  k = 0;
//  for (i = 1; i < 9; i++)
//  {
//    k += temp[i];
//  }
  
//  k /= 8;
  k /= 10;
  
  return k;
}
*/