#include "get_id.h"

uint8_t ChipUniqueID[12];

/********************************************************
 * 函数名：Get_ChipID
 * 描述  ：获取芯片ID, 0x48CD to 0x48D8
 * 输入  ：无
 * 输出  ：无
 ********************************************************/
void Get_ChipID(void)
{
  uint8_t i = 0;
  
  for (; i < 12; i++)
  {
    ChipUniqueID[i] = (*(__IO uint8_t *)(FLASH_UNIQUE_ID_ADDRESS +i));
  }
}