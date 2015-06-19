#ifndef __NET_FUNCTION_H
#define __NET_FUNCTION_H

#include "stm8s.h"
#include "main.h"

#define SEND(data, length)	UART1_SendString(data, length)
#define BUFFERSIZE  400
#define SEND_DATA_LENGTH	10
#define FAIL_COUNT_OVER         10  //连接失败次数上限
#define AP_Message        "AT+CWJAP=\"t.tt\",\"huangguozheng\"\r\n"
#define Server_Message    "AT+CIPSTART=\"TCP\",\"192.168.191.1\",6000\r\n"

#define NET_OK                  0x1000
#define NET_ERROR               0x2000
#define NET_CLOSED              0x4000
#define NET_READY               0x8000
#define NET_SEND_OK             0x0100
#define NET_RECV_DATA           0x0200
#define NET_BUSY                0x0400
#define NET_FAIL                0x0800
#define NET_CONNECT             0x0010
#define NET_ALREADY_CONNECTED   0x0020
#define NET_RECONNECT           0x0040
#define NET_SEND_DATA           0x0080
#define NET_ACTION_SEND         0x0001
#define NET_SEND_FAIL           0x0002
#define NET_CONNECT_FAILED      0x0004

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
void TestNetMode(void);
void SetNetMode(uint8_t Mode);
void ResetNetMode(void);
void ClosedAP(void);
void ClosedConnectet(void);
void NetModeConnectAP(void);
void NetModeConnectServer(void);
void isRecvOK(String RxRecvData);
void isRecvReady(String RxRecvData);
void isRecvError(String RxRecvData);
void isRecvData(String RxRecvData);
void isRecvSendOK(String RxRecvData);
void isRecvSendFAIL(String RxRecvData);
void isRecvClosed(String RxRecvData);
void isRecvBusy(String RxRecvData);
void isRecvAL_Connected(String RxRecvData);
void isRecvFail(String RxRecvData);
void isRecvConnect(String RxRecvData);
void NetSendDataLength(void);
void NetSendData(String Data, uint16_t Length);
void GetNetFlags(String RxRecvData);
void DataResolve(String Data, uint16_t Length);
void NetModeErrorFix(void);
void NetProcess(void);
uint16_t strlen(String str);

#endif