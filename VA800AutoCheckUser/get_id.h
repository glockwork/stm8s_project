#ifndef __GET_ID_H
#define __GET_ID_H
#include "stm8s.h"

#define FLASH_UNIQUE_ID_ADDRESS         0x48CD

void Get_ChipID(void);

#endif