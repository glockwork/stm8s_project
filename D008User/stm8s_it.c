/**
  ******************************************************************************
  * @file    stm8s_it.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service 
  *          routine.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s_it.h"
#include "main.h"

/** @addtogroup Template_Project
  * @{
  */


extern uint8_t RxRecvBuffer[BUFFERSIZE];
extern uint16_t DataSize;
extern NET_RECV    NetRecv;
extern DEVICE_STATUS    DeviceStatus;

#ifdef _COSMIC_
/**
  * @brief Dummy Interrupt routine
  * @par Parameters:
  * None
  * @retval
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*_COSMIC_*/

/**
  * @brief TRAP Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Top Level Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TLI_IRQHandler, 0)

{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Auto Wake Up Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Clock Controller Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTA Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTB Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */

}

#if defined (STM8S903) || defined (STM8AF622x) 
/**
  * @brief External Interrupt PORTF Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler, 8)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined (STM8AF52Ax)
/**
  * @brief CAN RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

/**
  * @brief CAN TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_TX_IRQHandler, 9)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S208) || (STM8AF52Ax) */

/**
  * @brief SPI Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer1 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined (STM8S903) || defined (STM8AF622x)
/**
  * @brief Timer5 Update/Overflow/Break/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
 
/**
  * @brief Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, 14)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

#else /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8S103) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */
/**
  * @brief Timer2 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
     500ms
  */
    static uint8_t TimeCount = 0, PreheatTimeOut = 0;
    static uint16_t PreheatTimeCount = 0;
    static uint8_t BatCount = 0;
    static uint8_t CheckCount = 0;
    static uint8_t PreheatFlash = 0;
    
    DeviceStatus.flashLight++;

    DeviceRemind();
    
    if(DeviceStatus.enterMode == ENTER_PREHEAT)
    {
        if(DeviceStatus.preheat == ON)
        {
            if(++CheckCount >= 6)
            {
                AutoControl(DeviceStatus.up_Temperature, DeviceStatus.down_Temperature);
                CheckCount = 0;
            }
            if(DeviceStatus.temperatureOK == OK)
            {
                PreheatFlash++;
                if(PreheatFlash == 1)
                {
                    showPreheat(ON);
                }
                else if(PreheatFlash >= 2)
                {
                    showPreheat(OFF);
                    PreheatFlash = 0;
                }
                if(PreheatTimeCount == 0)       // 预热温度OK时提醒
                {
                    // 10声蜂鸣声提醒
                    KeyBeep();
                    DeviceStatus.startWorkBeep = PREHEAT_RING;
                }
                if(++PreheatTimeCount >= 600)   // 每5分钟提醒一次
                {
                    if(++PreheatTimeOut >= 6)   // 30分钟后自动退出到待机模式
                    {
                        // 退出预热模式，进入功能选择模式
                        CancelKey();
                        Relay_Off_All();
                        PreheatTimeOut = 0;
                    }
                    PreheatTimeCount = 0;
                }
            }
        }
    }
    else        // 退出预热模式 计数变量清零，否则第二次预热模式工作时预热温度OK后无提醒。
    {
        PreheatTimeCount = 0;
        PreheatTimeOut = 0;
    }
    
    if(DeviceStatus.startWork == ON)
    {
        if(++CheckCount >= 6)
        {
            AutoControl(DeviceStatus.up_Temperature, DeviceStatus.down_Temperature);
            CheckCount = 0;
        }
        if(++TimeCount >= 120)  // 1分钟时间到
        {
            DeviceStatus.workTime--;
            if(DeviceStatus.workTime == 0) // 定时工作时间到
            {
                Relay_Off_All();
                CancelKey();
                DeviceStatus.startWorkBeep = END;
                DeviceStatus.startWork = OFF;
                DeviceStatus.timeOut = 1;
            }
            TimeCount = 0;
        }
    }
    
    if(++BatCount >= 2)
    {
#if 0
        Send_BAT_Voltage(Get_UP_NTC_Value());
        Send_BAT_Voltage(Get_DOWN_NTC_Value());
#endif
        BatCount = 0;
    }

    TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
 }

/**
  * @brief Timer2 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8AF626x)
/**
  * @brief Timer3 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
     0.5 MS
  */
    static int count = 0;
  
    if(DeviceStatus.beepSW) // 蜂鸣器处理
    {
        DeviceStatus.beep++;

        if(DeviceStatus.startWorkBeep == OFF)   // 单声按键音
        {
            if(DeviceStatus.beep == 1)
            {
                BEEP_Cmd(ENABLE);
                BEEP_H;
            }
            else if(DeviceStatus.beep == 200) BEEP_L;
            else if(DeviceStatus.beep >= 1000)
            {
                BEEP_Cmd(DISABLE);
                DeviceStatus.beep = 0;
                DeviceStatus.beepSW = OFF;
            }
        }
        else if(DeviceStatus.startWorkBeep == ON) // 双声工作开始提示音
        {
            if(DeviceStatus.beep == 1)
            {
                BEEP_Cmd(ENABLE);
                BEEP_H;
            }
            else if(DeviceStatus.beep == 200) BEEP_L;
            else if(DeviceStatus.beep == 800) BEEP_H;
            else if(DeviceStatus.beep == 1000) BEEP_L;
            else if(DeviceStatus.beep >= 2400)
            {
                BEEP_Cmd(DISABLE);
                DeviceStatus.beep = 0;
                DeviceStatus.beepSW = OFF;
                DeviceStatus.startWorkBeep = OFF;
            }
        }
        else if(DeviceStatus.startWorkBeep == END) // 三声工作结束提示音
        {
            if(DeviceStatus.beep == 1)
            {
                BEEP_Cmd(ENABLE);
                BEEP_H;
            }
            else if(DeviceStatus.beep == 200) BEEP_L;
            else if(DeviceStatus.beep == 800) BEEP_H;
            else if(DeviceStatus.beep == 1000) BEEP_L;
            else if(DeviceStatus.beep == 1600) BEEP_H;
            else if(DeviceStatus.beep == 1800) BEEP_L;
            else if(DeviceStatus.beep >= 3200)
            {
                BEEP_Cmd(DISABLE);
                DeviceStatus.beep = 0;
                DeviceStatus.beepSW = OFF;
                DeviceStatus.startWorkBeep = OFF;
            }
        }
        else if(DeviceStatus.startWorkBeep == PREHEAT_RING)  // 十声预热完成提示音
        {
            if(DeviceStatus.beep == 1)
            {
                BEEP_Cmd(ENABLE);
                BEEP_H;
            }
            else if(DeviceStatus.beep == 200) BEEP_L;
            else if(DeviceStatus.beep == 1200) BEEP_H;
            else if(DeviceStatus.beep == 1400) BEEP_L;
            else if(DeviceStatus.beep == 2600) BEEP_H;
            else if(DeviceStatus.beep == 2800) BEEP_L;
            else if(DeviceStatus.beep == 4000) BEEP_H;
            else if(DeviceStatus.beep == 4200) BEEP_L;
            else if(DeviceStatus.beep == 5400) BEEP_H;
            else if(DeviceStatus.beep == 5600) BEEP_L;
            else if(DeviceStatus.beep == 6800) BEEP_H;
            else if(DeviceStatus.beep == 7000) BEEP_L;
            else if(DeviceStatus.beep == 8200) BEEP_H;
            else if(DeviceStatus.beep == 8400) BEEP_L;
            else if(DeviceStatus.beep == 9600) BEEP_H;
            else if(DeviceStatus.beep == 9800) BEEP_L;
            else if(DeviceStatus.beep == 11000) BEEP_H;
            else if(DeviceStatus.beep == 11200) BEEP_L;
            else if(DeviceStatus.beep == 12400) BEEP_H;
            else if(DeviceStatus.beep == 12600) BEEP_L;
            else if(DeviceStatus.beep >= 14000)
            {
                BEEP_Cmd(DISABLE);
                DeviceStatus.beep = 0;
                DeviceStatus.beepSW = OFF;
                DeviceStatus.startWorkBeep = OFF;
            }
        }
    }
    CodingSwitchPolling();
    count++;
    if(count == 3)
    {
        DeviceStatus.Time_30ms = 1;     
        count = 0;
    }

    TIM3_ClearITPendingBit(TIM3_IT_UPDATE);
 }

/**
  * @brief Timer3 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)
/**
  * @brief UART1 TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART1 RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    uint8_t Recv;

    if(UART1_GetITStatus(UART1_IT_RXNE ) != RESET)  // 接收数据
    {
        Recv = UART1_ReceiveData8();
        
        RxRecvBuffer[DataSize++] = Recv;
        if(DataSize > BUFFERSIZE-1) DataSize = 0;
//        UART1_SendByte(Recv);
//        UART3_SendByte(Recv);
        
    } 
 }
#endif /* (STM8S208) || (STM8S207) || (STM8S103) || (STM8S903) || (STM8AF62Ax) || (STM8AF52Ax) */

#if defined(STM8AF622x)
/**
  * @brief UART4 TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART4_TX_IRQHandler, 17)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART4 RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART4_RX_IRQHandler, 18)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8AF622x) */

/**
  * @brief I2C Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
/**
  * @brief UART2 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART2 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S105) || (STM8AF626x) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief UART3 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_TX_IRQHandler, 20)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART3 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
//   uint8_t Recv;
//
//    if(UART3_GetITStatus(UART3_IT_RXNE ) != RESET)  
//    {
//        Recv = UART3_ReceiveData8();
//        
////        UART1_SendByte(Recv);
//        //    UART3_SendByte(Recv);
//    } 
 }
#endif /* (STM8S208) || (STM8S207) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief ADC2 interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(ADC2_IRQHandler, 22)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#else /* STM8S105 or STM8S103 or STM8S903 or STM8AF626x or STM8AF622x */
/**
  * @brief ADC1 interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
 INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined (STM8S903) || defined (STM8AF622x)
/**
  * @brief Timer6 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM6_UPD_OVF_TRG_IRQHandler, 23)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#else /* STM8S208 or STM8S207 or STM8S105 or STM8S103 or STM8AF52Ax or STM8AF62Ax or STM8AF626x */
/**
  * @brief Timer4 Update/Overflow Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
    1 MS
  */
    static unsigned char NET_RECV = 0, key = 0;

    TimingDelay_Decrement();

    NET_RECV++;
    key++;
    if(key >= 30)
    {
        code_key_read();
        key = 0;
    }
    if(NET_RECV >= 100)
    {
        DeviceStatus.Time_100ms = 1;
        read_key();
        key_process();
        NET_RECV = 0;
    }
    
    /* Cleat Interrupt Pending bit */
    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
 }
#endif /* (STM8S903) || (STM8AF622x)*/

/**
  * @brief Eeprom EEC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/