#include "tim2.h"

#define TIM2_PERIOD       15624


/**
  * @brief  Configure TIM2 to generate an update interrupt each 1ms 
  * @param  None
  * @retval None
  */
void TIM2_Config(void)
{
    /* TIM2 configuration:
    - TIM2CLK is set to 16 MHz, the TIM2 Prescaler is equal to 128 so the TIM2 counter
    clock used is 16 MHz / 1024 = 15 625 Hz

    - In this example we need to generate a time base equal to 1 s
    so TIM2_PERIOD = (1 * 15625 - 1) = 15624 */

    /* Time base configuration */
    TIM2_TimeBaseInit(TIM2_PRESCALER_1024, TIM2_PERIOD);

    TIM2_ARRPreloadConfig(ENABLE);
    
    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
    /* Enable update interrupt */
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);

    /* Enable TIM2 */
    TIM2_Cmd(ENABLE);
}
