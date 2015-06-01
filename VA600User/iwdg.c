#include "iwdg.h"

__IO uint8_t LsiFreq = 0xFF;	//����Ϊ1.02��

/*
����: void IWDG_Config(void)
����: ����IWDGΪ1.02�����ϵͳ��λ
�β�: ��
����: ��
*/
void IWDG_Config(void)
{
  IWDG_Enable();
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  IWDG_SetReload(LsiFreq);
  IWDG_ReloadCounter();
}
