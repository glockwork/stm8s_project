#ifndef __DATA_BACKUP_H
#define __DATA_BACKUP_H

#include "stm8s.h"
#include "main.h"

#define FLASH_ADDRESS   0x40A5
#define PASSED          1
#define FAILED          0

uint8_t save_data(uint32_t address, uint8_t *data, uint8_t count);
uint8_t get_data(uint32_t address, uint8_t *data, uint8_t count);
uint8_t clear_flash_data(uint32_t address, uint8_t count);

#endif