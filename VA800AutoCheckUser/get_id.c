#include "get_id.h"

uint8_t ChipUniqueID[12];

/********************************************************
 * ��������Get_ChipID
 * ����  ����ȡоƬID, 0x48CD to 0x48D8
 * ����  ����
 * ���  ����
 ********************************************************/
void Get_ChipID(void)
{
  uint8_t i = 0;
  
  for (; i < 12; i++)
  {
    ChipUniqueID[i] = (*(__IO uint8_t *)(FLASH_UNIQUE_ID_ADDRESS +i));
  }
}