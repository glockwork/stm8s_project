#include "pwm_out.h"


void PWM_Config(void)
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
  TIM1_OC1Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE,
               CCR1_Val, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
               TIM1_OCNIDLESTATE_RESET); 

  /*TIM1_Pulse = CCR2_Val Red LED*/
  TIM1_OC2Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, 
               CCR2_Val,TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET, 
               TIM1_OCNIDLESTATE_RESET);

  /*TIM1_Pulse = CCR4_Val Yellow LED*/
  TIM1_OC4Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, CCR3_Val, TIM1_OCPOLARITY_LOW,
               TIM1_OCIDLESTATE_SET);

  /* TIM1 counter enable */
  TIM1_Cmd(ENABLE);

  /* TIM1 Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
}

void LED_PWM_Show(uint8_t nPort, uint16_t nCCR_Val)
{
  switch (nPort)
  {
    case 0: TIM1_SetCompare2(0); 
            TIM1_SetCompare4(0); 
            break;
    case 1: TIM1_SetCompare2(nCCR_Val); 
            TIM1_SetCompare4(0); 
            break;
    case 3: TIM1_SetCompare4(nCCR_Val);
            TIM1_SetCompare2(0); 
            break;
    case 2: TIM1_SetCompare2(nCCR_Val); 
            TIM1_SetCompare4(nCCR_Val); 
            break; 
    default : break;
  }
}