#include "gpio.h"

/**
  * @brief  Configure GPIO for buttons and Leds available on the evaluation board
  * @param  None
  * @retval None
  */
void GPIO_FN_Init(void)
{
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD);
  GPIO_DeInit(GPIOE);
  GPIO_DeInit(GPIOF);

  /* Initialize I/Os in Output Mode */
  GPIO_Init(LEDS_PORT, (GPIO_Pin_TypeDef)LED1_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(FAN_PORT, (GPIO_Pin_TypeDef)FAN_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPRS_PW_PORT, (GPIO_Pin_TypeDef)GPRS_PW_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(BT_TO_12V_PORT, (GPIO_Pin_TypeDef)BT_TO_12V_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(ANION_PORT, (GPIO_Pin_TypeDef)ANION_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(DS_PORT, (GPIO_Pin_TypeDef)DS_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(DS_PW_PORT, (GPIO_Pin_TypeDef)DS_PW_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);
  
  /* Initialize I/O in Input Mode with Interrupt for Cap Button */
  GPIO_Init(CAP_PORT, (GPIO_Pin_TypeDef)(CAP_PIN), GPIO_MODE_IN_FL_IT);
  GPIO_Init(DC_IN_PORT, (GPIO_Pin_TypeDef)DC_IN_PIN, GPIO_MODE_IN_FL_IT);
  GPIO_Init(TOUCH_PORT, (GPIO_Pin_TypeDef)TOUCH_PIN, GPIO_MODE_IN_FL_IT);
  GPIO_Init(RTC_INT_PORT, (GPIO_Pin_TypeDef)RTC_INT_PIN, GPIO_MODE_IN_PU_IT);
  
  /*  Init GPIO for ADC2 */
  GPIO_Init(GPIOE, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);  //battery AIN9
  GPIO_Init(GPIOE, GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);  //pm2.5   AIN8
}

/**
  * @brief  Configure GPIO for VA800
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
  GPIO_FN_Init();
  
  GPIO_WriteHigh(GPRS_PW_PORT, (GPIO_Pin_TypeDef)GPRS_PW_PIN);
  GPIO_WriteHigh(ANION_PORT, (GPIO_Pin_TypeDef)ANION_PIN);
  GPIO_WriteHigh(DS_PW_PORT, (GPIO_Pin_TypeDef)DS_PW_PIN);
  GPIO_WriteHigh(BT_TO_12V_PORT, (GPIO_Pin_TypeDef)BT_TO_12V_PIN);
  
  if ((GPIO_ReadInputData(DC_IN_PORT) & DC_IN_PIN) == 0x00)
  {
//    DC_IN_Status = DC_INPUT;
  }
  if ((GPIO_ReadInputData(CAP_PORT) & CAP_PIN) == 0x00)
  {
    GPIO_WriteHigh(LEDS_PORT, (GPIO_Pin_TypeDef)LED1_PIN);
    GPIO_WriteHigh(FAN_PORT, (GPIO_Pin_TypeDef)FAN_PIN);
  }
  /* Initialize the Interrupt sensitivity */  
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_RISE_FALL);
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_FALL);
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOE, EXTI_SENSITIVITY_RISE_FALL);
}