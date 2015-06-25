#ifndef __NET_FUNCTION_H
#define __NET_FUNCTION_H

#include "stm8s.h"
#include "main.h"

#define SEND(data, length)	UART1_SendString(data, length)
#define BUFFERSIZE  400

typedef uint8_t* String;


typedef struct
{
    uint16_t    Status;
    uint8_t     ErrorCode;
    uint8_t     Wait;
    uint8_t     RecvData[400];
    uint8_t     SendData[128];
    uint8_t     RecvDataSize;
} NET_RECV;

void get_next(String T, uint16_t *next);
uint16_t Index_KMP(String S, String T, uint16_t pos);

void DataResolve(String Data, uint16_t Length);

uint16_t strlen(String str);


void GetMessageFunction(uint8_t* Message);
void FunctionResponse(uint8_t *MessageID, uint8_t Token);
void ResolveMessage(uint8_t *Message);
uint8_t stringCMP(uint8_t *str1, uint8_t *str2, uint8_t len);
void PingResponse(uint8_t* MessageID);
#endif