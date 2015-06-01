/*
*ADC��ȡ
*/
#include "adc.h"


/*
����: uint16_t MathData(uint8_t *Data)
����: ��ƽ��ֵ
�β�: *Data ���ݻ�������ַ
����ֵ�����ݵ�ƽ��ֵ��
*/ 
uint16_t MathData(uint16_t *Data)
{
  uint8_t i, j;
  uint16_t temp;
  uint32_t value;
  
  for(i = 0; i < 50; i++)
  {
    for(j = i; j < 50; j++)
    {
      if(Data[i] < Data[j])
      {
        temp = Data[i];
        Data[i] = Data[j];
        Data[j] = temp;
      }
    }
  }
  
  value = 0;
  for(i = 10; i < 40; i++)
    value += Data[i];
  
  temp = value / 30;
  
  return temp;
}

/*
����: uint16_t Get_BAT_Value(void)
����: ��ȡ��ص�ѹ
�β�: ��
����ֵ����ص�ѹ
*/ 
uint16_t Get_BAT_Value(void)
{
  uint16_t i, j, temp, buf[50];
  
  GPIO_Init(GPIOE, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);
  
  ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL9, DISABLE);
  ADC2_ConversionConfig(ADC2_CONVERSIONMODE_SINGLE, ADC2_CHANNEL_9, ADC2_ALIGN_RIGHT);
  
  ADC2_StartConversion();
  
  for (i = 0; i < 50; i++)
  {
    ADC2_StartConversion();
    buf[i] = ADC2_GetConversionValue();
    for (j = 0; j < 0xFF; j++);
  }

  temp = MathData(buf);
  
  return temp;
}


/*
����: uint16_t Get_DS_Value(void)
����: ��ȡ�����ﴫ������ѹ
�β�: ��
����ֵ�������ﴫ�����������ѹ
*/ 
uint16_t Get_DS_Value(void)
{
  uint16_t i, j, temp, buf[50];
  
  GPIO_Init(GPCTRL_PORT, GPCTRL, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOE, GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);
  
  ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL8, DISABLE);
  ADC2_ConversionConfig(ADC2_CONVERSIONMODE_SINGLE, ADC2_CHANNEL_8, ADC2_ALIGN_RIGHT);

  ADC2_StartConversion();
  
  for(i = 0; i < 50; i++) 
  {
    GPIO_WriteHigh(GPCTRL_PORT, GPCTRL);        
    for(j = 0; j < DELAY_1; j++);
    ADC2_StartConversion();
    buf[i] = ADC2_GetConversionValue(); 
    buf[i] = ADC2_GetConversionValue(); 
    for(j = 0; j < DELAY_2; j++);    
    GPIO_WriteLow(GPCTRL_PORT, GPCTRL);
    for(j = 0; j < DELAY_3; j++);
  }

  temp = MathData(buf);
    
  return temp;
}

    