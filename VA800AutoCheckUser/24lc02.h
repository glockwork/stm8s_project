#ifndef __24LC02_H
#define __24LC02_H
//#include "stdint.h"
#include "stm8s.h"



#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	                8191
#define AT24C128	16383
#define AT24C256	32767  
 
#define EE_TYPE AT24C02
					  
uint8_t AT24CXX_Check(void);  
void At24cxx_Init(void);
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr);
void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);


 
#endif

/*********************************************************************************************************
** End of File
*********************************************************************************************************/
