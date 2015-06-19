/******************** (C) COPYRIGHT  ��۵���Ƕ��ʽ���������� ********************/


#include "beep.h"
#define Beep_OptionAdd  0x4803



void Beep_Init(BEEP_Frequency_TypeDef BEEP_Frequency)
{
    BEEP_Init(BEEP_Frequency);
    CLK_LSICmd(ENABLE);
    BEEP_Cmd(ENABLE);
}

void Set_Beep_OptionByte(void)
{
    uint16_t Beep_Option_status;/*��¼���ѡ����Beep��״̬*/

    /* Define FLASH programming time */
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

    /* Unlock Data memory */
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    
    Beep_Option_status = FLASH_ReadOptionByte(Beep_OptionAdd);

    /*Beep_Option_status�����λΪ1������Beep,���򲻼���*/
    if(!(Beep_Option_status & 0x8000))
    {
        FLASH_ProgramOptionByte(Beep_OptionAdd, (uint8_t)(Beep_Option_status | 0x8000));
        /*��Beep_OptionAdd  0x4803 ��1��������Beep*/
    }
    
    FLASH_Lock(FLASH_MEMTYPE_PROG);
}




/******************* (C) COPYRIGHT ��۵���Ƕ��ʽ���������� *****END OF FILE****/