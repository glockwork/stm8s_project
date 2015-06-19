#include "tim2.h"

#define TIM2_PERIOD       149

__IO uint32_t TIM2_TimingDelay = 0;
__IO uint8_t TIM2_Flag = 0;
__IO uint32_t TIM2_Counter = 0;

/**
  * @brief  Configure TIM2 to generate an update interrupt each 1ms 
  * @param  None
  * @retval None
  */
void TIM2_Config(void)
{
  /* TIM2 configuration:
   - TIM2CLK is set to 16 MHz, the TIM2 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 1 = 16 000 000 Hz

  - In this example we need to generate a time base equal to 10 us
   so TIM2_PERIOD = (0.00001 * 16000000 - 1) = 149 */

  /* Time base configuration */
  TIM2_TimeBaseInit(TIM2_PRESCALER_1, TIM2_PERIOD);
  TIM2_PrescalerConfig(TIM2_PRESCALER_1,TIM2_PSCRELOADMODE_IMMEDIATE);
  TIM2_ARRPreloadConfig(ENABLE);
  /* Enable update interrupt */
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  

  /* Enable TIM2 */
  TIM2_Cmd(ENABLE);
}


/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay_us(__IO uint32_t nTime)
{
  TIM2_TimingDelay = nTime;

  while (TIM2_TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TIM2_TimingDelay_Decrement(void)
{
  if (TIM2_TimingDelay != 0x00)
  {
    TIM2_TimingDelay--;
  }
}