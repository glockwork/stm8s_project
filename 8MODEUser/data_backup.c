#include "data_backup.h"

/*
名称: uint8_t save_data(uint8_t *data, uint8_t count)
功能: 将数据存储到MCU的Flash里
形参: *data 数据指针
      count 数据长度
返回值：保存成功返回PASSED, 保存失败返回FAILED
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
名称: uint8_t get_data(uint8_t *data, uint8_t count)
功能: 读取MCU的Flash里存储的数据
形参: *data 数据指针
      count 数据长度
返回值：读取成功返回PASSED, 读取失败返回FAILED
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
名称: uint8_t clear_flash_data(uint8_t count)
功能: 擦除MCU的Flash里存储的数据
形参: count 数据长度
返回值：擦除成功返回PASSED, 擦除失败返回FAILED
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

