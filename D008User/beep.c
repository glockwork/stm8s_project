/******************** (C) COPYRIGHT  风驰电子嵌入式开发工作室 ********************/


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
    uint16_t Beep_Option_status;/*记录激活备选功能Beep的状态*/

    /* Define FLASH programming time */
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

    /* Unlock Data memory */
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    
    Beep_Option_status = FLASH_ReadOptionByte(Beep_OptionAdd);

    /*Beep_Option_status的最高位为1激活了Beep,否则不激活*/
    if(!(Beep_Option_status & 0x8000))
    {
        FLASH_ProgramOptionByte(Beep_OptionAdd, (uint8_t)(Beep_Option_status | 0x8000));
        /*向Beep_OptionAdd  0x4803 置1，激活了Beep*/
    }
    
    FLASH_Lock(FLASH_MEMTYPE_PROG);
}




/******************* (C) COPYRIGHT 风驰电子嵌入式开发工作室 *****END OF FILE****/