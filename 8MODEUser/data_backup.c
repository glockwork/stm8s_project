#include "data_backup.h"

/*
����: uint8_t save_data(uint8_t *data, uint8_t count)
����: �����ݴ洢��MCU��Flash��
�β�: *data ����ָ��
      count ���ݳ���
����ֵ������ɹ�����PASSED, ����ʧ�ܷ���FAILED
*/ 
uint8_t save_data(uint32_t address, uint8_t *data, uint8_t count)
{
  uint8_t i = 0x00, val_comp = 0x00;
  uint32_t add = address;

  /* Define FLASH programming time */
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

  /* Unlock Data memory */
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  
  for(i = 0; i < count; i++)
  {
    FLASH_ProgramByte((add + i), *data);
    
    val_comp = FLASH_ReadByte((add + i));
    
    if(val_comp != *data)
      return FAILED;
    data++;
  }
  
  FLASH_Lock(FLASH_MEMTYPE_PROG);
  
  return PASSED;
}

/*
����: uint8_t get_data(uint8_t *data, uint8_t count)
����: ��ȡMCU��Flash��洢������
�β�: *data ����ָ��
      count ���ݳ���
����ֵ����ȡ�ɹ�����PASSED, ��ȡʧ�ܷ���FAILED
*/ 
uint8_t get_data(uint32_t address, uint8_t *data, uint8_t count)
{
  uint8_t i = 0x00, val_comp = 0x00;
  uint32_t add = address;

  /* Define FLASH programming time */
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

  /* Unlock Data memory */
  FLASH_Unlock(FLASH_MEMTYPE_DATA);

  
  for(i = 0; i < count; i++)
  {
    val_comp = FLASH_ReadByte((add + i));
    
    *data = FLASH_ReadByte((add + i));
    
    if(val_comp != *data)
      return FAILED;
    data++;
  }
  
  FLASH_Lock(FLASH_MEMTYPE_PROG);
  
  return PASSED;
}

/*
����: uint8_t clear_flash_data(uint8_t count)
����: ����MCU��Flash��洢������
�β�: count ���ݳ���
����ֵ�������ɹ�����PASSED, ����ʧ�ܷ���FAILED
*/ 
uint8_t clear_flash_data(uint32_t address, uint8_t count)
{
  uint8_t i, val = 0x00, val_comp = 0x00;
  uint32_t add = address;

  /* Define FLASH programming time */
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

  /* Unlock Data memory */
  FLASH_Unlock(FLASH_MEMTYPE_DATA);

  
  for(i = 0; i < count; i += 2)
  {
    FLASH_EraseByte((add + i));
    
    FLASH_EraseByte((add + 1 + i));
    /* Erase action */
    val = FLASH_ReadByte((add + i));
    val_comp = FLASH_ReadByte((add + 1 + i));
    if ((val != 0x00) & (val_comp != 0x00))
      return FAILED;
  }
  
  FLASH_Lock(FLASH_MEMTYPE_PROG);
  
  return PASSED;
}

