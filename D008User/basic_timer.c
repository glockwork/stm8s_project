#include "basic_timer.h"

#define TIM2_PERIOD             15624
#define TIM3_PERIOD             499
#define TIM4_PERIOD             124

__IO uint32_t TimingDelay = 0;
__IO uint8_t TIM4_Flag = 0;
__IO uint32_t TIM4_Counter = 0;

void TIM2_Config(void)
{
    /* TIM2 configuration:
    - TIM2CLK is set to 16 MHz, the TIM2 Prescaler is equal to 128 so the TIM2 counter
    clock used is 16 MHz / 512 = 31250 Hz

    - In this example we need to generate a time base equal to 500 ms
    so TIM2_PERIOD = (0.5 * 312500 - 1) = 15624 */

    /* Time base configuration */
    TIM2_TimeBaseInit(TIM2_PRESCALER_512, TIM2_PERIOD);

    TIM2_ARRPreloadConfig(ENABLE);
    
    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
    /* Enable update interrupt */
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);

    /* Enable TIM2 */
    TIM2_Cmd(ENABLE);
}

void TIM3_Config(void)
{
    /* TIM3 configuration:
    - TIM3CLK is set to 16 MHz, the TIM3 Prescaler is equal to 128 so the TIM3 counter
    clock used is 16 MHz / 16= 1 000 000 Hz

    - In this example we need to generate a time base equal to 0.5 ms
    so TIM3_PERIOD = (0.00005 * 1000000 - 1) = 499 */

    /* Time base configuration */
    TIM3_TimeBaseInit(TIM3_PRESCALER_16, TIM3_PERIOD);

    TIM3_ARRPreloadConfig(ENABLE);
    
    TIM3_ClearFlag(TIM3_FLAG_UPDATE);
    /* Enable update interrupt */
    TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);

    /* Enable TIM2 */
    TIM3_Cmd(ENABLE);
}

/**
  * @brief  Configure TIM4 to generate an update interrupt each 1ms 
  * @param  None
  * @retval None
  */
void TIM4_Config(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
   so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while (TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

