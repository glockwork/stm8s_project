#include "gpio.h"


void GPIO_Config_Init(void)
{
    /* Initialize I/Os in Output Mode */
    GPIO_Init(GPIOE, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);    //����ָʾ��
    GPIO_Init(GPIOF, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST);    //����ָʾ��
    GPIO_Init(GPIOF, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);    //���ȵ�Դ
    GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);    //GPRS��Դ
    GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_OUT_PP_LOW_FAST);    //���ת12V
    GPIO_Init(GPIOB, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);    //������
    GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);    //PM2.5 LED
    GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);    //PM2.5��Դ
    GPIO_Init(GPIOB, GPIO_PIN_7, GPIO_MODE_IN_PU_IT);        //DC����
    GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_IN_PU_IT);        //��������
    GPIO_Init(GPIOE, GPIO_PIN_0, GPIO_MODE_IN_PU_NO_IT);        //������λ
    GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT);        //��������
  
  
//    BitStatus bit_status;
//    bit_status = GPIO_ReadInputPin(GPIOB, GPIO_PIN_7);    //DC����
//    if (bit_status == RESET)  //SET or RESET
//    DeviceStatus.FN_Status |= PWOER_MODE;
//    else
//    DeviceStatus.FN_Status &= ~PWOER_MODE;
//
//    bit_status = GPIO_ReadInputPin(GPIOD, GPIO_PIN_0);    //��������
//    if (bit_status == RESET)  //SET or RESET
//    {
//    FAN_POWER_ON;
//    DeviceStatus.PW_Status |= FAN_POWER;
//    DeviceStatus.DS_Status &= ~CAP_OPEN;
//    }
//    else
//    {
//    FAN_POWER_OFF;
//    DeviceStatus.PW_Status &= ~FAN_POWER;
//    DeviceStatus.DS_Status |= CAP_OPEN;
//    }
//
//    FAN_SPEED_OFF;
//    if(((DeviceStatus.FN_Status & BATTERY_LEVELS) != BATTERY_LEVEL_0) || (DeviceStatus.FN_Status & PWOER_MODE)) //���ǵ͵�״̬ or DC����
//    {
//    D12_POWER_ON;
//    NET_POWER_ON;
//    PM_POWER_ON;
//    DeviceStatus.PW_Status |= (D12_POWER | NET_POWER | PM_POWER);
//    }
//    else
//    {
//    DeviceStatus.PW_Status &= ~(D12_POWER | NET_POWER | PM_POWER);
//    }

    D12_POWER_ON;
    NET_POWER_ON;
    PM_POWER_ON;
//    FAN_POWER_ON;
    /* Initialize the Interrupt sensitivity */  
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_RISE_FALL);
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_FALL);
}

void DeviceWork(void)
{
    D12_POWER_ON;
    PM_POWER_ON;
    FAN_POWER_ON;
//    NET_POWER_ON;
    ANION_POWER_ON;
}

void DeviceWait(void)
{
    D12_POWER_OFF;
    FAN_POWER_OFF;
    ANION_POWER_OFF;
}