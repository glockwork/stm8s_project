#include "tim3.h"

#define TIM3_PERIOD       1249


/**
  * @brief  Configure TIM3 to generate an update interrupt each 10ms 
  * @param  None
  * @retval None
  */
void TIM3_Config(void)
{
    /* TIM3 configuration:
    - TIM3CLK is set to 16 MHz, the TIM3 Prescaler is equal to 128 so the TIM3 counter
    clock used is 16 MHz / 128= 125 000 Hz

    - In this example we need to generate a time base equal to 10 ms
    so TIM3_PERIOD = (0.01 * 125000 - 1) = 1249 */

    /* Time base configuration */
    TIM3_TimeBaseInit(TIM3_PRESCALER_128, TIM3_PERIOD);

    TIM3_ARRPreloadConfig(ENABLE);
    
    TIM3_ClearFlag(TIM3_FLAG_UPDATE);
    /* Enable update interrupt */
    TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);

    /* Enable TIM2 */
    TIM3_Cmd(ENABLE);
}