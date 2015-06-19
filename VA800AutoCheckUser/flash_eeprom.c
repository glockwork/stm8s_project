#include "flash_eeprom.h"
extern void Delay(u16 nCount);
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

void Flash_eeprom_Init(void)
{
    /* Define flash programming Time*/
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
    /* Unlock Data memory */
    FLASH_Unlock(FLASH_MEMTYPE_DATA); 
}
