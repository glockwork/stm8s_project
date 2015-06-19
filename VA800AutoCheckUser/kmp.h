#ifndef __KMP_H
#define __KMP_H
#include "stm8s.h"

typedef uint8_t* String;

void get_next(String T, int *next);
int Index_KMP(String S, String T, int pos);
#endif