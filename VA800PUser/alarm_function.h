#ifndef __ALARM_FUNCTION_H
#define __ALARM_FUNCTION_H

#include "main.h"

void GetTime(unsigned char *TimeBuffer);
unsigned char ArrayCMP(unsigned char *Array1, unsigned char *Array2, unsigned char Length);
void ArrayCopy(unsigned char *Array1, unsigned char *Array2, unsigned char Length);
unsigned int TimerCMP(unsigned char (*Time1)[3], unsigned char *Time2, unsigned char Number);
unsigned int CheckAlarmTime(unsigned char (*Alarm)[3]);

#endif