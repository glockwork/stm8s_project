#include "basic_timer.h"

__IO uint32_t TimingDelay = 0;

void TIM1_Config(void)
{
    TIM1_DeInit();

    /* Time Base configuration */
    /*
    TIM1_Period = 999
    TIM1_Prescaler = 0
    TIM1_CounterMode = TIM1_COUNTERMODE_UP
    TIM1_RepetitionCounter = 0
    TIM1 counter clock = 16MHz
    */

    TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, 999, 0);

    /* Channel 1, 2 and 4 Configuration in PWM mode */

    /*
    TIM1_OCMode = TIM1_OCMODE_PWM2
    TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
    TIM1_OutputNState = TIM1_OUTPUTNSTATE_ENABLE
    TIM1_Pulse = CCR1_Val
    TIM1_OCPolarity = TIM1_OCPOLARITY_LOW
    TIM1_OCNPolarity = TIM1_OCNPOLARITY_HIGH
    TIM1_OCIdleState = TIM1_OCIDLESTATE_SET
    TIM1_OCNIdleState = TIM1_OCIDLESTATE_RESET

    TIM1 Channeln duty cycle = TIM1_CCRn /( TIM1_Period + 1)
    */
    /*TIM1_Pulse = CCR2_Val Feng_2*/
    TIM1_OC2Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, 
               CCR2_Val,TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET, 
               TIM1_OCNIDLESTATE_RESET);

    /*TIM1_Pulse = CCR4_Val Feng_1*/
    TIM1_OC3Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, 
               CCR3_Val,TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET, 
               TIM1_OCNIDLESTATE_RESET);

    /* TIM1 counter enable */
    TIM1_Cmd(ENABLE);

    /* TIM1 Main Output Enable */
    TIM1_CtrlPWMOutputs(ENABLE);
}

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

void BasicTimer_Init(void)
{
    TIM1_Config();

    TIM2_Config();

    TIM3_Config();
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