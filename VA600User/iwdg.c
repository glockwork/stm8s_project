#include "iwdg.h"

__IO uint8_t LsiFreq = 0xFF;	//设置为1.02秒

/*
名称: void IWDG_Config(void)
功能: 配置IWDG为1.02秒产生系统复位
形参: 无
返回: 无
*/
void IWDG_Config(void)
{
  IWDG_Enable();
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  IWDG_SetReload(LsiFreq);
  IWDG_ReloadCounter();
}
